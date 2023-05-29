#pragma once
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>
#include <iostream>
#include <unordered_map>
#include "../NetworkData.h"

class AudioManager {
private:
	FMOD::System* audioSystem;
	int errorCheck(FMOD_RESULT result);
	FMOD::Sound* soundArray[NUM_MODELS][NUM_SOUND_TYPES]{};
	FMOD::Sound* music;
	FMOD::Channel* musicChannel;
public:
	AudioManager();
	void loadSound(const char* path, int model, int soundType);
	void playSound(int model, int soundType, glm::vec3 pos);
	void update(glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp);
	void loadMusic(const char* path);
	void playMusic();
	void setMusicVolume(float vol);
};