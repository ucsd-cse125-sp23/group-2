#include "AudioManager.h"
#include <string.h>
AudioManager::AudioManager() {
    audioSystem = nullptr;

    AudioManager::errorCheck(FMOD::System_Create(&audioSystem));      // Create the main system object.

    AudioManager::errorCheck(audioSystem->init(512, FMOD_INIT_NORMAL, 0));    // Initialize FMOD.

    AudioManager::loadSound("../assets/sounds/sports.mp3", "bgm");
    AudioManager::loadSound("../assets/sounds/pew.mp3", "laser");
    AudioManager::loadSound("../assets/sounds/oof.mp3", "damage");
    AudioManager::loadSound("../assets/sounds/sentry.mp3", "tower");
    AudioManager::playSound("bgm");
}

void AudioManager::loadSound(const char* path, std::string name) {
    FMOD::Sound* newSound = nullptr;
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_DEFAULT, nullptr, &newSound));
    soundMap.insert(std::pair<std::string, FMOD::Sound*>(name, newSound));
}

void AudioManager::playSound(std::string name) {
    FMOD::Channel* newChannel = nullptr;
    AudioManager::errorCheck(audioSystem->playSound(soundMap.find(name)->second, nullptr, false, &newChannel));
}

int AudioManager::errorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return 1;
    }
    //printf("FMOD workin!");
    return 0;
}