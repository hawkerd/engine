#include "mesh.h"

using namespace std;

void Mesh::draw() {
    // pass the texture number to the shader, and bind the correct texture by its id
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNum++);
        } else {
            number = std::to_string(specularNum++);
        }
        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    
    // bind vertex array and draw
    vao.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.unbind();
}

void Mesh::setupMesh() {
    vao.bind();
    vbo.bind();
    ebo.bind();
    vao.linkAttribs(vbo, shader);
    vbo.unbind();
    vao.unbind();
}