cmake_minimum_required(VERSION 3.6)
project(OpenGL_Model_Loader)
set(CMAKE_CXX_STANDARD  17)

include_directories(

        "libs/glfw/include"
        "libs/glad/include/"
        "libs/assimp/"
        "libs/glm"
        "includes/"
        "includes/imgui"
        "src/includes/"
)

# link_directories(${CMAKE_SOURCE_DIR}/libs/assimp)
# link_directories(${CMAKE_SOURCE_DIR}/libs/zlib)
find_library(ASSIMP_LIB NAMES assimp REQUIRED HINTS ${CMAKE_SOURCE_DIR}/libs/assimp)
find_library(ZLIB_LIB NAMES zlibstatic REQUIRED HINTS ${CMAKE_SOURCE_DIR}/libs/zlib )

add_subdirectory(libs/glfw/)
add_subdirectory(libs/glm/)

SET(SRC_FILES 
    "libs/glad/src/glad.c"
    "libs/imgui/imgui.cpp"
    "libs/imgui/imgui_demo.cpp"
    "libs/imgui/imgui_draw.cpp"
    "libs/imgui/imgui_impl_glfw.cpp"
    "libs/imgui/imgui_impl_opengl3.cpp"
    "libs/imgui/imgui_tables.cpp"
    "libs/imgui/imgui_widgets.cpp"
    "src/shader.cpp"
    "src/texture.cpp"
    "src/renderer.cpp"
    "src/mesh.cpp"
    "src/camera.cpp"
    "src/model.cpp"
    "src/main.cpp"


)

add_executable(OpenGL_Model_Loader
    ${SRC_FILES}
)

if(WIN32)
        target_link_libraries(OpenGL_Model_Loader PRIVATE glfw opengl32 ${ASSIMP_LIB} ${ZLIB_LIB})
endif(WIN32)

if(UNIX)
        target_link_libraries(OpenGL_Model_Loader PRIVATE glfw GL assimp zlibstatic)
endif(UNIX)