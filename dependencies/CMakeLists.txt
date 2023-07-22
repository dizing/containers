#add_subdirectory(googletest EXCLUDE_FROM_ALL)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
)
FetchContent_MakeAvailable(googletest)

target_compile_options(gtest PRIVATE "-w")
target_compile_options(gmock PRIVATE "-w") 