cmake_minimum_required(VERSION 3.10)
project(UnitKernel C ASM)

set(CMAKE_C_STANDARD 11)

# Pick up all .c and .s files inside src/
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.s")

add_executable(unit.elf ${SOURCES})

target_include_directories(unit.elf PRIVATE ${CMAKE_SOURCE_DIR})

target_compile_options(unit.elf PRIVATE
    "-mno-red-zone"
    "-mcmodel=kernel"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
)

target_link_options(unit.elf PRIVATE
    "-static"
    "-z"
    "max-page-size=0x1000"
    "-T${CMAKE_SOURCE_DIR}/src/arch/x86_64/linker.ld"
)

# Verify the linker script exists before building
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src/arch/x86_64/linker.ld")
    message(FATAL_ERROR "Linker script not found!")
endif()

# Copy the final kernel to the ISO root
add_custom_command(TARGET unit.elf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:unit.elf> "${CMAKE_SOURCE_DIR}/iso_root/unit.elf"
)