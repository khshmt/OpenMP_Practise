cmake_minimum_required(VERSION 3.20.0)
project(OpenMP_Practise)

find_package(OpenMP REQUIRED)

add_executable(work_sharing work_sharing.cpp)
target_link_libraries(work_sharing PRIVATE OpenMP::OpenMP_CXX) 