#ifndef BUBBA_3D_RESOURCEMANAGER_H
#define BUBBA_3D_RESOURCEMANAGER_H

#include <map>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include <sstream>
#include <SFML/Audio.hpp>

class ResourceManager {
public:
    static void loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name);
    static Shader* getShader(std::string name);

    static Texture* loadAndFetchTexture(const std::string &fileName);
    static Mesh*    loadAndFetchMesh   (const std::string &fileName);
    static sf::SoundBuffer* loadAndFetchSoundBuffer(const std::string &fileName);
    static sf::Music*       loadAndFetchMusic(const std::string &fileName);

    template<typename Type>
    static Type* getItemFromMap(std::map<std::string, Type> *map, std::string id) {
        typename std::map<std::string, Type>::iterator it = map->find(id);
        if( it != map->end()) {
            return &it->second;
        } else {
            std::stringstream errorMessage;
            errorMessage << id << " hasn't been loaded into ResourceManager before fetched";
            throw std::invalid_argument(errorMessage.str());
        }
    }

private:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
    static std::map<std::string, Mesh> meshes;
    static std::map<std::string, sf::Music> musics;
    static std::map<std::string, sf::SoundBuffer> soundBuffers;

    static void loadTexture(const std::string &fileName);
    static Texture* getTexture(std::string fileName);

    static void loadMesh(const std::string &fileName);
    static Mesh* getMesh(std::string fileName);

    static void loadSoundBuffer(const std::string &fileName);
    static sf::SoundBuffer* getSoundBuffer(std::string fileName);

    static void loadMusic(const std::string &fileName);
    static sf::Music* getMusic(std::string fileName);
};

#endif //BUBBA_3D_RESOURCEMANAGER_H
