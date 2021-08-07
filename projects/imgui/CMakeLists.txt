cmake_minimum_required(VERSION 3.14)
set(TARGET Imgui)
project(${TARGET})

# Paths
set(src_path ${PROJECT_SOURCE_DIR}/../../src)
set(third_path ${PROJECT_SOURCE_DIR}/../../thirdparty)

set(src
	${third_path}/imgui/imconfig.h
	${third_path}/imgui/imgui.h
    ${third_path}/imgui/imgui.cpp
	${third_path}/imgui/imgui_internal.h
    ${third_path}/imgui/imstb_rectpack.h
    ${third_path}/imgui/imgui_widgets.cpp
    ${third_path}/imgui/imstb_truetype.h
    ${third_path}/imgui/imstb_textedit.h
    ${third_path}/imgui/imgui_demo.cpp
    ${third_path}/imgui/imgui_draw.cpp
)

add_library(${TARGET} STATIC  
${src}
)
target_include_directories(${TARGET} PUBLIC ${src_path})

set_property(TARGET ${TARGET} PROPERTY FOLDER "Thirdparty")

