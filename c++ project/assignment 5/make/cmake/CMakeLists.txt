cmake_minimum_required(VERSION 2.8.7)
project(assignment5)

find_package(VTK COMPONENTS vtkRenderingOpenGL2 vtkInteractionStyle vtkRenderingVolumeOpenGL2 vtkRenderingFreeType NO_MODULE)

include(${VTK_USE_FILE})

set(SOURCES ../../source/assignment5.cpp)

add_executable(assignment5 ${SOURCES})
target_link_libraries(assignment5 ${VTK_LIBRARIES})
