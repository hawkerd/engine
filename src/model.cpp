#include <model.h>

Model::Model(const char* path, Shader& shader, bool gamma) : gammaCorrection(gamma), shader(shader) {
    loadModel(path);
}

void Model::draw() {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw();
    }
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("error loading model");
    }
    
    directory = path.substr(0, path.find_last_of('/'));
    
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
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

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
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
        if (mesh->HasNormals()) {
            v.x = mesh->mNormals[i].x;
            v.y = mesh->mNormals[i].y;
            v.z = mesh->mNormals[i].z;
            vertex.normal = v;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoordinates = vec;

            if (mesh->HasTangentsAndBitangents()) {
                // tangent
                v.x = mesh->mTangents[i].x;
                v.y = mesh->mTangents[i].y;
                v.z = mesh->mTangents[i].z;
                vertex.tangent = v;

                //bitangent
                v.x = mesh->mBitangents[i].x;
                v.y = mesh->mBitangents[i].y;
                v.z = mesh->mBitangents[i].z;
                vertex.bitangent = v;
            }

        } else {
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    
    // process faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<Texture> heightMaps = loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return Mesh(vertices, indices, textures, shader);
}

// retrieve, load, and initilaize the texture from the material
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), directory, gammaCorrection);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::textureFromFile(const char* path, const string& directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + path;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, numComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 0);
    if (!data) {
        throw runtime_error("Failed to load texture");
    }

    GLenum format;
    if (numComponents == 1) {
        format = GL_RED;
    } else if (numComponents == 3) {
        format = GL_RGB;
    } else if (numComponents == 4) {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureId;
}