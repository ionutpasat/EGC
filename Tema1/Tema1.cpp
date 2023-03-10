#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    window->HidePointer();

    glm::vec3 corner = glm::vec3(0, 0, 0); //in case of a circle it represents its center
    side = 80;
    recside = 100;
    radius = 50;
    elapsedTime = 0;
    totElapsedTime = 0;

    // corner
    cx = corner.x;
    cy = corner.y;

    // center of rectangle
    reccx = corner.x + recside;
    reccy = corner.y + recside / 2;

    //center of the equilateral triangle
    eqcx = corner.x + side / 2;
    eqcy = corner.y + side / sqrt(3);

    // random left or right movement at the start
    int leftOrRight = rand() % (2);
    if (leftOrRight) {
        movingLeftRight = true;
    }
    else {
        movingLeftRight = false;
    }

    // random start position below the grass
    srand(time(NULL));
    txI = tx1 = 10 + (rand() % 1100);
    tyI = ty1 = 20;
    xSpeed = 80 + (rand() % 140);
    ySpeed = 80 + (rand() % 140);

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, 100, glm::vec3(0.1f, 0.3f, 0.7f), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, 100, glm::vec3(0.33f, 0.49f, 0.27f), true);
    AddMeshToList(square2);

    Mesh* beak = object2D::CreateRectangle("beak", corner, 20, 20, glm::vec3(1, 1, 0), true);
    AddMeshToList(beak);

    Mesh* equilateralTriangle= object2D::CreateEquilateralTriangle("equilateralTriangle", corner, side, glm::vec3(0.588f, 0.294f, 0), true);
    AddMeshToList(equilateralTriangle);

    Mesh* isoscelesTriangle= object2D::CreateIsoscelesTriangle("isoscelesTriangle", corner, side, glm::vec3(1, 0, 1), true);
    AddMeshToList(isoscelesTriangle);

    Mesh* circle = object2D::CreateCircle("circle", corner, radius, glm::vec3(0.588f, 0.294f, 0), true);
    AddMeshToList(circle);

    Mesh* hitbox = object2D::CreateCircle("hitbox", corner, 85, glm::vec3(1, 0, 1), true);
    AddMeshToList(hitbox);

    Mesh* head = object2D::CreateCircle("head", corner, 25, glm::vec3(0.32f, 0.4f, 0.36f), true);
    AddMeshToList(head);

    Mesh* rectangle = object2D::CreateRectangle("rectangle", corner, recside, recside, glm::vec3(1, 0, 1), true);
    AddMeshToList(rectangle);

    Mesh* crosshair = object2D::CreateCrossHair("crosshair", corner, 5, glm::vec3(1, 0, 0));
    AddMeshToList(crosshair);

    Mesh* crosshairAxis = object2D::CreateRectangle("crosshairAxis", corner, 20, 4, glm::vec3(1, 1, 1), true);
    AddMeshToList(crosshairAxis);

    Mesh* bullet = object2D::CreateCircle("bullet", corner, 10, glm::vec3(1, 1, 0), true);
    AddMeshToList(bullet);

    Mesh* halfHeart = object2D::CreateCircle("halfHeart", corner, 10, glm::vec3(1, 0, 1), true);
    AddMeshToList(halfHeart);

    Mesh* scoreRec = object2D::CreateRectangle("scoreRec", corner, 12, 6, glm::vec3(0, 0, 0), true);
    AddMeshToList(scoreRec);

    Mesh* finMesRec = object2D::CreateRectangle("finMesRec", corner, 12, 6, glm::vec3(0, 0, 0), true);
    AddMeshToList(finMesRec);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(bgRed, bgGreen, bgBlue, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


// every part of the duck is corelated to the body's center 
void Tema1::RenderLivesAndBullets(int lives, int bullets) {
    // bullets
    for (int i = 0; i < bullets; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(25 + 40 * i, 630);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Scale(2, 0.85f);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }
    // bullets

    // lives
    for (int i = 0; i < lives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(20 + 40 * i, 680);
        modelMatrix *= transform2D::Rotate(-M_PI / 2.8f);
        modelMatrix *= transform2D::Scale(1.85f, 0.85f);
        RenderMesh2D(meshes["halfHeart"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(31 + 40 * i, 680);
        modelMatrix *= transform2D::Rotate(M_PI / 2.8f);
        modelMatrix *= transform2D::Scale(1.85f, 0.85f);
        RenderMesh2D(meshes["halfHeart"], shaders["VertexColor"], modelMatrix);
    }
    // lives
}

void Tema1::RenderCrossHair() {
    // crosshair
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(crossX, crossY);
    RenderMesh2D(meshes["crosshair"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(crossX, crossY);
    modelMatrix *= transform2D::Translate(-20, -2);
    RenderMesh2D(meshes["crosshairAxis"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(crossX, crossY);
    modelMatrix *= transform2D::Rotate(M_PI / 2);
    modelMatrix *= transform2D::Translate(-20, -2);
    RenderMesh2D(meshes["crosshairAxis"], shaders["VertexColor"], modelMatrix);
    //crosshair
}

void Tema1::RenderGrass() {
    // grass
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(20, 2);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderHead(bool escape, bool shot, int x, int y) {
    // head
    if (escape) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(cx, cy + 80);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
        return;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(rotAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    modelMatrix *= transform2D::Translate(cx + 80, cy);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderBeak(bool escape, bool shot, int x, int y) {
    // beak
    if (escape) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(cx + -10 , cy + 80);
        modelMatrix *= transform2D::Translate(10 , 10);
        modelMatrix *= transform2D::Rotate(M_PI/2);
        modelMatrix *= transform2D::Translate(-10 , -10);
        RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
        return;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(rotAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    modelMatrix *= transform2D::Translate(cx + 80, cy - 10);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderWing(bool escape, bool shot, int x, int y) {
    // wing
    float newAngle;
    if (escape) {
        if (escape)
            newAngle = M_PI / 2;
        else
            newAngle = -M_PI / 2;
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(cx - 5, cy);
        if (rotateLeftRight)
            modelMatrix *= transform2D::Rotate(-angularStep);
        else
            modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(-side * sqrt(3) / 2, 0);
        modelMatrix *= transform2D::Rotate(newAngle);
        modelMatrix *= transform2D::Translate(-side / 2, -side * sqrt(3) / 2);
        modelMatrix *= transform2D::Scale(1.0f, 1.2f);
        RenderMesh2D(meshes["equilateralTriangle"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(cx + 5, cy);
        if (rotateLeftRight)
            modelMatrix *= transform2D::Rotate(angularStep);
        else
            modelMatrix *= transform2D::Rotate(-angularStep);
        modelMatrix *= transform2D::Translate(side * sqrt(3) / 2, 0);
        modelMatrix *= transform2D::Rotate(-newAngle);
        modelMatrix *= transform2D::Translate(-side / 2, -side * sqrt(3) / 2);
        modelMatrix *= transform2D::Scale(1.0f, 1.2f);
        RenderMesh2D(meshes["equilateralTriangle"], shaders["VertexColor"], modelMatrix);
        return;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(rotAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    modelMatrix *= transform2D::Translate(cx , cy + 5);
    modelMatrix *= transform2D::Rotate(-angularStep);
    modelMatrix *= transform2D::Translate(-eqcx, 0);
    modelMatrix *= transform2D::Scale(1.0f, 1.2f);
    RenderMesh2D(meshes["equilateralTriangle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(rotAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    modelMatrix *= transform2D::Translate(cx , cy - 5);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(0, -side * sqrt(3) / 2);
    modelMatrix *= transform2D::Rotate(M_PI);
    modelMatrix *= transform2D::Translate(-side / 2, -side * sqrt(3) / 2);
    modelMatrix *= transform2D::Scale(1.0f, 1.2f);
    RenderMesh2D(meshes["equilateralTriangle"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderBody(bool escape, bool shot, float tx, float ty) {
    // body
    if (escape) {
        rotAngle = M_PI / 2;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tx, ty);
    modelMatrix *= transform2D::Rotate(rotAngle);
    modelMatrix *= transform2D::Scale(1.5f, 0.8f);
    cx = tx; cy = ty;
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FlashScreen(bool flash) {
    if (flash) {
        bgRed = 1; bgGreen = 0; bgBlue = 0;
    }
    else {
        bgRed = 0.1f; bgGreen = 0.3f; bgBlue = 0.7f;
    }
}

void Tema1::CheckFlash(float elapsedTime) {
    // flashing the bg
    if (elapsedTime <= 0.1f)
        FlashScreen(true);
    if (elapsedTime >= 0.2f && elapsedTime <= 0.3f)
        FlashScreen(false);
}

void Tema1::DuckEscape() {
    RenderLivesAndBullets(lives, bullets);
    RenderCrossHair();
    RenderGrass();
    RenderHead(true, false, 0, 0);
    RenderBeak(true, false, 0, 0);
    RenderWing(true, false, 0, 0);
    RenderBody(true, false, tx1, ty1);
}

void Tema1::DuckHit() {
    RenderLivesAndBullets(lives, bullets);
    RenderCrossHair();
    RenderGrass();
    RenderHead(false, true, 0, 0);
    RenderBeak(false, true, 0, 0);
    RenderWing(false, true, 0, 0);
    RenderBody(false, true, tx1, ty1);
}

void Tema1::GameOver() {
    // g
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(500, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(490, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(510, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(500, 340);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(510, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(500, 320);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }

    // a
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(530, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(540, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(530, 340);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(520, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(540, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(530, 320);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }
     // m
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(560, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(550, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(570, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(550, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(570, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(580, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(570, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(590, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(570, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(590, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

    }

    // e
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(610, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(600, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(620, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(610, 340);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(600, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(610, 320);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix); 
    }

    // o
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(670, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(660, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(680, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(660, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(680, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(670, 320);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }

    // v
    {

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(695, 340);
        modelMatrix *= transform2D::Scale(0.9f, 2.1f);
        modelMatrix *= transform2D::Rotate(-M_PI / 3.2f);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(705, 340);
        modelMatrix *= transform2D::Scale(0.9f, 2.1f);
        modelMatrix *= transform2D::Rotate(M_PI / 3.2f);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }

    // e
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(730, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(720, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(740, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(730, 340);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(720, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(730, 320);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }

    // r
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(760, 360);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750, 350);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750, 330);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Translate(-12, -3);
        RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
    }

    if (score >= 100) {
        // y
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(520, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(540, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(530, 280);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(540, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(530, 260);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
        }

        // o
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(560, 300);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(550, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(570, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(550, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(570, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(560, 260);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
        }

        // u
        {

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(580, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(600, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(580, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(600, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(590, 260);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
        }

        // w
        {

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(630, 280);
            modelMatrix *= transform2D::Scale(0.9f, 2.1f);
            modelMatrix *= transform2D::Rotate(-M_PI / 3.2f);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(640, 280);
            modelMatrix *= transform2D::Scale(0.9f, 2.1f);
            modelMatrix *= transform2D::Rotate(M_PI / 3.2f);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(650, 280);
            modelMatrix *= transform2D::Scale(0.9f, 2.1f);
            modelMatrix *= transform2D::Rotate(-M_PI / 3.2f);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(660, 280);
            modelMatrix *= transform2D::Scale(0.9f, 2.1f);
            modelMatrix *= transform2D::Rotate(M_PI / 3.2f);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
        }

        // i
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(678, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(678, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);
        }

        // n
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(700, 300);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(690, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(710, 290);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(690, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(710, 270);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["finMesRec"], shaders["VertexColor"], modelMatrix);

        }

    }

    RenderScore(score);
    RenderCrossHair();
    RenderGrass();

}

void Tema1::RenderScore(int score) {

    if (score == 5) {
        third = 5;
        second = 0;
        first = 0;
    }

    if (score < 100 && score >= 10) {
        third = score % 10;
        second = (score / 10) % 10;
        first = 0;
    }

    if (score < 1000 && score >= 100) {
        third = score % 10;
        second = (score / 10) % 10;
        first = (score / 100) % 10;
    }

    {
        //first number
        if (first == 0 || first == 2 || first == 3 || first == 5 || first == 6 || first == 7 || first == 8 || first == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1060, 700);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (first == 0 || first == 4 || first == 5 || first == 6 || first == 8 || first == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1050, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (first == 0 || first == 1 || first == 2 || first == 3 || first == 4 || first == 7 || first == 8 || first == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1070, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        // middle segment
        if (first == 8 || first == 4 || first == 5 || first == 2 || first == 6 || first == 9 || first == 3) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1060, 680);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (first == 0 || first == 2 || first == 6 || first == 8) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1050, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (first == 0 || first == 1 || first == 3 || first == 4 || first == 5 || first == 6 || first == 7 || first == 8 || first == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1070, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (first == 0 || first == 2 || first == 3 || first == 5 || first == 6 || first == 8 || first == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1060, 660);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        //second number
        if (second == 0 || second == 2 || second == 3 || second == 5 || second == 6 || second == 7 || second == 8 || second == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1100, 700);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (second == 0 || second == 4 || second == 5 || second == 6 || second == 8 || second == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1090, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (second == 0 || second == 1 || second == 2 || second == 3 || second == 4 || second == 7 || second == 8 || second == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1110, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        // middle segment
        if (second == 8 || second == 4 || second == 5 || second == 2 || second == 6 || second == 9 || second == 3) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1100, 680);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (second == 0 || second == 2 || second == 6 || second == 8) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1090, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (second == 0 || second == 1 || second == 3 || second == 4 || second == 5 || second == 6 || second == 7 || second == 8 || second == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1110, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (second == 0 || second == 2 || second == 3 || second == 5 || second == 6 || second == 8 || second == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1100, 660);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        //third number
        if (third == 0 || third == 2 || third == 3 || third == 5 || third == 6 || third == 7 || third == 8 || third == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1140, 700);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (third == 0 || third == 4 || third == 5 || third == 6 || third == 8 || third == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1130, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (third == 0 || third == 1 || third == 2 || third == 3 || third == 4 || third == 7 || third == 8 || third == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1150, 690);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        // middle segment
        if (third == 8 || third == 4 || third == 5 || third == 2 || third == 6 || third == 9 || third == 3) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1140, 680);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (third == 0 || third == 2 || third == 6 || third == 8) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1130, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (third == 0 || third == 1 || third == 3 || third == 4 || third == 5 || third == 6 || third == 7 || third == 8 || third == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1150, 670);
            modelMatrix *= transform2D::Rotate(M_PI / 2);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }

        if (third == 0 || third == 2 || third == 3 || third == 5 || third == 6 || third == 8 || third == 9) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1140, 660);
            modelMatrix *= transform2D::Translate(-12, -3);
            RenderMesh2D(meshes["scoreRec"], shaders["VertexColor"], modelMatrix);
        }
    }

}

void Tema1::Update(float deltaTimeSeconds)
{
    // check if game over
    if (lives <= 0 || score >= 100) {
        GameOver();
        return;
    }

    // rendering the score
    RenderScore(score);

    // wings rotation step 
    if (angularStep > M_PI / 3) {
        wingStep = false;
    }
    else if (angularStep <= -M_PI / 12) {
        wingStep = true;
    }

    // when going down wing rotation speed is slower to simulate floating like flight
    if (totElapsedTime <= 5.0f && !isHit) {
        if (!movingUpDown)
            wingSpeed = 2;
        else
            wingSpeed = 4;
    }

    // changing the angle that the wings are rotating
    if (wingStep)
        angularStep += wingSpeed * deltaTimeSeconds;
    else
        angularStep -= wingSpeed * deltaTimeSeconds;

    // touch right barrier
    if (tx1 >= 1280) {
        if (movingUpDown)
            rotAngle = M_PI - rotAngle;
        else
            rotAngle = 3 * M_PI - rotAngle;
        movingLeftRight = false;
        rotateLeftRight = false;
    }
    // touch left barrier
    else if (tx1 <= 0) {
        if (movingUpDown)
            rotAngle = M_PI - rotAngle;
        else
            rotAngle = 3 * M_PI - rotAngle;
        rotateLeftRight = true;
        movingLeftRight = true;
    }

    // touch upper barrier
    if (ty1 >= 720) {
        if (movingLeftRight)
            rotAngle = 2 * M_PI - rotAngle;
        else
            rotAngle = 2 * M_PI - rotAngle;
        movingUpDown = false;
    }
    // touch lower barrier
    else if (ty1 <= 0) {
        if (movingLeftRight)
            rotAngle = 2 * M_PI - rotAngle;
        else
            rotAngle = 2 * M_PI - rotAngle;
        movingUpDown = true;
    }

    // while not escaped, make the duck move
    if (totElapsedTime <= 5.0f && bullets != 0) {
        if (!isHit) {
            if (movingLeftRight)
                tx1 += ((int)xSpeed * deltaTimeSeconds + (int)speedInc);
            else
                tx1 -= ((int)xSpeed * deltaTimeSeconds + (int)speedInc);
            if (movingUpDown)
                ty1 += ((int)ySpeed * deltaTimeSeconds + (int)speedInc);
            else
                ty1 -= ((int)ySpeed * deltaTimeSeconds + (int)speedInc);
        }
        else {
            // if hit, make the duck fall
            ty1 -= (int)200 * deltaTimeSeconds;
            // wings don t rotate anymore
            wingSpeed = 0;
        }
    }
    else {
        // if escaped, fly up (away) | can still be shot
        wingSpeed = 6;
        ty1 += ((int)ySpeed * deltaTimeSeconds + (int)speedInc);
    }
    
    // get first angle by using arctan in the right triangle made by xf, yf, xi, yi
    if (getRotAngle) {
        rotAngle = atan(abs(ty1 - tyI) / abs(tx1 - txI));
        if (!movingLeftRight)
            rotAngle = M_PI - rotAngle;
        getRotAngle = false;
    }

    // if not hit
    if (!isHit) {
        totElapsedTime += deltaTimeSeconds;
        if (totElapsedTime >= 5.0f || bullets == 0) {
            // render duck getting away
            DuckEscape();
            //if escape successfull reset all params so next duck can come
            if (cy >= 820) {
                getRotAngle = true;
                totElapsedTime = 0;
                txI = tx1 = 10 + (rand() % 1100);
                tyI = ty1 = 20;
                int leftOrRight = rand() % (2);
                if (leftOrRight) {
                    movingLeftRight = true;
                }
                else {
                    movingLeftRight = false;
                }
                lives--;
                if (lives == 0)
                    GameOver();
                bullets = 3;
                duckCount++;
                // from 5 to 5 ducks their speed increases
                if (duckCount % 5 == 0)
                    speedInc += 2.0f;
                movingUpDown = true;
            }
        }
        else {
            //if still alive
            RenderLivesAndBullets(lives, bullets);
            RenderCrossHair();
            RenderGrass();
            RenderHead(false, false, 80, 5);
            RenderBeak(false, false, 90, 15);
            RenderWing(false, false, 40, 30);
            RenderBody(false, false, tx1, ty1);
        }
    }

    // if shot
    if (isHit) {
        // flash the screen red
        CheckFlash(elapsedTime);
        elapsedTime += deltaTimeSeconds;
        // rotate duck while falling
        rotAngle += 6 * deltaTimeSeconds;
        totElapsedTime = 0;
        DuckHit();
        // when dissapeared of the screen, reset all params for the next duck to come
        if (cy <= 0) {
            score += 5;
            getRotAngle = true;
            txI = tx1 = 10 + (rand() % 1100);
            tyI = ty1 = 20;
            isHit = false;
            elapsedTime = 0;
            int leftOrRight = rand() % (2);
            if (leftOrRight) {
                movingLeftRight = true;
            }
            else {
                movingLeftRight = false;
            }
            movingUpDown = true;
        }
    }

    //// shootable area | to be deleted
    //modelMatrix = glm::mat3(1); 
    //if (rotateLeftRight)
    //    modelMatrix *= transform2D::Translate(cx, cy);
    //else
    //    modelMatrix *= transform2D::Translate(cx, cy);

    //RenderMesh2D(meshes["hitbox"], shaders["VertexColor"], modelMatrix);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    crossX = mouseX * 1280 / resolution.x;
    crossY = 720 - (mouseY * 720 /resolution.y);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (bullets > 0) {
        glm::ivec2 resolution = window->GetResolution();
        // Add mouse button press event
        int distance = sqrt(pow(((mouseX * 1280 / resolution.x) - cx), 2) + pow((720 - (mouseY * 720 / resolution.y) - cy), 2));
        // distance of the shooting point to the center of the shooting area must be lower than the shootings area radius
        if (distance < 80) {
            duckCount++;
            if (duckCount % 5 == 0)
                speedInc += 2.0f;
            isHit = true;
            bullets = 3;
        }
        if (!isHit)
            bullets--;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
