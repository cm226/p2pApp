 cmake_minimum_required(VERSION 2.8.12)
 project(P2PTest)

 add_definitions("-std=c++2a")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(p2p 
    src/p2p.cpp
    src/UDPClient.cpp
)
 target_link_libraries(p2p ${CONAN_LIBS})
