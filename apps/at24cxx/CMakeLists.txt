# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (at24cxx_diags)

# 设置顶层目录
set(TOP_DIR ${PROJECT_SOURCE_DIR}/../..)

# 设置代码分布路径
set(CODE_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(INC_SOURCE_DIR ${TOP_DIR}/inc)
set(LIB_SOURCE_DIR ${TOP_DIR}/libs)


#添加 C/C++ 包含路径
include_directories(${INC_SOURCE_DIR})

# 查找目录下的所有源文件,并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)


# 指定生成目标
add_executable(at24cxx_diags ${DIR_SRCS})

# ---------- 附加部分:输出调试信息 ----------
message(STATUS "TOP DIR:  ${TOP_DIR}")

message(STATUS "src code path : ${CODE_SOURCE_DIR}")
message(STATUS "src inc path : ${INC_SOURCE_DIR}")
message(STATUS "src lib path : ${LIB_SOURCE_DIR}")


