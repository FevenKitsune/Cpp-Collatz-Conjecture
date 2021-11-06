# CMakeList.txt : CMake project for CppCollatzConjecture, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (CppCollatzConjecture "CppCollatzConjecture.cpp" "CppCollatzConjecture.h")

set_property(TARGET CppCollatzConjecture PROPERTY CXX_STANDARD 20)