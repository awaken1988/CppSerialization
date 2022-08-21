cmake_minimum_required (VERSION 3.24)
project (CppSerialization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

#------------------------------------
# CppSerialization
#------------------------------------
add_library(CppSerialization SHARED
    src/lib/CppSerialization.cpp    src/lib/export/CppSerialization.h
)
target_include_directories(CppSerialization PUBLIC src/lib/export)


#------------------------------------
# Example1
#------------------------------------
add_executable(Example1
    src/example1/example1.cpp
)

target_link_libraries(Example1
    CppSerialization
)
