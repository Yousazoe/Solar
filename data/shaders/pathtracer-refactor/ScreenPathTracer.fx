#include"../common/PostProcessVs.fx"

#ifdef PIXEL_SHADER
#include"defines.fx"
#include"Geometry.fx"
#include"Camera.fx"

cbuffer PTFrame___
{
	Mat4f _camera_to_world;
	Mat4f _project_matrix;
	uint2 _resolution;
	float2 _pt_seed;
	float3 _ws_camera_pos;
	float4 _tile_scale_offset;
}

float4 main(in float4 ndc_pos : SV_Position, in float2 uv : TEXTURE0) : SV_Target
{
	uint2 resolution = _resolution;

	Ray ray = pixel_to_ray(uv * resolution, resolution, _ws_camera_pos, _camera_to_world, _project_matrix);
	float4 result = pathtrace_loop(ray, uv, resolution);
	return result;
}

#endif