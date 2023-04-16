# CMakeList.txt : CMake project for fcm, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("fcm")

# Add source to this project's executable.
add_executable (fcm "main.cpp" "a429_lib.cpp" "a429_lib.h" "plane.cpp" "plane.h" "refresher.cpp" "refresher.h" "socket_rw.cpp" "socket_rw.h")

find_library(LIBRT rt) 
if(LIBRT)
target_link_libraries(fcm ${LIBRT})
endif()

# TODO: Add tests and install targets if needed.
