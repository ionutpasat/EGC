#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }    
    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }    
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(color1, color2, color3, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes[renderedObj], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(x, y, z));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(meshes["bunny"], glm::vec3(-1, 1.5f, 0), glm::vec3(0.03f));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (window->KeyHold(GLFW_KEY_D)) {
        x += 0.3*deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        x -= 0.3*deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        z += 0.3*deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        z -= 0.3*deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        y += 0.3*deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        y -= 0.3*deltaTime;
    }

}


void Lab1::OnKeyPress(int key, int mods)
{
    srand(time(NULL));
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color1 = rand() / static_cast<float>(RAND_MAX);
        color2 = rand() / static_cast<float>(RAND_MAX);
        color3 = rand() / static_cast<float>(RAND_MAX);
    }

    if (key == GLFW_KEY_C) {
        if (next == 0) {
            renderedObj = "teapot";
            next++;
            return;
        }  
        if (next == 1) {
            renderedObj = "sphere";
            next++;
            return;
        }  
        if (next == 2) {
            renderedObj = "box";
            next = 0;
            return;
        }
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color1 = 0;
        color2 = 0;
        color3 = 0;

    }
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    x += deltaX / 100.f;
    y -= deltaY / 100.f;
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
