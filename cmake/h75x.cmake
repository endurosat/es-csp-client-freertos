set(MCU_FAMILY STM32H7xx)
set(MCU_MODEL STM32H755xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=hard)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/app/stm32h755xx_CM7.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/app/stm32h755xx_flash_CM7.ld)

add_compile_definitions(CORE_CM7
			USE_HAL_DRIVER
			STM32H755xx
)
add_compile_options(
		    ${CPU_PARAMETERS}
)

set(LIBS -lc -lm)

add_link_options( -specs=nano.specs
       		-T${MCU_LINKER_SCRIPT}
		 ${CPU_PARAMETERS}
		 -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--cref 
		 -Wl,--gc-sections
		 -Wl,--start-group
		 ${LIBS}
		 -Wl,--end-group
		 -Wl,--print-memory-usage
)
