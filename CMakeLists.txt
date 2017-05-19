project (MeshKit) 
cmake_minimum_required (VERSION 2.6) 

# platform
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	MESSAGE(STATUS "current platform: Linux ")  
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	MESSAGE(STATUS "current platform: Windows")  
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
	MESSAGE(STATUS "current platform: FreeBSD")  
ELSE ()  
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

# OpenGL, glut, glew
find_package (OpenGL REQUIRED) 
find_package (GLUT REQUIRED)
find_package (GLEW REQUIRED)
include_directories (${OPENGL_INCLUDE_DIR}) 
include_directories (${GLUT_INCLUDE_DIR})
include_directories (${GLEW_INCLUDE_DIR})
#find_package(PkgConfig)
#pkg_search_module(glew glew)

# cxx flags
set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated -std=c++14") 
# Release
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
SET(CMAKE_C_FLAGS_RELEASE   "-O3 -Wall")
# Debug
SET(CMAKE_CXX_FLAGS_DEBUG   "-O0 -Wall -g -D_DEBUG")
SET(CMAKE_C_FLAGS_DEBUG     "-O0 -Wall -g -D_DEBUG")
# 设置缺省的版本为Release
SET(CMAKE_BUILD_TYPE Release) 
# 设置缺省的版本为Debug
SET(CMAKE_BUILD_TYPE Debug) 

# 搜索src目录下的源程序文件
# 文件名都保存于变量 DIR_SRCS
aux_source_directory (src mpksrc)
aux_source_directory (glutgui guisrc)


# 添加源代码文件生成可执行程序
add_executable (meshkit ${mpksrc} ${guisrc} )
target_link_libraries (meshkit ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

