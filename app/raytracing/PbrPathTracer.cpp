// Desc :
// Disney principle material, pathtracing scene
// single ball only 
#include"../Demo.hpp"
#include"Pathtracer.hpp"
using namespace tutorial;
using namespace graphics;
using namespace ptgi;

auto cornel_box_scene() -> PTGIScene&
{
	static PTGIScene scene;
	static bool init = false;
	if (init) { return scene; }
	init = true;

	// cornel box
	scene.add_light(MakeQuadLight(math::Vec3f(-1.f, 2.f, -1.f), math::Vec3f(1, 1, 1) * 2, math::Vec3f(2, 0, 0), math::Vec3f(0, 0, 2)));

	scene.add_material(Material{ math::Vec3f(1, 1, 1)});
	scene.add_material(Material{ math::Vec3f(1, 0, 0)});
	scene.add_material(Material{ math::Vec3f(0, 1, 0)});
	scene.add_material(Material{ math::Vec3f(1, 1, 1)});
	int base = 0, red = 1, green = 2, white = 3;
	scene.add_ball(SphereMesh(math::Vec3f(0, -1.5, 0), 1, base));
	scene.add_quad(QuadMesh(math::Vec3f(-2.5f, 2.5f, 2.5f), math::Vec3f(5, 0, 0), math::Vec3f(0, -5, 0), white)); // front
	scene.add_quad(QuadMesh(math::Vec3f(-2.5f, 2.5f, -2.5f), math::Vec3f(0, 0, 5), math::Vec3f(0, -5, 0), red)); // left
	scene.add_quad(QuadMesh(math::Vec3f(2.5f, 2.5f, 2.5f), math::Vec3f(0, 0, -5), math::Vec3f(0, -5, 0), green));// right
	scene.add_quad(QuadMesh(math::Vec3f(-2.5f, -2.5f, 2.5f), math::Vec3f(5, 0, 0), math::Vec3f(0, 0, -5), white));// down
	scene.add_quad(QuadMesh(math::Vec3f(-2.5f, 2.5f, -2.5f), math::Vec3f(5, 0, 0), math::Vec3f(0, 0, 5), white));// up
	scene.add_quad(QuadMesh(math::Vec3f(2.5f, 2.5f, -2.5f), math::Vec3f(-5, 0, 0), math::Vec3f(0, -5, 0), white));// back
	scene.build();

	return scene;
}

Demo(___PbrPathTracer)
{
	static Pathtracer pathtracer;
	PTGIScene& scene = cornel_box_scene();
	Shader::_has_shader_cachae = true;
	
	
	RenderCamera camera{ app.camera, app.camera_trans };

	Viewport vp{ camera, app.swapchain.backbuffer() };
	pathtracer.render(vp, scene);
	if (app.event && (hw::Mouse::isButtonPressed(hw::Mouse::Right) || 
		app.event.value().type == hw::Event::MouseWheelScrolled))
	{
		pathtracer.frame_index = 0;
	}

	ImGui::Begin("path tracer");

	ImGui::Text(std::to_string(pathtracer.frame_index).c_str());
	if (ImGui::Button("reset")) { pathtracer.frame_index = 0; }

	
	ImGui::End();
}
