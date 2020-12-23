cmake_minimum_required(VERSION 3.17)
project(ProgettoA C)

set(CMAKE_C_STANDARD 99)

add_executable(conquest main.c utility.c utility.h conquest.c conquest.h)
target_link_libraries(conquest m) # Per far funzionare math.h su linux
