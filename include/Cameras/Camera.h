#ifndef __CAMERA__
#define __CAMERA__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Vector3D.h"
#include "Point3D.h"
#include "World.h"
#include "Constants.h"

class Camera {

    public:
        Camera();
        Camera(Point3D e, Point3D l);
        Camera(Point3D e, Point3D l, Vector3D u);
        Camera(Point3D e, Point3D l, Vector3D u, float exp);
        virtual ~Camera();

        void set_eye(Point3D e);
        void set_eye(double x, double y, double z);
        Point3D get_eye();

        void set_lookat(Point3D l);
        void set_lookat(double x, double y, double z);
        Point3D get_lookat();

        void set_up(Vector3D u);
        void set_up(double x, double y, double z);
        Vector3D get_up();
        void set_roll_angle(double angle);

    // Camera functions
        void
        compute_uvw();

        virtual void
        render_scene(World& w) = 0;

		virtual void
        opencl_render_scene(World& w) = 0;

    protected:
        Point3D eye;
        Point3D lookat;
        Vector3D up;
        Vector3D u, v, w;
        float exposure_time;
};

inline void Camera::set_eye(Point3D e){
    eye = e;
}

inline void Camera::set_eye(double x, double y, double z){
    eye = Point3D(x,y,z);
}

inline Point3D Camera::get_eye(){
    return eye;
}

inline void Camera::set_lookat(Point3D l){
    lookat = l;
}

inline void Camera::set_lookat(double x, double y, double z){
    lookat = Point3D(x,y,z);
}

inline Point3D Camera::get_lookat(){
    return lookat;
}

inline void Camera::set_up(Vector3D u){
    up = u;
}

inline void Camera::set_up(double x, double y, double z){
    up = Vector3D(x,y,z);
}

inline Vector3D Camera::get_up(){
    return up;
}

/**
 * Rotates the up vector `angle` degrees
 */
inline void Camera::set_roll_angle(double angle){
    up = Vector3D(sin(angle * TWO_PI / 360), cos(angle * TWO_PI / 360), 0);
}

#endif
