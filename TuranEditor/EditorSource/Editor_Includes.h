#pragma once
#include "TuranAPI/TuranAPI_Core.h"
#include "EngineSource/TuranEngine.h"

namespace TuranEditor {
	class TE_DependentSystems {
	public:
		TuranAPI::TAPI_Systems TAPISystems;

		TE_DependentSystems();
		~TE_DependentSystems();
	};
}