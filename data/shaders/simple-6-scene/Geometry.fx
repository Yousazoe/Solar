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

#endif //GEOMETRY

