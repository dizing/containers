add_subdirectory(googletest EXCLUDE_FROM_ALL)

target_compile_options(gtest PRIVATE "-w")
target_compile_options(gmock PRIVATE "-w") 