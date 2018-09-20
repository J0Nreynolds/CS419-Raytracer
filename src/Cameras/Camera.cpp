// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Camera
#include "Camera.h"

/**
 * Camera constructor for camera at (0,0,0), looking (0,0,-1),
 * with up vector (0, 1, 0)
 */
Camera::Camera()
: Camera(Point3D(0,0,0), Point3D(0,0,-1)) {}

/**
 * Camera constructor with default up vector (0, 1, 0)
 */
Camera::Camera(Point3D e, Point3D l)
: Camera(e, l, Vector3D(0, 1, 0)) {}

/**
 * Camera constructor with eye, lookat, and up supplied
 */
Camera::Camera(Point3D e, Point3D l, Vector3D u)
: Camera(e, l, u, 1.0) {}

/**
 * Camera constructor with eye, lookat, up, and exposure
 */
Camera::Camera(Point3D e, Point3D l, Vector3D u, float exp)
: eye(e), lookat(l), up(u), exposure_time(exp){
}

Camera::~Camera() {}

/**
 * Computes orthonormal basis for local view space
 */
void Camera::compute_uvw() {
	w = eye - lookat;
	w.normalize();
	u = up ^ w;
	u.normalize();
	v = w ^ u;
}
