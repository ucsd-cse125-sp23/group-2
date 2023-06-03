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
    AudioManager::errorCheck(audioSystem->init(2048, FMOD_INIT_3D_RIGHTHANDED, 0));
    //AudioManager::errorCheck(audioSystem->setDSPBufferSize(512, 2));
    music = nullptr;
    musicChannel = nullptr;

    AudioManager::loadSound("../assets/sounds/Bluezone_BC0254_wood_falling_log_002_006.wav", MODEL_ID_RESOURCE, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0254_wood_falling_log_002_006.wav", MODEL_ID_RESOURCE_STONE, SOUND_ID_DEATH); //TODO Make stone sounds

    AudioManager::loadSound("../assets/sounds/Alarm_Fast_DDM23.wav", MODEL_ID_BASE, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Destruction_Wooden_2.wav", MODEL_ID_RESOURCE, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Destruction_Wooden_2.wav", MODEL_ID_RESOURCE_STONE, SOUND_ID_DAMAGE); //TODO make stone sound

    AudioManager::loadSound("../assets/sounds/Big Blast 4.wav", MODEL_ID_ROVER, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/HeavyGun_Experimental_3.wav", MODEL_ID_TOWER, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/PM_SFG_VOL1_WEAPON_4_4_GUN_GUNSHOT_FUTURISTIC.wav", MODEL_ID_RAILGUN, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/Magic_Spells_CastLong_Electricity11.wav", MODEL_ID_TESLA, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/MINE Placing 01.wav", MODEL_ID_ROVER, SOUND_ID_BUILD);
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1.wav", MODEL_ID_MOB, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1.wav", MODEL_ID_MOB_TANK, SOUND_ID_DEATH); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1.wav", MODEL_ID_MOB_MINI, SOUND_ID_DEATH); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB_TANK, SOUND_ID_DAMAGE); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB_MINI, SOUND_ID_DAMAGE); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/IMPACT METAL Scratchy Short Punch_03.wav", MODEL_ID_MOB_FLYING, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/IMPACT METAL Scratchy Short Punch_03.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_DAMAGE); //Maybe change?
    AudioManager::loadSound("../assets/sounds/Vehicle5_BlastOff1.wav", MODEL_ID_MOB_FLYING, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Vehicle5_BlastOff1.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_DEATH); //Maybe change?
    AudioManager::loadSound("../assets/sounds/Big Blast 4.wav", MODEL_ID_MOB_FLYING, SOUND_ID_ATTACK); //Maybe change
    AudioManager::loadSound("../assets/sounds/4B8NZTJ-tractor-beam.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/Robotic Lifeforms 2 - Impacts Source - Metal Tool Drawer Punch 03.wav", MODEL_ID_ROVER, SOUND_ID_LAND);
    AudioManager::loadSound("../assets/sounds/PM_FSSF2_EXOSKELETON_11_SERVO_MOVEMENT_ROTATION.wav", MODEL_ID_ROVER, SOUND_ID_JUMP);

    //AudioManager::playMusic();
    AudioManager::setMusicVolume(0.25);

    AudioManager::errorCheck(audioSystem->set3DSettings(1, 1, 0.5));
}

// Loads a sound into the sound map
void AudioManager::loadSound(const char* path, int model, int soundType) {
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_3D, nullptr, &soundArray[model][soundType]));
}

// Play a sound from the given sound map
void AudioManager::playSound(int model, int soundType, glm::vec3 pos) {
    if (soundArray[model][soundType] == nullptr) {
        //printf("Invalid play sound call! Model ID: %d Sound ID: %d\n", model, soundType);
        return;
    }
    //printf("Sound call! Model ID: %d Sound ID: %d\n", model, soundType);
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