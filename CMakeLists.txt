cmake_minimum_required(VERSION 3.10)
project(chess_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDAR_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set(SOURCES
    src/main.cpp
    src/board/GameBoard.cpp
    src/pieces/ChessPiece.cpp
)

add_executable(chess_cpp ${SOURCES})

target_link_libraries(chess_cpp
    sfml-graphics 
    sfml-window 
    sfml-system
)