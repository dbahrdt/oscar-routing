cmake_minimum_required(VERSION 3.9)
project(oscar-routing)

add_subdirectory(vendor/liboscar liboscar)

set(SOURCES_CPP
	apps/main.cpp
)

set(LIB_CPP
	src/Edge2CellIds.cpp
)

set(LIB_H
	include/liboscar/routing/support/Edge2CellIds.cpp
)

add_library(${PROJECT_NAME} ${LIB_CPP})
target_link_libraries(${PROJECT_NAME} liboscar)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME}_example ${SOURCES_CPP})
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})
