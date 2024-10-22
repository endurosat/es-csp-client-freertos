cmake_minimum_required(VERSION 3.20)
project(csp)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)


include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/csp>)
include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/csp/arch>)
include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/csp/crypto>)
include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/csp/drivers>)
# include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/csp/interfaces>)

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

add_library(csp STATIC ${SRC_FILES})
target_link_libraries(csp freertos)

target_include_directories(csp
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
