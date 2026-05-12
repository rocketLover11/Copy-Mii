cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{DEVKITPRO})
    set(DEVKITPRO $ENV{DEVKITPRO})
elseif(EXISTS "/opt/devkitpro")
    set(DEVKITPRO "/opt/devkitpro")
elseif(EXISTS "C:\\devkitpro")
    set(DEVKITPRO "C:\\devkitpro")
else()
    message(FATAL_ERROR "DEVKITPRO is not set.")
endif()

if(DEFINED ENV{DEVKITPPC})
    set(DEVKITPPC $ENV{DEVKITPPC})
elseif(EXISTS "${DEVKITPRO}/devkitPPC")
    set(DEVKITPPC "${DEVKITPRO}/devkitPPC")
else()
    message(FATAL_ERROR "DEVKITPPC is not set. Please install devkitPPC. Run: (sudo) (dkp-)pacman -S wii-dev")
endif()

message(STATUS "DEVKITPRO: ${DEVKITPRO}")
message(STATUS "DEVKITPPC: ${DEVKITPPC}")

include("${DEVKITPRO}/cmake/Wii.cmake")

project(CopyMii VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/Main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
    ogc
    m
)

ogc_create_dol(${PROJECT_NAME})