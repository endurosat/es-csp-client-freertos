# Enforce C/C++ standard level
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


# Flags that apply to both C and C++
set(COMMON_WARN_FLAGS "${COMMON_WARN_FLAGS}")

# Flags that apply only to C OR C++
set(C_WARN_FLAGS "${COMMON_WARN_FLAGS} -fdata-sections -ffunction-sections \
    -Wl,--gc-sections")

set(CXX_WARN_FLAGS "${COMMON_WARN_FLAGS} -fno-rtti -fno-exceptions \
    -fno-threadsafe-statics")

# Pass them back to the CMake variable
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARN_FLAGS}")
