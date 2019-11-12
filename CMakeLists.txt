cmake_minimum_required(VERSION 3.8)
# project name
project(max_robotics)
set(CMAKE_CXX_STANDARD 11)
# define executable path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(warnings "-Wall -Wextra -Werror")
endif()

# Include Dependencies
find_package (Eigen3 REQUIRED)
find_package (OpenCV REQUIRED)
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

# Ensure dependencies exist
if (APPLE)
    # default library path
    message(STATUS "Checking Eigen availability...")
    if (NOT EXISTS ${EIGEN3_INCLUDE_DIR})
        message(WARNING "Unable to find EIGEN3_INCLUDE_DIR...")
        message(STATUS "Installing Eigen...")
        # now we need to download the Eigen library
        execute_process(COMMAND brew install eigen)
    endif()
endif()

# path to project header
include_directories(include)

# adds lib to sys env
add_library(max_robotics SHARED src/max_robotics.cpp include/max_robotics.h)

# define source files
# explicit
#set(SOURCES
	#${CMAKE_CURRENT_SOURCE_DIR}/src/lib_test.cpp
	#)
# implicit - global
#file(GLOB SOURCES "src/*.cpp")
set(ALGORITHMS_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms
)
set(EXERCISES_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/src/exercises
)


add_executable(lib_test src/test_lib.cpp)
target_link_libraries( lib_test max_robotics ${OpenCV_LIBS} )

add_executable(find_clusters ${ALGORITHMS_SRC}/find_clusters.cpp)
target_link_libraries( find_clusters max_robotics ${OpenCV_LIBS})

add_executable(find_rectangles ${EXERCISES_SRC}/find_rectangles.cpp)
target_link_libraries( find_rectangles max_robotics ${OpenCV_LIBS})
