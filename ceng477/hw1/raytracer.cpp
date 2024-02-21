#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <math.h>
#include <vector>
#include <limits>
#include <thread>

typedef unsigned char RGB[3];

struct Intersection
{
	bool isintersect;
	parser::Vec3f point;
	parser::Vec3f normal;
	int mat_id;
	};
parser::Vec3f veclecarp( parser::Vec3f a , parser::Vec3f b)
{
    parser::Vec3f c;
    c.x=a.x*b.x;
    c.y=a.y*b.y;
    c.z=a.z*b.z;
    return c;
}

float dot( parser::Vec3f a , parser::Vec3f b)
{
    float c=0;
    c+=a.x*b.x;
    c+=a.y*b.y;
    c+=a.z*b.z;
    return c;
}

parser::Vec3f sabitlecarp( parser::Vec3f a , float b)
{
    parser::Vec3f c;
    c.x=a.x*b;
    c.y=a.y*b;
    c.z=a.z*b;
    return c;
}

parser::Vec3f cross(parser::Vec3f a , parser::Vec3f b)
{
	parser::Vec3f sonuc;
	sonuc.x = a.y*b.z-a.z*b.y;
	sonuc.y = a.z*b.x - a.x*b.z;
	sonuc.z = a.x*b.y - a.y*b.x;
	return sonuc;
}
parser::Vec3f topla( parser::Vec3f a , parser::Vec3f b)
{
    parser::Vec3f c;
    c.x=a.x+b.x;
    c.y=a.y+b.y;
    c.z=a.z+b.z;
    return c;
}

parser::Vec3f eksi( parser::Vec3f a)
{
    parser::Vec3f c;
    c.x=-a.x;
    c.y=-a.y;
    c.z=-a.z;
    return c;
}


parser::Vec3f normalize(parser::Vec3f ray) {
    parser::Vec3f newRay;
    float total = ray.x * ray.x + ray.y *ray.y + ray.z* ray.z;
    newRay.x = sqrt(ray.x*ray.x/total);
    if(ray.x<0) newRay.x = newRay.x*-1;
    newRay.y = sqrt(ray.y*ray.y/ total);
    if(ray.y<0) newRay.y = newRay.y*-1;
    newRay.z = sqrt(ray.z*ray.z/ total);
    if(ray.z<0) newRay.z = newRay.z*-1;
    return newRay;
}
parser::Vec3f CameRay(parser::Camera cam, int x, int y) {
    parser::Vec3f rightVec;
    parser::Vec3f rayDir;
    parser::Vec3f m, q, s, e;
    e = cam.position;
    rightVec.x = (cam.up.y * -1*cam.gaze.z +cam.gaze.y * cam.up.z);
    rightVec.y = (cam.up.z * -1*cam.gaze.x +cam.gaze.z * cam.up.x);
    rightVec.z = (cam.up.x * -1*cam.gaze.y +cam.gaze.x * cam.up.y);
    parser::Vec3f position = cam.position;
    m.x = cam.position.x + cam.gaze.x*cam.near_distance;
    m.y = cam.position.y + cam.gaze.y*cam.near_distance;
    m.z = cam.position.z + cam.gaze.z*cam.near_distance;
    q.x = m.x + cam.near_plane.x * rightVec.x + cam.near_plane.w * cam.up.x;
    q.y = m.y + cam.near_plane.x * rightVec.y + cam.near_plane.w * cam.up.y;
    q.z = m.z + cam.near_plane.x * rightVec.z + cam.near_plane.w * cam.up.z;
    s.x = q.x + rightVec.x*((float)x + 0.5) * ((cam.near_plane.y - cam.near_plane.x) / cam.image_width) - cam.up.x*((float)y +0.5)* ((cam.near_plane.w-cam.near_plane.z)/cam.image_height);
    s.y = q.y + rightVec.y*((float)x + 0.5) * ((cam.near_plane.y - cam.near_plane.x) / cam.image_width) - cam.up.y*((float)y + 0.5) * ((cam.near_plane.w - cam.near_plane.z) / cam.image_height);
    s.z = q.z + rightVec.z*((float)x + 0.5) * ((cam.near_plane.y - cam.near_plane.x) / cam.image_width) - cam.up.z*((float)y + 0.5) * ((cam.near_plane.w - cam.near_plane.z) / cam.image_height);
    rayDir.x = s.x - e.x;
    rayDir.y = s.y - e.y;
    rayDir.z = s.z - e.z;
    return rayDir;
    //return normalize(rayDir);
}
float Distance(parser::Vec3f a, parser::Vec3f b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}

float sDistance(parser::Vec3f a)
{
    return sqrt((a.x)*(a.x)+(a.y)*(a.y)+(a.z)*(a.z));
}

float Det(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return (a1*(c3*b2-b3*c2)-a2*(c3*b1-c1*b3)+a3*(c2*b1-c1*b2));
}

Intersection FindNearestIntersect(parser::Vec3f ray,parser::Scene scene, parser::Vec3f cameraPos){
	float discriminant = -1;
	float minDist = std::numeric_limits<float>::infinity();
	//int flag=0;
	Intersection intersect;
	intersect.isintersect = false;
	ray=normalize(ray);
	for(int k = 0; k< scene.spheres.size(); k++){
		//flag =0;
			parser::Vec3f closestPos;
			parser::Sphere sphere = scene.spheres[k];
			parser::Vec3f oeksic = topla(cameraPos, eksi(scene.vertex_data[sphere.center_vertex_id-1]));
			discriminant = dot(ray,oeksic)*dot(ray,oeksic)-(dot(ray,ray)*(dot(oeksic,oeksic)-sphere.radius*sphere.radius));
			if(discriminant>0)
			{
				//std::cout<<"@";
				//flag=1;
				float ts = (-1* dot(ray,oeksic) - sqrt(discriminant))/dot(ray,ray);
				if(ts>0)
                {
                    float tempDist =  Distance(cameraPos,topla(cameraPos,sabitlecarp(ray,ts)));
                    if(tempDist<minDist)
                    {
                        minDist = tempDist;
                        intersect.point = topla(cameraPos,sabitlecarp(ray,ts));
                        intersect.mat_id = sphere.material_id;
                        intersect.normal = topla(intersect.point, eksi(scene.vertex_data[sphere.center_vertex_id-1]));
                        intersect.isintersect = true;
                    }
                }
			}
			//else std::cout<<" ";
		}

	for (int l =0; l< scene.meshes.size(); l++)
	{
		parser::Mesh mesh = scene.meshes[l];
		for(int m =0; m<mesh.faces.size();m++)
		{

			float tm, alpha, beta, gama, determinant;
			determinant =
			Det(scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v1_id-1].x,
			scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v2_id-1].x,
			ray.x,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v1_id-1].y,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v2_id-1].y,
			ray.y,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v1_id-1].z,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v2_id-1].z,
			ray.z
			);


			beta = Det(scene.vertex_data[mesh.faces[m].v0_id-1].x-cameraPos.x,
			scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v2_id-1].x,
			ray.x,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-cameraPos.y,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v2_id-1].y,
			ray.y,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-cameraPos.z,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v2_id-1].z,
			ray.z
			)/determinant;


			gama = Det(scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v1_id-1].x,
			scene.vertex_data[mesh.faces[m].v0_id-1].x-cameraPos.x,
			ray.x,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v1_id-1].y,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-cameraPos.y,
			ray.y,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v1_id-1].z,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-cameraPos.z,
			ray.z
			)/determinant;


			tm= Det(scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v1_id-1].x,
			scene.vertex_data[mesh.faces[m].v0_id-1].x-scene.vertex_data[mesh.faces[m].v2_id-1].x,
			scene.vertex_data[mesh.faces[m].v0_id-1].x-cameraPos.x,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v1_id-1].y,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-scene.vertex_data[mesh.faces[m].v2_id-1].y,
			scene.vertex_data[mesh.faces[m].v0_id-1].y-cameraPos.y,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v1_id-1].z,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-scene.vertex_data[mesh.faces[m].v2_id-1].z,
			scene.vertex_data[mesh.faces[m].v0_id-1].z-cameraPos.z)/determinant;

			if(beta+gama<=1 && beta>=0 && gama >=0 && tm>0 &&  sDistance(sabitlecarp(ray,tm))<minDist) {
                minDist=sDistance(sabitlecarp(ray,tm));
				intersect.isintersect = true;
				intersect.point = topla(cameraPos,sabitlecarp(ray,tm));
				intersect.normal = cross(topla(scene.vertex_data[mesh.faces[m].v2_id-1],eksi(scene.vertex_data[mesh.faces[m].v1_id-1])),
				topla(scene.vertex_data[mesh.faces[m].v0_id-1],eksi(scene.vertex_data[mesh.faces[m].v1_id-1])));
				intersect.mat_id = mesh.material_id;
				//if()
				//std::cout<<"@";
				31;
				}
			//else if (flag == 0)std::cout<<" ";
		}




	}


        for(int m =0; m< scene.triangles.size();m++)
		{
			float tm, alpha, beta, gama, determinant;
			determinant =
			Det(scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v1_id-1].x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v2_id-1].x,
			ray.x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v1_id-1].y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v2_id-1].y,
			ray.y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v1_id-1].z,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v2_id-1].z,
			ray.z
			);


			beta = Det(scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-cameraPos.x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v2_id-1].x,
			ray.x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-cameraPos.y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v2_id-1].y,
			ray.y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-cameraPos.z,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v2_id-1].z,
			ray.z
			)/determinant;


			gama = Det(scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v1_id-1].x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-cameraPos.x,
			ray.x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v1_id-1].y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-cameraPos.y,
			ray.y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v1_id-1].z,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-cameraPos.z,
			ray.z
			)/determinant;


			tm= Det(scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v1_id-1].x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-scene.vertex_data[scene.triangles[m].indices.v2_id-1].x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].x-cameraPos.x,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v1_id-1].y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-scene.vertex_data[scene.triangles[m].indices.v2_id-1].y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].y-cameraPos.y,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v1_id-1].z,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-scene.vertex_data[scene.triangles[m].indices.v2_id-1].z,
			scene.vertex_data[scene.triangles[m].indices.v0_id-1].z-cameraPos.z)/determinant;

			if(beta+gama<=1 && beta>=0 && gama >=0 && tm>0 &&  sDistance(sabitlecarp(ray,tm))<minDist) {
                		minDist=sDistance(sabitlecarp(ray,tm));
				intersect.isintersect = true;
				intersect.point = topla(cameraPos,sabitlecarp(ray,tm));
				intersect.normal = cross(topla(scene.vertex_data[scene.triangles[m].indices.v2_id-1],eksi(scene.vertex_data[scene.triangles[m].indices.v1_id-1])),
				topla(scene.vertex_data[scene.triangles[m].indices.v0_id-1],eksi(scene.vertex_data[scene.triangles[m].indices.v1_id-1])));
				intersect.mat_id = scene.triangles[m].material_id;
				//if()
				//std::cout<<"@";
				31;
            }
		}
			//else if (flag == 0)std::cout<<" ";		}

	return intersect;
}



parser::Vec3f renkbul( parser::Material madde , parser::Vec3f yer , parser::Vec3f normal , parser::Camera kamera, parser::Scene &sahne , int derinlik )
{
    normal=normalize(normal);
    parser::Vec3f toplam;
    toplam.x=0;
    toplam.y=0;
    toplam.z=0;
    parser::Vec3f ambient= veclecarp(madde.ambient , sahne.ambient_light);
    toplam=ambient;
    if(dot(normal, topla(kamera.position, eksi(yer))) < 0) normal=eksi(normal);
    parser::Vec3f epsilonluyer = topla( yer , sabitlecarp(normalize(normal), sahne.shadow_ray_epsilon  ));
    for(int i=0 ; i<sahne.point_lights.size() ; i++)
    {
        parser::Vec3f ve = topla( eksi(sahne.point_lights[i].position) , (epsilonluyer)  );
        if( dot(normal, eksi(ve))<0 ) continue;
        Intersection ints = FindNearestIntersect( ve , sahne , sahne.point_lights[i].position);
        float yermessafe = sDistance( topla( sahne.point_lights[i].position , eksi(epsilonluyer)  )  );
        float intsmesafe;
        if(ints.isintersect)  intsmesafe = sDistance( topla( sahne.point_lights[i].position , eksi(ints.point)  )  );
        else intsmesafe = yermessafe +500;
        if(intsmesafe>=yermessafe)
        {
            float isiklamesafekare = dot( topla(sahne.point_lights[i].position , eksi(yer) )   , topla(sahne.point_lights[i].position , eksi(yer) )   );
            float cosi=dot(topla(sahne.point_lights[i].position , eksi(yer) ) , normal ) / std::sqrt(dot( topla(sahne.point_lights[i].position , eksi(yer) )   , topla(sahne.point_lights[i].position , eksi(yer) )   )) / std::sqrt( dot( normal, normal) );
            parser::Vec3f diffuse = veclecarp(madde.diffuse, sabitlecarp( sahne.point_lights[i].intensity , cosi/isiklamesafekare ) );
            parser::Vec3f hvec=   sabitlecarp( topla( normalize(topla( sahne.point_lights[i].position , eksi(yer)  )) , normalize(topla( kamera.position , eksi(yer)  ) )) , 0.5);
            float cosalfa=  dot( hvec , normal) / sDistance(hvec) / sDistance(normal);
            parser::Vec3f specular = veclecarp( madde.specular ,  sabitlecarp( sahne.point_lights[i].intensity, std::pow(cosalfa, madde.phong_exponent)/isiklamesafekare)) ;
            toplam=topla(topla(specular,diffuse) , toplam);
        }

    }
    if(madde.is_mirror && derinlik<sahne.max_recursion_depth)
    {
        float costeta = dot(normal , normalize(topla(kamera.position, eksi(yer)) )) ;
        parser::Vec3f cameRay= topla( eksi(normalize(topla(kamera.position, eksi(yer)))) , sabitlecarp( normal, 2*costeta ));
        Intersection inter = FindNearestIntersect(cameRay,sahne,epsilonluyer);
        if(inter.isintersect)
        {
            parser::Camera cam;
            cam.position=epsilonluyer;
            toplam=topla(toplam, veclecarp( madde.mirror , renkbul(sahne.materials[inter.mat_id-1], inter.point, inter.normal, cam, sahne, derinlik+1 )));
        }


    }
    return toplam;
}


void jgez( int i, unsigned char* arr, parser::Scene scene, parser::Camera camera, int son)
{
    for(;i<son;i++)
    {
        for (int j = 0; j < camera.image_width; j++)
        {

            parser::Vec3f cameRay = CameRay(camera, j, i);
            Intersection inter = FindNearestIntersect(cameRay,scene,camera.position);
            parser::Vec3f renk;
            if(inter.isintersect)
            {
                renk= renkbul(scene.materials[inter.mat_id-1], inter.point, inter.normal, camera, scene, 0);
            }
            else
            {
                renk.x=scene.background_color.x;
                renk.y=scene.background_color.y;
                renk.z=scene.background_color.z;
            }
            arr[3*(i*camera.image_width + j) ] = std::min((float)(255.0),renk.x);
            arr[3*(i*camera.image_width + j) +1] = std::min((float)(255.0),renk.y);
            arr[3*(i*camera.image_width + j) +2] = std::min((float)(255.0),renk.z);



        }
    }

}

int main(int argc, char* argv[])
{

    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);
    for(int kacincikamera=0; kacincikamera<scene.cameras.size(); kacincikamera++)
    {

        parser::Camera camera = scene.cameras[kacincikamera];

        unsigned char *arr = new unsigned char [camera.image_height*camera.image_width*3];
        int t1i = camera.image_height/4;
        int t2i = t1i+camera.image_height/4;
        int t3i = t2i+camera.image_height/4;
        int t4i =t3i + camera.image_height/4;
        std::thread t1(jgez, 0, arr , scene, camera, t1i);
        std::thread t2(jgez, t1i, arr, scene, camera, t2i);
        std::thread t3(jgez, t2i, arr, scene, camera, t3i);
        std::thread t4(jgez, t3i, arr, scene, camera, camera.image_height);
        t1.join();
        t2.join();
        t3.join();
        t4.join();

            //jgez(i, arr, scene, camera);

        write_ppm(&camera.image_name[0], arr, camera.image_width, camera.image_height);
        delete arr;
    }
}
