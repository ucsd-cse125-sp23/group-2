#pragma once
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
#include "RenderEntity.h"

struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(1.0f) {}

};


class ParticleGenerator {
public:
	ParticleGenerator(Shader* shader, ObjectModel* ourModel, unsigned int amount);

	void Update(float dt, glm::vec3 position, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

	void draw(const glm::mat4& viewProjMtx, float time, Camera* cam);

private:
	std::vector<Particle> particles;
	unsigned int amount;

	Shader* shader;
	ObjectModel* ourModel;


	void setShader(Shader* s) { shader = s; }
	void setModel(ObjectModel* m) { ourModel = m; }

	void init();

	unsigned int firstUnusedParticle();

	void respawnParticle(Particle& particle, glm::vec3 position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

};