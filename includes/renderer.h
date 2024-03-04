#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <model.h>
#include <camera.h>
#include <shader.h>

namespace OGML {
class Render {
   public:
    Render(unsigned, unsigned);
    ~Render();
    void LoadModel(const char *path);
    void AttachCamera(Camera *);
    void AttachShader(Shader);
    void Draw();

   private:
    GLFWwindow *pWindow;
    unsigned SCR_WIDTH, SCR_HEIGHT;
    Model *m_model;
    Camera *m_camera;
    Shader m_shader;

    static void framebuffer_size_cb(GLFWwindow *, int, int);
    static void mouse_cb(GLFWwindow *, double, double);

    bool mouseEnable = true;
    void processInput();
    GLFWwindow *initRenderer();
};

}  // namespace OGML