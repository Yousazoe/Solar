#ifndef PBRBRDF
#define PBRBRDF

float wang_hash(uint seed)
{
	seed = (seed ^ 61) ^ (seed >> 16);
	seed *= 9;
	seed = seed ^ (seed >> 4);
	seed *= 0x27d4eb2d;
	seed = seed ^ (seed >> 15);
	return seed * (1.0 / 4294967296.0);
}

struct Random
{
	float2 seed;
	float2 random_vec;
};

float rand(inout Random random)
{
	float g = random.random_vec.x * random.random_vec.y;
	random.seed -= float2(g, g);
	return frac(sin(dot(random.seed, float2(12.9898, 78.233))) * 43758.5453);
}


//float rand(inout Random random)
//{
//	uint seed = random.seed.x;
//	random.seed.x += 10;
//	return wang_hash(seed);
//}

float2 rand2(inout Random random)
{
	return float2(
		rand(random),
		rand(random)
		);
}



#endif
