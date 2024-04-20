# Set the minimum required version of CMake, project name and standard
cmake_minimum_required(VERSION 3.27)
project(EditionFinder C)
set(CMAKE_C_STANDARD 11)


# Find the cJSON package
include_directories("C:/Program Files/cJSON/install/include")
link_directories("C:/Program Files/cJSON/install/lib")

set(CURL_LIBRARY "C:/Program Files/curl/lib/libcurl.dll.a")
set(CURL_INCLUDE_DIR "C:/Program Files/curl/include")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(EditionFinder main.c)
target_link_libraries(EditionFinder ${CURL_LIBRARIES} cjson)
