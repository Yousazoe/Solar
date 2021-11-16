float sqr(float x) { return x * x; }


float3 mon2lin(float3 x)
{
	return float3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

float3 mix(float3 x, float3 y, float a )
{
	return (x * (1 - (a)) + y * (a));
}

float mix(float x, float y, float a)
{
	return (x * (1 - (a)) + y * (a));
}
