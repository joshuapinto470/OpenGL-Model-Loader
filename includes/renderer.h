#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <model.h>

namespace OGML {
class Render {
   public:
    Render(unsigned, unsigned);
    void LoadModel(const char *path);
    void Draw();

   private:
    GLFWwindow *pWindow;
    unsigned SCR_WIDTH, SCR_HEIGHT;
    Model *m_model;
    static void framebuffer_size_cb(GLFWwindow *, int, int);

    GLFWwindow *initRenderer();
};
}  // namespace OGML