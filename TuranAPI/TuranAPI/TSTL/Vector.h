#pragma once
#include "TuranAPI/MemoryManager/MemoryManager_Core.h"
#include <iostream>
namespace TuranAPI {
	namespace STL {
		//To avoid unnecessary copies and wasting memory blocks, you should guess the optimum size for your buffer!
		//Your class doesn't have to have a default constructor (except you use resize)
		template <typename T>
		class Vector {
			T* pointer_toBuffer = nullptr;
			size_t buffer_size = 0, buffer_length = 0, expand_size = 0;
			TuranAPI::MemoryManagement::IAllocator* Allocator;
		public:
			//Expanding size variable is used to know how much to expand the vector if it's reached its limits.
			Vector(TuranAPI::MemoryManagement::IAllocator* Allocator, size_t expand_size, size_t size = 0);
			Vector(TuranAPI::MemoryManagement::IAllocator* Allocator, size_t expand_size, std::initializer_list<T>& list);
			~Vector();
			void push_back(const T& object);
			void erase(size_t index);
			void clear();
			T& operator[] (size_t index) const;
			Vector<T>& operator= (std::initializer_list<T>& list);
			Vector<T>& operator= (const Vector<T>& vector);
			size_t size() const;
			size_t max_size() const;
			//Use this function when you have to access Vector with data()!
			//This function uses default constructor for new elements!
			void resize(size_t resize);
			//Use this function when you have to create Vector with zero capacity!
			//Doesn't create elements, only allocates buffer!
			void grow(size_t new_size);

			T* data();
			T& Get(size_t index) const;
			bool is_size_const() const;
		};

		template<class T>
		Vector<T>::Vector(TuranAPI::MemoryManagement::IAllocator* Allocator_, size_t expanding_size, size_t size) : Allocator(Allocator_), expand_size(expanding_size) {
			if (size == 0) {
				return;
			}
			pointer_toBuffer = (T*)Allocator->Allocate_MemoryBlock(size * sizeof(T));
			if (pointer_toBuffer) {
				buffer_size = size;
				buffer_length = 0;
			}
			else {
				std::cout << "A vector of " << size << "elements isn't created!\n";
			}
		}
		template<class T>
		Vector<T>::Vector(TuranAPI::MemoryManagement::IAllocator* Allocator_, size_t expanding_size, std::initializer_list<T>& list)
			: Allocator(Allocator_), expand_size(expanding_size), buffer_size(list.size()) {
			for (auto element : list) {
				this->push_back(element);
			}
		}
		template<class T>
		Vector<T>::~Vector() {
			clear();
			buffer_size = 0;
			Allocator->Delete_MemoryBlock(pointer_toBuffer);
		}
		template<class T>
		void Vector<T>::push_back(const T& object) {
			if (expand_size == 0 && buffer_length == buffer_size) {
				std::cout << "Vector's size is const and full!\n";
				return;
			}
			else if (!pointer_toBuffer) {
				pointer_toBuffer = (T*)Allocator->Allocate_MemoryBlock((buffer_size) * sizeof(T));
				if (pointer_toBuffer) {
					memcpy(pointer_toBuffer, &object, sizeof(T));
					buffer_length++;
					return;
				}
				else {
					std::cout << "Vector was zero size and tried to grow but Allocation has failed! Nothing has changed!\n";
					return;
				}
			}
			else if (buffer_length == buffer_size) {
				void* PTR = Allocator->Allocate_MemoryBlock((buffer_size + expand_size) * sizeof(T));
				if (PTR) {
					memcpy(PTR, pointer_toBuffer, buffer_size * sizeof(T));
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
					pointer_toBuffer = (T*)PTR;
					buffer_size += expand_size;

					memcpy(pointer_toBuffer + buffer_length, &object, sizeof(T));
					buffer_length++;
				}
				else {
					std::cout << "Vector tried to grow but Allocation has failed! Nothing has changed!\n";
					return;
				}
			}
			else {
				memcpy(pointer_toBuffer + buffer_length, &object, sizeof(T));
				buffer_length++;
			}
		}
		template<class T>
		void Vector<T>::erase(size_t index) {
			if (index > buffer_length) {
				std::cout << "There is no initialized element of index: " << index << std::endl;
				return;
			}
			if (index == buffer_length - 1) {
				--buffer_length;
				pointer_toBuffer[index].~T();
				memset(pointer_toBuffer + index, 0, sizeof(T));
				return;
			}

			pointer_toBuffer[index].~T();
			for (unsigned int i = 0; i < (buffer_length - 1) - index; i++) {
				memcpy(pointer_toBuffer + (index + i), pointer_toBuffer + (index + i + 1), sizeof(T));
			}
			memset(pointer_toBuffer + (buffer_length - 1), 0, sizeof(T));
			--buffer_length;
		}
		template<class T>
		void Vector<T>::clear() {
			for (size_t i = 0; i < buffer_length; i++) {
				pointer_toBuffer[i].~T();
			}
			memset(pointer_toBuffer, 0, buffer_length * sizeof(T));
			buffer_length = 0;
		}
		template<class T>
		T& Vector<T>::operator[](size_t index) const {
			if (index < buffer_length) {
				return pointer_toBuffer[index];
			}
			else {
				std::cout << "There is no element with index " << index << " in the Vector!\n";
			}
		}
		template<class T>
		Vector<T>& Vector<T>::operator= (std::initializer_list<T>& list) {
			buffer_size = list.size();
			for (auto element : list) {
				this->push_back(element);
			}
			return *this;
		}
		template<class T>
		Vector<T>& Vector<T>::operator= (const Vector<T>& vector) {
			buffer_size = vector.buffer_size;
			buffer_length = vector.buffer_length;
			expand_size = vector.expand_size;
			pointer_toBuffer = (T*)Allocator->Allocate_MemoryBlock(buffer_size * sizeof(T));
			memcpy(pointer_toBuffer, vector.pointer_toBuffer, buffer_size * sizeof(T));
			return *this;
		}
		template<class T>
		size_t Vector<T>::size() const {
			return buffer_length;
		}
		template<class T>
		size_t Vector<T>::max_size() const {
			return buffer_size;
		}
		template<class T>
		void Vector<T>::resize(size_t resize) {
			if (resize > buffer_size) {
				T* PTR = (T*)Allocator->Allocate_MemoryBlock((resize) * sizeof(T));
				if (PTR) {
					memcpy(PTR, pointer_toBuffer, buffer_length * sizeof(T));
					//Construct new elements!
					T default_element = {};
					for (unsigned int i = 0; i < resize - buffer_length; i++) {
						memcpy(PTR + buffer_length + i, &default_element, sizeof(T));
					}
					Allocator->Delete_MemoryBlock(pointer_toBuffer);
					pointer_toBuffer = PTR;
					buffer_size = resize;
					buffer_length = resize;
				}
				else {
					std::cout << "Resize operation has failed because Allocate_MemoryBlock has failed!\n";
				}
			}
			else if (resize < buffer_size && resize > buffer_length) {
				//Construct new elements!
				T default_element = {};
				for (unsigned int i = 0; i < resize - buffer_length; i++) {
					memcpy(pointer_toBuffer + buffer_length + i, &default_element, sizeof(T));
				}
				buffer_length = resize;
			}
			else if (buffer_length == resize || buffer_size == resize) {
				//Construct new elements!
				T default_element = {};
				for (unsigned int i = 0; i < buffer_length; i++) {
					memcpy(pointer_toBuffer + i, &default_element, sizeof(T));
				}

				//If buffer_size == resize
				buffer_length = resize;
			}
			else if (resize < buffer_length) {
				memset(pointer_toBuffer + resize, 0, (buffer_length - resize) * sizeof(T));
				buffer_length = resize;
			}
		}
		template<class T>
		void Vector<T>::grow(size_t resize) {
			if (resize <= buffer_length) {
				std::cout << "Vector can't grow to a lower or equal size!\n";
				return;
			}
			T* new_block = (T*)Allocator->Allocate_MemoryBlock(resize * sizeof(T));
			if (pointer_toBuffer) {
				memset(new_block, 0, resize * sizeof(T));
				memcpy(new_block, pointer_toBuffer, buffer_length * sizeof(T));
				Allocator->Delete_MemoryBlock(pointer_toBuffer);
				pointer_toBuffer = new_block;
				buffer_size = resize;
				return;
			}
			memset(new_block, 0, resize * sizeof(T));
			buffer_size = resize;
			buffer_length = 0;
			return;
		}
		template<class T>
		T* Vector<T>::data() {
			return pointer_toBuffer;
		}
		template<class T>
		T& Vector<T>::Get(size_t index) const {
			return pointer_toBuffer[index];
		}
		template<class T>
		bool Vector<T>::is_size_const() const {
			if (expand_size) {
				return false;
			}
			return true;
		}

	}
}