#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader* shader, ObjectModel* ourModel, unsigned int amount) : shader(shader), ourModel(ourModel), amount(amount) {
	this->init();
}

void ParticleGenerator::Update(float dt, glm::vec3 position, unsigned int newParticles, glm::vec3 offset) {

	for (unsigned int i = 0; i < newParticles; ++i) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], position, offset);
	}

	for (unsigned int i = 0; i < this->amount; ++i) {
		Particle& p = this->particles[i];
		p.Life -= dt;

		if (p.Life > 0.0f) {
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::draw(const glm::mat4& viewProjMtx, float time, Camera* cam) {

	glm::mat4 model = glm::mat4(1.0f);
	int id = 0;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader->use();
	shader->setMat4("viewProj", viewProjMtx);
	shader->setMat4("model", model);
	shader->setFloat("time", time);
	shader->setFloat("rand", id);
	shader->setVec3("viewPos", cam->getCameraPosition());

	for (Particle particle : this->particles) {

		if (particle.Life > 0.0f) {


			//this->shader->setVec2("offset", particle.Position);
			//this->shader->setVec4("color", particle.Color);
			this->ourModel->Draw(*shader);

		}


	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void ParticleGenerator::init() {

	for (unsigned int i = 0; i < this->amount; ++i) {
		this->particles.push_back(Particle());
	}

}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {

	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;

}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec3 position, glm::vec3 offset) {

	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f;

}