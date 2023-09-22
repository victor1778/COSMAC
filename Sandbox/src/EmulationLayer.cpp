#include "EmulationLayer.h"
#include <imgui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace COSMAC;

EmulationLayer::EmulationLayer()
	: Layer("EmulationLayer"), m_Camera(-2.0f, 2.0f, -1.0f, 1.0f), m_CameraPosition(0.0f)
{
	auto& window = Application::Get().GetWindow();

	m_Chip8.LoadProgram();

	m_VertexArray.reset(COSMAC::VertexArray::Create());

	float square_vertices[] = {
		-1.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		 1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
		 1.0f,  0.5f, 0.0f, 1.0f, 0.0f,
		-1.0f,  0.5f, 0.0f, 0.0f, 0.0f
	};

	COSMAC::Ref<COSMAC::VertexBuffer> VertexBuffer;
	VertexBuffer.reset(COSMAC::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
	COSMAC::BufferLayout layout = {
		{ COSMAC::ShaderDataType::Float3, "a_Position" },
		{ COSMAC::ShaderDataType::Float2, "a_TexCoord" }
	};
	VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(VertexBuffer);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	COSMAC::Ref<COSMAC::IndexBuffer> IndexBuffer;
	IndexBuffer.reset(COSMAC::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(IndexBuffer);

	std::string texture_shader_vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string texture_shader_fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

	m_TextureShader.reset(COSMAC::Shader::Create(texture_shader_vertex_src, texture_shader_fragment_src));

	m_Texture = COSMAC::Texture::Create(m_Chip8.framebuffer.data());

	std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->Bind();
	std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
}

void EmulationLayer::OnAttach()
{
}

void EmulationLayer::OnDetach()
{
}

void EmulationLayer::OnUpdate(COSMAC::Timestep ts)
{
	int fps = 1.0f / ts.GetSeconds();
	COSMAC_TRACE("FPS: {0} | Delta time: {1}ms", fps, ts.GetMilliseconds());

	COSMAC::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);

	COSMAC::Renderer::BeginScene(m_Camera);

	m_Texture->Bind();
	COSMAC::Renderer::Submit(m_TextureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.8f)));

	COSMAC::Renderer::EndScene();

	m_Chip8.Tick();
	m_Texture = COSMAC::Texture::Create(m_Chip8.framebuffer.data());
}

void EmulationLayer::OnImGuiRender()
{
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Open", "Ctrl+O");
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL, false))
				p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void EmulationLayer::OnEvent(COSMAC::Event& e)
{
}