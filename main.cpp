#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

int WIDTH = 600;
int HEIGHT = 800;
const char* title = "Phantom Engine";

// Frame timing
float lastFrame = 0, currentFrame = 0;

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

// Function Declarations
std::string readFromFile(const char* path);
void resize_windowCB(GLFWwindow* window, int width, int height);

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // GLFW Callbacks
    glfwSetFramebufferSizeCallback(window, resize_windowCB);

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
    glm::mat4 model = glm::mat4(1);
    model = glm::rotate(model, glm::radians(-55.0f),glm::vec3(1, 0, 0));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5f));

    lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Input Processing and Rendering Here

        currentFrame = glfwGetTime();
        float delta = lastFrame - currentFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 0.4);

        model = glm::rotate(model, 10 * glm::sin(glm::radians(delta)), glm::vec3(0, 0, 1));
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)WIDTH / (float) HEIGHT, 0.01f, 100.0f);

        glUseProgram(prog);
        // Setting Uniforms
        glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, GL_FALSE, &projection[0][0]);
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

void resize_windowCB(GLFWwindow* window, int width, int height){
    WIDTH = width;
    HEIGHT= height;
    glViewport(0, 0, WIDTH, HEIGHT);
}
