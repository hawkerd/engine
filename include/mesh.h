#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

#include <string>
#include <vector>

#include "vbo.h"
#include "vao.h"
#include "ebo.h"

using namespace std;

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Shader& shader)
            : vertices(vertices), indices(indices), textures(textures), vbo(vertices), ebo(indices), shader(shader) {
            setupMesh();
        }

		void draw();

	private:
		VAO vao;
		VBO vbo;
		EBO ebo;
		Shader& shader;
		void setupMesh();
};

#endif