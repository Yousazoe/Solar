
#include"../common/PostProcessVs.fx"

#ifdef PIXEL_SHADER
#include"../common/Samplers.fx"
Texture2D _main_tex;

float4 main(
	in float4 ndc_pos : SV_Position,
	in float2 texcoord : TEXTURE0
) : SV_Target
{
	return _main_tex.Sample(_samp_point, texcoord);
}

#endif
