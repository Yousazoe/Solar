set(TARGET App)

# Paths
set(src_path ${PROJECT_SOURCE_DIR}/src)
set(app_path ${src_path}/../app)
set(third_path ${PROJECT_SOURCE_DIR}/thirdparty)

set(raytracing_path ${app_path}/raytracing)

# Files


file(GLOB app "${raytracing_path}/*.hpp" "${raytracing_path}/*.cpp"  "${app_path}/demo.hpp" "${app_path}/main.cpp")
file(GLOB gi "${app_path}/gi/*.hpp" "${app_path}/gi/*.cpp")


add_executable(${TARGET} 
	${app}
	${gi}
)

if(MSVC)
  set_target_properties(
		${TARGET}
		PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${g_bin_path}"
  )
  add_definitions(/MP)
endif()

# Dependency
target_include_directories(${TARGET} PUBLIC ${src_path})
target_link_directories(
	${TARGET}
	PUBLIC
	${g_bin_path}
)

target_link_libraries(${TARGET}  
PUBLIC 
d3d11
d3dcompiler
dxguid
Core 
Hardware 
Gui
Imgui
)


# Filters
set_property(TARGET ${TARGET} PROPERTY FOLDER "app")
source_group("app" FILES ${app})
source_group("gi" FILES ${gi})





