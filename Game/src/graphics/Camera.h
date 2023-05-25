////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#include "glm/gtx/euler_angles.hpp"

// The Camera class provides a simple means to controlling the 3D camera. It could
// be extended to support more interactive controls. Ultimately. the camera sets the
// GL projection and viewing matrices.

class Camera {
private:

    //Third person cam vars
    float distanceFromPlayer;
    float angleAroundPlayer;

    glm::vec3 position;
    float pitch;
    float yaw;
    float roll;

    glm::vec3 playerPosition;


    // Old Camera Variables
    // Perspective controls
    float FOV;       // Field of View Angle (degrees)
    float Aspect;    // Aspect Ratio
    float NearClip;  // Near clipping plane distance
    float FarClip;   // Far clipping plane distance

    // Polar controls
    float Distance;  // Distance of the camera eye position to the origin (meters)
    float Azimuth;   // Rotation of the camera eye position around the Y axis (degrees)
    float Incline;   // Angle of the camera eye position over the XZ plane (degrees)

    // Computed data
    glm::mat4 ViewProjectMtx;

public:
    Camera();

    void update(glm::vec3 & playerPos, float dx, float dy, float sy);
    void Reset();

    void calcZoom();
    void calcPitch(float dy);
    void calcAngle(float dx);

    float calcHorizontalDist();
    float calcVeticalDist();
    void calcCameraPosition(float hDist, float vDist);

    void calcViewProjectMtx();
    // Access functions
    
    void SetAspect(float a) { Aspect = a; }
    void SetDistance(float d) { Distance = d; }
    void SetAzimuth(float a) { Azimuth = a; }
    void SetIncline(float i) { Incline = i; }

    float getAngleAroundPlayer() { return angleAroundPlayer; };
    glm::vec3 getDirectionVector();
    glm::vec3 getUpVector();
    glm::vec3 getCameraPosition() { return position; };

    const glm::mat4 &GetViewProjectMtx() { return ViewProjectMtx; }


};