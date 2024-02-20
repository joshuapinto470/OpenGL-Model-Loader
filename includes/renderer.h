#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Render
{
public:
    Render();
    void RenderLoop();

private:
    GLFWwindow *pWindow;
    unsigned int SCR_WIDTH, SCR_HEIGHT;

    GLFWwindow* initRenderer();
};