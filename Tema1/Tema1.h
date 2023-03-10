#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderLivesAndBullets(int lives, int bullets);
        void RenderCrossHair();
        void RenderGrass();
        void RenderHead(bool escape, bool shot, int x, int y);
        void RenderBeak(bool escape, bool shot, int x, int y);
        void RenderWing(bool escape, bool shot, int x, int y);
        void RenderBody(bool escape, bool shot, float tx, float ty);
        void FlashScreen(bool flash);
        void CheckFlash(float elapsedTime);
        void DuckEscape();
        void DuckHit();
        void GameOver();
        void RenderZero();
        void RenderOne();
        void RenderTwo();
        void RenderThree();
        void RenderFour();
        void RenderFive();
        void RenderSix();
        void RenderSeven();
        void RenderEight();
        void RenderScore(int score);

     protected:
        float cx, cy, eqcx, eqcy, iscx, iscy, reccx, reccy, side, crossX = 0, crossY = 0, radius, speedInc = 0;
        float bgRed = 0.1f, bgGreen = 0.3f, bgBlue = 0.7f, elapsedTime, totElapsedTime, rotAngle;
        int recside, wingRotate = 30, xSpeed, ySpeed, duckCount = 1, score = 0, first = 0, second = 0, third = 0;;
        int wingSpeed;
        bool rotateLeftRight = true, getRotAngle = true;
        bool wingStep = true;
        bool movingLeftRight;
        bool movingUpDown = true;
        bool isHit = false;
        int bullets = 3, lives = 3;;
        glm::mat3 modelMatrix;
        float angularStep = 0;
        float tx1, ty1, txI, tyI, angularStep2 = 0;
    };
}   // namespace m1
