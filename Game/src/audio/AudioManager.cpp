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
    music = nullptr;
    musicChannel = nullptr;
    AudioManager::loadMusic("../assets/sounds/sports.mp3");
    AudioManager::loadSound("../assets/sounds/pew.mp3", MODEL_ID_ROVER, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/oof.mp3", MODEL_ID_MOB, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/sentry.wav", MODEL_ID_ROVER, SOUND_ID_BUILD);
    AudioManager::loadSound("../assets/sounds/minecrafthit.mp3", MODEL_ID_MOB, SOUND_ID_DAMAGE);
    AudioManager::playMusic();
    AudioManager::setMusicVolume(0.25);
}

// Loads a sound into the sound map
void AudioManager::loadSound(const char* path, int model, int soundType) {
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_3D, nullptr, &soundArray[model][soundType]));
}

// Play a sound from the given sound map
void AudioManager::playSound(int model, int soundType, glm::vec3 pos) {
    if (soundArray[model][soundType] == nullptr) {
        printf("Invalid play sound call!");
        return;
    }
    FMOD::Channel* newChannel = nullptr;
    AudioManager::errorCheck(audioSystem->playSound(soundArray[model][soundType], nullptr, false, &newChannel));
    AudioManager::errorCheck(newChannel->set3DAttributes(&vecConvert(pos), &zeroVec));
    AudioManager::errorCheck(newChannel->setPaused(false));
}

// Load music track
void AudioManager::loadMusic(const char* path) {
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_DEFAULT, nullptr, &music));
}

void AudioManager::playMusic() {
    AudioManager::errorCheck(audioSystem->playSound(music, nullptr, false, &musicChannel));
}

void AudioManager::setMusicVolume(float vol) {
    musicChannel->setVolume(vol);
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