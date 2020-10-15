#include "Color.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b) : Color(r, g, b, 1) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r / 255.f;
	this->g = g / 255.f;
	this->b = b / 255.f;
	this->a = a / 255.f;
}