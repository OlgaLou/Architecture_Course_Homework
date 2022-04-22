project(test_solve LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(test main.cpp exceptions.h commands.h change_velocity_command.h
               change_velocity_tests.h  fuel_commands.h macro_command_tests.h
               motion.h  process_fuel_tests.h rotation.h fuel_commands.cpp)

target_link_libraries(test  
  PRIVATE 
    gtest
    gmock
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