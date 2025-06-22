cmake_minimum_required(VERSION 3.31)

project(KeyValueStore VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS system)

include(FetchContent)

FetchContent_Declare(
  boost_asio
  GIT_REPOSITORY https://github.com/boostorg/asio.git
  GIT_TAG boost-1.88.0
)

FetchContent_MakeAvailable(boost_asio)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fmt)

add_subdirectory(storage)
add_subdirectory(server)
add_subdirectory(app)
add_subdirectory(command)
add_subdirectory(utils)
add_subdirectory(client)