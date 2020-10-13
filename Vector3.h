#pragma once

struct Vector3
{
	float x, y, z;

	Vector3(float x, float y, float z);
	Vector3 operator*(float value);
};