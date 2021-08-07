#pragma once
#include<math/Math3d.hpp>

namespace tutorial::ecs
{
	class Transform final
	{
	public:
		void set_pos(const math::Vec3f &pos);
		void set_rot(const math::Quat &rot);
		void set_scale(const math::Vec3f &scale);
		void set_transform(const math::M4f &trans);

	private:
		void construct_transform_matrix();

	public:
		math::Vec3f scale{ 1.f,1.f,1.f };
		math::Quat rotation{ 0,0,0 };
		math::Vec3f position{ 0,0,0 };
		math::M4f world_matrix;
	};

	inline void Transform::set_pos(const math::Vec3f &pos)
	{
		position = pos;
		construct_transform_matrix();
	}

	inline void Transform::set_rot(const math::Quat &rot)
	{
		rotation = rot;
		construct_transform_matrix();
	}

	inline void Transform::set_scale(const math::Vec3f &s)
	{
		scale = s;
		construct_transform_matrix();
	}

	inline void Transform::set_transform(const math::M4f &trans)
	{
		trans.decompose(position, rotation, scale);
		world_matrix = trans;
	}

	inline void Transform::construct_transform_matrix()
	{
		math::M4f t, r, s;
		t.set_translation(position.x, position.y, position.z);
		r = math::M4f(rotation);
		s.initScale(scale.x, scale.y, scale.z);
		world_matrix = math::M4f::fastMult(s, r);
		world_matrix = math::M4f::fastMult(world_matrix, t);
	}
}
