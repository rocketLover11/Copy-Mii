cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{DEVKITPRO})
    set(DEVKITPRO $ENV{DEVKITPRO})
elseif(EXISTS "/opt/devkitpro")
    set(DEVKITPRO "/opt/devkitpro")
elseif(EXISTS "C:\\devkitPro")
    set(DEVKITPRO "C:\\devkitPro")
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
    src/gx/Renderer.cpp
    src/gx/Texture.cpp
    src/ui/Font.cpp
    src/ui/Cursor.cpp
    src/ui/Label.cpp
    src/ui/Button.cpp
    src/ui/Keyboard.cpp
    src/ui/InputBox.cpp
    src/net/Network.cpp
    src/net/Installer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${DEVKITPRO}/portlibs/ppc/include
    ${DEVKITPRO}/portlibs/ppc/include/freetype2
    ${DEVKITPRO}/libogc/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${DEVKITPRO}/portlibs/ppc/lib
    ${DEVKITPRO}/libogc/lib/wii
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Helvetica.o
    COMMAND ${DEVKITPPC}/bin/powerpc-eabi-objcopy -I binary -O elf32-powerpc -B powerpc Helvetica.ttf ${CMAKE_BINARY_DIR}/Helvetica.o
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/assets/fonts
    DEPENDS ${CMAKE_SOURCE_DIR}/assets/fonts/Helvetica.ttf
)

add_custom_target(EmbedFont DEPENDS ${CMAKE_BINARY_DIR}/Helvetica.o)
add_dependencies(${PROJECT_NAME} EmbedFont)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_BINARY_DIR}/Helvetica.o
    fat
    freetype
    brotlidec
    brotlicommon
    bz2
    png
    zip
    z
    ogc
    m
)

ogc_create_dol(${PROJECT_NAME})

set(STAGING_DIR "${CMAKE_CURRENT_BINARY_DIR}/CopyMii")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${STAGING_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/CopyMii.dol" "${STAGING_DIR}/boot.dol"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/hbc/meta.xml" "${STAGING_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/hbc/icon.png" "${STAGING_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/CopyMii.zip" --format=zip "${STAGING_DIR}"
    COMMENT "Packaging..."
)