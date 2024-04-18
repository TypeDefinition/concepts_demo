# CMakeList.txt : CMake project for concepts_demo, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (concepts_demo main.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET concepts_demo PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
