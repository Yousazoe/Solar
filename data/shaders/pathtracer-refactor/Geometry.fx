// RaytracingTutorial
// Author : JackSqurrel
#ifndef GEOMETRY
#define GEOMETRY
//static const float INFINITY = 1e9;
//static const float EPS = 0.01f;
//static const float PI = 3.14159265358979323f;
//static const float TWO_PI = 6.28318530717958648f;
struct Box { float3 min; float3 max; };
struct Ray { float3 origin; float3 direction; };
struct Sphere { float3 position; float radius; };

// return hit distance, or INFINITY if not intersect
float ray_sphere_intersect(float3 pos, float radius, in Ray r)
{
	float3 op = pos - r.origin;
	float eps = 0.001;
	float b = dot(op, r.direction);
	float det = b * b - dot(op, op) + radius * radius;
	[branch]
	if (det < 0.0)
		return INFINITY;

	det = sqrt(det);
	float t1 = b - det;
	[branch]
	if (t1 > eps)
		return t1;

	float t2 = b + det;
	[branch]
	if (t2 > eps)
		return t2;

	return INFINITY;
}

// return hit distance, or INFINITY if not intersect
float ray_rect_intersect(in float3 pos, in float3 u, in float3 v, in float3 n, in Ray r)
{
	float dt = dot(r.direction, n);
	float w = dot(n, pos);
	float t = (w - dot(n, r.origin)) / dt;
	[branch]
	if (t > EPS)
	{
		float3 p = r.origin + r.direction * t;
		float3 op = p - pos;

		// op project onto u,v, and its length <= |u| and |v|
		// 0 <= dot(nomalize(u), op) <= length(u)
		// --->  0 <= dot(normalize(u)/length(u), op) <= 1
		// --->  0 <= dot(u/dot(u,u), op) <= 1
		u = u / dot(u, u);
		v = v / dot(v, v);
		float a1 = dot(u, op);
		[branch]
		if (a1 >= 0 && a1 <= 1)
		{
			float a2 = dot(v, op);
			[branch]
			if (a2 >= 0 && a2 <= 1)
				return t;
		}
	}

	return INFINITY;
}


bool ray_box_intersect(in const Ray r, in const Box b)
{
	float3 invdir = 1.0f / r.direction;

	float3 f = (b.max - r.origin) * invdir;
	float3 n = (b.min - r.origin) * invdir;

	float3 tmax = max(f, n);
	float3 tmin = min(f, n);

	float t1 = min(tmax.x, min(tmax.y, tmax.z));
	float t0 = max(tmin.x, max(tmin.y, tmin.z));

	// ignore box that behind origin
	return t1 >= t0 && max(t0, t1) >= 0;
}

float ray_box_intersect_distance(in const Ray r, in const Box b)
{
	float3 invdir = 1.0 / r.direction;

	float3 f = (b.max - r.origin) * invdir;
	float3 n = (b.min - r.origin) * invdir;

	float3 tmax = max(f, n);
	float3 tmin = min(f, n);

	float t1 = min(tmax.x, min(tmax.y, tmax.z));
	float t0 = max(tmin.x, max(tmin.y, tmin.z));

	float d = (t1 >= t0) ? (t0 > 0.f ? t0 : t1) : -INFINITY;
	// ignore box that behind origin
	return d < 0 ? -INFINITY : d;
}


inline float3 bary_centric_coord(in float3 p, in float3 v0, in float3 v1, in float3 v2)
{
	float3 ab = v1 - v0;
	float3 ac = v2 - v0;
	float3 ah = p - v0;

	float ab_ab = dot(ab, ab);
	float ab_ac = dot(ab, ac);
	float ac_ac = dot(ac, ac);
	float ab_ah = dot(ab, ah);
	float ac_ah = dot(ac, ah);

	float inv_denom = 1.0 / (ab_ab * ac_ac - ab_ac * ab_ac);

	float v = (ac_ac * ab_ah - ab_ac * ac_ah) * inv_denom;
	float w = (ab_ab * ac_ah - ab_ac * ab_ah) * inv_denom;
	float u = 1.0 - v - w;

	return float3(u, v, w);
}


inline float3 uniform_sample_sphere(float2 u)
{
	float y = 1.0f - 2.0f * u.x;
	float r = sqrt(max(0.f, 1.0f - y * y));
	float phi = 2.0f * PI * u.y;
	float x = r * cos(phi);
	float z = r * sin(phi);

	return float3(x, y, z);
}

inline float3 uniform_sample_hemisphere(float2 u)
{
	float sinTheta = sqrt(1 - u.x * u.x);
	float phi = 2 * PI * u.y;
	float x = sinTheta * cos(phi);
	float z = sinTheta * sin(phi);

	return normalize(float3(x, u.x, z));
}

#endif //GEOMETRY

