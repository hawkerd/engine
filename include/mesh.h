#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

#include <string>
#include <vector>

using namespace std;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
};

struct Texture {
	unsigned int id;
	string type;
};

class Mesh {
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}
		void draw(Shader& shader) {
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
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		unsigned int VAO, VBO, EBO;
		void setupMesh() {
			// generate VAO and buffers
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			
			// bind and populate VBO with vertex data
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
			
			// bind and populate EBO with indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			
			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
			// vertex texture coordinates
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, textureCoordinates));
		
			// unbind the VAO
			glBindVertexArray(0);
		}
};

#endif