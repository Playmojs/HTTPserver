cmake_minimum_required(VERSION 3.14)
project(HTTPServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

aux_source_directory(src/ SRC_DIR)
add_executable(HTTPserver ${SRC_DIR})

target_link_libraries(HTTPserver PRIVATE nlohmann_json::nlohmann_json)

target_compile_features(HTTPserver PRIVATE cxx_std_20)
