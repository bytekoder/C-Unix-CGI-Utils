cmake_minimum_required(VERSION 3.8)
project(Assignment_2)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES tt2ht1.c tt2ht2.c wow.c wtf.c)
add_executable(Assignment_2 ${SOURCE_FILES})
add_executable(tt2ht1 tt2ht1.c)
add_executable(tt2ht2 tt2ht2.c)
add_executable(wow wow.c)
add_executable(wtf wtf.c)