#pragma once
#include "Engine_Includes.h"
#include "GFXSource/GFX_Core.h"

namespace Engine {
	using ShouldEngineWait_Callback = bool (*)();

	class ENGINE TuranEngine_Core {
		bool ShouldApplicationClose;
		string ApplicationClose_Reason;

		void Start_GFX(TuranAPI::TAPI_Systems* TAPISystems);
		ShouldEngineWait_Callback Waiting_inClose_Callback;
	public:
		TuranEngine_Core(TuranAPI::TAPI_Systems* TAPISystems);
		~TuranEngine_Core();

		static TuranEngine_Core* SELF;
		static GFX_API::GFX_Core* Get_GFXSystems();

		void Take_Inputs();
		void Close_Engine(const char* reason);
		bool ShouldEngine_Close();
		const char* Why_Engine_Closed();
		void SetEngineClosing_Callback(ShouldEngineWait_Callback Callback_Function);
		ShouldEngineWait_Callback GetEngineClosing_Callback();
	};

	ENGINE TuranEngine_Core* Start_EngineDLL(TuranAPI::TAPI_Systems* TAPISystems);
	ENGINE void Close_EngineDLL();
}

#define TuranEngine Engine::TuranEngine_Core::SELF