cmake_minimum_required(VERSION 3.6)
project(OpenGL_Model_Loader)
set(CMAKE_CXX_STANDARD  17)

include_directories(
        "libs/glfw/include"
        "libs/glad/include/"
        "libs/glm"
        "includes/"
        "src/includes/"
)

add_subdirectory(libs/glfw/)
add_subdirectory(libs/glm/)

SET(SRC_FILES 
    "libs/glad/src/glad.c"
    "src/shader.cpp"
    "src/texture.cpp"
    "src/camera.cpp"
    "src/main.cpp"
)

add_executable(OpenGL_Model_Loader
    ${SRC_FILES}
)

if(WIN32)
        target_link_libraries(OpenGL_Model_Loader PRIVATE glfw opengl32)
endif(WIN32)

if(UNIX)
        target_link_libraries(OpenGL_Model_Loader PRIVATE glfw GL)
endif(UNIX)