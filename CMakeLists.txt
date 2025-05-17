cmake_minimum_required(VERSION 3.16)

project(SharedPtr
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -fdiagnostics-color=always -g3")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_library(SharedPtr INTERFACE)
target_include_directories(SharedPtr INTERFACE .)
target_sources(SharedPtr PUBLIC ./SharedPtr.hpp)


option(TESTING "Enable tests" ON)

if (TESTING)
include (CTest)
    enable_testing()
    add_subdirectory(test)
endif ()
