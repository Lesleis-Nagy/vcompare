cmake_minimum_required(VERSION 3.22)

PROJECT(VCompare)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

###############################################################################
# Find VTK                                                                    #
###############################################################################

find_package(VTK REQUIRED)

###############################################################################
# Find Qt                                                                     #
###############################################################################

find_package(Qt6 REQUIRED COMPONENTS Core)

###############################################################################
# Find Boost                                                                  #
###############################################################################

find_package(Boost REQUIRED regex)

###############################################################################
# Define executable and library dependencies.                                 #
###############################################################################

qt_standard_project_setup()
qt_add_executable(vcompare
        src/VCompare.ui
        src/Convert.cpp
        src/DirectoryDatabase.cpp
        src/TecplotLoader.cpp
        src/TreeItem.cpp
        src/TreeModel.cpp
        src/Validate.cpp
        src/VCompare.cpp
        src/VCompareDriver.cpp
        src/VectorField.cpp
        src/VectorFieldSet.cpp
)

message(STATUS "VTK_LIBRARIES:    ${VTK_LIBRARIES}")
message(STATUS "VTK_INCLUDE_DIRS: ${VTK_INCLUDE_DIRS}")

target_link_libraries(vcompare
        PRIVATE Qt6::Core
        ${VTK_LIBRARIES}
)

target_include_directories(vcompare
        PUBLIC ${VTK_INCLUDE_DIRS}
)

set_target_properties(vcompare PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)
