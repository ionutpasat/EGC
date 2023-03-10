#include "object2D.h"

#include <vector>
#include <math.h>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateEquilateralTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, (length * sqrt(3)) / 2, 0), color),
    };

    Mesh* equilateralTriangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    //if (!fill) {
    //    equilateralTriangle->SetDrawMode(GL_LINE_LOOP);
    //}
    //else {
    //    // Draw 2 triangles. Add the remaining 2 indices
    //    indices.push_back(0);
    //    indices.push_back(2);
    //}

    equilateralTriangle->InitFromData(vertices, indices);
    return equilateralTriangle;
}

Mesh* object2D::CreateIsoscelesTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, sqrt(length * length - (length / 2) * (length / 2)), 0), color),
    };

    Mesh* isoscelesTriangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        isoscelesTriangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    isoscelesTriangle->InitFromData(vertices, indices);
    return isoscelesTriangle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;

    for (int i = 0; i <= 360;i++) {
        vertices.push_back(VertexFormat(
            corner + glm::vec3(length * cos(i * 2 * M_PI / 360),
                (length * sin(i * 2 * M_PI / 360)), 0), color));
    }

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i <= 360;i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
        VertexFormat(corner + glm::vec3(2 * length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCrossHair(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;

    for (int i = 0; i <= 360;i++) {
        vertices.push_back(VertexFormat(
            corner + glm::vec3(length * cos(i * 2 * M_PI / 360),
                (length * sin(i * 2 * M_PI / 360)), 0), color));
    }

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 1; i <= 359;i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
