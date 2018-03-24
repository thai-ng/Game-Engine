#pragma once
#include "Ray.h"
#include "Vec3.h"

class Camera
{
public:
	Camera() : 
		origin(Vec3F(0, 0, 0)), 
		lowerLeft(Vec3F(-2, -1, 1)),
		horizontal(Vec3F(4,0,0)),
		vertical(Vec3F(0, 2, 0))
	{
	}

	Ray GetRay(float u, float v)
	{
		return Ray(origin, lowerLeft + u * horizontal + v * vertical);
	}

private:
	Vec3F origin;
	Vec3F lowerLeft;
	Vec3F horizontal;
	Vec3F vertical;
};