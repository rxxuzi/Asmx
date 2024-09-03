cmake_minimum_required(VERSION 3.27)
project(asmx C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

add_executable(asmx
        src/main.c
        src/fis.h
        src/fis.c
        src/asmx.c
        src/asmx.h
        src/asmc.c
        src/asmb.c
        src/util.c
        src/options.c
        src/utils/sha256.c
        src/cson/cson.c
        src/cson/csonp.c
        src/cson/csonx.c
        src/cson/cson_io.c
        src/cson/csonx2.c
        src/utils/console.c
)

set_target_properties(asmx PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "C:/rxxuzi/bin/"
)