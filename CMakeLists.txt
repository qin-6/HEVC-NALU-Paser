cmake_minimum_required(VERSION 2.8.0)
project(H265_Parser_Prj)

option(SDK_TEST "whether have a test" ON)

include_directories(./inc)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -fPIC")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fPIC")

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/h265_nalu_finder.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_aud.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_pps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_sei.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_sps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nal_vps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/h265_parser.cpp
    )

add_library(iqa SHARED ${SRC_FILES})

if (SDK_TEST)
    set(PARSE_FILES ./main.cc)
    add_executable(parser ${PARSE_FILES})
    target_link_libraries(parser
    iqa)
endif()
