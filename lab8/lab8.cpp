#include "lab_m1/lab8/lab8.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab8::Lab8()
{
}


Lab8::~Lab8()
{
}


void Lab8::Init()
{
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

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        //first light source
        l_positions.push_back(glm::vec3(0, 1, 1));
        l_directions.push_back(glm::vec3(0, -1, 0));

        //second light source
        l_positions.push_back(glm::vec3(1, 1, 1));
        l_directions.push_back(glm::vec3(0, -1, 0));

        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;

        spotlights.push_back(0);
        spotlights.push_back(0);
        currentLightSource = 0;

        for (int i = 0; i < l_positions.size(); i++) {
            angles.push_back(std::make_pair(0, 0));
            cutOffAngles.push_back(30.0f);
        }
    }
}


void Lab8::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab8::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 1));

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.5f, 0.5f));

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.25, 0.75, 0.75));
    }

    // Render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.8f, 0));
    }

    // Render the point light in the scene
    {
        for (int i = 0; i < l_positions.size(); i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, l_positions[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
        }
    }
}


void Lab8::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    int lightsCount = glGetUniformLocation(shader->program, "light_count");
    glUniform1i(lightsCount, (int)l_positions.size());

    GLuint location = glGetUniformLocation(shader->program, "light_pos");
    glUniform3fv(location, l_positions.size(), glm::value_ptr(l_positions[0]));

    GLuint direction = glGetUniformLocation(shader->program, "light_dir");
    glUniform3fv(direction, l_directions.size(), glm::value_ptr(l_directions[0]));

    // Get the location of the uniform variable in the shader
    GLint spotlight = glGetUniformLocation(shader->program, "spotlights");
    glUniform1iv(spotlight, spotlights.size(), spotlights.data());

    GLfloat cut_off = glGetUniformLocation(shader->program, "cut_off_angles");
    glUniform1fv(cut_off, cutOffAngles.size(), cutOffAngles.data());

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab8::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) l_positions[currentLightSource] -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) l_positions[currentLightSource] -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) l_positions[currentLightSource] += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) l_positions[currentLightSource] += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) l_positions[currentLightSource] += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) l_positions[currentLightSource] -= up * deltaTime * speed;

        // TODO(student): Set any other keys that you might need
        if (window->KeyHold(GLFW_KEY_I)) angles[currentLightSource].first += deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_K)) angles[currentLightSource].first -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_J)) angles[currentLightSource].second -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_L)) angles[currentLightSource].second += deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_N)) cutOffAngles[currentLightSource] -= deltaTime * 10.f;
        if (window->KeyHold(GLFW_KEY_M)) cutOffAngles[currentLightSource] += deltaTime * 10.f;

        glm::mat4 rotMat = glm::mat4(1);
        rotMat = glm::rotate(rotMat, angles[currentLightSource].first, glm::vec3(1, 0, 0));
        rotMat = glm::rotate(rotMat, angles[currentLightSource].second, glm::vec3(0, 0, 1));

        l_directions[currentLightSource] = glm::vec3(rotMat * glm::vec4(glm::vec3(0, -1, 0), 1));
    }
}

void Lab8::OnKeyPress(int key, int mods)
{
    // Add key press event

    // TODO(student): Set keys that you might need
    if (key == GLFW_KEY_0) {
        currentLightSource++;
        if (currentLightSource >= l_positions.size()) {
            currentLightSource = 0;
        }
    }

    if (key == GLFW_KEY_F) {
        if (spotlights[currentLightSource] == 1) {
            spotlights[currentLightSource] = 0;
        }
        else {
            spotlights[currentLightSource] = 1;
        }
    }
}


void Lab8::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab8::OnWindowResize(int width, int height)
{
}
