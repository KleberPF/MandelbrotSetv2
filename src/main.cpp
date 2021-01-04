#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

std::string readTextFile(const char* path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double scrollxoffset, double scrollyoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float xoffset = -0.5f;
float yoffset = -0.08f;
float velocity = 1.0f;

float zoomMultiplier = 1.0f;
float zoomVelocity = 3.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mandelbrot Set", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    gl3wInit();

    std::string vertexShaderSource = readTextFile("./src/shaders/shader.vert");
    std::string fragmentShaderSource = readTextFile("./src/shaders/shader.frag");

    const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
    const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[12] = 
    {
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f,  -1.0f, 0.0f
    };

    unsigned int indices[6] = 
    {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        deltaTime = (float)glfwGetTime() - lastFrame;
        lastFrame = (float)glfwGetTime();

        std::cout << 1 / deltaTime << std::endl;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "zoomMultiplier"), zoomMultiplier);
        glUniform1f(glGetUniformLocation(shaderProgram, "xoffset"), xoffset);
        glUniform1f(glGetUniformLocation(shaderProgram, "yoffset"), yoffset);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

std::string readTextFile(const char* path)
{
    std::string text;
    std::ifstream textFile(path);

    std::string line;
    while (std::getline(textFile, line))
    {
        text += line + "\n";
    }

    return text;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        yoffset += velocity * deltaTime * (1 / zoomMultiplier);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        yoffset -= velocity * deltaTime * (1 / zoomMultiplier);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        xoffset -= velocity * deltaTime * (1 / zoomMultiplier);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        xoffset += velocity * deltaTime * (1 / zoomMultiplier);
}

void scroll_callback(GLFWwindow* window, double scrollxoffset, double scrollyoffset)
{
    zoomMultiplier += (float)scrollyoffset * deltaTime * zoomVelocity * zoomMultiplier;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}