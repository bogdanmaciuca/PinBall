cmake_minimum_required(VERSION 3.15)
project(glad LANGUAGES C)

add_library(glad src/glad.c)

target_include_directories(glad PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(UNIX AND NOT APPLE)
    target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
endif()

add_library(glad::glad ALIAS glad)
