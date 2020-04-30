#include "TuranAPI/TSTL/TSTL_Core.h"
#include <iostream>
namespace TuranAPI {
	namespace STL {
		unsigned int Find_Optimum_BufferCapacity(unsigned int text_length) {
			if (text_length < 9) {
				return 9;
			}
			else if (text_length > 8 && text_length < 29) {
				return 29;
			}
			else if (text_length > 28 && text_length < 49) {
				return 49;
			}
			else if (text_length > 48 && text_length < 149) {
				return 149;
			}
			else if (text_length > 148 && text_length < 999) {
				return 999;
			}
			else if (text_length > 998 && text_length < 9999) {
				return 9999;
			}
			else {
				return 63999;
			}
		}


		String::String(const char* text, TuranAPI::MemoryManagement::IAllocator* MemoryAllocator) : Allocator(MemoryAllocator) {
			if (text == nullptr) {
				pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(1);
				return;
			}
			*this = text;
		}
		String::String(const String& text, TuranAPI::MemoryManagement::IAllocator* Allocator) : String(text.c_str(), text.Allocator){
		}
		String::~String() {
			if (pointer_toBuffer == nullptr) {
				return;
			}
			if (pointer_toBuffer != nullptr) {
				Allocator->Delete_MemoryBlock(pointer_toBuffer);
			}
		}
		String& String::operator= (const String& text) {
			*this = text.c_str();
			return *this;
		}
		String& String::operator= (const char* text) {
			unsigned int cstrtext_length = 0;
			while (text[cstrtext_length] != 0) {
				cstrtext_length++;
			}

			if (Allocator == nullptr) {
				Allocator = LASTUSEDALLOCATOR;
			}

			//If capacity is bigger than assigned text_length
			if (text_capacity > cstrtext_length) {
				memset(pointer_toBuffer, 0, text_capacity + 1);
				memcpy(pointer_toBuffer, text, cstrtext_length + 1);
				text_length = cstrtext_length;
				return *this;
			}
			//If assigned text_length is bigger than capacity
			else {
				if (pointer_toBuffer) {
					delete pointer_toBuffer;
				}
				text_capacity = Find_Optimum_BufferCapacity(cstrtext_length);
				char* new_block = (char*)Allocator->Allocate_MemoryBlock(text_capacity);
				memcpy(new_block, text, cstrtext_length + 1);
				pointer_toBuffer = new_block;
				text_length = cstrtext_length;
			}
		}
		char& String::operator[] (unsigned int index) const {
			return pointer_toBuffer[index];
		}
		bool String::operator== (const char* text) const {
			if (pointer_toBuffer == nullptr) {
				return false;
			}
			unsigned int cstrtext_length = 0;
			while (text[cstrtext_length] != 0) {
				cstrtext_length++;
			}
			if (cstrtext_length != text_length) {
				return false;
			}
			for (unsigned int i = 0; i < text_length; i++) {
				if (text[i] != pointer_toBuffer[i]) {
					return false;
				}
			}
			return true;
		}
		bool String::operator== (const String& text) const {
			return *this == text.c_str();
		}
		bool String::operator!=(const char* text) const {
			return !(*this == text);
		}
		bool String::operator!=(const String& text) const {
			return !(*this == text.c_str());
		}
		String::operator const char* () const {
			return pointer_toBuffer;
		}
		std::ostream& operator<<(std::ostream& os, const String& string_toshow) {
			if (string_toshow.pointer_toBuffer) {
				const char* pointer = string_toshow.pointer_toBuffer;
				os << string_toshow.pointer_toBuffer;
				return os;
			}
			else {
				return os;
			}
		}
		const char* String::c_str() const {
			return *this;
		}
		unsigned int String::find_lastof(char letter) const {
			for (unsigned int index = text_length; index > 0; index) {
				index--;
				if (letter == pointer_toBuffer[index]) {
					return index;
				}
			}
			//Failed!
			return text_length;
		}
		unsigned int String::length() const {
			return text_length;
		}
		unsigned int String::capacity() const {
			return text_capacity;
		}
		void String::append(const char* text) {
			if (text == nullptr && pointer_toBuffer != nullptr) {
				std::cout << "Nothing appended to a non-empty text!\n";
				return;
			}
			else if (text == nullptr) {
				std::cout << "Append Text and Pointer_toBuffer string is empty!\n";
				return;
			}
			unsigned int cstrtext_length = 0;
			while (text[cstrtext_length] != 0) {
				cstrtext_length++;
			}

			if (text_length > 0) {
				//Capacity allows to append a text without expansion!
				if (text_capacity >= (text_length + cstrtext_length)) {
					memcpy(pointer_toBuffer + text_length, text, cstrtext_length + 1);
					text_length += cstrtext_length;
					return;
				}
				//Capacity should be expanded!
				else {
					text_capacity = Find_Optimum_BufferCapacity(text_length + cstrtext_length);
					char* new_block = (char*)Allocator->Allocate_MemoryBlock(text_capacity + 1);
					if (new_block) {
						memcpy(new_block, pointer_toBuffer, text_length);
						memcpy(new_block + text_length, text, cstrtext_length + 1);
						delete pointer_toBuffer;
						pointer_toBuffer = new_block;
						text_length += cstrtext_length;
					}
					return;
				}
			}
			//This means there is no text, so operation will be same as assignment!
			else {
				*this = text;
			}
		}
		void String::resize(unsigned int size) {
			if (text_length >= size) {
				memset(pointer_toBuffer + size, 0, text_length + 1 - size);
				text_length = size;
			}
			else {
				text_capacity = size;
				char* new_block = (char*)Allocator->Allocate_MemoryBlock(size + 1);
				if (new_block) {
					memcpy(new_block, pointer_toBuffer, text_length + 1);
					delete pointer_toBuffer;
					pointer_toBuffer = new_block;
					text_length = size;
				}
			}
		}



	}
}