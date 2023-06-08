#include "AudioManager.h"
#include <string.h>
const FMOD_VECTOR zeroVec = { 0, 0, 0 };

FMOD_VECTOR vecConvert(glm::vec3 inVec) {
    FMOD_VECTOR outVec = { inVec.x, inVec.y, inVec.z };
    return outVec;
}

AudioManager::AudioManager() {
    musicvol = 0.7f;
    sfxvol = 0.7f;
    audioSystem = nullptr;
    // Create the main system object.
    AudioManager::errorCheck(FMOD::System_Create(&audioSystem));      
    // Initialize FMOD.
    AudioManager::errorCheck(audioSystem->init(2048, FMOD_INIT_3D_RIGHTHANDED, 0));
    //AudioManager::errorCheck(audioSystem->setDSPBufferSize(512, 2));
    music = nullptr;
    musicChannel = nullptr;

    AudioManager::loadSound("../assets/sounds/Bluezone_BC0279_forest_wood_log_debris_falling_3_007.wav", MODEL_ID_RESOURCE, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/PM_RI_Designed_7 Rocks Impact Hit Big LFE Heavy Designed.wav", MODEL_ID_RESOURCE_STONE, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Alarm_Fast_DDM23.wav", MODEL_ID_BASE, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Destruction_Wooden_2.wav", MODEL_ID_RESOURCE, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Destruction_Wooden_2.wav", MODEL_ID_BARRIER, SOUND_ID_DAMAGE); //TODO: Change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0279_forest_wood_log_debris_falling_3_007.wav", MODEL_ID_BARRIER, SOUND_ID_DEATH);

    AudioManager::loadSound("../assets/sounds/PM_RI_Source_92 Rocks Impact Hit Single Stone.wav", MODEL_ID_RESOURCE_STONE, SOUND_ID_DAMAGE);

    AudioManager::loadSound("../assets/sounds/Big Blast 4.wav", MODEL_ID_ROVER, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/HeavyGun_Experimental_3.wav", MODEL_ID_TOWER, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/PM_SFG_VOL1_WEAPON_4_4_GUN_GUNSHOT_FUTURISTIC.wav", MODEL_ID_RAILGUN, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/Magic_Spells_CastLong_Electricity11.wav", MODEL_ID_TESLA, SOUND_ID_ATTACK);
    AudioManager::loopSound(MODEL_ID_TESLA, SOUND_ID_ATTACK, 1400, 2400);
    AudioManager::loadSound("../assets/sounds/MINE Placing 01.wav", MODEL_ID_ROVER, SOUND_ID_BUILD);
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1.wav", MODEL_ID_MOB, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1 Pitch Down.wav", MODEL_ID_MOB_TANK, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Screeching Monster 2-1 Pitch Up.wav", MODEL_ID_MOB_MINI, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB_TANK, SOUND_ID_DAMAGE); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0263_creature_crab_organic_texture_carapace_crack_squishy_003.wav", MODEL_ID_MOB_MINI, SOUND_ID_DAMAGE); //TODO: CHANGE
    AudioManager::loadSound("../assets/sounds/IMPACT METAL Scratchy Short Punch_03.wav", MODEL_ID_MOB_FLYING, SOUND_ID_DAMAGE);
    AudioManager::loadSound("../assets/sounds/IMPACT METAL Scratchy Short Punch_03.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_DAMAGE); //Maybe change?
    AudioManager::loadSound("../assets/sounds/Vehicle5_BlastOff1.wav", MODEL_ID_MOB_FLYING, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/Vehicle5_BlastOff1.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_DEATH); //Maybe change?
    AudioManager::loadSound("../assets/sounds/Big Blast 4.wav", MODEL_ID_MOB_FLYING, SOUND_ID_ATTACK); //Maybe change
    AudioManager::loadSound("../assets/sounds/PM_SFG_VOL1_WEAPON_8_2_GUN_GUNSHOT_FUTURISTIC.wav", MODEL_ID_BEAR_LARM, SOUND_ID_ATTACK); //Maybe change
    AudioManager::loadSound("../assets/sounds/PM_AG_20_3_ABSTRACT_GUNS.wav", MODEL_ID_BEAR_RARM, SOUND_ID_ATTACK); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_weapon_scifi_shot_002.wav", MODEL_ID_BEAR_HEAD, SOUND_ID_ATTACK); //Maybe change

    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_LARM, SOUND_ID_DAMAGE); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_RARM, SOUND_ID_DAMAGE); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_LLEG, SOUND_ID_DAMAGE); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_RLEG, SOUND_ID_DAMAGE); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_HEAD, SOUND_ID_DAMAGE); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0286_invader_debris_impact_011.wav", MODEL_ID_BEAR_BODY, SOUND_ID_DAMAGE); //Maybe change


    AudioManager::loadSound("../assets/sounds/Bluezone_BC0290_demolisher_debris_rubble_texture_004.wav", MODEL_ID_BEAR_LARM, SOUND_ID_DEATH); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0290_demolisher_debris_rubble_texture_004.wav", MODEL_ID_BEAR_RARM, SOUND_ID_DEATH); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0290_demolisher_debris_rubble_texture_004.wav", MODEL_ID_BEAR_LLEG, SOUND_ID_DEATH); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0290_demolisher_debris_rubble_texture_004.wav", MODEL_ID_BEAR_RLEG, SOUND_ID_DEATH); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0290_demolisher_debris_rubble_texture_004.wav", MODEL_ID_BEAR_HEAD, SOUND_ID_DEATH); //Maybe change
    AudioManager::loadSound("../assets/sounds/Bluezone_BC0275_building_collapse_debris_falling_rock_rubble_008.wav", MODEL_ID_BEAR_BODY, SOUND_ID_DEATH); //Maybe change



    AudioManager::loadSound("../assets/sounds/4B8NZTJ-tractor-beam.wav", MODEL_ID_MOB_TRACTOR, SOUND_ID_ATTACK);
    AudioManager::loadSound("../assets/sounds/Robotic Lifeforms 2 - Impacts Source - Metal Tool Drawer Punch 03.wav", MODEL_ID_ROVER, SOUND_ID_LAND);
    AudioManager::loadSound("../assets/sounds/PM_FSSF2_EXOSKELETON_11_SERVO_MOVEMENT_ROTATION.wav", MODEL_ID_ROVER, SOUND_ID_JUMP);
    AudioManager::loadSound("../assets/sounds/PowerUp01.wav", MODEL_ID_POWERUP_SPREAD, SOUND_ID_DEATH);
    AudioManager::loadSound("../assets/sounds/PowerUp01.wav", MODEL_ID_POWERUP_RAPID, SOUND_ID_DEATH);


    AudioManager::loadMusic("../assets/sounds/Free Video Game Music - HeatleyBros - Game On.mp3");
    AudioManager::playMusic();
    AudioManager::errorCheck(audioSystem->set3DSettings(1, 1, 0.5));
}

// Loads a sound into the sound map
void AudioManager::loadSound(const char* path, int model, int soundType) {
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_3D, nullptr, &soundArray[model][soundType]));
}

// Play a sound from the given sound map
void AudioManager::playSound(int model, int soundType, glm::vec3 pos, Entity e) {
    if (soundArray[model][soundType] == nullptr) {
        //printf("Invalid play sound call! Model ID: %d Sound ID: %d\n", model, soundType);
        return;
    }
    //printf("Sound call! Model ID: %d Sound ID: %d\n", model, soundType);
    FMOD_MODE mode = FMOD_DEFAULT;
    AudioManager::errorCheck(soundArray[model][soundType]->getMode(&mode));
    if ((mode & FMOD_LOOP_NORMAL) == FMOD_LOOP_NORMAL) {
        if(channelArray[e] == nullptr){
            AudioManager::errorCheck(audioSystem->playSound(soundArray[model][soundType], nullptr, false, &channelArray[e]));
            AudioManager::errorCheck(channelArray[e]->set3DAttributes(&vecConvert(pos), &zeroVec));
            AudioManager::errorCheck(channelArray[e]->setPaused(false));
            AudioManager::errorCheck(channelArray[e]->setVolume(sfxvol));
        }
    }
    else {
        FMOD::Channel* newChannel = nullptr;
        AudioManager::errorCheck(audioSystem->playSound(soundArray[model][soundType], nullptr, false, &newChannel));
        AudioManager::errorCheck(newChannel->set3DAttributes(&vecConvert(pos), &zeroVec));
        AudioManager::errorCheck(newChannel->setPaused(false));
        AudioManager::errorCheck(newChannel->setVolume(sfxvol));
    }
}

// Set a sound to loop, where loopStart and loopEnd are times in ms
void AudioManager::loopSound(int model, int soundType, unsigned int loopStart, unsigned int loopEnd) {
    AudioManager::errorCheck(soundArray[model][soundType]->setMode(FMOD_3D | FMOD_LOOP_NORMAL));
    AudioManager::errorCheck(soundArray[model][soundType]->setLoopPoints(loopStart, FMOD_TIMEUNIT_MS, loopEnd, FMOD_TIMEUNIT_MS));
}

// End a looping sound for an entity (assumption that each entity can only have 1 looping sound
void AudioManager::stopSound(Entity e) {
    if (channelArray[e] != nullptr) {
        AudioManager::errorCheck(channelArray[e]->setLoopCount(0));
        channelArray[e] = nullptr;
    }
}

// Load music track
void AudioManager::loadMusic(const char* path) {
    AudioManager::errorCheck(audioSystem->createSound(path, FMOD_LOOP_NORMAL | FMOD_CREATESTREAM, nullptr, &music));
}

void AudioManager::playMusic() {
    AudioManager::errorCheck(audioSystem->playSound(music, nullptr, false, &musicChannel));
    AudioManager::errorCheck(musicChannel->setVolume(musicvol * 0.05));
}

void AudioManager::setMusicVolume(float vol) {
    if (vol < 0 || vol > 1) {
        printf("Trying to set music volume out of range!\n");
        return;
    }
    musicvol = vol;
}

void AudioManager::setSFXVolume(float vol) {
    if (vol < 0 || vol > 1) {
        printf("Trying to set SFX volume out of range!\n");
        return;
    }
    sfxvol = vol;
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

void AudioManager::update(glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp, ServertoClientData& incomingData) {
    AudioManager::errorCheck(audioSystem->set3DListenerAttributes(0, &vecConvert(camPos), &zeroVec, &vecConvert(camForward), &vecConvert(camUp)));
    AudioManager::errorCheck(audioSystem->update());
    for (int i = 0; i < incomingData.activity.size(); i++) {
        if (incomingData.activity[i] && channelArray[i] != nullptr) {
            AudioManager::errorCheck(channelArray[i]->set3DAttributes(&vecConvert(incomingData.positions[i]), &zeroVec));
        }
    }
}