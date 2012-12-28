#pragma once
#include <math.h>

typedef struct { double x; double y; } vec2;

static vec2 vec2New()
{
	vec2 v;
	v.x = 0.; v.y = 0.;
	return v;
}

static vec2 vec2New(double x, double y)
{
	vec2 v;
	v.x = x; v.y = y;
	return v;
}

static double vec2LengthSqr(const vec2& v)
{
	return (v.x*v.x + v.y*v.y);
}

static double vec2Length(const vec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

static vec2 vec2Normalize(const vec2& v, double* out_len = 0)
{
	double sr;
	vec2 rv;
	sr = sqrt(v.x*v.x + v.y*v.y);
	rv.x = v.x/sr;
	rv.y = v.y/sr;
	if (out_len) *out_len = sr;
	return rv;
}
