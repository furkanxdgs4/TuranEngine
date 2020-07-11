#pragma once
#include "Engine_Includes.h"


namespace Engine {

	class ENGINE TuranLang_Core {
		struct CODEFILE {
			String PATH;
			unsigned int FILE_ID;
			void* BINARY_CODE;
		};
		Vector<CODEFILE> CODEFILEs;
		//EXPLANATION IS AT THE END!
		//The code will use an index to this Vector to load back intended variable
		Vector<void*> MAX_PTRCount;

		void** PTR_Cache;
	public:
		TuranLang_Core(const char* MainCode_FilePath);
	};
}
/*

MAX_PTRCount's usage: If some other procceses are gonna fill cache, "i" should be stored somewhere rather than cache. That's the purpose of the MAX_PTRCount
int i = 0;
EngineRunLoop();
i = 5;



*/