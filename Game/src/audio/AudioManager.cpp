#include "AudioManager.h"
#include <string.h>
const FMOD_VECTOR zeroVec = { 0, 0, 0 };

FMOD_VECTOR vecConvert(glm::vec3 inVec) {
    FMOD_VECTOR outVec = { inVec.x, inVec.y, inVec.z };
    return outVec;
}

AudioManager::AudioManager() {
    audioSystem = nullptr;
    // Create the main system object.
    AudioManager::errorCheck(FMOD::System_Create(&audioSystem));      
    // Initialize FMOD.
    AudioManager::errorCheck(audioSystem->init(512, FMOD_INIT_3D_RIGHTHANDED, 0));
    //AudioManager::errorCheck(audioSystem->setDSPBufferSize(512, 2));

    AudioManager::loadSound("../assets/sounds/sports.mp3", "bgm");
    AudioManager::loadSound3D("../assets/sounds/pew.mp3", "laser");
    AudioManager::loadSound3D("../assets/sounds/oof.mp3", "death");
    AudioManager::loadSound3D("../assets/sounds/sentry.wav", "tower");
    AudioManager::loadSound3D("../assets/sounds/minecrafthit.mp3", "damage");
    music = nullptr;
    AudioManager::playMusic("bgm");
    AudioManager::setMusicVolume(0.25);
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

void AudioManager::playMusic(std::string name) {
    AudioManager::errorCheck(audioSystem->playSound(soundMap.find(name)->second, nullptr, false, &music));
}

void AudioManager::setMusicVolume(float vol) {
    music->setVolume(vol);
}

// Play a 3D sound at the position
void AudioManager::playSound3D(std::string name, glm::vec3 pos) {
    FMOD::Channel* newChannel = nullptr;
    AudioManager::errorCheck(audioSystem->playSound(soundMap.find(name)->second, nullptr, true, &newChannel));
    AudioManager::errorCheck(newChannel->set3DAttributes(&vecConvert(pos), &zeroVec));
    AudioManager::errorCheck(newChannel->setPaused(false));
}

void AudioManager::loadSound3D(const char* path, std::string name) {
    FMOD::Sound* newSound = nullptr;
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_3D, nullptr, &newSound));
    soundMap.insert(std::pair<std::string, FMOD::Sound*>(name, newSound));
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

void AudioManager::update(glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp) {
    AudioManager::errorCheck(audioSystem->set3DListenerAttributes(0, &vecConvert(camPos), &zeroVec, &vecConvert(camForward), &vecConvert(camUp)));
    AudioManager::errorCheck(audioSystem->update());
}