#include <stdio.h>
#include <stdlib.h>

struct vec3
{
	float x, y, z;
};
typedef struct vec3 vec3;

struct Triangle
{
	vec3 a;
	vec3 b;
	vec3 c;
};
typedef struct Triangle Triangle;

struct Plane
{
	vec3 normal;
	float distance;
};
typedef struct Plane Plane;

float dot(vec3 a, vec3 b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

vec3 interpolate(vec3 a, vec3 b, float t)
{
	return (vec3)
	{
		a.x + t * (b.x - a.x),
		a.y + t * (b.y - a.y),
		a.z + t * (b.z - a.z)
	};
}

void splitTriangle(Triangle triangle, Plane plane, Triangle** out)
{
	//this honestly might be the worst code ive ever written.
	//ive never felt more shame than when i pushed this

	//in my defense, my BSP was bugging for 2 whole days and i wanted to cry so i stopped caring
	//i will never touch this ever again and i will never ask what any of this does 

    float da = dot(triangle.a, plane.normal) - plane.distance;
    float db = dot(triangle.b, plane.normal) - plane.distance;
    float dc = dot(triangle.c, plane.normal) - plane.distance;

    int ina = da >= 0, inb = db >= 0, inc = dc >= 0;
    int totalIn = ina + inb + inc;

    if (totalIn == 0 || totalIn == 3) return; // No intersection

	vec3 intersectionAB;
	vec3 intersectionBC;
	vec3 intersectionAC;
    int intersectionCount = 0;
	int intersectionEdge[2];
	int intersectionEdgeCount = 0;

    if (ina != inb) {
        float t = -da / (db - da);
        intersectionAB = interpolate(triangle.a, triangle.b, t);
    }
    if (ina != inc) {
		float t = -da / (dc - da);
		intersectionAC = interpolate(triangle.a, triangle.c, t);
	}
	if (inb != inc && intersectionCount < 2) {
		float t = -db / (dc - db);
		intersectionBC = interpolate(triangle.b, triangle.c, t);
	}

	//cursed boolean magic with unreadable variable names
	if ((ina != inb && ina != inc) && (inb == inc)) 
	{
		//a is isolated by plane

		(*out)[0] = (Triangle) { triangle.a, intersectionAB, intersectionAC };
		(*out)[1] = (Triangle) { intersectionAB, triangle.b, triangle.c };
		(*out)[2] = (Triangle) { intersectionAC, intersectionAB, triangle.c };
	}
	else if ((inb != ina && inb != inc) && (ina == inc))
	{
		//b is isolated by plane

		(*out)[0] = (Triangle) { triangle.b, intersectionBC, intersectionAB };
		(*out)[1] = (Triangle) { intersectionBC, triangle.c, triangle.a };
		(*out)[2] = (Triangle) { intersectionAB, intersectionBC, triangle.a };
	}
	else if ((inc != ina && inc != inb) && (ina == inb))
	{	
		//c is isolated by plane 

		(*out)[0] = (Triangle) { triangle.c, intersectionAC, intersectionBC };
		(*out)[1] = (Triangle) { intersectionAC, triangle.a, triangle.b };
		(*out)[2] = (Triangle) { intersectionBC, intersectionAC, triangle.b };
	}
}

int main(int argc, char** argv)
{
	Triangle triangleUnsplit = 
	{
		(vec3) {-0.5f, -0.5f, 0},
		(vec3) {0.5f, -0.5f, 0},
		(vec3) {0.0f, 0.5f, 0}
	};
	Plane splittingPlane = 
	{
		(vec3) {0, 1, 0},
		0.0f
	};
	
	Triangle* splitTriangles;
	splitTriangles = (Triangle*)malloc(sizeof(Triangle) * 3);
	splitTriangle(triangleUnsplit, splittingPlane, &splitTriangles);
	
	for (int i = 0; i < 3; i++)
	{
		Triangle t = splitTriangles[i];
		printf("Split triangle %d vertices: "
			   "\n%f %f %f"
			   "\n%f %f %f"
			   "\n%f %f %f\n", i, t.a.x, t.a.y,t.a.z,t.b.x,t.b.y,t.b.z,t.c.x,t.c.y,t.c.z);
	}
}
