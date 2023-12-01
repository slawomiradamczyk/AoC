set(SOURCE_FILES
    src/input_reader.cpp
)

add_library(common STATIC ${SOURCE_FILES})

target_include_directories(common PUBLIC include)
