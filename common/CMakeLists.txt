set(SOURCE_FILES
    src/input_reader.cpp
        include/parser.h
        ../Problem10/main.cpp
)

add_library(common STATIC ${SOURCE_FILES})

target_include_directories(common PUBLIC include)
