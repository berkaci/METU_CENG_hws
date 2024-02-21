#include <iostream>
#include <cmath>
#include "Helpers.h"

void DrawTrig(Vec3 ucgens[3], std::vector<std::vector<Color>> &colarr, std::vector<std::vector<double>> &deptharr, int hor, int ver, Color v0c, Color v1c, Color v2c){
 ucgens[0].x = (ucgens[0].x+1)/2*hor;
 ucgens[0].y = (ucgens[0].y+1)/2*ver;
 ucgens[0].z = (ucgens[0].z+1)/2;
 ucgens[1].x = (ucgens[1].x+1)/2*hor;
 ucgens[1].y = (ucgens[1].y+1)/2*ver;
 ucgens[1].z = (ucgens[1].z+1)/2;
 ucgens[2].x = (ucgens[2].x+1)/2*hor;
 ucgens[2].y = (ucgens[2].y+1)/2*ver;
 ucgens[2].z = (ucgens[2].z+1)/2;
 int ymin = ucgens[0].y;
 int xmin = ucgens[0].x;
 int ymax = ucgens[0].y;
 int xmax = ucgens[0].x;
 if(ucgens[1].y<ymin) {ymin = ucgens[1].y;}
 if(ucgens[2].y<ymin) {ymin = ucgens[2].y;}
 if(ucgens[1].x<xmin) {xmin = ucgens[1].x;}
 if(ucgens[2].x<xmin) {xmin = ucgens[2].x;}
 if(ucgens[1].y>ymax) {ymax = ucgens[1].y;}
 if(ucgens[2].y>ymax) {ymax = ucgens[2].y;}
 if(ucgens[1].x>xmax) {xmax = ucgens[1].x;}
 if(ucgens[2].x>xmax) {xmax = ucgens[2].x;}
 for(int y= ymin; y<=ymax; y++){
  for(int x=xmin; x<=xmax; x++){
   double alpha = (x*(ucgens[1].y-ucgens[2].y) + y*(ucgens[2].x-ucgens[1].x)+ ucgens[1].x*ucgens[2].y-ucgens[1].y*ucgens[2].x)/
    (ucgens[0].x*(ucgens[1].y-ucgens[2].y) + ucgens[0].y*(ucgens[2].x-ucgens[1].x)+ ucgens[1].x*ucgens[2].y-ucgens[1].y*ucgens[2].x);
   double beta = (x*(ucgens[2].y-ucgens[0].y) + y*(ucgens[0].x-ucgens[2].x)+ ucgens[2].x*ucgens[0].y-ucgens[2].y*ucgens[0].x)/
    (ucgens[1].x*(ucgens[2].y-ucgens[0].y) + ucgens[1].y*(ucgens[0].x-ucgens[2].x)+ ucgens[2].x*ucgens[0].y-ucgens[2].y*ucgens[0].x);
   double gama = (x*(ucgens[0].y-ucgens[1].y) + y*(ucgens[1].x-ucgens[0].x)+ ucgens[0].x*ucgens[1].y-ucgens[0].y*ucgens[1].x)/
    (ucgens[2].x*(ucgens[0].y-ucgens[1].y) + ucgens[2].y*(ucgens[1].x-ucgens[0].x)+ ucgens[0].x*ucgens[1].y-ucgens[0].y*ucgens[1].x);
   double depth = alpha*ucgens[0].z + beta*ucgens[1].z + gama*ucgens[2].z;
   if(alpha >= 0 && beta >=0 && gama >= 0 && depth < deptharr[x][y]){
    deptharr[x][y] = depth;
    colarr[x][y] = Color(v0c.r*alpha+v1c.r*beta+ v2c.r*gama,
    v0c.g*alpha+v1c.g*beta+ v2c.g*gama,
    v0c.b*alpha+v1c.b*beta+ v2c.b*gama);
    }
  }
 }


}


double f12(double x, double y, Vec3 v1, Vec3 v2){
return x*(v1.y-v2.y) + y*(v2.x-v1.x) + v1.x*v2.y - v2.x*v1.y;
}

double Dist(double ax, double ay,double bx,double by){ return ((bx-ax)*(bx-ax) + (by-ay)*(by-ay));}

void Rasterizer(Vec3 v0, Vec3 v1,std::vector<std::vector<Color> > &colarr,std::vector<std::vector<double> > &deptharr, int width, int height , Color v0c, Color v1c){
    if(v0.x>v1.x) {std::swap(v0,v1); std::swap(v0c,v1c);}


    v0.x=(v0.x+1)/2;
    v0.y=(v0.y+1)/2;
    v0.z=(v0.z+1)/2;
    v1.x=(v1.x+1)/2;
    v1.y=(v1.y+1)/2;
    v1.z=(v1.z+1)/2;

	int y = v0.y*height;
	int y1 = height*v1.y;
	int y0 = height*v0.y;
	int x0 = width*v0.x;
	int x1 = width *v1.x;
	if(v1.x==v0.x){//dimdik berk gibi
		if(y1<y0){
			for(int k = y0; k<y1; k--){
                double depth=((double)(y0-k))/(y0-y1)*v1.z + (1-((double)(y0-k))/(y0-y1))*v0.z;
                if(depth<deptharr[x0][k])
                {
                    deptharr[x0][k]=depth;
                    double weightold = 1-((double)(y0-k))/(y0-y1);
                    colarr[x0][k] = Color(v0c.r* weightold + v1c.r * (1-weightold),
                    v0c.g* weightold + v1c.g * (1-weightold),
                    v0c.b* weightold + v1c.b * (1-weightold));
                    //std::cout << colarr[height-1-x0][k]<< std::endl;
                }

			}

		}
		else{
			for(int k = y0; k<y1; k++){
                double depth=(((double)(k-y0))/(y1-y0))*v1.z + (1-((double)(k-y0))/(y1-y0))*v0.z;
			    if(depth<deptharr[x0][k])
			    {
                    deptharr[x0][k]=depth;
                    double weightold = 1-((double)(k-y0))/(y1-y0);
                    colarr[x0][k] = Color(v0c.r* weightold + v1c.r * (1-weightold),
                    v0c.g* weightold + v1c.g * (1-weightold),
                    v0c.b* weightold + v1c.b * (1-weightold));
			    }

			}
		}
        return;
	}
	double c = (v1.y-v0.y)/(v1.x-v0.x); //slope
	int d = 2*(y0-y1) + (x1-x0);
	double slope = c;
	//double dist = ((v1.x-v0.x)*(v1.x-v0.x) + (v1.y-v0.y)*(v1.y-v0.y) + (v1.z-v0.z)*(v1.z-v0.z));
	if(slope < 0) d = -2*(y0-y1) + (x1-x0);
	int counter =0;
	if(slope <1 && slope > -1){
		for (int i=x0; i<x1; i++){
			//i = x0;
			double weightold =1- ((double)(i-x0))/(x1-x0);
			double depth=weightold*v0.z+(1-weightold)*v1.z;
			//double weightnew = (Dist(v0.x,v0.y,v1.x,v1.y)-Dist(v0.x,v0.y,newx,newy))/Dist(v0.x,v0.y,v1.x,v1.y);
			if(depth<deptharr[i][y])
            {
                deptharr[i][y]=depth;
                colarr[i][y] = Color(v0c.r* weightold + v1c.r * (1-weightold),
                v0c.g* weightold + v1c.g * (1-weightold),
                v0c.b* weightold + v1c.b * (1-weightold));
                //std::cout << colarr[height-1-i][y]<< std::endl;
            }
			if(slope>=0){
				if(d<0){
					y++;
					d+= 2*(y0-y1+x1-x0);
				}
				else {
					d+=2*(y0-y1);
				}
			}

			else{

				if(d<0){
					y--;
					d+= 2*(y1-y0+x1-x0);
				}
				else {
					d+=2*(y1-y0);
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////


	else
    {
        y = x0;
        if(slope>=0)
        {
            d = 2*(x0-x1) + (y1-y0);

            for (int i=y0; i<y1 && y<colarr.size(); i++)
            {
                double weightold =1-((double)(i-y0))/(y1-y0);
                double depth=weightold*v0.z+(1-weightold)*v1.z;
                if(depth<deptharr[y][i])
                {
                    deptharr[y][i]=depth;

                    //double weightnew = (Dist(v0.x,v0.y,v1.x,v1.y)-Dist(v0.x,v0.y,newx,newy))/Dist(v0.x,v0.y,v1.x,v1.y);
                    colarr[y][i] = Color(v0c.r* weightold + v1c.r * (1-weightold),
                    v0c.g* weightold + v1c.g * (1-weightold),
                    v0c.b* weightold + v1c.b * (1-weightold));
                                //std::cout << colarr[height-1-i][y]<< std::endl;
                }

                if(d<0){
                    y++;
                    d+= -2*(y0-y1+x1-x0);
                }
                else {
                    d+=2*(x0-x1);
                }


            }
        }
        else
        {
            d = 2*(x0-x1) - (y1-y0);

            for (int i=y0; i>y1; i--)
            {
                double weightold =((double)(i-y1))/(y0-y1);
                double depth=weightold*v0.z+(1-weightold)*v1.z;
                if(depth<deptharr[y][i])
                {
                    deptharr[y][i]=depth;

                    //double weightnew = (Dist(v0.x,v0.y,v1.x,v1.y)-Dist(v0.x,v0.y,newx,newy))/Dist(v0.x,v0.y,v1.x,v1.y);
                    colarr[y][i] = Color(v0c.r* weightold + v1c.r * (1-weightold),
                    v0c.g* weightold + v1c.g * (1-weightold),
                    v0c.b* weightold + v1c.b * (1-weightold));
                                //std::cout << colarr[height-1-i][y]<< std::endl;
                }

                if(d<0){
                    y++;
                    d+= 2*(y0-y1+x0-x1);
                }
                else {
                    d+=2*(x0-x1);
                }


            }
        }

	}

}




bool Visible(double den, double num, double & te, double & tl){
	double t = num/den;
	if(den > 0){
	if(t>tl) return false;
	if(t>te) te = t;
	}

	else if(den < 0){
		t = num/den;
		if(t<te) return false;
		if (t<tl) tl = t;
	}

	else{
		if(num > 0) return false;
	}

	return true;
}


Vec3 Normal (Vec3 *ucgenkoseleri)
{

	return crossProductVec3(subtractVec3(ucgenkoseleri[2], ucgenkoseleri[1]), subtractVec3(ucgenkoseleri[0], ucgenkoseleri[1]));
}

std::vector<Vec3> LiangBarsky(Vec3 v0, Vec3 v1)
{
    Vec3 v00=v0, v10=v1;
    double mesafe= magnitudeOfVec3(subtractVec3(v0,v1));
    std::vector<Vec3> retvec(4);
	double dx = v1.x-v0.x;
	double dy = v1.y-v0.y;
	double dz = v1.z-v0.z;

	double te =0;
	double tl =1;
	bool visible = false;
	if(Visible(dx,-1-v0.x,te,tl)){
		if(Visible(-dx,v0.x-1,te,tl)){
			if(Visible(dy,-1 - v0.y,te,tl)){
				if(Visible(-dy,v0.y-1,te,tl)){
					if(Visible(dz,-1 - v0.z,te,tl)){
						if(Visible(-dz,v0.z-1,te,tl)){
                            visible = true;
                            if(tl <1) { v1.x = v0.x + dx * tl; v1.y = v0.y + dy * tl; v1.z = v0.z + dz * tl;  }
                            if(te >0) { v0.x = v0.x + dx * te; v0.y = v0.y + dy * te; v0.z = v0.z + dz * te;  }
                            double v0icinv0inkatsayisi= magnitudeOfVec3(subtractVec3(v10,v0))/mesafe;
                            double v1icinv0inkatsayisi= magnitudeOfVec3(subtractVec3(v10,v1))/mesafe;

                            retvec[0] = Vec3(v0.x,v0.y,v0.z, v0.colorId);
                            retvec[1] = Vec3(v1.x,v1.y,v1.z, v1.colorId);
                            retvec[2]=v00;
                            retvec[3]=v10;
                            return retvec;
                        }
					}
				}
			}
		}
	}

    retvec[0] = Vec3(v0.x,v0.y,v0.z,-1);
    retvec[1] = Vec3(v1.x,v1.y,v1.z,-1);
	return retvec;

}

/*
 * Calculate cross product of vec3 a, vec3 b and return resulting vec3.
 */
Vec3 crossProductVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y);
}

/*
 * Calculate dot product of vec3 a, vec3 b and return resulting value.
 */
double dotProductVec3(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 * Find length (|v|) of vec3 v.
 */
double magnitudeOfVec3(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
 * Normalize the vec3 to make it unit vec3.
 */
Vec3 normalizeVec3(Vec3 v)
{
    double d = magnitudeOfVec3(v);
    return Vec3(v.x / d, v.y / d, v.z / d);
}

/*
 * Return -v (inverse of vec3 v)
 */
Vec3 inverseVec3(Vec3 v)
{
    return Vec3(-v.x, -v.y, -v.z);
}

/*
 * Add vec3 a to vec3 b and return resulting vec3 (a+b).
 */
Vec3 addVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/*
 * Subtract vec3 b from vec3 a and return resulting vec3 (a-b).
 */
Vec3 subtractVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/*
 * Multiply each element of vec3 with scalar.
 */
Vec3 multiplyVec3WithScalar(Vec3 v, double c)
{
    return Vec3(v.x * c, v.y * c, v.z * c);
}

/*
 * Prints elements in a vec3. Can be used for debugging purposes.
 */
void printVec3(Vec3 v)
{
    std::cout << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
}

/*
 * Check whether vec3 a and vec3 b are equal.
 * In case of equality, returns 1.
 * Otherwise, returns 0.
 */
int areEqualVec3(Vec3 a, Vec3 b)
{

    /* if x difference, y difference and z difference is smaller than threshold, then they are equal */
    if ((ABS((a.x - b.x)) < EPSILON) && (ABS((a.y - b.y)) < EPSILON) && (ABS((a.z - b.z)) < EPSILON))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Returns an identity matrix (values on the diagonal are 1, others are 0).
 */
Matrix4 getIdentityMatrix()
{
    Matrix4 result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                result.values[i][j] = 1.0;
            }
            else
            {
                result.values[i][j] = 0.0;
            }
        }
    }

    return result;
}

/*
 * Multiply matrices m1 (Matrix4) and m2 (Matrix4) and return the result matrix r (Matrix4).
 */
Matrix4 multiplyMatrixWithMatrix(Matrix4 m1, Matrix4 m2)
{
    Matrix4 result;
    double total;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            total = 0;
            for (int k = 0; k < 4; k++)
            {
                total += m1.values[i][k] * m2.values[k][j];
            }

            result.values[i][j] = total;
        }
    }

    return result;
}

/*
 * Multiply matrix m (Matrix4) with vector v (vec4) and store the result in vector r (vec4).
 */
Vec4 multiplyMatrixWithVec4(Matrix4 m, Vec4 v)
{
    double values[4];
    double total;

    for (int i = 0; i < 4; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += m.values[i][j] * v.getNthComponent(j);
        }
        values[i] = total;
    }

    return Vec4(values[0], values[1], values[2], values[3], v.colorId);
}
