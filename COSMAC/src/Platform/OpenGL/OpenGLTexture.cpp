#include "cpch.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"

namespace COSMAC
{
	OpenGLTexture::OpenGLTexture()
	{
		const int w = 64, h = 32;
		uint32_t data[w * h];

		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				if ((x + y) % 2 == 0)
					data[y * w + x] = 0xFFFFFFFF;
				else
					data[y * w + x] = 0x00000000;
			}
		}

		m_Width = w;
		m_Height = h;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}