cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

##########################################
# Project
##########################################
project(
  ABI
    VERSION
      1.0.0.0
    LANGUAGES
      C
)

#########################################################################
#                              Main Target                              #
#########################################################################
# create executable target
add_library(
  ${PROJECT_NAME}
  INTERFACE
)
add_library(eyenseo::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

##########################################
# IDE sugar
##########################################
target_sources(
  ${PROJECT_NAME}
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ABI.h
)
