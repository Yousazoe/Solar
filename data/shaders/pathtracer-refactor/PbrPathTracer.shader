#define PT_CPU_BUFFER 1


#include"Samplers.fx"
#include"Tonemapping.fx"
#include"defines.fx"
#include"Coordinate.fx"
#include"Geometry.fx"
#include"Random.fx"

#include"Entity.fx"
#include"Light.fx"

#include"Material.fx"

#include"Scene.fx"
#include"TraceResult.fx"
#include"SceneTrace.fx"
#include"Bounce.fx"

#include"Brdf.fx"
#include"BxdfSample.fx"

#include"LightSample.fx"
#include"PbrPathTracer.fx"

#define pathtrace_loop pathtracer_demo
#include"ScreenPathTracer.fx"

