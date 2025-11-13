cmake_minimum_required(VERSION 3.16)
project(JsonParser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # ensures shared lib compatibility

# Define include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/Utils
)

# Collect source files
file(GLOB_RECURSE JSONPARSER_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# If src/ only has dummy.cpp for now, that's fine.
# Later, add your real source files to src/.

# === Shared Library ===
add_library(JsonParser SHARED
        ${JSONPARSER_SOURCES}
)

target_include_directories(JsonParser
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/Utils
)

# === Executable (for testing) ===
add_executable(JsonParserMain
        tests/main.cpp
)

target_link_libraries(JsonParserMain
        PRIVATE JsonParser
)

# Optional: specify output directories for clarity
set_target_properties(JsonParser PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)

set_target_properties(JsonParserMain PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)