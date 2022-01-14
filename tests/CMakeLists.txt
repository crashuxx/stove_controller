cmake_minimum_required(VERSION 3.12)

project(tests C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(utest 
    test.cc 
    potential_difference.cc
    lib/estyma.cc
    lib/kty.cc
    lib/pt.cc
    ../src/lib/estyma/ct2.c
    ../src/lib/kty/81_210.c
    ../src/lib/pt/pt1000.c
)
target_link_libraries(utest gtest_main)
add_test(NAME example_test COMMAND utest)