#pragma once

#include "COSMAC/Renderer/Texture.h"

namespace COSMAC
{

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(uint32_t* data);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_Width, m_Height, m_RendererID;
	};

}
