cmake_minimum_required(VERSION 3.20)
project(freertos)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(freertos STATIC ${SRC_FILES})

target_include_directories(freertos
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../app/inc>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../CMSIS/Include/>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../CMSIS/STM32H7xx/Include/>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../STM32H7xx_HAL_Driver/inc/>

)
