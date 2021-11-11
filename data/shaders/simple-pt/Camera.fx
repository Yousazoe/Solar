// RaytracingTutorial
// Author : JackSqurrel



Ray pixel_to_ray(
	in const float2 pixel, // [0...width), [0...height)
	in const uint2 resolution,
	in const float3 ws_camera_pos, 
	in const Mat4f camera_to_world, 
	in const Mat4f project_matrix
)
{
	float x = pixel.x;
	float y = pixel.y;
	float z = 1.f;
	float tan_fov = tan(60.f * PI / 180.f * 0.5f);
	float ratio = (float)resolution.x / (float)resolution.y;
	x = (x / resolution.x * 2 - 1) * tan_fov * ratio;
	y = (-(y / resolution.y * 2 - 1)) * tan_fov;
	float3 mouse3D = float3(x, y, z);
	float3 ray_dir = normalize(mul(mouse3D, (float3x3)camera_to_world));

	Ray ray = { ws_camera_pos, normalize(ray_dir) };
	return ray;
}