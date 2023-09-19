#include <COSMAC.h>
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public COSMAC::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.5f, 0.5f, 0.0f), m_SquarePos(0.0f)
	{

		std::string vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;	

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader.reset(COSMAC::Shader::Create(vertex_src, fragment_src));

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

		m_VertexArray.reset(COSMAC::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};


		float square_vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
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

		m_Texture = COSMAC::Texture::Create();

		std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(COSMAC::Timestep ts) override
	{
		int fps = 1.0f / ts.GetSeconds();
		COSMAC_TRACE("FPS: {0} | Delta time: {1}ms", fps, ts.GetMilliseconds());

		if (COSMAC::Input::IsKeyPressed(COSMAC_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (COSMAC::Input::IsKeyPressed(COSMAC_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (COSMAC::Input::IsKeyPressed(COSMAC_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (COSMAC::Input::IsKeyPressed(COSMAC_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        COSMAC::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        COSMAC::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

        COSMAC::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<COSMAC::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				COSMAC::Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}

		m_Texture->Bind();
		COSMAC::Renderer::Submit(m_TextureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
        ImGui::End();

        ImGui::End();
	}

	void OnEvent(COSMAC::Event &event) override
	{
	}

private:
    COSMAC::Ref<COSMAC::Shader> m_Shader, m_TextureShader;
    COSMAC::Ref<COSMAC::VertexArray> m_VertexArray;
	COSMAC::Ref<COSMAC::Texture> m_Texture;

	glm::vec3 m_SquarePos;

	COSMAC::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	glm::vec3 m_Color = { 0.8f, 0.2f, 0.3f };
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