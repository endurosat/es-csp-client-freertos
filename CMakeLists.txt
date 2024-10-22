cmake_minimum_required(VERSION 3.20)
# ##############################################################################
project(es_client_lib_CM7)
# ##############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})


enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")

# MCU Configs / Linker / Startup
include (${CMAKE_SOURCE_DIR}/cmake/h75x.cmake)

# Compiler flags
include (${CMAKE_SOURCE_DIR}/cmake/warnings.cmake)

# HAL
add_subdirectory(libs/STM32H7xx_HAL_Driver ${CMAKE_CURRENT_BINARY_DIR}/HAL)

# Freertos
add_subdirectory(libs/FreeRTOS ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS)

# FATFS
add_subdirectory(libs/FATFS ${CMAKE_CURRENT_BINARY_DIR}/FATFS)

# Libcsp
add_subdirectory(libs/libcsp ${CMAKE_CURRENT_BINARY_DIR}/csp)

# ES Client API
add_subdirectory(libs/esclient-api ${CMAKE_CURRENT_BINARY_DIR}/esclient-api)

# Application specifics
file(GLOB APP_SRC_FILES ${PROJECT_SOURCE_DIR}/app/src/*.c)
add_executable(${EXECUTABLE} ${APP_SRC_FILES} ${STARTUP_SCRIPT})

target_include_directories(${EXECUTABLE}
       		           PUBLIC 
			   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/inc>
)

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/esclient-api/include>)
target_link_libraries(${EXECUTABLE} freertos fatfs stm32h7_hal_driver csp esclient-api)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.bin)
