#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Model {
	public:
        vector<Texture> loadedTextures;
        vector<Mesh> meshes;
        string directory;


		Model(char* path) {
			loadModel(path);
		}
		
		void Draw(Shader& shader) {
			for (unsigned int i = 0; i < meshes.size(); i++) {
				meshes[i].draw(shader);
			}
		}
	
	private:
		
		void loadModel(string path) {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				throw std::runtime_error("error loading model");
			}
			
			directory = path.substr(0, path.find_last_of('/'));
			
			processNode(scene->mRootNode, scene);
		}
		
		void processNode(aiNode* node, const aiScene* scene) {
			// process all the meshes of the node
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			
			//recurse
			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}
		
		Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;
			
			// process vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;
				glm::vec3 v;
				
				// position
				v.x = mesh->mVertices[i].x;
				v.y = mesh->mVertices[i].y;
				v.z = mesh->mVertices[i].z;
				vertex.position = v;
				
				// normals
				v.x = mesh->mNormals[i].x;
				v.y = mesh->mNormals[i].y;
				v.z = mesh->mNormals[i].z;
				vertex.normal = v;
				
				// texture coordinates
				if (mesh->mTextureCoords[0]) {
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.textureCoordinates = vec;
				} else {
					vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
				}
				
				vertices.push_back(vertex);
			}
			
			// process indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}
			
			// process material
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
				// TODO
			}
			
			return Mesh(vertices, indices, textures);
		}
};

#endif