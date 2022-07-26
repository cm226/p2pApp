 cmake_minimum_required(VERSION 2.8.12)
 project(P2PTest)

 add_definitions("-std=c++2a")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(p2p 
    src/p2p.cpp
    src/UDPClient.cpp
    src/STUN/stunbuilder.cpp
    src/STUN/datastream.cpp
    src/STUN/buffer.cpp
    src/STUN/socketaddress.cpp
    src/STUN/atomichelpers.cpp
    src/STUN/stringhelper.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

target_link_libraries(p2p ${CONAN_LIBS})

