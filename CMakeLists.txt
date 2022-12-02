project(logger)

cmake_minimum_required(VERSION 3.12)

include_directories(
    include/
)

add_executable(${CMAKE_PROJECT_NAME}-service
    src/impl/main.cpp
)

target_compile_features(${CMAKE_PROJECT_NAME}-service PRIVATE cxx_std_17)

find_package(Threads REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}-service
    Threads::Threads
)

