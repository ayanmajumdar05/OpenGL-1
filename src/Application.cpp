#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); // OpenGl Major version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGl Minor version 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Core Profile


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OPENGL-1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    //Set Vsync to 1
    glfwSwapInterval(1);

    /* Initialize GLEW library and check for error */
    if (glewInit() != GLEW_OK) {
        std::cout << "Error in glewInit()";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        /*
        float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, //0
			 0.5f, -0.5f, 1.0f, 0.0f, //1
			 0.5f, 0.5f, 1.0f, 1.0f,   //2
			- 0.5f, 0.5f, 0.0f, 1.0f   //3
		};
        */
		/*
        float positions[] = {
			 100.0f, 100.0f, 0.0f, 0.0f, //0
			 200.0f, 100.0f, 1.0f, 0.0f, //1
			 200.0f, 200.0f, 1.0f, 1.0f,   //2
			 100.0f, 200.0f, 0.0f, 1.0f   //3
		};
        */

		float positions[] = {
			 -50.0f, -50.0f, 0.0f, 0.0f, //0
			 50.0f, -50.0f, 1.0f, 0.0f,  //1
			 50.0f, 50.0f, 1.0f, 1.0f,   //2
			 -50.0f, 50.0f, 0.0f, 1.0f   //3
		};

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};
		GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
		layout.Push<float>(2);
        layout.Push<float>(2);

		va.AddBuffer(vb,layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
		//glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); placed inside loop

        //glm::mat4 mvp = proj * view * model;

        //glm::mat4 proj = glm::ortho(0.0F, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //Creating shader
		Shader shader("res/shaders/basic.shader");
        shader.Bind();
        Texture texture("res/textures/sillycat.png");   //preparing texture
		texture.Bind();                                 //binding texture
		shader.setUniform1i("u_Texture", 0);			//setting texture slot
		//shader.setUniform1i(uniform, slot);

        va.UnBind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        
        ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontDefault();
		io.Fonts->Build();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 330");
		
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.setUniformMat4f("u_MVP", mvp);
			    
                renderer.Draw(va, ib, shader);

            }

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
                shader.Bind();
				shader.setUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);

			}
            
            
            
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				
			}
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

        //glDeleteProgram(shader);
    }
    
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}