cmake_minimum_required(VERSION 2.84)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2)
project(HighwayTutor)

include_directories(include)

add_library(libhighway SHARED src/Highway.cpp)

add_library(libdatagenerator SHARED src/DataGenerator.cpp)
target_link_libraries(libdatagenerator libhighway)

add_library(libdataprocessor SHARED src/DataProcessor.cpp)
target_link_libraries(libdataprocessor libhighway)

add_executable(Simulator src/Simulator.cpp)
target_link_libraries(Simulator libdatagenerator)

add_executable(Tutor src/Tutor.cpp)
target_link_libraries(Tutor libdataprocessor)






