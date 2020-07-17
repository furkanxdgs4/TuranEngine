#include "GameComponent.h"
#include "GameComponentTypes.h"

namespace TuranAPI {

	GameComponent::GameComponent(GameComponentType type) : TYPE(type) {}

	//Transport a component to a new position!
	void GameComponent::Translate(vec3 new_position) {
		if (POSITION != new_position) {
			POSITION = new_position;
			is_TRANSFORM_CHANGED = true;
		}
	}

	//Rotate a component to a new rotation!
	void GameComponent::Rotate(vec3 new_rotation) {
		if (ROTATION != new_rotation) {
			ROTATION = new_rotation;
			is_TRANSFORM_CHANGED = true;
		}
	}

	//Scale a component to a new rotation!
	void GameComponent::Scale(vec3 new_scale) {
		if (SCALE != new_scale) {
			SCALE = new_scale;
			is_TRANSFORM_CHANGED = true;
		}
	}

	vec3 GameComponent::Get_Position() { return POSITION; }
	vec3 GameComponent::Get_Rotation() { return ROTATION; }
	vec3 GameComponent::Get_Scale() { return SCALE; }
}