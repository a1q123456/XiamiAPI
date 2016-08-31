cmake_minimum_required(VERSION 3.5)
project(XiamiAPI)

option(STATIC_LINK_ALL "set to on if you want to static link all libs even include libstdgcc and libstdc++" OFF)
option(STATIC_LINK_OPENSSL "set to on if you want curl to static link libopenssl" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if (STATIC_LINK_OPENSSL)
  set(CURL_STATICLIB ON)
endif()

if (STATIC_LINK_ALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
endif()

add_subdirectory(cpr)

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
        )
add_library(xiamiapi SHARED ${SOURCE_FILES})

target_link_libraries(xiamiapi ${CPR_LIBRARIES})

link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/build/Debug/libxiamiapi.so)

