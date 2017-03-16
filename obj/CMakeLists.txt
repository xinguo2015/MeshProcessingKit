################################################################
#
# 所有的以＃开始的行都是注释
# 
#  1. CMakeLists.txt 内容不区分大小写
#
################################################################
# 工程的名字，不是必须的，最好有
project (obj) 
cmake_minimum_required (VERSION 2.6) 
# 编译器的参数标志
# -g 生成调试信息 
# -Wall 输出所有的警告（Warning All）
# -O 优化程度: 0=无优化，3=最优化
#set (CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -Wno-deprecated -std=c++14") 
set (CMAKE_CXX_FLAGS "-Wall -Wno-deprecated -std=c++14") 
# Debug版本的参数标志
set (CMAKE_CXX_FLAGS_DEBUG "-g") 
# Release版本的参数标志
set (CMAKE_CXX_FLAGS_RELEASE "-O3") 
# 设置缺省的版本为Release
#set (CMAKE_BUILD_TYPE Release) 
# 设置缺省的版本为Debug
set (CMAKE_BUILD_TYPE Debug)

# 项目目标
add_executable (obj .c)
target_link_libraries (obj m)

