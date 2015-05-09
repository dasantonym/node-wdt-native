cmake_minimum_required(VERSION 2.8)
set( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
project (node_wdt_native)
find_package( OpenCV REQUIRED )
include_directories(${CMAKE_JS_INC} /usr/local/include/node/ /usr/local/include/opencv/ /usr/local/include/opencv2/ ./node_modules/opencv/src ./node_modules/nan)
file(GLOB SOURCE_FILES
    "src/*.cc" "src/*.h"
    "node_modules/opencv/src/**.cc"
    "node_modules/opencv/src/**.h"
    )
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/opencv/src/init.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_JS_LIB})