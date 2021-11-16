// RaytracingTutorial
// Author : JackSqurrel
#ifndef GFX_COORDINATE
#define GFX_COORDINATE
#include"Defines.fx"

// coordinate system

//static const float INFINITY = 1e9;
//static const float EPS = 0.01f;
//static const float PI = 3.14159265358979323f;
//static const float TWO_PI = 6.28318530717958648f;
//static const float INV_PI = 1.f / PI;

// @spherical_to_catalan
// phi : [0, 2PI]
// theta : [0, PI]
inline float3 spherical_to_catalan(float phi, float theta)
{
	float sinTheta = sin(theta);
	phi -= PI;   // reverse of catalan_to_spherical
	return float3(sinTheta * cos(phi), cos(theta), sinTheta * sin(phi));
}

// @catalan_to_spherical
// v.xyz : normaliz(v)
// return float2(phi, theta) 
// phi : [0, 2PI]
// theta : [0, PI]
inline float2 catalan_to_spherical(in const float3 v)
{
	float phi = atan2(v.z, v.x) + PI;
	float theta = acos(v.y);
	return float2(phi, theta);
}



// @get_tangent_space 
// return 3x3 tbn
// [tangent]
// [normal]
// [bitangent]
float3x3 get_tangent_space(in const float3 n)
{
	float3 UpVector = abs(n.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 TangentX = normalize(cross(UpVector, n));
	float3 TangentY = cross(n, TangentX);
	return float3x3(TangentX, n, TangentY);
}



// @tangent_to_world
// normalize vector from tangent to world
float3 tangent_to_world(
	in const float3 v, 
	in const float3x3 tbn
)
{
	// use [3x3]tbn mul [3x1]v
	return normalize(v.x * tbn[0] + v.y * tbn[1] + v.z * tbn[2]);
}

// @world_to_tangent
float3 world_to_tangent(in const float3 v, in const float3x3 tbn)
{
	// inverse(tbn) = transpose(tbn)
	// use [1x3]v mul [3x3]inv_tbn
	return normalize(mul(v, tbn));
}
#endif
