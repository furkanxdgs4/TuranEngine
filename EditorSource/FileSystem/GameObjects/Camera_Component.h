#pragma once
#include "GFXSource/GFX_Includes.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"

namespace TuranEditor {
	//This Component doesn't use ROTATION and SCALE components of GameComponent!
	//Instead, you will specify the target vector of camera to look at
	class Camera_Component : public TuranAPI::GameComponent {
		mat4 View_Matrix, Projection_Matrix;
		unsigned short FOV_in_Angle = 45, Aspect_Width = 1920.0f, Aspect_Height = 1080.0f;
		float Near_Plane = 0.01f, Far_Plane = 100000.0f;
		bool is_Projection_Matrix_changed = true, is_Target_Changed = true;
		vec3 Target;
	public:
		Camera_Component(vec3 target);

		//Getters
		mat4x4 Calculate_Projection_Matrix();
		mat4x4 Calculate_View_Matrix();
		unsigned short Get_FOV_inAngle();
		vec2 Get_Aspect_Width_and_Height();
		vec2 Get_Near_and_FarPlane();
		vec3 Get_Target();

		//Setters
		void Set_Camera_Properties(unsigned short fov_in_Angle, float aspect_Width, float aspect_Height, float near_plane, float far_plane);
		void Set_Camera_Target(vec3 Target);
	};
}
