cmake_minimum_required(VERSION 3.2)

project(AoC
    LANGUAGES CXX
)

set(days 
    day-01
    day-02
    day-03
    day-04
)

foreach(day ${days})
    add_subdirectory(${day})
endforeach()
