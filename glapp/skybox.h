#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glu.h>
#include <QOpenGLFunctions_3_3_Core>
#include "mesh.h"


class Skybox : protected QOpenGLFunctions_3_3_Core
{
    GLuint handle;
    Mesh *m_mesh;

    void createMesh(vec3 size);
public:
    Skybox();

    bool loadFromImage(const char* path);


    void bind(int tex_unit=0);
    void unbind();
    Mesh *mesh() const;
};

#endif // SKYBOX_H
