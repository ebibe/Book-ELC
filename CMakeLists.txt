# Set the minimum required version of CMake, project name and standard
cmake_minimum_required(VERSION 3.27)
project(EditionFinder C)
set(CMAKE_C_STANDARD 11)


# Find the cJSON package
include_directories("C:/Program Files/cJSON/install/include")
link_directories("C:/Program Files/cJSON/install/lib")

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(EditionFinder main.c credentials.c)
target_link_libraries(EditionFinder ${CURL_LIBRARIES} cjson)
