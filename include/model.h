#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
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
#include <iostream>
#include <spdlog/spdlog.h>
#include <filesystem>

using namespace std;

class Model {
	public:
		Model(string path, Shader& shader, bool gamma = false);
		
		void draw();
	
	private:
        vector<Texture> loadedTextures;
        vector<Mesh> meshes;
        string directory;
        bool gammaCorrection;
        Shader& shader;
		
		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // retrieve, load, and initilaize the texture from the material
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif