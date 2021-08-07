set(CMAKE_CXX_STANDARD 17)
# Paths
set(src_path ${PROJECT_SOURCE_DIR}/src)
set(third_path ${PROJECT_SOURCE_DIR}/thirdparty)

# Files
file(GLOB container "${src_path}/container/*.hpp" "${src_path}/container/*.cpp")
file(GLOB memory "${src_path}/memory/*.hpp" "${src_path}/memory/*.cpp")

file(GLOB core "${src_path}/core/*.hpp" "${src_path}/core/*.cpp")

set(math
	${src_path}/math/Math3d.hpp
	${src_path}/math/Math3d.cpp
	${src_path}/math/MathUtil.hpp
	${src_path}/math/Plane.hpp
	${src_path}/math/BoundingBox.hpp
)


file(GLOB gfx_ "${src_path}/gfx/*.hpp" "${src_path}/gfx/*.cpp")
file(GLOB gfx_entry "${src_path}/gfx/entry/*.hpp" "${src_path}/gfx/entry/*.cpp")
file(GLOB gfx_middle "${src_path}/gfx/middle/*.hpp" "${src_path}/gfx/middle/*.cpp")
file(GLOB gfx_d3d11 "${src_path}/gfx/d3d11/*.hpp" "${src_path}/gfx/d3d11/*.cpp")
file(GLOB dds_loader "${third_path}/dds-loader/*.hpp" "${third_path}/dds-loader/*.cpp")

set(gfx 
	${gfx_}
	${gfx_entry}
	${gfx_middle}
	${gfx_d3d11}
	${dds_loader}
)

set(tinyxml
	${third_path}/tinyxml/tinystr.h
	${third_path}/tinyxml/tinystr.cpp
	${third_path}/tinyxml/tinyxml.h
	${third_path}/tinyxml/tinyxml.cpp
	${third_path}/tinyxml/tinyxmlerror.cpp
	${third_path}/tinyxml/tinyxmlparser.cpp
)

set(xml
	${src_path}/xml/Xml.hpp
)

set(fs
	${src_path}/fs/Path.hpp
	${src_path}/fs/IFile.hpp
	${src_path}/fs/IFileDevice.hpp
	${src_path}/fs/DiskFile.hpp
	${src_path}/fs/DiskFileDevice.hpp
	${src_path}/fs/FileSystem.hpp
)

set(ecs
	${src_path}/ecs/Transform.hpp

)

file(GLOB render "${src_path}/render/*.hpp" "${src_path}/render/*.cpp")


# Target
add_library(Core STATIC
	${core}
	${math}
	${tinyxml}
	${xml}
	${fs}
	${ecs}
	${gfx}
	${memory}
	${render}
)

target_compile_options(Core PRIVATE "/MP")
	
# Dependency
target_include_directories(Core PUBLIC ${src_path})


# Filters
source_group("core" FILES ${core})
source_group("math" FILES ${math})
source_group("fs" FILES ${fs})
source_group("xml" FILES ${xml})
source_group("memory" FILES ${memory})
source_group("xml\\tinyxml" FILES ${tinyxml})
source_group("ecs" FILES ${ecs})
source_group("gfx" FILES ${gfx_})
source_group("gfx\\entry" FILES ${gfx_entry})
source_group("gfx\\d3d11" FILES ${gfx_d3d11})
source_group("gfx\\middle" FILES ${gfx_middle})
source_group("gfx\\texture" FILES ${dds_loader})
source_group("render" FILES ${render})

