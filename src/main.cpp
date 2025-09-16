#define M_PI 3.14159265358979323846

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define NOMINMAX           // Exclude <windows.h> min/max macros
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used services from Windows headers
#include <windows.h> // Required for AllocConsole and freopen

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Sphere.h"
#include "Axes.h"
#include "Camera.h"
#include "StateVector.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void glfw_error_callback(int error, const char* description);
void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);

// Helper function to convert Cartesian to Spherical coordinates
void cartesianToSpherical(glm::vec3 cartesian, float& theta, float& phi);

// Settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(5.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float mouseSensitivity = 0.25f;

// Bloch Sphere State
float theta = 0.0f; // Polar angle (0 to PI)
float phi = 0.0f;   // Azimuthal angle (0 to 2*PI)
float line_thickness = 1.0f;

int main()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Program started." << std::endl << std::flush;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);
    std::cout << "GLFW initialized." << std::endl << std::flush;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bloch Sphere", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "GLFW window created." << std::endl;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "GLAD initialized." << std::endl;

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    std::cout << "OpenGL state configured." << std::endl << std::flush;

    // Enable OpenGL debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // build and compile our shader programs
    // -------------------------------------
    Shader sphereShader("C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/vertex.vert", "C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/fragment.frag");
    Shader axesShader("C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/axes.vert", "C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/axes.frag");
    Shader stateVectorShader("C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/state_vector.vert", "C:/Users/ASD/Desktop/Bloch Sphere/Bloch-Sphere/resources/state_vector.frag");
    std::cout << "Shaders compiled." << std::endl;

    // create the objects
    // ------------------
    Sphere sphere(1.0f, 8, 8);
    Axes axes(1.5f);
    StateVector stateVector;
    std::cout << "Objects created." << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    std::cout << "ImGui context created." << std::endl;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "ImGui backends initialized." << std::endl << std::flush;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // create transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 pvMatrix = projection * view;

        // render the sphere
        sphereShader.use();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        sphereShader.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        sphere.draw();

        // render the axes
        axes.draw(axesShader, view, projection, line_thickness);

        // render the state vector
        stateVector.update(theta, phi);
        stateVector.draw(stateVectorShader, view, projection);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Calculate Dirac notation
        float alpha_real = cos(glm::radians(theta) / 2.0f);
        float beta_real = sin(glm::radians(theta) / 2.0f) * cos(glm::radians(phi));
        float beta_imag = sin(glm::radians(theta) / 2.0f) * sin(glm::radians(phi));

        // ImGui State Display
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, 0), ImGuiCond_Always, ImVec2(0.5f, 0));
        ImGui::Begin("State Display", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        ImGui::Text("|psi> = (%.2f)|0> + (%.2f + %.2fi)|1>", alpha_real, beta_real, beta_imag);
        ImGui::End();

        ImGui::Begin("Bloch Sphere Controls");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::SliderFloat("Theta (deg)", &theta, 0.0f, 180.0f)) {
            stateVector.hidePrevious();
        }
        if (ImGui::SliderFloat("Phi (deg)", &phi, 0.0f, 360.0f)) {
            stateVector.hidePrevious();
        }

        ImGui::SliderFloat("Mouse Sensitivity", &mouseSensitivity, 0.01f, 1.0f);

        ImGui::Separator();
        ImGui::Text("Apply Pauli Gates");
        if (ImGui::Button("Pauli X"))
        {
            stateVector.storePreviousState();
            // Convert current spherical to Cartesian
            float currentRadTheta = glm::radians(theta);
            float currentRadPhi = glm::radians(phi);
            glm::vec3 currentCartesian;
            currentCartesian.x = sin(currentRadTheta) * cos(currentRadPhi);
            currentCartesian.y = cos(currentRadTheta);
            currentCartesian.z = sin(currentRadTheta) * sin(currentRadPhi);

            // Apply Pauli X transformation (x, y, z) -> (x, -y, -z)
            glm::vec3 transformedCartesian = glm::vec3(currentCartesian.x, -currentCartesian.y, -currentCartesian.z);

            // Convert back to spherical
            cartesianToSpherical(transformedCartesian, theta, phi);

            // Convert back to degrees for display
            theta = glm::degrees(theta);
            phi = glm::degrees(phi);
        }
        if (ImGui::Button("Pauli Y"))
        {
            stateVector.storePreviousState();
            // Convert current spherical to Cartesian
            float currentRadTheta = glm::radians(theta);
            float currentRadPhi = glm::radians(phi);
            glm::vec3 currentCartesian;
            currentCartesian.x = sin(currentRadTheta) * cos(currentRadPhi);
            currentCartesian.y = cos(currentRadTheta);
            currentCartesian.z = sin(currentRadTheta) * sin(currentRadPhi);

            // Apply Pauli Z transformation (x, y, z) -> (-x, -y, z)
            glm::vec3 transformedCartesian = glm::vec3(-currentCartesian.x, -currentCartesian.y, currentCartesian.z);

            // Convert back to spherical
            cartesianToSpherical(transformedCartesian, theta, phi);

            // Convert back to degrees for display
            theta = glm::degrees(theta);
            phi = glm::degrees(phi);
        }
        if (ImGui::Button("Pauli Z"))
        {
            stateVector.storePreviousState();
            // Convert current spherical to Cartesian
            float currentRadTheta = glm::radians(theta);
            float currentRadPhi = glm::radians(phi);
            glm::vec3 currentCartesian;
            currentCartesian.x = sin(currentRadTheta) * cos(currentRadPhi);
            currentCartesian.y = cos(currentRadTheta);
            currentCartesian.z = sin(currentRadTheta) * sin(currentRadPhi);

            // Apply Pauli Y transformation (x, y, z) -> (-x, y, -z)
            glm::vec3 transformedCartesian = glm::vec3(-currentCartesian.x, currentCartesian.y, -currentCartesian.z);

            // Convert back to spherical
            cartesianToSpherical(transformedCartesian, theta, phi);

            // Convert back to degrees for display
            theta = glm::degrees(theta);
            phi = glm::degrees(phi);
        }

        ImGui::End();

        ImGui::Begin("Axis Controls");
        ImGui::SliderFloat("Axis Thickness", &line_thickness, 1.0f, 10.0f);
        static float imgui_scale = 1.0f;
        if (ImGui::SliderFloat("ImGui Scale", &imgui_scale, 0.5f, 2.0f)) {
            ImGui::GetIO().FontGlobalScale = imgui_scale;
        }
        ImGui::End();

        // Render axis labels
        glm::mat4 identityModel = glm::mat4(1.0f);
        glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);

        auto draw_label_if_visible = [&](const char* id, const glm::vec3& pos_3d, const char* text) {
            glm::vec3 screen_pos = glm::project(pos_3d, identityModel, pvMatrix, viewport);
            if (screen_pos.z < 1.0f) {
                ImGui::SetNextWindowPos(ImVec2(screen_pos.x, screen_pos.y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin(id, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
                ImGui::Text(text);
                ImGui::End();
            }
        };

        draw_label_if_visible("X Label", glm::vec3(1.5f, 0.0f, 0.0f), "(|0> - |1>)/sqrt(2)");
        draw_label_if_visible("Y Label", glm::vec3(0.0f, 1.5f, 0.0f), "|1>");
        draw_label_if_visible("Z Label", glm::vec3(0.0f, 0.0f, 1.5f), "(|0> - i|1>)/sqrt(2)");
        draw_label_if_visible("Neg X Label", glm::vec3(-1.5f, 0.0f, 0.0f), "(|0> + |1>)/sqrt(2)");
        draw_label_if_visible("Neg Y Label", glm::vec3(0.0f, -1.5f, 0.0f), "|0>");
        draw_label_if_visible("Neg Z Label", glm::vec3(0.0f, 0.0f, -1.5f), "(|0> + i|1>)/sqrt(2)");

        ImGui::Render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GLFW error callback
void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY; // Corrected: y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    // Only process mouse movement if left mouse button is held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        camera.ProcessMouseMovement(xoffset, yoffset, mouseSensitivity);
    }
}

// Helper function to convert Cartesian to Spherical coordinates
void cartesianToSpherical(glm::vec3 cartesian, float& theta, float& phi)
{
    // Ensure the vector is normalized for unit sphere
    cartesian = glm::normalize(cartesian);

    // Calculate theta (polar angle)
    theta = glm::acos(cartesian.y); // acos(z) for standard physics convention, acos(y) for OpenGL/math convention

    // Calculate phi (azimuthal angle)
    phi = glm::atan(cartesian.z, cartesian.x); // atan2(y, x) for standard math, atan2(z, x) for OpenGL/math convention

    // Adjust phi to be in [0, 2*PI)
    if (phi < 0.0f)
        phi += 2.0f * M_PI;
}