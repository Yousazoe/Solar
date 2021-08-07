#include<render/Camera.hpp>

namespace tutorial::graphics
{
	math::Vec3f yAxis(0, 1.f, 0);

	Camera::Camera(float fovy, float aspectRatio, float zNear, float zFar) :
		_fovy(fovy),
		_aspect_ratio(aspectRatio),
		_znear(zNear),
		_zfar(zFar)
	{
		update_projection_matrix();
	}

	void Camera::set_fov(float fov)
	{
		_fovy = fov;
		update_projection_matrix();
	}

	void Camera::set_aspect_ratio(float ratio)
	{
		_aspect_ratio = ratio;
		update_projection_matrix();
	}

	void Camera::set_znear(float z)
	{
		_znear = z;
		update_projection_matrix();

	}

	void Camera::set_zfar(float z)
	{
		_zfar = z;
		update_projection_matrix();
	}

	//const Frustum* Camera::getFrust() const
	//{
	//	return &_frust;
	//}

	/*void Camera::getPlaneCorner(float d, Vec4f *points) const
	{
		const float halfHeight = d * tanf(0.5f*_fovy);
		const float halfWidth = _aspect_ratio * halfHeight;

		points[0] = Vec4f(-halfWidth, -halfHeight, d, 0);
		points[1] = Vec4f(-halfWidth, +halfHeight, d, 0);
		points[2] = Vec4f(+halfWidth, -halfHeight, d, 0);
		points[3] = Vec4f(+halfWidth, +halfHeight, d, 0);
	}*/
	
	void Camera::update_projection_matrix()
	{
		float half_height = _zfar * std::tanf(0.5f * _fovy);
		float half_width = _aspect_ratio * half_height;

		_farPlanePoints[0] = math::Vec4f(-half_width, -half_height, _zfar, 0);
		_farPlanePoints[1] = math::Vec4f(-half_width, +half_height, _zfar, 0);
		_farPlanePoints[2] = math::Vec4f(+half_width, -half_height, _zfar, 0);
		_farPlanePoints[3] = math::Vec4f(+half_width, +half_height, _zfar, 0);
	}
}
