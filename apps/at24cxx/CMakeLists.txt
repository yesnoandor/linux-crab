# 设置app名字
set(APP_NAME "at24cxx_diags")

# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (${APP_NAME})

# 设置顶层目录
set(TOP_DIR ${PROJECT_SOURCE_DIR}/../..)

# 设置代码分布路径
set(CODE_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(INC_SOURCE_DIR ${TOP_DIR}/inc)
set(LIB_SOURCE_DIR ${TOP_DIR}/libs)

# ---------- 中间部分:指定工程依赖 ----------
# set c++11 编译选项 : -std=c++11 包含C++11特性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#添加 C/C++ 包含路径
include_directories(${INC_SOURCE_DIR})

# 设定链接库的位置
link_directories("${LIB_SOURCE_DIR}/so/${PLATFORM}")
link_directories("${LIB_SOURCE_DIR}/a/${PLATFORM}")

## 链接线程库
#link_libraries(libpthread.so)

# 查找目录下的所有源文件,并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)


# 指定生成目标
add_executable(${APP_NAME} ${DIR_SRCS})

# 添加链接库
target_link_libraries(${APP_NAME} libmxml.a)
target_link_libraries(${APP_NAME} libnode.a)
target_link_libraries(${APP_NAME} libpthread.so)


# ---------- 附加部分:输出调试信息 ----------
message(STATUS "App Name:  ${APP_NAME}")
message(STATUS "TOP DIR:  ${TOP_DIR}")

message(STATUS "src code path : ${CODE_SOURCE_DIR}")
message(STATUS "src inc path : ${INC_SOURCE_DIR}")
message(STATUS "src lib path : ${LIB_SOURCE_DIR}")


