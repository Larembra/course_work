cmake_minimum_required(VERSION 3.10.2)
project(course_work)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(course_work
        main.cpp
        revolver.cpp
        contestant.cpp
        nn.cpp
        player.cpp
        telegram.cpp
        game.cpp
        #tgbottest.cpp

)



target_link_libraries(course_work C:/vcpkg/installed/x64-windows/lib/TgBot.lib ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ws2_32)


message(STATUS "OpenSSL: ${OPENSSL_INCLUDE_DIR}, ${OPENSSL_LIBRARIES}")
message(STATUS "Boost: ${Boost_INCLUDE_DIR}, ${Boost_LIBRARIES}")
message(STATUS "CURL: ${CURL_INCLUDE_DIRS}, ${CURL_LIBRARIES}")