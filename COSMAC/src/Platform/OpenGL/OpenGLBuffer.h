#pragma once

#include "COSMAC/Renderer/Buffer.h"

namespace COSMAC
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual uint32_t GetCount() const { return m_Count; }

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}