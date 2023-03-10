#include "lab_m1/Tema3/Tema3.h"
#include "lab_m1/Tema3/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    camera = new implemented::HwCamera();
    camera->Set(glm::vec3(0, 3.5f, 7.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    camera->RotateThirdPerson_OX(-M_PI / 100);
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "black.jpg").c_str(), GL_REPEAT);
        mapTextures["black"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "tree.jpg").c_str(), GL_REPEAT);
        mapTextures["tree"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "trunk.jpg").c_str(), GL_REPEAT);
        mapTextures["trunk"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.jpg").c_str(), GL_REPEAT);
        mapTextures["stone"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "present.jpg").c_str(), GL_REPEAT);
        mapTextures["present"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pole.jpg").c_str(), GL_REPEAT);
        mapTextures["pole"] = texture;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("snowmobile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "snowmobile.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    srand(time(NULL));
    fov = 60.0f;
    left = -15.0f;
    right = 15.0f;
    up = 15.0f;
    down = -15.0f;

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema3::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(-0.2f, 0.1f, 0);
        modelMatrix *= transform3D::Scale(0.0075f, 0.0075f, 0.0075f);
        modelMatrix *= transform3D::RotateOX(-M_PI / 3);
        modelMatrix *= transform3D::RotateOZ(atan2(crossY, crossX));
        RenderSimpleMesh(meshes["snowmobile"], shaders["LabShader"], modelMatrix, mapTextures["black"]);
    }

    RenderTree(-5, 3, -2);
    RenderTree(-2, 2, -2);
    RenderTree(3, 2, -2);
    RenderTree(6, 3, -2);
    RenderTree(-3, 0, 2);
    RenderTree(2, 0, 2);

    RenderStone(-4, 2, -1.8f);
    RenderStone(-1, 1, -1.2f);
    RenderStone(3, 1, -1.5f);
    RenderStone(6, 2, -1.9f);
    RenderStone(-3, -1.0f, 2.5f);
    RenderStone(2, -1.0f, 2.8f);

    RenderPresent(0, 2, -2);
    RenderPresent(0, -1, 2.5f);
    RenderPresent(-5.5f, -0.05f, 2.f);
    RenderPresent(6, -0.05f, 1.5f);

    RenderPole(-5, 0, -1);
    RenderPole(6.5f, 0, -0.7f);


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 0, 0);
        modelMatrix *= transform3D::Scale(0.7f, 0.7f, 0.7f);
        modelMatrix *= transform3D::RotateOX(M_PI / 6);
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["snow"]);
    }
}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{

    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    if (texture2)
    {

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
        glUniform1i(glGetUniformLocation(shader->program, "mixT"), 1);
    }
    else {
        glUniform1i(glGetUniformLocation(shader->program, "mixT"), 0);
    }

    if (texture1 == mapTextures["snow"]) {
        glUniform1f(glGetUniformLocation(shader->program, "elapsedTime"), Engine::GetElapsedTime());
    }
    else {
        glUniform1f(glGetUniformLocation(shader->program, "elapsedTime"), 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->TranslateForward(deltaTime * cameraSpeed);

    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateThirdPerson_OY(sensivityOY * 400 * deltaTime);

    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->TranslateForward(-deltaTime * cameraSpeed);

    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateThirdPerson_OY(-sensivityOY * 400 * deltaTime);

    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-deltaTime * cameraSpeed);

    }

    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(deltaTime * cameraSpeed);

    }
}


void Tema3::OnKeyPress(int key, int mods)
{
}


void Tema3::OnKeyRelease(int key, int mods)
{
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);

        }
    }
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}

void Tema3::RenderTree(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(0.1f, 0.8f, 0.1f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["trunk"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y + 0.85f, z);
        modelMatrix *= transform3D::Scale(0.45f, 0.45f, 0.45f);
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["tree"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y + 1.25f, z);
        modelMatrix *= transform3D::Scale(0.3f, 0.35f, 0.3f);
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["tree"]);
    }
}

void Tema3::RenderStone(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"]);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x - 0.15f, y, z);
        modelMatrix *= transform3D::Scale(0.35f, 0.35f, 0.35f);
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"]);
    }
}

void Tema3::RenderPresent(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(0.6f, 0.6f, 0.6f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["present"]);
    }
}

void Tema3::RenderPole(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(0.25f, 7.f, 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pole"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y + 3.5f, z);
        modelMatrix *= transform3D::RotateOZ(M_PI / 2);
        modelMatrix *= transform3D::Scale(0.25f, 2.5f, 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pole"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x - 1.125f, y + 3.25f, z);
        modelMatrix *= transform3D::RotateOZ(M_PI / 2);
        modelMatrix *= transform3D::Scale(0.25f, 0.25f, 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pole"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x + 1.125f, y + 3.25f, z);
        modelMatrix *= transform3D::RotateOZ(M_PI / 2);
        modelMatrix *= transform3D::Scale(0.25f, 0.25f, 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pole"]);
    }
}
