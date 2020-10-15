#pragma once

class Color
{
private:
	float r;
	float g;
	float b;
	float a;

public:
	Color(unsigned char r, unsigned char g, unsigned char b);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};
