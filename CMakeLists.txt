CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(HTTPServer)

SET(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

SET(SOURCE_DIR ${CMAKE_SOURCE_DIR}/static)
FILE(COPY ${SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

FILE(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

ADD_EXECUTABLE(http_srv ${SRC} main.cpp)

TARGET_COMPILE_OPTIONS(http_srv PRIVATE
    # -Wall                    # Enable all warnings
    -Wextra                  # Extra warning flags
    -Wpedantic               # Enforce strict standards
    -O2                      # Optimization level 2
)
