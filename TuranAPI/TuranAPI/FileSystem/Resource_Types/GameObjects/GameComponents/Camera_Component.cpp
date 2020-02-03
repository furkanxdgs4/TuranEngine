#include "Camera_Component.h"
using namespace TuranAPI::Game_Object;

Camera_Component::Camera_Component(vec3 target) : Target(target) {}

TuranAPI::TuranAPI_ENUMs Camera_Component::Get_Component_Type() {
	return TuranAPI::TuranAPI_ENUMs::CAMERA_COMP;
}

void Camera_Component::Set_Camera_Properties(unsigned short fov_in_Angle, float aspect_Width, float aspect_Height, float near_plane, float far_plane) {
	FOV_in_Angle = fov_in_Angle;
	Aspect_Width = aspect_Width;
	Aspect_Height = aspect_Height;
	Near_Plane = near_plane;
	Far_Plane = far_plane;
	is_Projection_Matrix_changed = true;
}

mat4x4 Camera_Component::Calculate_Projection_Matrix() {
	if (is_Projection_Matrix_changed) {
		mat4x4 proj_mat;
		proj_mat = perspective(radians(float(FOV_in_Angle)), float(Aspect_Width / Aspect_Height), Near_Plane, Far_Plane);
		Projection_Matrix = proj_mat;
		is_Projection_Matrix_changed = false;
	}
	return Projection_Matrix;
}

mat4x4 Camera_Component::Calculate_View_Matrix() {
	if (is_TRANSFORM_CHANGED && is_Target_Changed) {
		mat4x4 view_mat;
		vec3 Front_Vector = -Target;
		vec3 World_UP = vec3(0, 1, 0);

		view_mat = lookAt(POSITION, Front_Vector + POSITION, World_UP);
		View_Matrix = view_mat;
		is_TRANSFORM_CHANGED = false;
		is_Target_Changed = false;
	}
	return View_Matrix;
}


unsigned short Camera_Component::Get_FOV_inAngle() { return FOV_in_Angle; }
vec2 Camera_Component::Get_Aspect_Width_and_Height() { return vec2(Aspect_Width, Aspect_Height); }
vec2 Camera_Component::Get_Near_and_FarPlane() { return vec2(Near_Plane, Far_Plane); }
vec3 Camera_Component::Get_Target() { return Target; }

void Camera_Component::Set_Camera_Target(vec3 target) { Target = target; is_Target_Changed = true; }