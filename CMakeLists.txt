cmake_minimum_required(VERSION 3.5)
project(XiamiAPI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_MACOSX_RPATH 1)
add_subdirectory(cpr)

include_directories(${CPR_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpr/opt/googletest/include)

set(SOURCE_FILES
        needvalidationexception.cpp
        needvalidationexception.h
        networkexception.cpp
        networkexception.h
        stringutilities.h
        stringutilities.cpp
        xiamiapi_impl.cpp
        xiamiapi_impl.h
        iunknown.h
        xiamiapi.h
        xiamiapi.cpp
        str.cpp
        str.h
        GenericArray.cpp
        GenericArray.h
        iunknown.cpp keynotfoundexception.cpp keynotfoundexception.h)

add_library(xiamiapi SHARED ${SOURCE_FILES})
add_executable(apitest test/apitest.cpp)

target_link_libraries(xiamiapi ${CPR_LIBRARIES})
target_link_libraries(apitest xiamiapi)
target_link_libraries(apitest ${GTEST_LIBRARIES})
