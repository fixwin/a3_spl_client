cmake_minimum_required(VERSION 3.6)
project(a3_spl_client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/echoClient.cpp src/connectionHandler.cpp src/Message.cpp include/Message.h include/KeyboardTask.h include/SocketListenTask.h src/EncoderDecoder.cpp include/EncoderDecoder.h src/Protocol.cpp include/Protocol.h)
add_executable(a3_spl_client ${SOURCE_FILES})

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

set (Boost_USE_STATIC_LIBS ON) # enable dynamic linking
set (Boost_USE_MULTITHREAD ON)  # enable multithreading
find_package (Boost COMPONENTS REQUIRED chrono system thread filesystem)
target_link_libraries (a3_spl_client ${Boost_LIBRARIES})