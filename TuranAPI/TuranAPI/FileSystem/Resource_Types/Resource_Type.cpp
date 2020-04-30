#include "Resource_Type.h"

namespace TuranAPI {
	Resource_Type::Resource_Type(enum class RESOURCETYPEs type) : TYPE(type) {

	}
	enum class RESOURCETYPEs Resource_Type::Get_Type() {
		return TYPE;
	}
}