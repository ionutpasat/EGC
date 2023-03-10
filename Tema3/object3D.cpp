#include "object3D.h"

#include <vector>
#include <math.h>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>


Mesh* object3D::CreateTrack(std::vector<glm::vec3> in, std::vector<glm::vec3> ext, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;

    for (int i = 0; i < ext.size(); i++) {
        vertices.push_back(VertexFormat(ext[i]));
    }
    for (int i = 0; i < in.size(); i++) {
        vertices.push_back(VertexFormat(in[i]));
    }

    Mesh* track = new Mesh("track");
    track->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices;

    for (int i = 0, j = ext.size(); i < ext.size() && j < vertices.size() - 1; i++, j++) {
        indices.push_back(i);
        indices.push_back(j);
        indices.push_back(j + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(j + 1);
    }

    //indices.push_back(ext.size() - 1);
    //indices.push_back(in.size() + ext.size() - 1);
    //indices.push_back(in.size());
    //indices.push_back(ext.size() - 1);
    //indices.push_back(0);
    //indices.push_back(in.size());

    track->InitFromData(vertices, indices);
    return track;
}
