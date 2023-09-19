#include <COSMAC.h>
#include "imgui/imgui.h"

class ExampleLayer : public COSMAC::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

		std::string vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;		

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new COSMAC::Shader(vertex_src, fragment_src));

		m_VertexArray.reset(COSMAC::VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<COSMAC::VertexBuffer> VertexBuffer;
		VertexBuffer.reset(COSMAC::VertexBuffer::Create(vertices, sizeof(vertices)));
		COSMAC::BufferLayout layout = {
			{ COSMAC::ShaderDataType::Float3, "a_Position" },
			{ COSMAC::ShaderDataType::Float4, "a_Color" }
		};
		VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(VertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };
		std::shared_ptr<COSMAC::IndexBuffer> IndexBuffer;
		IndexBuffer.reset(COSMAC::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(IndexBuffer);

	}

	void OnUpdate() override
	{
        COSMAC::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        COSMAC::RenderCommand::Clear();

        COSMAC::Renderer::BeginScene();

        m_Shader->Bind();
        COSMAC::Renderer::Submit(m_VertexArray);

        COSMAC::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
        static bool p_open = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
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
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false))
                    p_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("COSMAC - Viewport");
        ImGui::Text("This is nothing but a humble test...");
        ImGui::End();

        ImGui::End();
	}

	void OnEvent(COSMAC::Event &event) override
	{

	}

private:
    std::shared_ptr<COSMAC::Shader> m_Shader;
    std::shared_ptr<COSMAC::VertexArray> m_VertexArray;
};

class Sandbox : public COSMAC::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

COSMAC::Application *COSMAC::CreateApplication()
{
	return new Sandbox();
}