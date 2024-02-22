#include <renderer.h>
#include <iostream>

Render::Render()
{
    pWindow = initRenderer();
    if (!pWindow)
    {
        std ::cout << "ERROR::Failed to init renderer." << std::endl;
    }
}

GLFWwindow *Render::initRenderer()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Model Loader", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int h, int w)
                                   { glViewport(0, 0, w, h); });

    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

void Render::Draw()
{
}