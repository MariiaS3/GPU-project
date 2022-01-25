#include "skybox.h"

#include <QImage>
#include "glmath.h"
#include "gldebug.h"



Skybox::Skybox()
{
    initializeOpenGLFunctions();
    glGenTextures(1, &handle);
    createMesh(vec3{200,200,200});
    //m_mesh = nullptr;
}

bool Skybox::loadFromImage(const char *path)
{
    QImage image;
    QString img_path = QString::fromLocal8Bit(path);

    bind();
    image.load(img_path + "_px.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    image.load(img_path + "_nx.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    image.load(img_path + "_py.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    image.load(img_path + "_ny.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    image.load(img_path + "_pz.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    image.load(img_path + "_nz.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unbind();
    PRINT_GL_ERRORS("Skybox::loadFromImage()");
    return true;

}

void Skybox::bind(int tex_unit)
{
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    PRINT_GL_ERRORS("Skybox::bind()");
}

void Skybox::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::createMesh(vec3 size) {
    m_mesh = new Mesh(GL_TRIANGLES);

    int n = 24;
    vec3 origin = {0.0, 0.0, 0.0};
    float w = size.x;
    float h = size.y;
    float d = size.z;

    std::vector<vec3> pos(n);
    std::vector<vec3> colors(n);
    std::vector<vec3> normals(n);
    std::vector<vec3> uv(n);
    std::vector<uint> indices(36);

    int it=0;
    // sciana z = +1  //front
    pos[it] = origin + vec3{-w,  h, d};
    pos[it+1] = origin + vec3{-w, -h, d};
    pos[it+2] = origin + vec3{ w, -h, d};
    pos[it+3] = origin + vec3{ w,  h, d};

    normals[it+0] = {0,0,-1};
    normals[it+1] = {0,0,-1};
    normals[it+2] = {0,0,-1};
    normals[it+3] = {0,0,-1};

    uv[it+0] = {-1,1,   1};
    uv[it+1] = {-1,-1,  1};
    uv[it+2] = {1,-1,   1};
    uv[it+3] = {1,1,    1};

    it+=4;

    // sciana x=+1   //right
    pos[it+0] = origin + vec3{w,  h, d};
    pos[it+1] = origin + vec3{w, -h, d};
    pos[it+2] = origin + vec3{w, -h, -d};
    pos[it+3] = origin + vec3{w,  h, -d};

    normals[it+0] = {-1,0,0};
    normals[it+1] = {-1,0,0};
    normals[it+2] = {-1,0,0};
    normals[it+3] = {-1,0,0};

    uv[it+0] = {1,  1,1};
    uv[it+1] = {1,  -1,1};
    uv[it+2] = {1,  -1,-1};
    uv[it+3] = {1,  1,-1};
    it+=4;

    // sciana z=-1   //back
    pos[it+0] = origin + vec3{-w,  h, -d};
    pos[it+1] = origin + vec3{-w, -h, -d};
    pos[it+2] = origin + vec3{ w, -h, -d};
    pos[it+3] = origin + vec3{ w,  h, -d};

    normals[it+0] = {0,0,1};
    normals[it+1] = {0,0,1};
    normals[it+2] = {0,0,1};
    normals[it+3] = {0,0,1};

    uv[it+0] = {-1, 1, -1};
    uv[it+1] = {-1,-1, -1};
    uv[it+2] = { 1,-1, -1};
    uv[it+3] = { 1, 1, -1};
    it+=4;

    // sciana x=-1   //left
    pos[it+0] = origin + vec3{-w,  h, d};
    pos[it+1] = origin + vec3{-w, -h, d};
    pos[it+2] = origin + vec3{-w, -h, -d};
    pos[it+3] = origin + vec3{-w,  h, -d};

    normals[it+0] = {1,0,0};
    normals[it+1] = {1,0,0};
    normals[it+2] = {1,0,0};
    normals[it+3] = {1,0,0};

    uv[it+0] = {-1,  1,  1};
    uv[it+1] = {-1, -1,  1};
    uv[it+2] = {-1, -1, -1};
    uv[it+3] = {-1,  1, -1};

    it+=4;

    // sciana y=1  //top
    pos[it+0] = origin + vec3{-w, h, -d};
    pos[it+1] = origin + vec3{-w, h,  d};
    pos[it+2] = origin + vec3{ w, h,  d};
    pos[it+3] = origin + vec3{ w, h, -d};

    normals[it+0] = {0,-1,0};
    normals[it+1] = {0,-1,0};
    normals[it+2] = {0,-1,0};
    normals[it+3] = {0,-1,0};

    uv[it+0] = {-1, 1, -1};
    uv[it+1] = {-1, 1,  1};
    uv[it+2] = {1,  1,  1};
    uv[it+3] = {1,  1, -1};
    it+=4;

    // sciana y=-1  //bottom
    pos[it+0] = origin + vec3{-w, -h, -d};
    pos[it+1] = origin + vec3{-w, -h,  d};
    pos[it+2] = origin + vec3{ w, -h,  d};
    pos[it+3] = origin + vec3{ w, -h, -d};

    normals[it+0] = {0,1,0};
    normals[it+1] = {0,1,0};
    normals[it+2] = {0,1,0};
    normals[it+3] = {0,1,0};

    uv[it+0] = {-1, -1, -1};
    uv[it+1] = {-1, -1,  1};
    uv[it+2] = { 1, -1,  1};
    uv[it+3] = { 1, -1, -1};

    it+=4;

    for(int i=0; i<n; i++) colors[i] = vec3{1,1,1};

    m_mesh->setVertices(pos.data(), n);
    //mesh->setAttribute(, colors.data(), n);
    m_mesh->setAttribute(Mesh::Normals, normals.data(), n);
    m_mesh->setAttribute(Mesh::UV, uv.data(), n);

    it=0;
    indices[it++] = 0;    indices[it++] = 1;    indices[it++] = 2;    indices[it++] = 0;    indices[it++] = 2;    indices[it++] = 3;
    indices[it++] = 4;    indices[it++] = 5;    indices[it++] = 6;    indices[it++] = 4;    indices[it++] = 6;    indices[it++] = 7;
    indices[it++] = 8;    indices[it++] = 11;   indices[it++] = 10;   indices[it++] = 8;    indices[it++] = 10;   indices[it++] = 9;
    indices[it++] = 12;   indices[it++] = 15;   indices[it++] = 14;   indices[it++] = 12;   indices[it++] = 14;   indices[it++] = 13;
    indices[it++] = 16;   indices[it++] = 17;   indices[it++] = 18;   indices[it++] = 16;   indices[it++] = 18;   indices[it++] = 19;
    indices[it++] = 20;   indices[it++] = 23;   indices[it++] = 22;   indices[it++] = 20;   indices[it++] = 22;   indices[it++] = 21;

    m_mesh->setIndices(indices.data(), indices.size());
}

Mesh *Skybox::mesh() const
{
    return m_mesh;
}
