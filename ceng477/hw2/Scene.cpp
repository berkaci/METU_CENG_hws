#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include <iostream>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "Scene.h"

using namespace tinyxml2;
using namespace std;

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true;
		}
		else
		{
			this->cullingEnabled = false;
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		this->vertices.push_back(vertex);
		this->colorsOfVertices.push_back(color);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		Translation *translation = new Translation();

		translationElement->QueryIntAttribute("id", &translation->translationId);

		str = translationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		Scaling *scaling = new Scaling();

		scalingElement->QueryIntAttribute("id", &scaling->scalingId);
		str = scalingElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		Rotation *rotation = new Rotation();

		rotationElement->QueryIntAttribute("id", &rotation->rotationId);
		str = rotationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = meshElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = WIREFRAME_MESH;
		}
		else
		{
			mesh->type = SOLID_MESH;
		}

		// read mesh transformations
		XMLElement *meshTransformationsElement = meshElement->FirstChildElement("Transformations");
		XMLElement *meshTransformationElement = meshTransformationsElement->FirstChildElement("Transformation");

		while (meshTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = meshTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			meshTransformationElement = meshTransformationElement->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *cloneStr;
		int v1, v2, v3;
		XMLElement *meshFacesElement = meshElement->FirstChildElement("Faces");
		str = meshFacesElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}
}

void Scene::assignColorToPixel(int i, int j, Color c)
{
	this->image[i][j].r = c.r;
	this->image[i][j].g = c.g;
	this->image[i][j].b = c.b;
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;
			vector<double> rowOfDepths;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
				rowOfDepths.push_back(1.01);
			}

			this->image.push_back(rowOfColors);
			this->depth.push_back(rowOfDepths);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				assignColorToPixel(i, j, this->backgroundColor);
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFilename.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFilename << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
*/
void Scene::convertPPMToPNG(string ppmFileName)
{
	string command;

	// TODO: Change implementation if necessary.
	command = "convert " + ppmFileName + " " + ppmFileName + ".png";
	system(command.c_str());
}



Vec3 translationyap(Vec3 nokta, double tx, double ty, double tz)
{
    nokta.x+=tx;
    nokta.y+=ty;
    nokta.z+=tz;
    return nokta;
}

Vec3 scaleyap(Vec3 nokta, double tx, double ty, double tz)
{
    nokta.x*=tx;
    nokta.y*=ty;
    nokta.z*=tz;
    return nokta;
}

Vec3 rotationyap(Vec3 nokta, double aci, double ux, double uy, double uz)
{
    Vec3 uvw[3];
    uvw[0]=Vec3(ux,uy,uz);
    if(ux<=uy && ux<=uz)
    {
        uvw[1]=Vec3(0,-uz,uy);
    }
    if(uy<=ux && uy<=uz)
    {
        uvw[1]=Vec3(-uz,0,ux);
    }
    if(uz<=uy && uz<=ux)
    {
        uvw[1]=Vec3(uy,-ux,0);
    }

    uvw[2]=crossProductVec3(uvw[0],uvw[1]);

    Matrix4 m, mt, rx, sonmatrix;
    rx.values[0][0]=rx.values[3][3]=1;
    rx.values[1][1]=rx.values[2][2]= std::cos(aci);
    rx.values[1][2]= -std::sin(aci);
    rx.values[2][1]=  std::sin(aci);

    m.values[3][3]=1;
    mt.values[3][3]=1;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            m.values[i][j]=uvw[i].getNthComponent(j);
            mt.values[i][j]=uvw[j].getNthComponent(i);
        }
    }

    sonmatrix= multiplyMatrixWithMatrix( multiplyMatrixWithMatrix(mt,rx), m);

    int colid=nokta.colorId;
    nokta.colorId=1;
    Vec3 son=multiplyMatrixWithVec4(sonmatrix, nokta);
    son.colorId=colid;
    return son;
}





Matrix4 translationmatrix(double tx, double ty, double tz)
{
    Matrix4 m;
    m.values[0][0]=1;
    m.values[1][1]=1;
    m.values[2][2]=1;
    m.values[3][3]=1;
    m.values[0][3]=tx;
    m.values[1][3]=ty;
    m.values[2][3]=tz;

    return m;

}

Matrix4 scalematrix( double tx, double ty, double tz)
{
    Matrix4 m;
    m.values[0][0]=tx;
    m.values[1][1]=ty;
    m.values[2][2]=tz;
    m.values[3][3]=1;
    return m;


}

Matrix4 rotationmatrix( double aci, double ux, double uy, double uz)
{
    aci=(aci*M_PI)/180;
    Vec3 uvw[3];
    uvw[0]=normalizeVec3(Vec3(ux,uy,uz));
    double aux=std::abs(ux), auy=std::abs(uy), auz=std::abs(uz);
    if(aux<=auy && aux<=auz)
    {
        uvw[1]=Vec3(0,-uz,uy);
    }
    else if(auy<=aux && auy<=auz)
    {
        uvw[1]=Vec3(-uz,0,ux);
    }
    else if(auz<=auy && auz<=aux)
    {
        uvw[1]=Vec3(uy,-ux,0);
    }
    uvw[1]=normalizeVec3(uvw[1]);
    uvw[2]=normalizeVec3(crossProductVec3(uvw[0],uvw[1]));

    Matrix4 m, mt, rx, sonmatrix;
    rx.values[0][0]=rx.values[3][3]=1;
    rx.values[1][1]=rx.values[2][2]= std::cos(aci);
    rx.values[1][2]= -std::sin(aci);
    rx.values[2][1]=  std::sin(aci);

    m.values[3][3]=1;
    mt.values[3][3]=1;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            m.values[i][j]=uvw[i].getNthComponent(j);
            mt.values[i][j]=uvw[j].getNthComponent(i);
        }
    }

    sonmatrix= multiplyMatrixWithMatrix( multiplyMatrixWithMatrix(mt,rx), m);
    return sonmatrix;


}


Matrix4 meshicintranslationmatrixibul(Mesh girenmesh, Scene sahne)
{
    Matrix4 m=getIdentityMatrix(), tm;
    for(int i=0; i<girenmesh.numberOfTransformations; i++)
    {
        if(girenmesh.transformationTypes[i]=='t')
        {
            Translation trans=*(sahne.translations[girenmesh.transformationIds[i]-1]);
            tm=translationmatrix(trans.tx, trans.ty, trans.tz);
            m=multiplyMatrixWithMatrix(tm,m);
        }
        else if(girenmesh.transformationTypes[i]=='s')
        {
            Scaling trans=*(sahne.scalings[girenmesh.transformationIds[i]-1]);
            tm=scalematrix(trans.sx, trans.sy, trans.sz);
            m=multiplyMatrixWithMatrix(tm,m);
        }
        else if(girenmesh.transformationTypes[i]=='r')
        {
            Rotation trans=*(sahne.rotations[girenmesh.transformationIds[i]-1]);
            tm=rotationmatrix(trans.angle, trans.ux, trans.uy, trans.uz);
            m=multiplyMatrixWithMatrix(tm,m);
        }
    }
    return m;
}

Matrix4 cameratransformationmatrix(Camera kamera)
{
    Matrix4 rx;
    Vec3 u=kamera.u, v=kamera.v, w=kamera.w, e=kamera.position;
    double arr[4][4] = {
        { u.x, u.y, u.z, -u.x*e.x - u.y*e.y - u.z*e.z },
        { v.x, v.y, v.z, -v.x*e.x - v.y*e.y - v.z*e.z },
        { w.x, w.y, w.z, -w.x*e.x - w.y*e.y - w.z*e.z },
        { 0, 0, 0, 1 }
    };

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            rx.values[i][j]=arr[i][j];
        }
    }
    return rx;

}

Matrix4 viewporttransformationmatrix(Camera kamera)
{
    Matrix4 rx, p2o;
    rx.values[0][0]= 2/(kamera.right-kamera.left);
    rx.values[1][1]= 2/(kamera.top-kamera.bottom);
    rx.values[2][2]= -2/(kamera.far-kamera.near);
    rx.values[3][3]= 1;
    rx.values[0][3]= -(kamera.right+kamera.left)/(kamera.right-kamera.left);
    rx.values[1][3]= -(kamera.top+kamera.bottom)/(kamera.top-kamera.bottom);
    rx.values[2][3]= -(kamera.far+kamera.near)/(kamera.far-kamera.near);

    if( kamera.projectionType==0) //ortho
    {
        return rx;
    }
    else
    {
        p2o.values[0][0]= kamera.near;
        p2o.values[1][1]= kamera.near;
        p2o.values[2][2]= kamera.far+kamera.near;
        p2o.values[2][3]= kamera.far*kamera.near;
        p2o.values[3][2]= -1;

        return multiplyMatrixWithMatrix(rx,p2o);

    }
}





/*
	Transformations, clipping, culling, rasterization are done here.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
    Matrix4 vt;
    vt=viewporttransformationmatrix(*camera);
    vt=multiplyMatrixWithMatrix(vt, cameratransformationmatrix(*camera));

	for(int i=0; i<meshes.size(); i++)
    {
        Matrix4 m=multiplyMatrixWithMatrix(vt, meshicintranslationmatrixibul(*(meshes[i]), *this));
        //m= meshicintranslationmatrixibul(*(meshes[i]), *this);
        for(int j=0; j<meshes[i]->triangles.size(); j++)
        {
            Vec3 ucgenkoseleri[3];
            ucgenkoseleri[0]= multiplyMatrixWithVec4(m,  *(vertices[meshes[i]->triangles[j].vertexIds[0]-1]));
            ucgenkoseleri[1]= multiplyMatrixWithVec4(m,  *(vertices[meshes[i]->triangles[j].vertexIds[1]-1]));
            ucgenkoseleri[2]= multiplyMatrixWithVec4(m,  *(vertices[meshes[i]->triangles[j].vertexIds[2]-1]));
            if( cullingEnabled && Normal(ucgenkoseleri).z < 0 ) continue;
            if(meshes[i]->type==0)
            {
                for(int k=0; k<3; k++)
                {
                    std::vector<Vec3> birkenar;
                    birkenar=LiangBarsky(ucgenkoseleri[k], ucgenkoseleri[(k+1)%3]);
                    if(birkenar[0].colorId==-1) continue;
                    //birkenar.push_back(ucgenkoseleri[k]);
                    //birkenar.push_back(ucgenkoseleri[(k+1)%3]);
                    Color v0e =  *colorsOfVertices[birkenar[0].colorId-1], v1e = *colorsOfVertices[birkenar[1].colorId-1] ;
                    double mesafe = magnitudeOfVec3(subtractVec3(birkenar[2], birkenar[3]));
                    double v0icinv0inkatsayisi= magnitudeOfVec3(subtractVec3(birkenar[3],birkenar[0]))/mesafe;
                    double v1icinv0inkatsayisi= magnitudeOfVec3(subtractVec3(birkenar[3],birkenar[1]))/mesafe;

                    Color v0c( v0e.r*v0icinv0inkatsayisi + v1e.r*(1-v0icinv0inkatsayisi) ,  v0e.g*v0icinv0inkatsayisi + v1e.g*(1-v0icinv0inkatsayisi) , v0e.b*v0icinv0inkatsayisi + v1e.b*(1-v0icinv0inkatsayisi)  );
                    Color v1c( v0e.r*v1icinv0inkatsayisi + v1e.r*(1-v1icinv0inkatsayisi) ,  v0e.g*v1icinv0inkatsayisi + v1e.g*(1-v1icinv0inkatsayisi) , v0e.b*v1icinv0inkatsayisi + v1e.b*(1-v1icinv0inkatsayisi)  );

                    Rasterizer(birkenar[0], birkenar[1], image, depth, camera->horRes, camera->verRes, v0c, v1c  );
                }
            }
            else{

                DrawTrig(ucgenkoseleri, image, depth,camera->horRes, camera->verRes
             , *colorsOfVertices[ucgenkoseleri[0].colorId -1], *colorsOfVertices[ucgenkoseleri[1].colorId -1], *colorsOfVertices[ucgenkoseleri[2].colorId -1]);
            }
        }
    }
}










































