#include "GFX_ENUMs.h"

namespace GFX_API {
	GFXAPI unsigned int Get_UNIFORMTYPEs_SIZEinbytes(UNIFORMTYPE uniform) {
		switch (uniform)
		{
		case UNIFORMTYPE::VAR_FLOAT32:
		case UNIFORMTYPE::VAR_INT32:
		case UNIFORMTYPE::VAR_UINT32:
			return 4;
		case UNIFORMTYPE::VAR_BYTE8:
		case UNIFORMTYPE::VAR_UBYTE8:
			return 1;
		case UNIFORMTYPE::VAR_VEC2:
			return 8;
		case UNIFORMTYPE::VAR_VEC3:
			return 12;
		case UNIFORMTYPE::VAR_VEC4:
			return 16;
		case UNIFORMTYPE::VAR_MAT4x4:
			return 64;
		default:
			TuranAPI::LOG_CRASHING("Uniform's size in bytes isn't set in GFX_ENUMs.cpp!");
			return 0;
		}
	}

	GFXAPI const char* Find_UNIFORM_VARTYPE_Name(UNIFORMTYPE uniform_var_type) {
		switch (uniform_var_type) {
		case UNIFORMTYPE::VAR_UINT32:
			return "Unsigned Integer 32-bit";
		case UNIFORMTYPE::VAR_INT32:
			return "Signed Integer 32-bit";
		case UNIFORMTYPE::VAR_FLOAT32:
			return "Float 32-bit";
		case UNIFORMTYPE::VAR_VEC2:
			return "Vec2 (2 float)";
		case UNIFORMTYPE::VAR_VEC3:
			return "Vec3 (3 float)";
		case UNIFORMTYPE::VAR_VEC4:
			return "Vec4 (4 float)";
		case UNIFORMTYPE::VAR_MAT4x4:
			return "Matrix 4x4";
		case UNIFORMTYPE::VAR_UBYTE8:
			return "Unsigned Byte 8-bit";
		case UNIFORMTYPE::VAR_BYTE8:
			return "Signed Byte 8-bit";
		case UNIFORMTYPE::TEXTURE2D:
			return "Texture 2D";
		default:
			return "Error, Uniform_Var_Type isn't supported by Find_UNIFORM_VARTYPE_Name!\n";
		}
	}
}