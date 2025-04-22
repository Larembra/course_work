cmake_minimum_required(VERSION 3.24)
project(course_work)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)

#add_executable(course_work main.cpp)
add_executable(course_work test.cpp revolver.cpp contestant.cpp player.cpp deepseek.cpp)

target_link_libraries(course_work PRIVATE CURL::libcurl)