cmake_minimum_required(VERSION 3.2)

project(AoC
    LANGUAGES C CXX
)

set(days
    day-01
    day-02
    day-03
    day-04
    day-05
    day-06
    day-07
    day-08
    day-09
    day-10
    day-11
)

foreach(day ${days})
    add_subdirectory(${day})
endforeach()
