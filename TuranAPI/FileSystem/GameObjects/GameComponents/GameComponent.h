#pragma once
#include "TuranAPI/API_includes.h"
enum class GameComponentType : unsigned char;

namespace TuranAPI {
	class TURANAPI GameComponent {
	protected:
	public:
		string NAME;
		enum class GameComponentType TYPE;

		vec3 POSITION, ROTATION, SCALE;
		//Is component's transform changed this frame? Default is true because application starts!
		bool is_TRANSFORM_CHANGED = true;
		GameComponent(enum class GameComponentType type);


		//Transport a component to a new position!
		void Translate(vec3 new_position);
		//Rotate a component to a new rotation!
		void Rotate(vec3 new_rotation);
		//Scale a component to a new rotation!
		void Scale(vec3 new_scale);

		vec3 Get_Position();
		vec3 Get_Rotation();
		vec3 Get_Scale();
	};
}