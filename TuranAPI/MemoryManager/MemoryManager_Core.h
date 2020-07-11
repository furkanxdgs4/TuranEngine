#pragma once

namespace TuranAPI {
	namespace MemoryManagement {
#define MAX_MEMORYBLOCKNUMBER 60000
#define MAX_ALLOCATORNUMBER 20
		struct MemoryBlockInfo {
			void* address = nullptr;
			//You shouldn't change size because all allocations are made according to them!
			//Only initialize them (Which is not possible because I can't use new)
			size_t size = 0;
		};

		class IAllocator {
		protected:
			friend class TMemoryManager;
		public:
			MemoryBlockInfo MemoryPool;
			virtual void* Allocate_MemoryBlock(size_t size) = 0;
			virtual void Delete_MemoryBlock(void* Pointer_toDelete) = 0;
			virtual const MemoryBlockInfo& Get_MemoryBlockInfo(const void* address) const = 0;
			virtual unsigned int Get_MemoryBlockIndex(const void* address) const = 0;
			virtual size_t Get_AllocatedMemorySize() const = 0;
			virtual size_t Get_NotFreeMemorySize() const = 0;
		};
		class TAPIMemoryAllocator : public IAllocator {
		private:
			friend class TMemoryManager;
			MemoryBlockInfo Allocated_MemoryBlocks[MAX_MEMORYBLOCKNUMBER];
			~TAPIMemoryAllocator();
		public:
			TAPIMemoryAllocator();
			virtual void* Allocate_MemoryBlock(size_t size);
			virtual void Delete_MemoryBlock(void* Pointer_ToDelete);
			virtual const MemoryBlockInfo& Get_MemoryBlockInfo(const void* address) const override;
			virtual unsigned int Get_MemoryBlockIndex(const void* address) const override;
			virtual size_t Get_AllocatedMemorySize() const override;
			virtual size_t Get_NotFreeMemorySize() const override;
		};
		//All of the applications written using Memory Manager will get a memory block from Main_MemoryBlock
		//So, TuranAPI should know the maximum limit of used memory size. We will specify it in cpp file.
		class TMemoryManager {
			MemoryBlockInfo MainMemoryBlock;
		public:
			static TMemoryManager* SELF;
			IAllocator* All_Allocators[MAX_ALLOCATORNUMBER];
			IAllocator* LastUsedAllocator;
			TAPIMemoryAllocator TAPIAllocator;
			//You shouldn't initialize a MemoryManager!
			TMemoryManager(size_t Main_MemoryBlock);
			void Add_Allocator(IAllocator* allocator);
			void Delete_Allocator(IAllocator* allocator);
			~TMemoryManager();
		};
	}
}

void* operator new(size_t size);
void* operator new(size_t size, TuranAPI::MemoryManagement::IAllocator* Allocator);
void operator delete(void* pointerToDelete);

#define TAPIAllocatorCreate(CONSTRUCTION) new (TuranAPI::MemoryManagement::TAPIMemoryAllocator::SELF) CONSTRUCTION
#define LASTUSEDALLOCATOR TuranAPI::MemoryManagement::TMemoryManager::SELF->LastUsedAllocator
#define TMEMORYMANAGER TuranAPI::MemoryManagement::TMemoryManager::SELF