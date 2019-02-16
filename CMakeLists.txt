##############################################################################################
# cmake -DCMAKE_INSTALL_PREFIX=/usr  --> default /usr/local
# 
# project(projectname [CXX] [C] [Java]) : 
# set(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])
#
# aux_source_directory(<dir> <variable>) : 查找指定目录下的所有源文件
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL]) : 为构建添加一个子路径
#
# file :
# file(MAKE_DIRECTORY [directory1 directory2 ...]) : 创建指定的目录
##############################################################################################


# ---------- 通用部分:宏定义 ----------
# 枚举指定目录下所有的子目录
macro(SUBDIRLIST result curdir)
file(GLOB children RELATIVE ${curdir} ${curdir}/*)
set(dirlist "")
foreach(child ${children})
if(IS_DIRECTORY ${curdir}/${child})
list(APPEND dirlist ${child})
endif()
endforeach()
set(${result} ${dirlist})
endmacro()


# 设置变量
# ubuntu renesas
#set(PLATFORM   ubuntu)
set(PLATFORM   renesas)

# ---------- 开头部分:指定输出路径 ----------
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Ratchet)

# 设置输出路径
set(PROJECT_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)

# 创建输出目录
file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR}) 
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

# 设置目标输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

# 设置
set(CODE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INC_SOURCE_DIR ${PROJECT_SOURCE_DIR}/inc)
set(LIB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs)
set(TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/test)
set(DEMO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/demo)


# 添加编译lib的子目录
# ADD_SUBDIRECTORY(debug)

# ---------- 中间部分:指定工程依赖 ----------
#find_package(OpenGL)
#find_package(OpenCV)

# set c++11 编译选项 : -std=c++11 包含C++11特性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 
#添加 C/C++ 包含路径
include_directories(${INC_SOURCE_DIR})

# ---------- 结尾部分:指定输出文件 ----------
# 设置枚举所有的子目录
# SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR})
# SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/src)

# 枚举每个子目录的cpp文件
#file(GLOB cpp_source  *.cpp)
#foreach(subdir ${SUBDIRS})
#file(GLOB cpp_source  ${cpp_source} ${subdir}/*.cpp)
#endforeach(subdir ${}) 

# 递归枚举所有c文件
file(GLOB_RECURSE CLISTS ${CODE_SOURCE_DIR}/*.c)
# 递归枚举所有cpp文件
file(GLOB_RECURSE CPPLISTS ${CODE_SOURCE_DIR}/*.cpp ${TEST_SOURCE_DIR}/*.cpp)

# 设定链接库的位置
link_directories("${LIB_SOURCE_DIR}/so/${PLATFORM}")
link_directories("${LIB_SOURCE_DIR}/a/${PLATFORM}")

link_libraries(libpthread.so)

# 指定生成可执行文件
add_executable(Ratchet ${CPPLISTS} ${CLISTS})
# 添加链接库
#target_link_libraries(Ratchet libflexdebug.so)
target_link_libraries(Ratchet libmxml.a)
target_link_libraries(Ratchet libevent.a)
target_link_libraries(Ratchet libzmq.a)

# 目标文件的安装
#install(TARGETS demo RUNTIME DESTINATION bin)

# 普通文件的安装
#INSTALL(FILES COPYRIGHT README DESTINATION share/doc/cmake/demo)
#INSTALL(DIRECTORY doc/ DESTINATION share/doc/cmake/demo/doc)
#INSTALL(PROGRAMS scripts/run_demo.sh DESTINATION bin)

# ---------- 附加部分:输出调试信息 ----------
message(STATUS "project name:  ${PROJECT_NAME}")
message(STATUS "system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
#message("----- binary path:  ${demo_BINARY_DIR}")
#message("----- source path:  ${demo_SOURCE_DIR}")
message(STATUS "binary path : ${PROJECT_BINARY_DIR}")
message(STATUS "source path : ${PROJECT_SOURCE_DIR}")
message(STATUS "output path : ${PROJECT_OUTPUT_DIR}")
message(STATUS "inc path : ${PROJECT_INCLUDE_DIR}")

message(STATUS "lib output path : ${LIBRARY_OUTPUT_PATH}")
message(STATUS "exe output path : ${EXECUTABLE_OUTPUT_PATH}")


message(STATUS "cmake runtime output path : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "cmake library output path : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "cmake active output path : ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

message(STATUS "src code path : ${CODE_SOURCE_DIR}")
message(STATUS "src inc path : ${INC_SOURCE_DIR}")
message(STATUS "src lib path : ${LIB_SOURCE_DIR}")
message(STATUS "src test path : ${TEST_SOURCE_DIR}")
message(STATUS "src demo path : ${DEMO_SOURCE_DIR}")


# 
message(STATUS "CFLAGS :  ${CMAKE_C_FLAGS}")
message(STATUS "CXXFLAGS :  ${CMAKE_CXX_FLAGS}")

#message(STATUS "cpp source list : ${cpp_source}")
#foreach(sources ${cpp_source})
#MESSAGE(STATUS "the ${PROJECT_NAME} contain ${sources}")     
#endforeach(sources ${})

#message(STATUS "SUBDIRS : ${SUBDIRS}")
#foreach(subdir ${SUBDIRS})
#message(STATUS "the subdir contain ${subdir}")
#endforeach(subdir ${}) 

# 打印所有的C源代码
message(STATUS "CLISTS : ${CLISTS}")
foreach(c_file ${CLISTS})
message(STATUS "the CLISTS contain ${c_file}")
endforeach(c_file ${})

# 打印所有的CPP源代码
message(STATUS "CPPLISTS : ${CPPLISTS}")
foreach(cpp_file ${CPPLISTS})
message(STATUS "the CPPLISTS contain ${cpp_file}")
endforeach(cpp_file ${})




