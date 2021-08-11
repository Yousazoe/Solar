static const int QuadLight = 0;
static const int SphereLight = 1;

struct Light
{
	float3 position;
	float radius;
	float3 energy;
	float type; // 0 : quad, 1 : sphere
	float3 u;
	float area;
	float3 v;
};

Light ___QuadLight(
	in float3 position, 
	in float3 energy,
	in float3 u,
	in float3 v
) 
{
	Light light = (Light)0;
	light.position = position;
	light.energy = energy;
	light.u = u;
	light.v = v;
	light.type = QuadLight;
	return light;
}




