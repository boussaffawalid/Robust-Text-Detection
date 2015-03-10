PROJECT( RobustTextDetection )
FIND_PACKAGE( OpenCV REQUIRED )

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
FIND_PACKAGE( TESSERACT REQUIRED )

ADD_EXECUTABLE( main main.cpp ConnectedComponent.cpp RobustTextDetection.cpp )
TARGET_LINK_LIBRARIES( main ${OpenCV_LIBS} ${TESSERACT_LIBRARY})
