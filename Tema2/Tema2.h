#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/tema_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, bool proj, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderScene(float deltaTimeSeconds);
        void RenderSceneOrtho(float deltaTimeSeconds);
        void GetPointsArray();
        float distance(glm::vec3 p1, glm::vec3 p2);
        float TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C);

        implemented::HwCamera* camera;
        implemented::HwCamera* camera2;
        glm::mat4 projectionMatrix;
        glm::mat4 projectionMatrixOrtho;
        float fov, left, right, up, down;
        std::vector<glm::vec3> initialPoints;
        std::vector<glm::vec3> in;
        std::vector<glm::vec3> ext;
        std::vector<glm::vec3> trees;
        std::vector<glm::vec3> car1;
        std::vector<glm::vec3> car2;
        std::vector<glm::vec3> car3;
        glm::vec3 firstCarPos;
        glm::vec3 secCarPos;
        glm::vec3 thCarPos;
        int car1Pos = 0;
        int car2Pos = 0;
        int car3Pos = 0;
        bool proj;
    protected:
        ViewportArea miniViewportArea;
        float miniViewportAreaX, miniViewportAreaY;
        float angle = M_PI / 2;
    };
}   // namespace m1
