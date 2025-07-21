include(FetchContent)

FetchContent_MakeAvailable(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.16.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(rendertest
    rendertest.cpp
)

target_link_libraries(rendertest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(rendertest)