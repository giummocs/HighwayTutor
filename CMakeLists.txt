cmake_minimum_required(VERSION 2.84)
set(CMAKE_CXX_STANDARD 14)
project(HighwayTutor)

include_directories(include)

add_library(libhighway SHARED src/Highway.cpp)

add_library(libdatagenerator SHARED src/DataGenerator.cpp)

add_library(libdataprocessor SHARED src/DataProcessor.cpp)

add_executable(Simulator src/Simulator.cpp)
target_link_libraries(Simulator libhighway libdatagenerator)

add_executable(Tutor src/Tutor.cpp)
target_link_libraries(Tutor libhighway libdataprocessor)


