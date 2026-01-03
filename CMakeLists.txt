cmake_minimum_required(VERSION 2.84)
set(CMAKE_CXX_STANDARD 11)
project(HighwayTutor)

include_directories(include)

add_library(libhighway SHARED src/Highway.cpp)

add_library(libvehicle SHARED src/Vehicle.cpp)

add_library(libdatagenerator SHARED src/DataGenerator.cpp)

add_library(libdataprocessor SHARED src/DataProcessor.cpp)

add_executable(Simulator src/Simulator.cpp)
target_link_libraries(Simulator libhighway libvehicle libdatagenerator)

add_executable(Tutor src/Tutor.cpp)
target_link_libraries(Tutor libhighway libdataprocessor)

##cmake_minimum_required(VERSION 3.10)
##project(HighwayTutor)

##set(CMAKE_CXX_STANDARD 17)

##set(CMAKE_CXX_FLAGS_RELEASE "-O2")
##include_directories(include)

# File comuni
##set(COMMON_SRCS src/Highway.cpp)

# Target Simulatore
##add_executable(simulator src/simulator.cpp ${COMMON_SRCS})

# Target Tutor

##add_executable(tutor src/tutor.cpp ${COMMON_SRCS})


