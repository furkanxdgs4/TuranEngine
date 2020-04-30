#include "TuranAPI/TSTL/TSTL_Core.h"
#include <iostream>
namespace TuranAPI {
	namespace STL {

		String::String(TuranAPI::MemoryManagement::IAllocator* MemoryAllocator) : Allocator(MemoryAllocator) {

		}
		String::String(const char* text, TuranAPI::MemoryManagement::IAllocator* MemoryAllocator) : Allocator(MemoryAllocator) {
			*this = text;
		}
		String::String() {
			Allocator = LASTUSEDALLOCATOR;
		}
		String::String(const char* text) : String(text, LASTUSEDALLOCATOR) {

		}
		String::String(const String& object) {
			String(object, object.Allocator);
		}
		String::String(const String& object, TuranAPI::MemoryManagement::IAllocator* Allocator_) : Allocator(Allocator_){
			std::cout << "Copy Constructor for string has started! String to assign from: " << object << std::endl;
			buffer_length = object.buffer_length;
			pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(object.buffer_length);
			memcpy(pointer_toBuffer, object.pointer_toBuffer, sizeof(char) * buffer_length);
		}
		String::~String() {
			if (pointer_toBuffer == nullptr) {
				return;
			}
			if (pointer_toBuffer != nullptr) {
				Allocator->Delete_MemoryBlock(pointer_toBuffer);
			}
		}
		//Just copies the content, nothing fancy here!
		String& String::operator= (const String& string_toAssignFrom) {
			std::cout << "Operator = for string has started! String to Assign From: " << string_toAssignFrom << std::endl;
			unsigned int unnecessary_lettersnumber = 0;
			if (string_toAssignFrom.buffer_length > buffer_length) {
				if (pointer_toBuffer) {
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
				}
				pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(string_toAssignFrom.buffer_length);
				buffer_length = string_toAssignFrom.buffer_length;
			}
			else {
				unnecessary_lettersnumber = buffer_length - string_toAssignFrom.buffer_length;
			}
			memcpy(pointer_toBuffer, string_toAssignFrom.pointer_toBuffer, sizeof(char)* string_toAssignFrom.buffer_length);
			//Delete unnecessary letters of previous char buffer!
			for (size_t erase_unnecessarylettersindex = 0; erase_unnecessarylettersindex < unnecessary_lettersnumber; erase_unnecessarylettersindex++) {
				pointer_toBuffer[buffer_length - erase_unnecessarylettersindex] = 0;
			}
			return *this;
		}
		//This is only called from Vector
		String& String::operator= (const char* text) {
			if (text == nullptr) {
				std::cout << "There is no text to assign!\n";
				return *this;
			}
			if (Allocator == nullptr) {
				std::cout << "There is no allocator for the String!\n";
				return *this;
			}
			if (pointer_toBuffer == nullptr && buffer_length == 0) {
				while (text[buffer_length] != 0) {
					buffer_length++;
				}
				//Given text doesn't need to be null terminated, we add null termination!
				buffer_length++;
				pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(buffer_length * sizeof(char));
				memcpy(pointer_toBuffer, text, buffer_length);
				return *this;
			}
			else {
				unsigned int text_length = 0;
				while (text[text_length] != 0) {
					text_length++;
				}
				text_length += 1;

				if (text_length < buffer_length) {
					std::cout << "1!\n";
					memcpy(pointer_toBuffer, text, sizeof(char) * text_length);
					unsigned int unnecessary_lettersnumber = buffer_length - text_length;
					//Delete unnecessary letters of previous char buffer!
					for (size_t erase_unnecessarylettersindex = 0; erase_unnecessarylettersindex < unnecessary_lettersnumber; erase_unnecessarylettersindex++) {
						pointer_toBuffer[buffer_length - erase_unnecessarylettersindex] = 0;
					}
				}
				else if (text_length == buffer_length) {
					memcpy(pointer_toBuffer, text, sizeof(char) * text_length);
				}
				else {
					if (pointer_toBuffer) {
						Allocator->Delete_MemoryBlock(pointer_toBuffer);
					}
					pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(text_length * sizeof(char));
					buffer_length = text_length;
					memcpy(pointer_toBuffer, text, sizeof(char) * text_length);
				}
			}
		}
		char& String::operator[] (unsigned int index) const {
			return pointer_toBuffer[index];
		}
		bool String::operator== (const char* text) const {
			if (pointer_toBuffer == nullptr) {
				return false;
			}
			unsigned int text_length = 0;
			while (text[text_length] != 0) {
				text_length++;
			}
			if (text_length != buffer_length - 1) {
				return false;
			}
			text_length++;
			for (size_t i = 0; i < buffer_length - 1; i++) {
				if (text[i] != pointer_toBuffer[i]) {
					return false;
				}
			}
			return true;
		}
		bool String::operator== (const String& text) const {
			if (pointer_toBuffer == nullptr) {
				return false;
			}
			if (text.text_length() + 1 != buffer_length) {
				return false;
			}
			for (size_t i = 0; i < buffer_length; i++) {
				if (text[i] != pointer_toBuffer[i]) {
					return false;
				}
			}
			return true;
		}
		bool String::operator!=(const char* text) const {
			return !(*this == text);
		}
		bool String::operator!=(const String& text) const {
			return !(*this == text);
		}
		String::operator const char* () const {
			return pointer_toBuffer;
		}
		std::ostream& operator<<(std::ostream& os, const String& string_toshow) {
			if (string_toshow.pointer_toBuffer != nullptr) {
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
		size_t String::find_lastof(char letter) const {
			for (size_t index = buffer_length - 1; index > 0; index) {
				index--;
				if (letter == pointer_toBuffer[index]) {
					return index;
				}
			}
			//Means failed!
			return buffer_length;
		}
		size_t String::text_length() const {
			return buffer_length;
		}
		String* String::substr(size_t from, size_t to) const {
			String* str = new (LASTUSEDALLOCATOR) String();
			size_t length = to - from + 2;
			str->pointer_toBuffer = (char*)str->Allocator->Allocate_MemoryBlock(length * sizeof(char));
			str->buffer_length = length;
			memcpy(str->pointer_toBuffer, pointer_toBuffer + (from * sizeof(char)), length - 1);
			str->pointer_toBuffer[buffer_length - 1] = 0;
			return str;
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
			unsigned int text_length = 0;
			while (text[text_length] != 0) {
				text_length++;
			}
			text_length += 1;
			if (buffer_length > 0) {
				char* new_block = (char*)Allocator->Allocate_MemoryBlock(text_length + buffer_length - 1);
				memcpy(new_block, pointer_toBuffer, (buffer_length - 2) * sizeof(char));
				memcpy(new_block + ((buffer_length - 2) * sizeof(char)), text, text_length * sizeof(char));
				Allocator->Delete_MemoryBlock(pointer_toBuffer);
				pointer_toBuffer = new_block;
				buffer_length += text_length - 1;
			}
			else {
				pointer_toBuffer = (char*)Allocator->Allocate_MemoryBlock(text_length * sizeof(char));
				memcpy(pointer_toBuffer, text, text_length * sizeof(char));
				buffer_length = text_length;
			}
		}



		Vector<String>::Vector(TuranAPI::MemoryManagement::IAllocator* Allocator_, size_t expanding_size, size_t size) : Allocator(Allocator_), expand_size(expanding_size) {
			pointer_toBuffer = (String*)Allocator->Allocate_MemoryBlock(size * sizeof(String));
			if (pointer_toBuffer) {
				buffer_size = size;
				buffer_length = 0;
			}
			else {
				std::cout << "A vector of " << size << "elements isn't created!\n";
			}
		}
		Vector<String>::Vector(TuranAPI::MemoryManagement::IAllocator* Allocator_, size_t expanding_size, std::initializer_list<const char*>& list) : Allocator(Allocator_), expand_size(expanding_size) {
			for (auto element : list) {
				this->push_back(element);
			}
		}
		Vector<String>::~Vector() {
			for (size_t i = 0; i < buffer_length; i++) {
				pointer_toBuffer[i].~String();
			}
			Allocator->Delete_MemoryBlock(pointer_toBuffer);
		}
		void Vector<String>::push_back(const String& object) {
			if (expand_size == 0 && buffer_length == buffer_size) {
				std::cout << "Vector's expand size is 0, so it's not expandable! No changes.\n";
				return;
			}
			if (!pointer_toBuffer) {
				std::cout << "Vector.push_back() has failed because vector is failed on creation already!\n";
			}
			if (buffer_length == buffer_size) {
				std::cout << "The vector's size is not enough! Expanding its size!";
				void* PTR = Allocator->Allocate_MemoryBlock((buffer_size + expand_size) * sizeof(String));
				if (PTR) {
					memcpy(PTR, pointer_toBuffer, buffer_size * sizeof(String));
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
					pointer_toBuffer = (String*)PTR;
					buffer_size += expand_size;

					memcpy(&pointer_toBuffer[buffer_length], &object, sizeof(String));
					buffer_length += 1;
				}
				else {
					std::cout << "Vector.push_back() has failed!\n";
				}
			}
			else {
				memcpy(&pointer_toBuffer[buffer_length], &object, sizeof(String));
				buffer_length += 1;
			}
		}
		void Vector<String>::push_back(const char* text) {
			if (expand_size == 0 && buffer_length == buffer_size) {
				std::cout << "Vector's expand size is 0, so it's not expandable! No changes.\n";
				return;
			}
			if (!pointer_toBuffer) {
				std::cout << "Vector.push_back() has failed because vector is failed on creation already!\n";
			}
			if (buffer_length == buffer_size) {
				std::cout << "The vector's size is not enough! Expanding its size!";
				void* PTR = Allocator->Allocate_MemoryBlock((buffer_size + expand_size) * sizeof(String));
				if (PTR) {
					memcpy(PTR, pointer_toBuffer, buffer_size * sizeof(String));
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
					pointer_toBuffer = (String*)PTR;
					buffer_size += expand_size;

					this->push_back(text);
				}
				else {
					std::cout << "Vector.push_back() has failed!\n";
				}
			}
			else {
				String empty;
				memcpy(&pointer_toBuffer[buffer_length], &empty, sizeof(String));
				pointer_toBuffer[buffer_length] = text;
				buffer_length += 1;
			}
		}
		void Vector<String>::erase(size_t index) {
			if (index > buffer_length) {
				std::cout << "There is no initialized element of index: " << index << std::endl;
				return;
			}
			if (index == buffer_length - 1) {
				std::cout << "Last element already!\n";
				buffer_length -= 1;
				pointer_toBuffer[index].~String();
				for (unsigned int i = 0; i < sizeof(String); i++) {
					*((char*)pointer_toBuffer + (sizeof(String) * index) + i) = 0;
				}
				return;
			}
			pointer_toBuffer[index].~String();
			for (unsigned int i = 0; i < (buffer_length - 1) - index; i++) {
				memcpy(&pointer_toBuffer[index + i], &pointer_toBuffer[index + i + 1], sizeof(String));
			}
			buffer_length -= 1;
		}
		void Vector<String>::clear() {
			for (unsigned int i = 0; i < buffer_length * sizeof(String); i++) {
				*((char*)pointer_toBuffer + i) = 0;
			}
			buffer_length = 0;
		}
		String& Vector<String>::operator[](size_t index) {
			if (index < buffer_length) {
				return pointer_toBuffer[index];
			}
			else {
				std::cout << "There is no element with index " << index << " in the Vector!\n";
				String defaultvar;
				return defaultvar;
			}
		}
		Vector<String>& Vector<String>::operator= (std::initializer_list<const char*>& list) {
			for (auto element : list) {
				this->push_back(element);
			}
			return *this;
		}
		const String& Vector<String>::operator[](size_t index) const {
			if (index < buffer_length) {
				return pointer_toBuffer[index];
			}
			else {
				std::cout << "There is no element with index " << index << " in the Vector!\n";
				String defaultvar;
				return defaultvar;
			}
		}
		size_t Vector<String>::size() const {
			return buffer_length;
		}
		size_t Vector<String>::max_size() const {
			return buffer_size;
		}
		void Vector<String>::resize(size_t resize) {
			std::cout << "The vector's resizing to " << resize << " elements!\n";
			if (resize > buffer_size) {
				void* PTR = Allocator->Allocate_MemoryBlock((resize) * sizeof(String));
				if (PTR) {
					memcpy(PTR, pointer_toBuffer, buffer_size * sizeof(String));
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
					pointer_toBuffer = (String*)PTR;
					buffer_size = resize;
				}
				else {
					std::cout << "Vector.push_back() has failed!\n";
				}
			}
			else if (resize < buffer_size && resize < buffer_length) {
				//Set every byte to 0
				for (unsigned int i = 0; i < (buffer_length - resize) * sizeof(String); i++) {
					*((char*)pointer_toBuffer + (resize * sizeof(String)) + i) = 0;
				}
				buffer_length = resize;
			}
			else if (resize < buffer_size && resize > buffer_length) {
				//Set every byte to 0
				for (unsigned int i = 0; i < (resize - buffer_length) * sizeof(String); i++) {
					*((char*)pointer_toBuffer + (buffer_length * sizeof(String)) + i) = 0;
				}
			}
		}
		String* Vector<String>::data() {
			return pointer_toBuffer;
		}
		String& Vector<String>::Get(size_t index) const {
			return pointer_toBuffer[index];
		}
	}
}