//
// Created by johan on 2015-12-12.
//


#include <SoundManager.h>
#include <Logger.h>

SoundManager::SoundManager() {
    Logger::logInfo("Generating sound data.");

    if(!ambient.openFromFile("../scenes/ambient.ogg")) {
        Logger::logDebug("Error loading ambient.ogg");
    }

    Logger::logInfo("Generating sound data completed.");
    ambient.play();
}

SoundManager::~SoundManager() {

}

