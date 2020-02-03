#include "GFX_Resource.h"

namespace GFX{
	unsigned int RenderTarget::Get_ID() const {
		return ID;
	}



	Framebuffer::Framebuffer(unsigned int width, unsigned int height) : WIDTH(width), HEIGHT(height) {

	}



	vector<GPU_MESH*> GPU_MESH::ALL_GPU_MESHes = vector<GPU_MESH*>{};

	GPU_MESH::GPU_MESH() {
		ALL_GPU_MESHes.push_back(this);
	}
}