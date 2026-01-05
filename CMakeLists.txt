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


/usr/bin/ld: liblibdatagenerator.so: undefined reference to `Highway::getDistance(char, int)'
/usr/bin/ld: liblibdatagenerator.so: undefined reference to `Highway::getSize(char)'
/usr/bin/ld: liblibdatagenerator.so: undefined reference to `Highway::Highway(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
collect2: error: ld returned 1 exit status
make[2]: *** [CMakeFiles/Simulator.dir/build.make:99: Simulator] Error 1
make[1]: *** [CMakeFiles/Makefile2:170: CMakeFiles/Simulator.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
