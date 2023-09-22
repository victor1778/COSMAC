#pragma once

#include <string>

#include "COSMAC/Core/Core.h"

namespace COSMAC
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		virtual void Bind(uint32_t slot = 0) const = 0;

		static Ref<Texture> Create(uint32_t* data);
	};

}
