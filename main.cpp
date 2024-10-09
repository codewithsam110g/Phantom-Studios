#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

const int WIDTH = 600;
const int HEIGHT = 800;
const char* title = "Phantom Engine";

float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

std::string readFromFile(const char* path);

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) throw std::runtime_error("Failed to Load OpenGL Loader");

    uint32_t vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    std::string vertCode, fragCode;
    vertCode = readFromFile("./Shaders/vert.glsl");
    fragCode = readFromFile("./Shaders/frag.glsl");

    const char* vc = vertCode.c_str();
    const char* fc = fragCode.c_str();

    uint32_t vert, frag, prog;
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert, 1, &vc, nullptr);
    glShaderSource(frag, 1, &fc, nullptr);

    glCompileShader(vert);
    glCompileShader(frag);

    int success;
    char infoLog[512];
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);

    if (!success)
    {
    	glGetShaderInfoLog(vert, 512, nullptr, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
    			<< infoLog << std::endl;
    }

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);

    if (!success)
    {
    	glGetShaderInfoLog(frag, 512, nullptr, infoLog);
    	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
    			<< infoLog << std::endl;
    }

    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &success);

    if (!success)
    {
    	glGetProgramInfoLog(prog, 512, nullptr, infoLog);
    	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
    			<< infoLog << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    glUseProgram(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //Processing Here and Render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 0.4);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwSwapInterval(0);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

std::string readFromFile(const char* path){
    std::string code;
    std::ifstream vertFile;
    try {
    	vertFile.open(path);
    	std::stringstream vertStream;
    	vertStream << vertFile.rdbuf();
    	code = vertStream.str();
    }catch (std::ifstream::failure e) {
        std::cout<<"Error: Shader File Not Read Successfully!"<<std::endl;
    }
    return code;
}
