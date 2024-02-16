#include <shader.h>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // Read the shaders into memory.
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::string vertexCode;
    std::string fragmentCode;

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std ::cout << "ERROR:SHADER:COMPILE::VERTEX - " << infoLog << std ::endl;
        std ::cout << vShaderCode << std::endl;
    }

    // Initialize and compile fragment shader.
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std ::cout << "ERROR:SHADER:COMPILE::FRAGMENT - " << infoLog << std ::endl;
        std ::cout << fShaderCode << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std ::cout << "ERROR:SHADER:LINKING - " << infoLog << std ::endl;
    }

    // Shader clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // delete vShaderCode;
    // delete fShaderCode;
}

void Shader::use()
{
    glUseProgram(ID);
}
