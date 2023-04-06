# CMakeList.txt : CMake project for CMakeProjectAER, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CMakeProjectAER")

# Add source to this project's executable.
add_executable (CMakeProjectAER "main.cpp" "a429_lib.cpp" "a429_lib.h" "plane.cpp" "plane.h" "refresher.cpp" "refresher.h" "socket_rw.cpp" "socket_rw.h")

# TODO: Add tests and install targets if needed.
