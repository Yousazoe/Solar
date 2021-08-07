set(TARGET Gui)

# Paths
set(src_path ${PROJECT_SOURCE_DIR}/src)
set(third_path ${PROJECT_SOURCE_DIR}/thirdparty)

# Files
set(
	gui
	${src_path}/gui/GuiRenderer.hpp
	${src_path}/gui/GuiRenderer.cpp
	${src_path}/gui/GuiSystem.hpp
	${src_path}/gui/GuiSystem.cpp
)


# Target
add_library(${TARGET} STATIC
	${gui}
)

# Dependency
target_include_directories(${TARGET} PUBLIC ${src_path})

# Filters
source_group("gui" FILES ${gui})

target_compile_options(${TARGET} PRIVATE "/MP")

