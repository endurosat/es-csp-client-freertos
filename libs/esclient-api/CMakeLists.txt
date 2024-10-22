cmake_minimum_required(VERSION 3.20)
project(esclient-api)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(COMMON_WARN_FLAGS "${COMMON_WARN_FLAGS} -pedantic -Wall -Wextra -fanalyzer \
-Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wdouble-promotion -Wduplicated-cond \
-Wduplicated-branches -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wlogical-op \
-Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wno-unused -Wnull-dereference -Wodr \
-Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default \
-Wswitch-enum -Wwrite-strings -Wundef -Wuninitialized -Wunreachable-code")

 set(C_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wbad-function-cast -Wmissing-prototypes -Wnested-externs \
-Wold-style-definition -Wstrict-prototypes")

set(CXX_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wctor-dtor-privacy \
-Wnoexcept -Wold-style-cast -Woverloaded-virtual \
-Wsign-promo -Wstrict-null-sentinel -Wuseless-cast -Wzero-as-null-pointer-constant")

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

add_library(esclient-api STATIC ${SRC_FILES})
target_link_libraries(esclient-api csp)
target_link_libraries(esclient-api fatfs)
