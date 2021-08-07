#pragma once
#include<math/Math3d.hpp>

namespace tutorial::graphics
{
	class Camera final
	{
	public:
		Camera(
			float fovy = math::PI / 4.f,
			float aspect_ratio = 1000.f / 600.f,
			float znear = 0.1f,
			float zfar = 1000.f
		);

	public:
		void set_fov(float fov /*radians*/);
		float get_fov(/*radians*/) const { return _fovy; }
		void set_aspect_ratio(float ratio);
		float get_aspect_ratio() const { return _aspect_ratio; }
		void set_znear(float z);
		float get_znear() const { return _znear; }
		void set_zfar(float z);
		float get_zfar() const { return _zfar; }

		const math::Vec4f* getFarPlanePoints() const { return _farPlanePoints; }
	private:
		void update_projection_matrix();

	public: 
		float _fovy;  // full angle
		float _aspect_ratio;
		float _znear;
		float _zfar;
		math::Vec4f _farPlanePoints[4];
	};

}
