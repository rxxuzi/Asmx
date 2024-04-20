cmake_minimum_required(VERSION 3.27)
project(asmx C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

add_executable(asmx
        main.c
        fis.h
        fis.c
        asmx.c
        asmx.h
        asmc.c
        asmb.c
        util.h
        util.c)

set_target_properties(asmx PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../"
)