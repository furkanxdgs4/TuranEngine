#include "EngineSource/Engine_Includes.h"


void Allocate(size_t size_inbytes, void* ptr) {
	ptr = new char[size_inbytes];
}
void Set_1Byte(void* ptr, char data_value) {
	memset(ptr, data_value, 1);
}
