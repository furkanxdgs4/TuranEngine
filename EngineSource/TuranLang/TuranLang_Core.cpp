#include "TuranLang_Core.h"

namespace Engine {
	TuranLang_Core::TuranLang_Core(const char* MainCode_FilePath) {
		PTR_Cache = new void* [16];

		CODEFILE MainCode;
		MainCode.PATH = MainCode_FilePath;
		CODEFILEs.push_back(MainCode);
		
		//After loading the MainCode, I should expand MAX_PTRCount to store variables of unfinished scopes.

	}
}