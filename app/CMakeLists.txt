
# Application specifics
file(GLOB APP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${EXECUTABLE} ${APP_SRC_FILES} ${STARTUP_SCRIPT})

target_include_directories(${EXECUTABLE}
                       PUBLIC
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libs/esclient-api/include>)
target_link_libraries(${EXECUTABLE} freertos fatfs stm32h7_hal_driver csp esclient-api)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.bin)
