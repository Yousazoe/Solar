
# Paths
set(src_path ${PROJECT_SOURCE_DIR}/src)

# Files

file(GLOB hw_hpp "${src_path}/hw/*.hpp")
file(GLOB hw_cpp "${src_path}/hw/*.cpp")

file(GLOB common_hpp "${src_path}/hw/Common/*.hpp")
file(GLOB common_cpp "${src_path}/hw/Common/*.cpp")

file(GLOB common_win32_hpp "${src_path}/hw/Common/Win32/*.hpp")
file(GLOB common_win32_cpp "${src_path}/hw/Common/Win32/*.cpp")

file(GLOB win32_hpp "${src_path}/hw/Win32/*.hpp")
file(GLOB win32_cpp "${src_path}/hw/Win32/*.cpp")

# Target
add_library(Hardware STATIC
	${hw_hpp}
	${hw_cpp}

	${common_hpp}
	${common_cpp}

	${common_win32_hpp}
	${common_win32_cpp}

	${win32_hpp}
	${win32_cpp}
)
target_compile_options(Hardware PRIVATE "/MP")

# Dependency
target_include_directories(Hardware PUBLIC ${src_path})


# Filters
source_group("hw" FILES ${hw_hpp} ${hw_cpp} )
source_group("common" FILES ${common_hpp} ${common_cpp} )
source_group("common\\win32" FILES ${common_win32_hpp} ${common_win32_cpp} )
source_group("win32" FILES ${win32_hpp} ${win32_cpp})

