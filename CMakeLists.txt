cmake_minimum_required(VERSION 3.16)

project(mmsens-app LANGUAGES CXX)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION /usr/bin)
