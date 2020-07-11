#include "GFX_Resource.h"

namespace GFX_API{
	Framebuffer::Framebuffer() : BOUND_RTs(LASTUSEDALLOCATOR, 1, 3) {
	}
}