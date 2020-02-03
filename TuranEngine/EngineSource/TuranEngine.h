#pragma once
#include "Engine_Includes.h"

#include "Graphics/SYSTEM_INSPECTOR.h"

//Get engine systems
#include "Engine/ENGINE_HEADER.h"


class ENGINE TuranEngine {
	//Turan Engine doesn't work as object! So, this is a empty constructor!
	TuranEngine();

	static bool ShouldApplicationClose;
	static string ApplicationClose_Reason;

	static void Start_GFX();
public:
	//Access GFX from everywhere in an application that uses TuranEngine!
	//But please, don't change or delete the pointer!
	static GFX::GFX_Core* GFX;

	static void Start_Engine();
	static void Take_Inputs();
	static void Close_Engine(string reason);
	static bool ShouldEngine_Close();
	static string Why_Engine_Closed();
};
