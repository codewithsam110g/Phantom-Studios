#include <iostream>
#include <glad/glad.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Model.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Frame timing
float lastFrame = 0, currentFrame = 0;

bool captureMouse = false;
bool isViewportHovered = false;
bool firstMouse = true;
float lastX = 400;
float lastY = 300;

// Flags
bool closeWindow = false;

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

Camera camera(glm::vec3(0, 0, 3));

// Function Declarations
void ProcessKeyboardInput(float delta);
void ProcessMouseInput();

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    Window window(WindowProps{800, 600, "Phantom Studios"});

    // GLFW Callbacks
    glfwSetCursorPosCallback(window.getWindow(), Mouse::mousePositionCB);
    glfwSetMouseButtonCallback(window.getWindow(), Mouse::mouseButtonCB);
    glfwSetScrollCallback(window.getWindow(), Mouse::mouseScrollCB);
    glfwSetKeyCallback(window.getWindow(), Keyboard::keyboardKeyCB);

    if(!gladLoadGL()) throw std::runtime_error("Failed to Load OpenGL Loader");

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330"); // GLSL version

    Framebuffer fbo(window.getWidth(), window.getHeight());
    fbo.unbind();

    VertexArray vao;

    Buffer vbo(GL_ARRAY_BUFFER, sizeof(vertices), vertices),
    ebo(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);

    vao.setAttribPointer(0, 3, sizeof(float) * 8, (void *)(0));
    vao.setAttribPointer(1, 3, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    vao.setAttribPointer(2, 2, sizeof(float) * 8, (void *)(sizeof(float) * 6));

    vao.unbind();

    Shader shader("./Shaders/vert.glsl", "./Shaders/frag.glsl");

    Texture texture("./container.jpg");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); For Wireframe Rendering

    glm::mat4 model = glm::mat4(1);
    model = glm::rotate(model, glm::radians(-55.0f),glm::vec3(1, 0, 0));

    lastFrame = glfwGetTime();

    Shader modelShader("./Shaders/model_vert.glsl","./Shaders/model_frag.glsl");
    modelShader.unbind();
    auto tp = glfwGetTime();
    Model m1("./backpack.obj");
    auto tn = glfwGetTime();
    std::cout<<tn-tp<<std::endl;
    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();

        // Input Processing and Rendering Here

        currentFrame = glfwGetTime();
        float delta = lastFrame - currentFrame;
        lastFrame = currentFrame;

        if(!io.WantCaptureKeyboard){
            ProcessKeyboardInput(delta);
        }

        if(isViewportHovered){
            ProcessMouseInput();
        }else{
            firstMouse = true;
        }

        int focused = glfwGetWindowAttrib(window.getWindow(), GLFW_FOCUSED);
        if(!focused){
            firstMouse = true;
        }

        fbo.bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.4, 0.4, 0.4, 0.4);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, window.getWidth(), window.getHeight());

        model = glm::rotate(model, 10 * glm::sin(glm::radians(delta)), glm::vec3(0, 0, 1));
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), window.getAspectRatio(), 0.01f, 100.0f);

        shader.bind();

        // Setting Uniforms

        shader.setMat4("model", model);
        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", projection);

        vao.bind();
        texture.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        modelShader.bind();

        modelShader.setMat4("model", model);
        modelShader.setMat4("view", camera.GetViewMatrix());
        modelShader.setMat4("projection", projection);

        m1.Draw(modelShader);
        fbo.unbind();

        glDisable(GL_DEPTH_TEST);
        glClearColor(0.06, 0.06, 0.06, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiStyle& style = ImGui::GetStyle();
        // style.WindowPadding = ImVec2(16.f, 16.f);    // No padding inside the window
        // style.WindowRounding = 32.f;                 // No rounding on window corners
        // style.FramePadding = ImVec2(16.0f, 16.0f);     // No padding around frames (optional)
        style.WindowBorderSize = 8.f;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);                 // Make it the size of the GLFW window
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID); // Tie it to the main viewport

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        // Create a full-size dockspace that fills the GLFW window
        ImGui::Begin("MainDockspace", nullptr, windowFlags);
        ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();

        // Now define the 4 windows that can be docked manually

        windowFlags = ImGuiWindowFlags_NoCollapse;
        // Top-left window
        ImGui::Begin("Hierarchy", nullptr, windowFlags);
        ImGui::Text("This is window 1 (top-left).");
        ImGui::End();

        // Top-center window
        ImGui::Begin("Viewport", nullptr);

        // Check if size changed
        ImVec2 wSize = ImGui::GetContentRegionAvail();
        static ImVec2 prevSize = wSize;

        if (wSize.x != prevSize.x || wSize.y != prevSize.y) {
            prevSize = wSize;
            window.setWidthandHeight(wSize.x, wSize.y); // Update GLFW window dimensions
            fbo.resize(wSize.x, wSize.y);              // Update framebuffer size
            glViewport(0, 0, wSize.x, wSize.y);        // Update OpenGL viewport
        }

        isViewportHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        // Render to ImGui
        ImGui::Image((ImTextureID)fbo.getTexture(), wSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();


        // Top-right window
        ImGui::Begin("Inspector", nullptr, windowFlags);
        ImGui::Text("This is window 3 (top-right).");
        ImGui::End();

        // Bottom window (spans full width)
        ImGui::Begin("Assets", nullptr, windowFlags);
        ImGui::Text("This is window 4 (bottom, full width).");
        ImGui::End();

        // Render ImGui
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Mouse::update();
        Keyboard::update();

        glfwSwapBuffers(window.getWindow());
        glfwSwapInterval(0);

        if(closeWindow)
            window.setWindowClose(true);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window.getWindow());
    glfwTerminate();
    return 0;
}

void ProcessKeyboardInput(float dt){

    // Keyboard Movement
    if (Keyboard::isMultiKeyPress({GLFW_KEY_LEFT_CONTROL,GLFW_KEY_Q}))
        closeWindow = true;
    if (Keyboard::isKeyDown(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_SPACE))
        camera.ProcessKeyboard(UP, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_LEFT_CONTROL))
        camera.ProcessKeyboard(DOWN, dt);
    if(isViewportHovered && Mouse::isButtonPressed(MOUSE_MIDDLE)){
        captureMouse = !captureMouse;
    }
}

void ProcessMouseInput(){
    float x, y;
    Mouse::getCursorPosition(&x, &y);
    if(captureMouse){
        if(firstMouse){
            lastX = x;
            lastY = y;
            firstMouse = false;
        }else{
            float delX, delY;
            delX = x - lastX;
            delY = lastY - y;
            lastX = x;
            lastY = y;
            camera.ProcessMouseMovement(delX, delY);
        }
    }else{
        firstMouse = true;
        lastX = x;
        lastY = y;
    }

    float scrollY = Mouse::getScrollDelta();
    camera.ProcessMouseScroll(scrollY);
}
