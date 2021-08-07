
#include"../common/PostProcessVs.fx"

#ifdef PIXEL_SHADER
static const float gamma = 2.2f;
static const float exposure = 1.0f;
static const float pureWhite = 1.0f;
SamplerState _samp_linear : register(s1);
Texture2D _main_tex;


float4 main(
	in float4 ndc_pos : SV_Position,
	in float2 texcoord : TEXTURE0
) : SV_Target
{
	float3 color = _main_tex.Sample(_samp_linear, texcoord).rgb * exposure;

	// Reinhard tonemapping operator.
	// see: "Photographic Tone Reproduction for Digital Images", eq. 4
	//float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
	//float mappedLuminance = (luminance * (1.0 + luminance/(pureWhite*pureWhite))) / (1.0 + luminance);

	// Scale color by ratio of average luminances.
	//float3 mappedColor = (mappedLuminance / luminance) * color;

	// Gamma correction.
	return float4(pow(color, 1.0f / gamma), 1.0f);
}

#endif
