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
    window = glfwCreateWindow(540, 540, "OPENGL-1", NULL, NULL);
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
             0.5f, 0.5f, 1.0f, 1.0f   //2
            -0.5f, 0.5f, 0.0f, 1.0f   //3
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };
        */
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, //0
			 0.5f, -0.5f, 1.0f, 0.0f, //1
			 0.5f, 0.5f, 1.0f, 1.0f,   //2
			- 0.5f, 0.5f, 0.0f, 1.0f   //3
		};

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

        //Vertex Array Object
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao)); //Bind VAO 

        VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
		layout.Push<float>(2);
        layout.Push<float>(2);

		va.AddBuffer(vb,layout);
        IndexBuffer ib(indices, 6);
        //send INDEX BUFFER DATA TO GPU
        unsigned int ibo; // IndexBufferObject
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

        //Creating shader
		Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        float r = 0.0f;
        float b = 0.0f;
        float g = 0.0f;
        

        Texture texture("res/textures/sillycat.png");
		texture.Bind();
        shader.setUniform1i("u_Texture", 0);
		//shader.setUniform1i(uniform, slot);

        va.UnBind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //shader.setUniform4f("u_Color", r, g, b, 1.0f);
            
			renderer.Draw(va, ib, shader);
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

        //glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}