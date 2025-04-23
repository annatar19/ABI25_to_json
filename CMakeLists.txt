cmake_minimum_required(VERSION 3.20)
project(to_json_v2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build: Debug, Release, RelWithDebInfo, MinSizeRel"
      FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON CACHE BOOL "Generate compile_commands.json")

set(ROOT_DIR "/home/annatar/studie/abi25/root/root-build"
    CACHE PATH "Path to the ROOT installation")

find_package(ROOT CONFIG REQUIRED
    COMPONENTS
      ROOTNTuple
)

add_executable(to_json_v2 to_json_v2.cpp)

target_link_libraries(to_json_v2
    PRIVATE
      ROOT::ROOTNTuple
)
