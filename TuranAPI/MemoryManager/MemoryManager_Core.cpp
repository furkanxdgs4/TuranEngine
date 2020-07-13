#include "TuranAPI/MemoryManager/MemoryManager_Core.h"
#include <stdlib.h>
#include <iostream>

namespace TuranAPI {
	namespace MemoryManagement {
		TAPIMemoryAllocator::TAPIMemoryAllocator() {
			std::cout << "TAPIMemoryAllocator is created!\n";
			MemoryPool.address = nullptr;
			MemoryPool.size = 0;
		}
		TAPIMemoryAllocator::~TAPIMemoryAllocator() {
			std::cout << "TAPIMemoryAllocator is being deleted!\n";
		}
		void* TAPIMemoryAllocator::Allocate_MemoryBlock(size_t size) {
			char* finaladdress = (char*)MemoryPool.address;
			for (unsigned int blockindex = 0; blockindex < MAX_MEMORYBLOCKNUMBER; blockindex++) {
				MemoryBlockInfo& MemoryBlock = Allocated_MemoryBlocks[blockindex];
				if (size <= MemoryBlock.size && size >= MemoryBlock.size * 3 / 5) {
					if (!MemoryBlock.address) {
						MemoryBlock.address = finaladdress;
#ifdef MEMORY_DEBUGGING
						std::cout << "Allocated Memory Address: " << static_cast<void*>(finaladdress) << std::endl;
						std::cout << "Allocated Block Index: " << blockindex << std::endl;
						std::cout << "Intended Memory Size: " << size << std::endl;
						std::cout << "Allocated Memory Size: " << MemoryBlock.size << std::endl;
#endif
						//You shouldn't change Memory Block's size because all of the allocations before this, is made upon previous size!
						//You should move all the previous allocated memory to set the size (Which is not ideal!)
						//If I'd want to find memory leaks causing this, I could write code here to log the leaks!
						return MemoryBlock.address;
					}
				}
				else if (MemoryBlock.size == 0) {
#ifdef MEMORY_DEBUGGING
					std::cout << "Allocated Memory Address: " << static_cast<void*>(finaladdress) << std::endl;
					std::cout << "Allocated Block Index: " << blockindex << std::endl;
					std::cout << "Intended Memory Size: " << size << std::endl;
					std::cout << "Allocated Memory Size: " << MemoryBlock.size << std::endl;
#endif
					//This means this index in the Allocated_MemoryBlocks has never been used, so we can add the data here!
					MemoryBlock.address = finaladdress;
					MemoryBlock.size = size;
					return MemoryBlock.address;
				}
				finaladdress = finaladdress + MemoryBlock.size;
			}
			//If you arrive here, that means there is no empty memory block in all Allocated_MemoryBlocks array!
			std::cout << "There is no empty memory block in all Allocated_MemoryBlocks array, so nullptr is returned!\n";
			return nullptr;
		}
		void TAPIMemoryAllocator::Delete_MemoryBlock(void* Pointer_ToDelete) {
			if (Pointer_ToDelete == nullptr) {
				std::cout << "Pointer is already a nullptr!\n";
				return;
			}
			for (unsigned int i = 0; i < MAX_MEMORYBLOCKNUMBER; i++) {
				MemoryBlockInfo& MemoryBlock = Allocated_MemoryBlocks[i];
				MemoryBlockInfo& NextBlock = Allocated_MemoryBlocks[i + 1];

				//Memory Block is the last allocated memory block!
				if (NextBlock.size == 0 && MemoryBlock.address == Pointer_ToDelete) {
#ifdef MEMORY_DEBUGGING
					std::cout << "Deleted Last Memory Block, Index: " << i << std::endl;
					std::cout << "Deleted Memory Address: " << static_cast<void*>(Pointer_ToDelete) << std::endl;
#endif
					memset(MemoryBlock.address, 0, MemoryBlock.size);
					MemoryBlock.address = nullptr;
					MemoryBlock.size = 0;
					break;
				}
				else if (MemoryBlock.address == Pointer_ToDelete) {
#ifdef MEMORY_DEBUGGING
					std::cout << "Deleted Memory Block Index: " << i << std::endl;
					std::cout << "Deleted Memory Address: " << static_cast<void*>(Pointer_ToDelete) << std::endl;
#endif
					//Set memory bytes as 0
					memset(MemoryBlock.address, 0, MemoryBlock.size);
					MemoryBlock.address = nullptr;
					//We don't change size of an MemoryBlock ever, because all allocations are made according to them!
					break;
				}
			}
		}
		const MemoryBlockInfo& TAPIMemoryAllocator::Get_MemoryBlockInfo(const void* address) const {
			for (unsigned int i = 0; i < MAX_MEMORYBLOCKNUMBER; i++) {
				const MemoryBlockInfo& MemoryBlock = Allocated_MemoryBlocks[i];
				if (address == MemoryBlock.address) {
					return MemoryBlock;
				}
				if (MemoryBlock.address == nullptr && MemoryBlock.size == 0) {
					return MemoryBlockInfo();
				}
			}
		}
		unsigned int TAPIMemoryAllocator::Get_MemoryBlockIndex(const void* address) const {
			for (unsigned int i = 0; i < MAX_MEMORYBLOCKNUMBER; i++) {
				const MemoryBlockInfo& MemoryBlock = Allocated_MemoryBlocks[i];
				if (address == MemoryBlock.address) {
					return i;
				}
				if (MemoryBlock.address == nullptr && MemoryBlock.size == 0) {
					return 0;
				}
			}
		}
		size_t TAPIMemoryAllocator::Get_AllocatedMemorySize() const {
			size_t Allocated_MemorySize = 0;
			for (unsigned int i = 0; i < MAX_MEMORYBLOCKNUMBER; i++) {
				const MemoryBlockInfo& blockinfo = Allocated_MemoryBlocks[i];
				if (blockinfo.size == 0) {
					return Allocated_MemorySize;
				}
				Allocated_MemorySize += blockinfo.size;
			}
			return Allocated_MemorySize;
		}
		size_t TAPIMemoryAllocator::Get_NotFreeMemorySize() const {
			size_t NotFree_MemorySize = 0;
			for (unsigned int i = 0; i < MAX_MEMORYBLOCKNUMBER; i++) {
				const MemoryBlockInfo& blockinfo = Allocated_MemoryBlocks[i];
				if (!blockinfo.address) {
					continue;
				}
				if (blockinfo.size == 0) {
					return NotFree_MemorySize;
				}
				NotFree_MemorySize += blockinfo.size;
			}
			return NotFree_MemorySize;
		}

		TMemoryManager* TMemoryManager::SELF = nullptr;
		TMemoryManager::TMemoryManager(size_t Main_MemoryBlockSize) {
			std::cout << "TMemoryManager is created!\n";
			MainMemoryBlock.address = malloc(Main_MemoryBlockSize);
			//Zero initialize the memory!
			memset(MainMemoryBlock.address, 0, Main_MemoryBlockSize);
			MainMemoryBlock.size = Main_MemoryBlockSize;
			std::cout << "Main Memory Block's start pointer: " << MainMemoryBlock.address << std::endl;
			SELF = this;

			MemoryBlockInfo TuranAPI_MemoryPool;
			TuranAPI_MemoryPool.address = MainMemoryBlock.address;
			TuranAPI_MemoryPool.size = Main_MemoryBlockSize;
			TAPIAllocator.MemoryPool = TuranAPI_MemoryPool;
			LastUsedAllocator = &TAPIAllocator;
			for (unsigned int i = 0; i < MAX_ALLOCATORNUMBER; i++) {
				All_Allocators[i] = nullptr;
			}
			All_Allocators[0] = &TAPIAllocator;
			void* AllocatedMemoryAddres = LastUsedAllocator->Allocate_MemoryBlock(10);
		}
		TMemoryManager::~TMemoryManager() {
			std::cout << "\nTMemoryManager is being deleted!\n";
			if (SELF == nullptr) {
				return;
			}
			memset(MainMemoryBlock.address, 0, MainMemoryBlock.size);
			free(MainMemoryBlock.address);
			std::cout << "TMemoryManager is deleted!\n";
		}
		//If you want your Allocator to use the Main Memory block,
		//you should specify your allocator's Memory Pool's size before calling this function!
		void TMemoryManager::Add_Allocator(IAllocator* NEW_ALLOCATOR) {
			char* finaladress = (char*)MainMemoryBlock.address;
			for (unsigned int i = 0; i < MAX_ALLOCATORNUMBER; i++) {
				IAllocator* allocator = All_Allocators[i];
				if (allocator != nullptr) {
					finaladress = finaladress + allocator->MemoryPool.size;
				}
				else {
					allocator = NEW_ALLOCATOR;
					break;
				}
			}
			NEW_ALLOCATOR->MemoryPool.address = finaladress;
		}
		void TMemoryManager::Delete_Allocator(IAllocator* Detach_Allocator) {
			std::cout << "Deleting Allocator!\n";
		}
	}
}

void* operator new(size_t size) {
	if (size > 1000000) {
		std::cout << "A memory block of " << size << " bytes will be allocated!\n";
	}
	return LASTUSEDALLOCATOR->Allocate_MemoryBlock(size);
}
void* operator new(size_t size, TuranAPI::MemoryManagement::IAllocator* Allocator) {
	return Allocator->Allocate_MemoryBlock(size);
}
void operator delete(void* pointerToDelete) {
	LASTUSEDALLOCATOR->Delete_MemoryBlock(pointerToDelete);
}