cmake_minimum_required(VERSION 3.10)
project(chess_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDAR_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/board/GameBoard.cpp
    src/pieces/ChessPiece.cpp)

add_executable(chess ${SOURCES})