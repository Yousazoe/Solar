// RaytracingTutorial
// Author : JackSqurrel
#include"PostProcessVs.fx"


#ifdef PIXEL_SHADER

#include"defines.fx"
#include"Geometry.fx"

float4 main(in float4 ndc_pos : SV_Position, in float2 uv : TEXTURE0) : SV_Target
{
	return float4(1, 0, 0, 1);
}

#endif