cmake_minimum_required(VERSION 3.10)
project(HighwayTutor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(include)

# File comuni
set(COMMON_SRCS src/Highway.cpp)

# Target Simulatore
add_executable(simulator src/simulator.cpp ${COMMON_SRCS})

# Target Tutor

add_executable(tutor src/tutor.cpp ${COMMON_SRCS})
