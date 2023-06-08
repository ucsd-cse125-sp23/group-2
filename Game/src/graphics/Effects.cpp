#include "Effects.h"
/*
Particle::Particle() {
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = 1;
}
Particle::Particle(float m, glm::vec3 v, glm::vec3 p, float r, float t, ObjectModel * mod) {
	bounces = 0;
	velocity = v;
	position = p;
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = m;
	radius = r;
	ttl = t;
	model = mod;
	timeAlive = 0;
}

void Particle::applyForce(glm::vec3& f) {
	force += f;
	
}
void Particle::applyImpulse(glm::vec3& imp) {
	velocity += imp / mass;

}
void Particle::integrate(float deltaTime) {
	glm::vec3 acceleration = (1.0f / mass) * force;
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
	force = glm::vec3(0.0f);
	//if (sphere) {
	//	sphere->update(position);
	//}
	if (ttl != -1) {
		timeAlive += deltaTime;
	}
}
void Particle::draw(const glm::mat4& viewProjMtx, GLuint shader) {
	glm::mat4 VW = viewProjMtx * glm::translate(position);
	//sphere->draw(VW, shader);
}

void EffectSystem::update(float dt) {
    time += dt;

    //apply forces

    //apply gravity
    for (auto p : particles) {
        if (p) {
            //apply gravity
            p->applyForce(p->getMass() * glm::vec3(0.0f, Gravity, 0.0f));
        }
    }

    //integrate
    for (auto p : particles) {
        if (p) {
            p->integrate(dt);
        }
    }
    //check for deleted particles
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles.at(i);
        if (p && p->getPosition().y < -1) {
            particles.at(i) = NULL;
            lastIndex.push(i);
            delete p;
            particleCount--;
        }
        else if (p && p->getTimeAlive() >= p->getTTL()) {
            particles.at(i) = NULL;
            lastIndex.push(i);
            delete p;
            particleCount--;
        }
    }


    //check collisions
    for (auto p : particles) {
        if (p) {
            p->checkColisions(ColElasticity, ColFriction);
        }
    }

}
*/