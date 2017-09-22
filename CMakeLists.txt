cmake_minimum_required(VERSION 3.7)
project(lightchat)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c main.h)
add_executable(lightchat ${SOURCE_FILES})