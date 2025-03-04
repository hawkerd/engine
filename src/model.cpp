#include <model.h>

Model::Model(std::string path, Shader& shader, bool gamma) : gammaCorrection(gamma), shader(shader) {
    spdlog::info("loading model from path: {}", path);
    loadModel(path);
}

void Model::draw() {
    shader.use();
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw();
    }
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("error loading model: {}", importer.GetErrorString());
    }

    directory = path.substr(0, path.find_last_of('/'));
    
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // log the nod einfo
    spdlog::info("recursively processing node: {}, which has {} meshes and {} children", node->mName.C_Str(), node->mNumMeshes, node->mNumChildren);

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
    vector<VertexPNTB> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    if (mesh->HasPositions()) spdlog::info("mesh has positions");
    if (mesh->HasNormals()) spdlog::info("mesh has normals");
    if (mesh->HasTextureCoords(0)) spdlog::info("mesh has texture coordinates");
    if (mesh->HasTangentsAndBitangents()) spdlog::info("mesh has tangents and bitangents");
    if (mesh->HasVertexColors(0)) spdlog::info("mesh has vertex colors");


    // process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        VertexPNTB vertex;
        glm::vec3 v;

        // position
        if (mesh->HasPositions()) vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        // normals
        if (mesh->HasNormals()) vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        
        // texture coordinates/ tangents and bitangents
        if (mesh->HasTextureCoords(0)) {
            vertex.textureCoordinates = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            }
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
    spdlog::info("loading {} material textures of type: {}", mat->GetTextureCount(type), typeName);

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
            if (!std::filesystem::exists(directory + "/" + str.C_Str())) {
                spdlog::warn("Skipping missing texture: {}", str.C_Str());
                continue;
            }
            Texture texture(directory + "/" + str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}