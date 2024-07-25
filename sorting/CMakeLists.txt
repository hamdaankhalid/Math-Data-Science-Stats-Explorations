cmake_minimum_required(VERSION 3.12)
project(sorting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "clang++")

# Add all your source files and header files here
set(SOURCES
    sorting.cc
)


set(HEADERS
  include *.h
)

# Add the executable target
# add_executable(sorting ${SOURCES} ${HEADERS})
add_executable(sorting ${SOURCES})