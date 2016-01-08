#include "ResourceManager.h"
#include <sstream>


std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Mesh> ResourceManager::meshes;
std::map<std::string, sf::Music> ResourceManager::musics;
std::map<std::string, sf::SoundBuffer> ResourceManager::soundBuffers;

void ResourceManager::loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name){
    Shader shaderProgram;
    shaderProgram.loadShader(vertexShader,fragmentShader);
    shaders.insert(std::pair<std::string, Shader>(name, shaderProgram));
}

Shader* ResourceManager::getShader(std::string name) {
    return getItemFromMap(&shaders, name);
}

Texture* ResourceManager::loadAndFetchTexture(const std::string &fileName) {
    try {
        return getTexture(fileName);
    } catch (std::invalid_argument exception) {
        loadTexture(fileName);
        return getTexture(fileName);
    }
}

void ResourceManager::loadTexture(const std::string &fileName) {
    Texture texture;
    texture.loadTexture(fileName);
    textures.insert(std::pair<std::string, Texture>(fileName, texture));
}

Texture* ResourceManager::getTexture(std::string fileName) {
    return getItemFromMap(&textures, fileName);
}


Mesh* ResourceManager::loadAndFetchMesh(const std::string &fileName){
    try {
        return getMesh(fileName);
    } catch (std::invalid_argument exception) {
        loadMesh(fileName);
        return getMesh(fileName);
    }
}

void ResourceManager::loadMesh(const std::string &fileName){
    Mesh mesh;
    mesh.loadMesh(fileName);
    meshes.insert(std::pair<std::string, Mesh>(fileName, mesh));
}

Mesh* ResourceManager::getMesh(std::string fileName)
{
    return getItemFromMap(&meshes, fileName);
}


sf::SoundBuffer* ResourceManager::loadAndFetchSoundBuffer(const std::string &fileName){
    try {
        return getSoundBuffer(fileName);
    } catch (std::invalid_argument exception) {
        loadSoundBuffer(fileName);
        return getSoundBuffer(fileName);
    }
}

void ResourceManager::loadSoundBuffer(const std::string &fileName) {

}

sf::SoundBuffer* ResourceManager::getSoundBuffer(std::string fileName){
    return getItemFromMap(&soundBuffers, fileName);
}


sf::Music* ResourceManager::loadAndFetchMusic(const std::string &fileName) {
    try {
        return getMusic(fileName);
    } catch (std::invalid_argument exception) {
        loadMusic(fileName);
        return getMusic(fileName);
    }
}

void ResourceManager::loadMusic(const std::string &fileName) {

}

sf::Music* ResourceManager::getMusic(std::string fileName) {
    return getItemFromMap(&musics, fileName);
}






