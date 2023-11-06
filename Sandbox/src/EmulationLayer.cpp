#include "EmulationLayer.h"
#include <imgui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "COSMAC/Utils/PlatformUtils.h"

#include <chrono>
#include <ctime>
#include <time.h>

int CPU_SPEED = 700;
static bool show_settings = false;
static bool show_keypad = true;

EmulationLayer::EmulationLayer()
	: Layer("EmulationLayer"), m_Camera(-2.0f, 2.0f, -1.0f, 1.0f), m_CameraPosition(0.0f)
{
	auto& window = COSMAC::Application::Get().GetWindow();

	m_VertexArray = COSMAC::VertexArray::Create();

	float square_vertices[] = {
		-1.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		 1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
		 1.0f,  0.5f, 0.0f, 1.0f, 0.0f,
		-1.0f,  0.5f, 0.0f, 0.0f, 0.0f
	};

	VertexBuffer = COSMAC::VertexBuffer::Create(square_vertices, sizeof(square_vertices));

	COSMAC::BufferLayout layout = {
		{ COSMAC::ShaderDataType::Float3, "a_Position" },
		{ COSMAC::ShaderDataType::Float2, "a_TexCoord" }
	};
	VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(VertexBuffer);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	IndexBuffer = COSMAC::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(IndexBuffer);

	m_TextureShader = COSMAC::Shader::Create("\\assets\\shaders\\Texture.glsl");

	std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->Bind();
	std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	m_Texture = COSMAC::Texture::Create(m_Chip8.framebuffer.data());
}

void EmulationLayer::OnAttach()
{
	COSMAC_PROFILE_FUNCTION();

	COSMAC::FramebufferSpecification fbSpec;
	fbSpec.Width = 640;
	fbSpec.Height = 320;
	m_Framebuffer = COSMAC::Framebuffer::Create(fbSpec);
}

void EmulationLayer::OnDetach()
{
	COSMAC_PROFILE_FUNCTION();
}

void EmulationLayer::OnUpdate(COSMAC::Timestep ts)
{
	COSMAC_PROFILE_FUNCTION(); 

	if (COSMAC::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	if (m_ViewportHovered)
			UpdateInput();

	if (!m_Chip8.PAUSE)
		for (int i = 0; i < CPU_SPEED / 60; i++)
		{	
			m_Chip8.Tick();
		}

	std::this_thread::sleep_for(std::chrono::duration<double>(0.01));

	Render();

	m_Chip8.UpdateTimers();
}

void EmulationLayer::OnImGuiRender()
{
	COSMAC_PROFILE_FUNCTION();

	static bool p_open = true;
	static bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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
	ImGui::Begin("DockSpace", &p_open, window_flags);
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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
				OpenRom();
			ImGui::Separator();
			if (ImGui::MenuItem("Close"))
				COSMAC::Application::Get().Close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game"))
		{
			if (ImGui::MenuItem("Pause", "Esc"))
				if (m_Chip8.PAUSE)
					m_Chip8.PAUSE = false;
				else
					m_Chip8.PAUSE = true;
			ImGui::Separator();
			if (ImGui::MenuItem("Settings"))
				show_settings = true;
			ImGui::Separator();
			if (ImGui::MenuItem("Reset"))
				m_Chip8.Restart();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	COSMAC::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

	if (show_settings)
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Settings", &show_settings, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking);
		ImGui::SliderInt("CPU Speed (Hz)", &CPU_SPEED, 100, 1000);
		ImGui::Checkbox("Show Keypad", &show_keypad);
		ImGui::End();
	}

	if (show_keypad)
	{
		static int location = 2;
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowBgAlpha(0.45f);
		ImGui::Begin("Keypad", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		ImGui::TextColored(m_Chip8.keypad[0x1] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "1");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x2] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "2");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x3] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "3");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0xC] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "C");
		ImGui::Separator();

		ImGui::TextColored(m_Chip8.keypad[0x4] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "4");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x5] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "5");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x6] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "6");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0xD] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "D");
		ImGui::Separator();

		ImGui::TextColored(m_Chip8.keypad[0x7] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "7");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x8] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "8");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x9] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "9");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0xE] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "E");
		ImGui::Separator();

		ImGui::TextColored(m_Chip8.keypad[0xA] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "A");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0x0] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "0");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0xB] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "B");
		ImGui::SameLine();
		ImGui::TextColored(m_Chip8.keypad[0xF] ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "F");
		ImGui::End();
	}

	ImGui::End();
}

void EmulationLayer::OnEvent(COSMAC::Event& e)
{
	COSMAC::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<COSMAC::KeyPressedEvent>(COSMAC_BIND_EVENT_FN(EmulationLayer::OnKeyPressedEvent));
}

bool EmulationLayer::OnKeyPressedEvent(COSMAC::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == COSMAC_KEY_ESCAPE)
	{
		switch (m_Chip8.PAUSE)
		{
		case false: m_Chip8.PAUSE = true; break;
		case true: m_Chip8.PAUSE = false; break;
		}
	}

	return false;
}

void EmulationLayer::UpdateInput()
{
	COSMAC_PROFILE_FUNCTION();
	
	m_Chip8.keypad[0x1] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_1) ? 1 : 0;
	m_Chip8.keypad[0x2] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_2) ? 1 : 0;
	m_Chip8.keypad[0x3] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_3) ? 1 : 0;
	m_Chip8.keypad[0xC] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_4) ? 1 : 0;

	m_Chip8.keypad[0x4] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_Q) ? 1 : 0;
	m_Chip8.keypad[0x5] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_W) ? 1 : 0;
	m_Chip8.keypad[0x6] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_E) ? 1 : 0;
	m_Chip8.keypad[0xD] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_R) ? 1 : 0;

	m_Chip8.keypad[0x7] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_A) ? 1 : 0;
	m_Chip8.keypad[0x8] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_S) ? 1 : 0;
	m_Chip8.keypad[0x9] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_D) ? 1 : 0;
	m_Chip8.keypad[0xE] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_F) ? 1 : 0;

	m_Chip8.keypad[0xA] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_Z) ? 1 : 0;
	m_Chip8.keypad[0x0] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_X) ? 1 : 0;
	m_Chip8.keypad[0xB] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_C) ? 1 : 0;
	m_Chip8.keypad[0xF] = COSMAC::Input::IsKeyPressed(COSMAC_KEY_V) ? 1 : 0;

}

void EmulationLayer::Render()
{
	COSMAC_PROFILE_FUNCTION();

	m_Framebuffer->Bind();

	COSMAC::RenderCommand::Clear();

	COSMAC::Renderer::BeginScene(m_Camera);

	m_Texture = COSMAC::Texture::Create(m_Chip8.framebuffer.data());

	m_Texture->Bind();

	COSMAC::Renderer::Submit(m_TextureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.8f)));

	COSMAC::Renderer::EndScene();

	m_Framebuffer->Unbind();
}

void EmulationLayer::OpenRom()
{
	std::string filepath = COSMAC::FileDialogs::OpenFile("ROM (*.ch8)\0*.ch8\0");
	
	if (!filepath.empty())
	{
		m_Chip8.Restart();
		m_Chip8.LoadProgram(filepath);
	}
}