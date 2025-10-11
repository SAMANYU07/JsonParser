cmake_minimum_required(VERSION 3.16)  # version 4.0 doesn't exist; use a valid one
project(JsonParser)

set(CMAKE_CXX_STANDARD 20)

# Add library (your parser logic, headers, etc.)
add_library(JsonParser
        src/dummy.cpp           # <- NEW: even empty, just to set language
        Utils/Utils.h
        include/JsonMetaData.h
        include/JsonParser.H
        include/JsonToken.h
        include/LexicalAnalyzer.h
        include/RuntimeError.h
)

# Add executable (main.cpp is here, contains the entry point)
add_executable(JsonParserMain tests/main.cpp
        src/dummy.cpp)

# Link the library to the executable
target_link_libraries(JsonParserMain PRIVATE JsonParser)
