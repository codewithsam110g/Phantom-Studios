#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int WIDTH = 600;
const int HEIGHT = 800;
const char* title = "Phantom Engine";

float vertices[] = {
    // Position - 3         Color - 3               TexCoords - 2
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,       0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,       0.0f, 1.0f,   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

std::string readFromFile(const char* path);

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) throw std::runtime_error("Failed to Load OpenGL Loader");

    uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
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

    uint32_t texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0); //Optional for single Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        throw std::runtime_error("Failed to Load Texture");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); For Wireframe Rendering

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // Input Processing and Rendering Here
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 0.4);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
