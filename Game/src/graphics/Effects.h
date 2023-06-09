#pragma once
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
#include <algorithm>
#include <stack>
#include <random>
#include <map>
#include "Camera.h"
class Particle {
private:
	glm::vec3 position, velocity, force, normal;
	glm::mat4 modelMtx;
	glm::vec3 color;
	ObjectModel* model;
	Shader* shader;
	bool rotate;
	float distFromCam;
	float mass;
	float bounces;
	float radius;
	float ttl;
	float timeAlive;
public:
	Particle();
	Particle(float m, glm::vec3 v, glm::vec3 p, float r, float t, ObjectModel* mod, Shader * s, bool rotate);
	~Particle();
	void applyForce(glm::vec3& f);
	void applyImpulse(glm::vec3& imp);
	void integrate(float deltaTime);
	void draw(const glm::mat4& viewProjMtx, Camera* cam);
	void setColor(glm::vec3& c) { color = c; }
	glm::vec3 getPosition() { return position; };
	glm::vec3 getVelocity() { return velocity; };
	glm::vec3 getNormal() { return normal; };
	float getTimeAlive() { return timeAlive; }
	float getTTL() { return ttl; }
	float getRadius() { return radius; };
	float getMass() { return mass; };
	float getDistFromCam() { return distFromCam; }
	void setDistFromCam(float dist) { distFromCam = dist; }
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
	float lastTime;
	float timeOfLastParticleSpawn;

	int particleCount;
	int maxParticles = 500;
	std::stack<size_t> lastIndex;
	std::vector<ObjectModel*> models;
	std::vector<Shader*> shader;

	//effect maps
	std::map<int, bool> playerJumpActive;
public:
	EffectSystem();
	bool load();
	void update(float dt);
	void spawnParticle(glm::vec3& location, ObjectModel* m, glm::vec3 color, Shader* s, bool rot);
	void draw(const glm::mat4& viewProjMtx, Camera* cam);
	void playerJumpEffect(glm::vec3& location);
	void teslaAttackEffect(glm::vec3& location);
	void resourceEffect(glm::vec3& location, int model_id);
};
