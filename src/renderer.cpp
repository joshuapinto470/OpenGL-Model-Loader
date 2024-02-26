#include <renderer.h>
#include <iostream>

namespace OGML
{
    Render::Render(unsigned W, unsigned H)
        : SCR_WIDTH(W), SCR_HEIGHT(H)
    {
        pWindow = initRenderer();
        if (!pWindow)
        {
            std::cout << "ERROR::Failed to init renderer." << std::endl;
        }
        m_model = nullptr;
    }

    GLFWwindow *Render::initRenderer()
    {
        if (!glfwInit())
        {
            std ::cout << "Failed to init GLFW\n";
            return nullptr;
        }

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
        
        glfwSetWindowUserPointer(window, static_cast<void *>(this));
        glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

        // glfwSetCursorPosCallback(window, mouse_callback);
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "ERROR::Failed to initialize GLAD" << std::endl;
            return nullptr;
        }

        return window;
    }

    void Render::Draw()
    {
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    void Render::LoadModel(const char* path) {
        m_model = new Model(path);
    }

    void Render::framebuffer_size_cb(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        Render* handler = static_cast<Render*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->SCR_HEIGHT = height;
            handler->SCR_WIDTH = width;
        }
    }
}