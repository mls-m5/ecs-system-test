
cmake_minimum_required(VERSION 3.23)
project(ecs-test)

add_executable(
    ecs-test
    src/main.cpp
    )

target_compile_features(
    ecs-test
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    ecs-test
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
