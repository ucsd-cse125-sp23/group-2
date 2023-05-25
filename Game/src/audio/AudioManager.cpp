#include "AudioManager.h"
#include <string.h>
AudioManager::AudioManager() {
    audioSystem = nullptr;
    // Create the main system object.
    AudioManager::errorCheck(FMOD::System_Create(&audioSystem));      
    // Initialize FMOD.
    AudioManager::errorCheck(audioSystem->init(512, FMOD_INIT_NORMAL, 0));   
    //AudioManager::errorCheck(audioSystem->setDSPBufferSize(512, 2));

    AudioManager::loadSound("../assets/sounds/sports.mp3", "bgm");
    AudioManager::loadSound("../assets/sounds/pew.mp3", "laser");
    AudioManager::loadSound("../assets/sounds/oof.mp3", "death");
    AudioManager::loadSound("../assets/sounds/sentry.wav", "tower");
    AudioManager::loadSound("../assets/sounds/minecrafthit.mp3", "damage");
    AudioManager::playSound("bgm");
}

// Loads a sound into the sound map
void AudioManager::loadSound(const char* path, std::string name) {
    FMOD::Sound* newSound = nullptr;
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_DEFAULT, nullptr, &newSound));
    soundMap.insert(std::pair<std::string, FMOD::Sound*>(name, newSound));
}

// Play a sound from the given sound map
void AudioManager::playSound(std::string name) {
    FMOD::Channel* newChannel = nullptr;
    AudioManager::errorCheck(audioSystem->playSound(soundMap.find(name)->second, nullptr, false, &newChannel));
}

// Helper function to check for FMOD errors
int AudioManager::errorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return 1;
    }
    //printf("FMOD workin!");
    return 0;
}


void AudioManager::update(ClienttoServerData& outgoingData, ServertoClientData& incomingData) {
    /*
    AudioManager::errorCheck(audioSystem->set3DListenerAttributes(0,
        const FMOD_VECTOR * pos,
        const FMOD_VECTOR * vel,
        const FMOD_VECTOR * forward,
        const FMOD_VECTOR * up
    );
    */
    
}

FMOD_VECTOR glmtofmod(glm::vec3 inVec) {
    FMOD_VECTOR outVec = { inVec.x, inVec.y, inVec.z };
    return outVec;
}