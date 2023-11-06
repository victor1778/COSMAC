#pragma once

#include "COSMAC.h"

#include "Chip8.h"

class EmulationLayer : public COSMAC::Layer
{
public:
	EmulationLayer();
	virtual ~EmulationLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(COSMAC::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(COSMAC::Event& e) override;
	
	bool OnKeyPressedEvent(COSMAC::KeyPressedEvent& e);

	void OpenRom();
	void UpdateInput();
	void Render();

private:
	COSMAC::Ref<COSMAC::VertexArray> m_VertexArray;
	COSMAC::Ref<COSMAC::VertexBuffer> VertexBuffer;
	COSMAC::Ref<COSMAC::IndexBuffer> IndexBuffer;
	COSMAC::Ref<COSMAC::Shader> m_TextureShader;
	COSMAC::Ref<COSMAC::Texture> m_Texture;
	COSMAC::Ref<COSMAC::Framebuffer> m_Framebuffer;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	COSMAC::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	Chip8 m_Chip8;
};
