CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(HTTPServer)

SET(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(include)

FILE(GLOB SOURCES src/*.cpp)

ADD_EXECUTABLE(http_srv ${SOURCES} main.cpp)

TARGET_COMPILE_OPTIONS(http_srv PRIVATE
    # -Wall                    # Enable all warnings
    -Wextra                  # Extra warning flags
    -Wpedantic               # Enforce strict standards
    -O2                      # Optimization level 2
)
