cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
 add_subdirectory(core)
 add_subdirectory(hw)
add_subdirectory(imgui)
add_subdirectory(gui)
 add_subdirectory(app)

set(projects 
Core 
Hardware 
Gui
Imgui
)

set_target_properties(
	${projects}

    PROPERTIES

		CXX_STANDARD 17

    
)


