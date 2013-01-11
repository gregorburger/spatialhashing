cmake_minimum_required(VERSION 2.8)
project(spatialhashing)

set(CMAKE_AUTOMOC TRUE)

find_package(OpenMP REQUIRED)
find_package(Qt4 REQUIRED QtGui QtCore)
find_package(Boost)
include(${QT_USE_FILE})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -Wall -std=c++11")

include_directories(${Boost_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(UI_SRCS mainwindow.ui)

set(SRC_LIST main.cpp mainwindow.cpp graphicsscene.cpp spatialhash.h)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${UI_SRCS})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} -ltbb)

