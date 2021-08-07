#ifdef VERTEX_SHADER

static const float4 g_pos[] =
{
	float4(-1, -1, 0, 1),
	float4(-1, 1, 0, 1),
	float4(1, -1, 0, 1),
	float4(1, 1, 0, 1),
};
static const float2 g_texcoord[] =
{
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0),
};

void main(uint vertexID : SV_VertexID,
	out float4 ndc_pos : SV_Position,
	out float2 texcoord : TEXTURE0
)
{
	ndc_pos = g_pos[vertexID];
	texcoord = g_texcoord[vertexID];
}

#endif