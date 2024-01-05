#include <memory>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Image.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Application.h"

class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
        : m_Shader("./assets/shaders/vertex.vs", "./assets/shaders/frag.fs"), m_Framebuffer(100, 80), m_Image("./assets/textures/Checkerboard.png")
	{
		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}
	
	virtual void ShowUI() override
	{

		ImGui::Begin("Image");

		ImGui::Image((void*)m_Image.GetTextureID(), 
			ImVec2(100, 100), 
			ImVec2{ 0, 1 }, 
			ImVec2{ 1, 0 });

		ImGui::End();



		ImGui::Begin("Viewport");
        // we access the ImGui window size
        float window_width = ImGui::GetContentRegionAvail().x;
        float window_height = ImGui::GetContentRegionAvail().y;


        // we rescale the framebuffer to the actual window size here and reset the glViewport 
        m_Framebuffer.Resize(window_width, window_height);
        glViewport(0, 0, window_width, window_height);


        ImGui::Image((void*)m_Framebuffer.GetTextureID(), ImVec2(window_width, window_height), ImVec2 { 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();


		m_Framebuffer.Bind();//Render here
		
		// make sure we clear the framebuffer's content
		glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		m_Shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		m_Framebuffer.Unbind();

		ImGui::ShowDemoWindow();
	}
private:
	unsigned int VBO, VAO;
    Framebuffer m_Framebuffer;
    Shader m_Shader;
	Image m_Image;
};


int main()
{
	Application* ParticleSystem = new Application("Particle System");
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	ParticleSystem->PushImGuiLayer(myimguilayer);

	ParticleSystem->Run();

	return 0;
}