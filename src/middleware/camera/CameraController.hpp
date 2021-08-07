#pragma once
#include<render/Camera.hpp>
#include<ecs/Transform.hpp>
#include<hw/Mouse.hpp>
#include<hw/Event.hpp>

namespace tutorial::render
{
	class CameraController final
	{
	public:
		CameraController(
			graphics::Camera& camera,
			ecs::Transform& camera_transform,
			float default_speed
		);

	public:
		void handle_input_events(const hw::Event &event);
		void update(float deltaSeconds);
		
	public:
		bool _lookat_mode = false;
		float _lookat_dist = 1.0f;

	private:
		graphics::Camera& _camera;
		ecs::Transform& _camera_transform;
		float _default_speed;
		hw::Vec2i _last_cursor;
		bool _moving = false;
		float _move_speed;
		math::Vec3f _move_dir;
	};


	inline CameraController::CameraController(
		graphics::Camera& camera,
		ecs::Transform& camera_transform,
		float default_speed
	) :
		_camera{ camera },
		_camera_transform{ camera_transform },
		_default_speed{ default_speed },
		_move_speed{ default_speed },
		_move_dir{ 0,0,0 }
	{
		_last_cursor = hw::Mouse::getPosition();
	}

	inline void CameraController::handle_input_events(const hw::Event &event)
	{
		auto &p = _camera_transform.position;
		auto &rot = _camera_transform.rotation;
		//float deltams = 0.016f;

		_moving = false;

		if (event.type == hw::Event::EventType::KeyPressed)
		{
			if (hw::Keyboard::Key::W == event.key.code) {
				_move_dir = rot.get_forward_vec();
				_moving = true;
			}
			else if (hw::Keyboard::Key::S == event.key.code) {
				_move_dir = -rot.get_forward_vec();
				_moving = true;
			}
			else if (hw::Keyboard::Key::A == event.key.code) {
				_move_dir = -rot.get_right_vec();
				_moving = true;
			}
			else if (hw::Keyboard::Key::D == event.key.code) {
				_move_dir = rot.get_right_vec();
				_moving = true;
			}
		}

		if (_moving) {
			_move_speed += _default_speed;
		}

		if (event.type == hw::Event::EventType::KeyReleased)
		{
			if (hw::Keyboard::Key::W == event.key.code ||
				hw::Keyboard::Key::S == event.key.code ||
				hw::Keyboard::Key::A == event.key.code ||
				hw::Keyboard::Key::D == event.key.code)
			{
				_move_speed = 0;
			}
		}

		if (event.type == hw::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == hw::Mouse::VerticalWheel ||
				event.mouseWheelScroll.wheel == hw::Mouse::HorizontalWheel
			)
			{
				if (_lookat_mode)
				{
					_lookat_dist += event.mouseWheelScroll.delta * 0.3f;
				}
				else
				{
					_move_dir = rot.get_forward_vec();
					auto amt = event.mouseWheelScroll.delta * 1.f;
					_camera_transform.set_pos(_camera_transform.position + _move_dir * amt);
				}
			}
		}



		auto new_cursor = hw::Mouse::getPosition();
		if (hw::Mouse::isButtonPressed(hw::Mouse::Right)) {
			float dx = (0.25f * static_cast<float>(new_cursor.x - _last_cursor.x));
			float dy = (0.25f * static_cast<float>(new_cursor.y - _last_cursor.y));
			dx = math::Deg2Rad(dx);
			dy = math::Deg2Rad(dy);
			auto angle = rot.to_eular() + math::Vec3f(0, dx, 0) + math::Vec3f(dy, 0, 0);
			_camera_transform.set_rot(math::Quat{ math::Rad2Deg(angle.x), math::Rad2Deg(angle.y), math::Rad2Deg(angle.z) });
		}
		_last_cursor.x = new_cursor.x;
		_last_cursor.y = new_cursor.y;
	}

	inline void CameraController::update(float ms)
	{
		if (_moving) {
			auto amt = ms * _move_speed;
			_camera_transform.set_pos(_camera_transform.position + _move_dir * amt);
		}

		if (_lookat_mode) {
			auto&& ws_cam_pos = _camera_transform.rotation.rotate(math::Vec3f(0, 0, _lookat_dist));
			_camera_transform.set_pos(ws_cam_pos);
		}
	}
}
