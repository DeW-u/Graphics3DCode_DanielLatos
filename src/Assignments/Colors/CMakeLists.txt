cmake_minimum_required(VERSION 3.15)
project(Colors)

add_compile_definitions(PROJECT_NAME="Colors" PROJECT_DIR="/home/daniel/3d-project/Graphics3DCode_DanielLatos/src/Assignments/Colors")

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        )
        
target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

