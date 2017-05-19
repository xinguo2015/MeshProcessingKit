################################################################
#
# 所有的以＃开始的行都是注释
# 
#  1. CMakeLists.txt 内容不区分大小写
#
################################################################
# 工程的名字，不是必须的，最好有
project (MeshProcessingKit) 

# 要求cmake的版本 > 2.6
cmake_minimum_required (VERSION 2.6) 

# 自动查找OpenGL库，以后用${OPENGL_LIBRARY}引用OpenGL
find_package (OpenGL REQUIRED) 
# 将OPENGL的头文件目录(变量 GLUT_INCLUDE_DIR中）添加到编译器的查找路径 
include_directories (${OPENGL_INCLUDE_DIR}) 

# 自动查找GLUT库，以后用${OPENGL_LIBRARY}引用OpenGL
find_package (GLUT REQUIRED)
# 将GLUT的头文件目录(变量 GLUT_INCLUDE_DIR中）添加到编译器的查找路径 
include_directories (${GLUT_INCLUDE_DIR})

# 编译器的参数标志
# -g 生成调试信息 
# -Wall 输出所有的警告（Warning All）
# -O 优化程度: 0=无优化，3=最优化
#set (CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -Wno-deprecated -std=c++14") 
set (CMAKE_CXX_FLAGS "-Wall -Wno-deprecated -std=c++14") 
# Debug版本的参数标志
set (CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG") 
# Release版本的参数标志
set (CMAKE_CXX_FLAGS_RELEASE "-O3") 
# 设置缺省的版本为Release
#set (CMAKE_BUILD_TYPE Release) 
# 设置缺省的版本为Debug
set (CMAKE_BUILD_TYPE Debug)
 
# 搜索src目录下的源程序文件
# 文件名都保存于变量 DIR_SRCS
aux_source_directory (src mpksrc)
aux_source_directory (imgui guisrc)

# 添加源代码文件生成可执行程序
add_executable (mpkit ${mpksrc} ${guisrc} )
target_link_libraries (mpkit ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

