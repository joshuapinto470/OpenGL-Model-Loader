cmake_minimum_required(VERSION 3.6)
project(OpenGL_Template)
set(CMAKE_CXX_STANDARD  17)

include_directories(
        "libs/glfw/include"
        "libs/glad/include/"
        "src/includes/"
)

add_subdirectory(libs/glfw/)

SET(SRC_FILES 
    "libs/glad/src/glad.c"
    "src/main.cpp"
)

add_executable(OpenGL_Template
    ${SRC_FILES}
)

if(WIN32)
        target_link_libraries(OpenGL_Template PRIVATE glfw opengl32)
endif(WIN32)

if(UNIX)
        target_link_libraries(OpenGL_Template PRIVATE glfw GL)
endif(UNIX)