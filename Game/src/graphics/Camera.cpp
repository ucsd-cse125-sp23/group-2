////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

float VerticalCamOffset = 3.0f;

Camera::Camera() {
    Reset();
}
void Camera::update(glm::vec3 & playerPos, float dx, float dy, float sy) {
    // Compute camera world matrix
    playerPosition = playerPos;
    calcPitch(dy);
    calcAngle(dx);
    float horizontalDist = calcHorizontalDist();
    float verticalDist = calcVeticalDist();
    calcCameraPosition(horizontalDist, verticalDist);
    yaw = 180 - angleAroundPlayer;
    calcViewProjectMtx();
    //std::cout << "angle: " << angleAroundPlayer << "\n";
    /*
    glm::mat4 world(1);
    world[3][2] = Distance;
    world = glm::eulerAngleY(glm::radians(-Azimuth)) * glm::eulerAngleX(glm::radians(-Incline)) * world;

    // Compute view matrix (inverse of world matrix)
    glm::mat4 view = glm::translate(-position) *
        glm::rotate(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    // Compute perspective projection matrix
    glm::mat4 project = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Compute final view-projection matrix
    ViewProjectMtx = project * view;
    */
    
}



void calcZoom();
void Camera::calcPitch(float dy) {
    pitch -= dy * 0.1f;
    pitch = glm::clamp(pitch, -40.0f, 85.0f);
}
void Camera::calcAngle(float dx) {
    angleAroundPlayer -= dx * 0.2f;
}

float Camera::calcHorizontalDist() {
    return distanceFromPlayer * glm::cos(glm::radians(pitch));
}
float Camera::calcVeticalDist() {
    return distanceFromPlayer * glm::sin(glm::radians(pitch));
}
void Camera::calcCameraPosition(float hDist, float vDist) {
    float theta = angleAroundPlayer;
    float offsetX = hDist * glm::sin(glm::radians(theta));
    float offsetZ = hDist * glm::cos(glm::radians(theta));
    position.x = playerPosition.x - offsetX;
    position.y = playerPosition.y + vDist + VerticalCamOffset;
    position.z = playerPosition.z - offsetZ;
}
void Camera::calcViewProjectMtx() {
    glm::mat4 vm(1);
    vm *= glm::rotate(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::translate(-position);
    glm::mat4 project = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);
    ViewProjectMtx = project * vm;
    
}

glm::vec3 Camera::getDirectionVector() {
    glm::vec3 directionVector = glm::vec3(glm::cos(glm::radians(-pitch)) * glm::sin(glm::radians(angleAroundPlayer)),
        glm::sin(glm::radians(-pitch)),
        glm::cos(glm::radians(-pitch)) * glm::cos(glm::radians(angleAroundPlayer)));
    //std::cout << "vector: " << directionVector.x << " " << directionVector.y << " " << directionVector.z << "\n ";
    return directionVector;
}
void Camera::Reset() {
    position = glm::vec3(0, 0, 0);
    distanceFromPlayer = 5;
    angleAroundPlayer = 0;
    pitch = 25;
    yaw = 0;

    FOV = 100.0f;
    Aspect = float(RES_WIDTH) / float(RES_HEIGHT);
    NearClip = 0.1f;
    FarClip = 10000.0f;

}