#pragma once
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
#include <stack>
class Particle {
private:
	glm::vec3 position, velocity, force, normal;
	ObjectModel* model;
	float mass;
	float bounces;
	float radius;
	float ttl;
	float timeAlive;
public:
	Particle();
	Particle(float m, glm::vec3 v, glm::vec3 p, float r, float t, ObjectModel* mod);
	~Particle();
	void applyForce(glm::vec3& f);
	void applyImpulse(glm::vec3& imp);
	void integrate(float deltaTime);
	void setFixed(bool f);
	void setPosition(glm::vec3 pos);
	void rotate(float degrees, glm::vec3& point);
	void addNormal(glm::vec3 normal);
	void update();
	void checkColisions();
	void checkColisions(float e, float f);
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	glm::vec3 getNormal();
	float getTimeAlive() { return timeAlive; }
	float getTTL() { return ttl; }
	float getRadius();
	float getMass();
};

class EffectSystem {
private:
	//particle parameters
	int ParticlesPerSecond;
	float PositionX, PositionY, PositionZ, VelocityX, VelocityY, VelocityZ;
	float PVarianceX, PVarianceY, PVarianceZ, VVarianceX, VVarianceY, VVarianceZ;
	float LifeSpan, LSVariance;
	float Gravity, AirDensity, AirFlow, DragCoef, ParticleRadius, ColElasticity, ColFriction;

	std::vector<Particle*> particles;
	float time;
	float timeOfLastParticleSpawn;
	int particleCount;
	int maxParticles = 100;
	std::stack<size_t> lastIndex;
public:
	bool load();
	void update(float dt);
	void spawnParticle();
	void draw(const glm::mat4& viewProjMtx);
};
