cmake_minimum_required(VERSION 3.17)
project(Module11 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Module11
        DataLoader.h
        PageTable.h
        Simulator.c
        GoodmanDataLoader.c
        GoodmanPageTable.c)
