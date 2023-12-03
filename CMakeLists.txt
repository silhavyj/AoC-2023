cmake_minimum_required(VERSION 3.2)

project(AoC
    LANGUAGES CXX
)

set(days 
    01
    02
    03
)

foreach(day ${days})
    add_subdirectory(${day})
endforeach()
