set(SOURCE_FILES
    src/input_reader.cpp
        include/parser.h
)

add_library(common STATIC ${SOURCE_FILES})

target_include_directories(common PUBLIC include)
