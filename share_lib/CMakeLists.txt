cmake_minimum_required (VERSION 3.5)

project(main)

set (CMAKE_CXX_STANDARD 20) 
set (CMAKE_CXX_EXTENSIONS ON)
set (CMAKE_CXX_STANDARD_REQUIRED ON) 

set (CMAKE_C_STANDARD 11) 
set (CMAKE_C_EXTENSIONS ON) 
set (CMAKE_C_STANDARD_REQUIRED ON) 

message(STATUS, "---------------: ${PROJECT_SOURCE_DIR}")
#link_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/build)

ADD_LIBRARY(first SHARED first.cpp)

ADD_LIBRARY(second SHARED second.cpp)
#target_link_libraries(second libfirst.so)

add_executable (main main.cpp)
#target_link_libraries(main libsecond.so libfirst.so)
#target_link_libraries(main libsecond.so)
target_link_libraries(main libfirst.so libsecond.so)
