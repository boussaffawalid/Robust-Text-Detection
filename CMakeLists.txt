PROJECT( RobustTextDetection )
FIND_PACKAGE( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# C++14: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
cmake_minimum_required(VERSION 2.8)

ADD_EXECUTABLE( main main.cpp  RobustTextDetection.cpp ConnectedComponent.cpp)
TARGET_LINK_LIBRARIES( main ${OpenCV_LIBS} )
