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

private:
	COSMAC::Ref<COSMAC::Shader> m_Shader, m_TextureShader;
	COSMAC::Ref<COSMAC::VertexArray> m_VertexArray;
	COSMAC::Ref<COSMAC::Texture> m_Texture;

	COSMAC::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	Chip8 m_Chip8;
};
