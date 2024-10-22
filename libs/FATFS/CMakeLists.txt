cmake_minimum_required(VERSION 3.20)
project(fatfs C)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(fatfs STATIC ${SRC_FILES})

target_include_directories(fatfs
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../app/inc>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../FreeRTOS/inc/>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../STM32H7xx_HAL_Driver/inc/>
)
target_link_libraries(fatfs PUBLIC freertos)
