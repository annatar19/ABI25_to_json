cmake_minimum_required(VERSION 3.20)
project(to_json_v2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find your local ROOT build:
# either pass -DROOT_DIR or set CMAKE_PREFIX_PATH
find_package(ROOT CONFIG REQUIRED
    COMPONENTS
      ROOTNTuple   # the RNTuple I/O library
      Hist         # TH1*
      Graf         # TCanvas
      Gpad         # <— TCanvas & TPad lives here
)

add_executable(to_json_v2 to_json_v2.cpp)

# Link against the imported targets.  ROOTNTuple pulls in Core/RIO etc. transitively.
target_link_libraries(to_json_v2
    PRIVATE
      ROOT::ROOTNTuple
      ROOT::Hist
      ROOT::Graf
      ROOT::Gpad
)
