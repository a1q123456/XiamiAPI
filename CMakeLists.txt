cmake_minimum_required(VERSION 3.5)
project(XiamiAPI)
add_subdirectory(cpr)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${CPR_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)


set(SOURCE_FILES
        needvalidationexception.cpp
        needvalidationexception.h
        networkexception.cpp
        networkexception.h
        stringutilities.h
        stringutilities.cpp
        xiamiapi_impl.cpp
        xiamiapi_impl.h
        global.h
        iunknown.h
        xiamiapi.h
        xiamiapi.cpp
        str.cpp
        str.h
        GenericArray.cpp
        GenericArray.h
        iunknown.cpp
        test/main.cpp
        )
add_library(xiamiapi SHARED ${SOURCE_FILES})

target_link_libraries(xiamiapi ${CPR_LIBRARIES})

add_executable(xiamiapitest ${SOURCE_FILES})
target_link_libraries(xiamiapitest ${CPR_LIBRARIES})

link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/build/Debug/libxiamiapi.so)

