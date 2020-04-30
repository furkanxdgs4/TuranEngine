#include "GPU_ContentManager.h"

namespace GFX_API {
	VertexAttribute& VertexAttribute::operator=(const VertexAttribute& attribute) {
		AttributeName = attribute.AttributeName;
		DATATYPE = attribute.DATATYPE;
		Index = attribute.Index;
		Stride = attribute.Stride;
		Start_Offset = attribute.Start_Offset;
		return *this;
	}

	VertexAttributeLayout::VertexAttributeLayout(unsigned char AttributeNumber) : Attributes(LASTUSEDALLOCATOR, 0, AttributeNumber) {

	}
	VertexAttributeLayout& VertexAttributeLayout::operator=(const VertexAttributeLayout& layout) {
		Attributes = layout.Attributes;
		size_pervertex = layout.size_pervertex;
		return *this;
	}
	void VertexAttributeLayout::Calculate_SizeperVertex() {
		size_pervertex = 0;
		for (unsigned int i = 0; i < Attributes.size(); i++) {
			VertexAttribute* Attribute = Attributes[i];
			size_pervertex += Get_UNIFORMTYPEs_SIZEinbytes(Attribute->DATATYPE);
		}
	}

	bool VertexAttributeLayout::VerifyAttributeLayout() const {
		int FirstContinuousAttributeIndex = -1;
		for (unsigned int attribute_index = 0; attribute_index < Attributes.size(); attribute_index++) {
			VertexAttribute* current_attribute = Attributes[attribute_index];


			//Wrong Index
			if (current_attribute->Index != attribute_index) {
				String Error_Message = "Wrong Index in Attribute: ";
				Error_Message.append(current_attribute->AttributeName);
				std::cout << "Attribute's index: " << current_attribute->Index << " but should be: " << attribute_index << std::endl;
				TuranAPI::LOG_ERROR(Error_Message);
				return false;
			}

			//Find the first Continuous Attribute and check its Start_Offset
			if (FirstContinuousAttributeIndex == -1 && current_attribute->Stride == 0 && attribute_index > 0) {
				//First Continuous Attribute has to have a Start_Offset, because previous data is Interleaved!
				if (current_attribute->Start_Offset == 0) {
					TuranAPI::LOG_CRASHING("Please read the specification about vertex Attribute Layout! It's not allowed to have a interleaved attribute after an continuous attribute!");
					return false;
				}
				FirstContinuousAttributeIndex = attribute_index;
			}

			//Layout starts with Continuous Attribute, there shouldn't be any Interleaved Attribute!
			else if (FirstContinuousAttributeIndex == -1 && current_attribute->Stride == 0) {
				FirstContinuousAttributeIndex = attribute_index;
			}

			if (attribute_index > 0) {
				VertexAttribute* previous_attribute = Attributes[attribute_index - 1];

				//Start Offset = 0 shouldn't be used if previous attribute isn't continous!
				if (current_attribute->Start_Offset == 0 && previous_attribute->Stride != 0) {
					TuranAPI::LOG_CRASHING("Please read the specification about Vertex Attribute Layout! Start_Offset = 0 shouldn't be used if previous attribute isn't continuous!");
					return false;
				}

				//It's not allowed to have a interleaved attribute after an continuous attribute!
				else if (current_attribute->Stride != 0 && previous_attribute->Stride == 0) {
					TuranAPI::LOG_CRASHING("Please read the specification about vertex Attribute Layout! It's not allowed to have a interleaved attribute after an continuous attribute!");
					return false;
				}
			}

			return true;
		}
	}

	bool VertexAttributeLayout::Does_BufferFits_Layout(const void* vertex_data, size_t datasize_inbytes, size_t vertex_count) const {
		if (vertex_data == nullptr) {
			TuranAPI::LOG_CRASHING("Vertex Data is nullptr!");
			return false;
		}

		if (vertex_count * size_pervertex != datasize_inbytes) {
			TuranAPI::LOG_CRASHING("Vertex Buffer's data size doesn't match with attribute layout's size!");
			return false;
		}

		for (unsigned int i = 0; i < Attributes.size(); i++) {
			VertexAttribute* attribute = Attributes[i];
			if (attribute->Start_Offset > datasize_inbytes) {
				TuranAPI::LOG_CRASHING("Attribute's start offset causes buffer overflow!");
				return false;
			}
		}

		return true;
	}

	char VertexAttributeLayout::Find_AttributeIndex_byName(const char* Attribute_Name) const {
		for (unsigned char i = 0; i < Attributes.size(); i++) {
			if (Attribute_Name == Attributes[i]->AttributeName) {
				return i;
			}
		}
		
		//Failed to find!
		TuranAPI::LOG_CRASHING("Failed to find Vertex Attribute!");
		return -1;
	}
	void* VertexAttributeLayout::Gather_AttributeData(const void* vertex_data, size_t datasize_inbytes, size_t vertex_count, unsigned char Attribute_Index, size_t& data_size) const {
		return nullptr;
	}





	GPU_ContentManager::GPU_ContentManager() : MESHID_BITSET(1000, LASTUSEDALLOCATOR), RTID_BITSET(20, LASTUSEDALLOCATOR), FBID_BITSET(30, LASTUSEDALLOCATOR){

	}

	GPU_ContentManager::~GPU_ContentManager() {
		std::cout << "GFX's GPU Content Manager's destructor is called!\n";
	}

	//ID OPERATIONs

	unsigned int GPU_ContentManager::Create_MeshBufferID() {
		unsigned int ID = MESHID_BITSET.GetIndex_FirstFalse() + 1;
		MESHID_BITSET.SetBit_True(ID - 1);
		return ID;
	}
	void GPU_ContentManager::Delete_MeshBufferID(unsigned int ID) {
		MESHID_BITSET.SetBit_False(ID - 1);
	}
	unsigned int GPU_ContentManager::Create_RenderTargetID() {
		unsigned int ID = RTID_BITSET.GetIndex_FirstFalse() + 1;
		RTID_BITSET.SetBit_True(ID - 1);
		return ID;
	}
	void GPU_ContentManager::Delete_RenderTargetID(unsigned int ID) {
		RTID_BITSET.SetBit_False(ID - 1);
	}
	unsigned int GPU_ContentManager::Create_FrameBufferID() {
		unsigned int ID = FBID_BITSET.GetIndex_FirstFalse() + 1;
		FBID_BITSET.SetBit_True(ID - 1);
		return ID;
	}
	void GPU_ContentManager::Delete_FrameBufferID(unsigned int ID) {
		FBID_BITSET.SetBit_False(ID - 1);
	}
}