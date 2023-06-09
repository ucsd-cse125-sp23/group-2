#include "Effects.h"


Particle::Particle() {
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = 1;
}
Particle::Particle(float m, glm::vec3 v, glm::vec3 p, float r, float t, ObjectModel * mod, Shader * s, bool rot) {
	bounces = 0;
	velocity = v;
	position = p;
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	mass = m;
	radius = r;
	ttl = t;
	model = mod;
    shader = s;
	timeAlive = 0;
    rotate = rot;
}

void Particle::applyForce(glm::vec3& f) {
	force += f;
	
}
void Particle::applyImpulse(glm::vec3& imp) {
	velocity += imp / mass;

}
Particle::~Particle() {
    
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
void Particle::draw(const glm::mat4& viewProjMtx, Camera* cam) {
    //rotate to look at player
    if (rotate) {
        glm::vec3 toVec = glm::normalize(cam->getCameraPosition() - position);
        float angle = glm::atan(toVec.z, toVec.x);
        modelMtx = glm::rotate(-angle, glm::vec3(0, 1, 0));
        modelMtx *= glm::scale(glm::vec3(0.4f, 0.4f, 0.4f));
    }
    else {
        modelMtx = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
    }
    modelMtx[3] = glm::vec4(position, 1.0f);
    // actiavte the shader program
    shader->use();
    // get the locations and send the uniforms to the shader
    shader->setMat4("viewProj", viewProjMtx);
    shader->setMat4("model", modelMtx);
    shader->setVec3("DiffuseColor", color);
    shader->setFloat("time", timeAlive);
    shader->setVec3("viewPos", cam->getCameraPosition());

    model->Draw(*shader);

    glUseProgram(0);

    glBindTexture(GL_TEXTURE_2D, 0);
	//sphere->draw(VW, shader);
}
EffectSystem::EffectSystem() {
    models.push_back(new ObjectModel("../assets/cube/cube.obj"));
    models.push_back(new ObjectModel("../assets/particles/particle_dust.obj"));
    models.push_back(new ObjectModel("../assets/particle_tesla/particle_tesla.obj"));
    shader.push_back(new Shader("../shaders/particleShader.vert", "../shaders/particleShader.frag"));
    shader.push_back(new Shader("../shaders/teslaEffectShader.vert", "../shaders/smokeFrag.frag"));
    shader.push_back(new Shader("../shaders/smokeVert.vert", "../shaders/smokeFrag.frag"));
    PositionX = 0.0f;
    PositionY = 5.0f;
    PositionZ = 0.0f;
    VelocityX = 0.0f;
    VelocityY = 0.0f;
    VelocityZ = 0.0f;
    PVarianceX = 0.5f;
    VVarianceX = 1.5f;
    PVarianceY = 0.5f;
    VVarianceY = 1.5f;
    PVarianceZ = 0.5f;
    VVarianceZ = 1.5f;
    LSVariance = 0.5f;
    LifeSpan = 1;
    Gravity = -0.4f;
}
void EffectSystem::update(float currTime) {
    //apply gravity
    float dt = glm::clamp(currTime - lastTime, 0.0f, 0.5f);
    lastTime = currTime;
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
    //for (auto p : particles) {
     //   if (p) {
      //      p->checkColisions(ColElasticity, ColFriction);
       // }
    //}

}
void EffectSystem::draw(const glm::mat4& viewProjMtx, Camera* cam) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<Particle*> activeParticles;
    for (auto p : particles) {
        if (p) {
            p->setDistFromCam(glm::distance(cam->getCameraPosition(), p->getPosition()));
            activeParticles.push_back(p);
        }
    }
    std::sort(activeParticles.begin(), activeParticles.end(), [](Particle* a, Particle* b)
        {
            if (a->getDistFromCam() > b->getDistFromCam()) return true;
            else return false;
        });
    for (auto p : activeParticles) {
        p->draw(viewProjMtx, cam);
    }
    glDisable(GL_BLEND);
}
void EffectSystem::teslaAttackEffect(glm::vec3& location) {
    glm::vec3 v = glm::vec3(0, 0, 0);
    glm::vec3 p = location;
    Particle* newP = new Particle(0.01, v, p, ParticleRadius, 2, models[2], shader[1], false);
    newP->setColor(glm::vec3(0.0, 0.4, 0.7));
    if (lastIndex.empty()) {
        particles.push_back(newP);
    }
    else {
        particles.at(lastIndex.top()) = newP;
        lastIndex.pop();
    }
    particleCount++;
}
void EffectSystem::resourceEffect(glm::vec3& location, int model_id) {
    glm::vec3 loc = location;
    PVarianceX = 0.5f;
    VVarianceX = 5.0f;
    PVarianceY = 0.5f;
    VVarianceY = 5.0f;
    PVarianceZ = 0.5f;
    VVarianceZ = 5.0f;
    LSVariance = 0.5f;
    for (int i = 0; i < 6; i++) {
        glm::vec3 c;
        if (model_id == MODEL_ID_RESOURCE) {
            c = glm::vec3(0.4, 0.2, 0.1);
        }
        else {
            c = glm::vec3(1.0, 0.2, 0.0);
        }
        spawnParticle(location, models[0], c, shader[0], false);
    }
}
void EffectSystem::playerJumpEffect(glm::vec3& location) {
    PVarianceX = 0.5f;
    VVarianceX = 1.5f;
    PVarianceY = 0.5f;
    VVarianceY = 0.5f;
    PVarianceZ = 0.5f;
    VVarianceZ = 1.5f;
    LSVariance = 0.5f;
    glm::vec3 loc = location;
    for (int i = 0; i < 1; i++) {
        spawnParticle(location, models[1], glm::vec3(0.6, 0.3, 0.2), shader[2], true);
    }
}
void EffectSystem::spawnParticle(glm::vec3 & location, ObjectModel * m, glm::vec3 color, Shader * s, bool rot) {
    //apply randomness
    std::uniform_real_distribution<double> pdisx(-PVarianceX, PVarianceX);
    std::uniform_real_distribution<double> pdisy(-PVarianceY, PVarianceY);
    std::uniform_real_distribution<double> pdisz(-PVarianceZ, PVarianceZ);
    std::uniform_real_distribution<double> vdisx(-VVarianceX, VVarianceX);
    std::uniform_real_distribution<double> vdisy(-VVarianceY, VVarianceY);
    std::uniform_real_distribution<double> vdisz(-VVarianceZ, VVarianceZ);
    std::uniform_real_distribution<double> ldis(-LSVariance, LSVariance);
    std::random_device rd;
    std::mt19937 gen(rd());
    glm::vec3 v = glm::vec3(VelocityX + vdisx(gen), VelocityY + vdisy(gen), VelocityZ + vdisz(gen));
    glm::vec3 p = glm::vec3(location.x + pdisx(gen), std::max(0.001, location.y + pdisy(gen) - 1.0f), location.z + pdisz(gen));
    Particle* newP = new Particle(0.01, v, p, ParticleRadius, std::max(0.0, LifeSpan + ldis(gen)), m, s, rot);
    newP->setColor(color);
    if (lastIndex.empty()) {
        particles.push_back(newP);
    }
    else {
        particles.at(lastIndex.top()) = newP;
        lastIndex.pop();
    }
    particleCount++;
}