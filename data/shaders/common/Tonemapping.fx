static const float gamma = 2.2f;
static const float inv_gamma = 1.f / gamma;

// input : color in gamma space
inline float3 to_linear(in const float3 v)
{
	return float3(pow(v, gamma));
}

// input : color in linear space
inline float3 to_gamma(in const float3 v)
{
	return float3(pow(v, inv_gamma));
}

float luminance(in float3 color)
{
	return dot(float3(0.3f, 0.6f, 0.1f), color);
}


inline float3 tonemap_func(float3 x, float a, float b, float c, float d, float e, float f)
{
	return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

float3 tonemap_aces(float3 color)
{
	float a = 0.22f; //Shoulder Strength
	float b = 0.30f; //Linear Strength
	float c = 0.10f; //Linear Angle
	float d = 0.20f; //Toe Strength
	float e = 0.01f; //Toe Numerator
	float f = 0.30f; //Toe Denominator
	float linear_white = 11.2f; //Linear White Point Value (11.2)
														 //Note: E/F = Toe Angle

	return tonemap_func(color, a, b, c, d, e, f) / tonemap_func(float3(linear_white, linear_white, linear_white), a, b, c, d, e, f);
}


// Reinhard tonemap
float3 tonemap(float3 x)
{
	return x / (x + 1);
}

float3 inverse_tonemap(float3 x)
{
	return x / (1 - x);
}
