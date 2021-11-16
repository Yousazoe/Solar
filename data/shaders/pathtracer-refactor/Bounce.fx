

struct Bounce
{
	uint d;
	float3 throughput;
	Ray ray;
	float pdf;
	float3 surface_position;
};