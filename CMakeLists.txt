cmake_minimum_required(VERSION 3.16)

project(mmsens-app)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION /usr/bin)
