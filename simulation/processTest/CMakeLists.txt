# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (randomRun)

# set c++11 编译选项 : -std=c++11 包含C++11特性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(randomRun ${DIR_SRCS})
