#pragma once
#include "TuranAPI/MemoryManager/MemoryManager_Core.h"
#include "Vector.h"
#include "Bitset.h"


namespace TuranAPI {
	namespace STL {
		//Null terminated char array!
		class String {
			char* pointer_toBuffer = nullptr;
			unsigned int text_length = 0, text_capacity = 0;			//Both of these variables exclude null terminator!
			TuranAPI::MemoryManagement::IAllocator* Allocator;
		public:
			String(const char* text = nullptr, TuranAPI::MemoryManagement::IAllocator* Allocator = LASTUSEDALLOCATOR);
			String(const String& text, TuranAPI::MemoryManagement::IAllocator* Allocator = LASTUSEDALLOCATOR);
			~String();
			String& operator= (const String& string_toAssignFrom);
			String& operator= (const char* text);
			char& operator[] (unsigned int index) const;
			bool operator== (const char* text) const;
			bool operator== (const String& text) const;
			bool operator!= (const char* text) const;
			bool operator!= (const String& text) const;
			operator const char* () const;
			friend std::ostream& operator<<(std::ostream& os, const String& string_toshow);
			unsigned int find_lastof(char letter) const;
			//Excludes null terminator!
			unsigned int length() const;
			unsigned int capacity() const;
			const char* c_str() const;
			void append(const char* text);
			//Size should exclude Null Terminator! 
			void resize(unsigned int size);
		};
		std::ostream& operator<<(std::ostream& os, const String& string_toshow);

	}
}