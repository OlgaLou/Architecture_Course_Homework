project(test_solve LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(test main.cpp exeptions.h commands.h commands.cpp exeption_handling_test.h)

target_link_libraries(test  
  PRIVATE 
    gtest
    gtest_main)

enable_testing()

include(FetchContent)
FetchContent_Declare(gtest gmock
  QUIET
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)