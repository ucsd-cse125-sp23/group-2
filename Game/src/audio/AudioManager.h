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
	std::unordered_map<std::string, FMOD::Sound *> soundMap;
public:
	AudioManager();
	void loadSound(const char* path, std::string name);
	void playSound(std::string name);
	void update(ClienttoServerData&, ServertoClientData&);
};