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

/*
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() 
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function,const char* file,int line)
{
    while( GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error <<")"<< function <<
            " " << file << ":" << file << std::endl;
        return false;
    }
    return true;
}

*/

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
        float positions[] = {
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.5f, 0.5f,  //2
            -0.5f, 0.5f   //3
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
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);




        /*
        VertexBuffer vb(positions, 4 * 2 * sizeof(float)); //Already Bound inside VertexBuffer.cpp
        //send VERTEX BUFFER DATA TO GPU
        unsigned int buffer;
        GLCall(glGenBuffers(1, &buffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

        
        //Specify format of Vertex Data
        GLCall(glEnableVertexAttribArray(0));
        //glVertexAttribPointer(index,size,type     ,normalized,stride            ,pointer)
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));//layout of data BINDS VERTEX BUFFER TO VAO
        */
        IndexBuffer ib(indices, 6);
        //send INDEX BUFFER DATA TO GPU
        unsigned int ibo; // IndexBufferObject
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

        //Creating shader
		shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        float r = 0.0f;
        float b = 0.0f;
        float g = 0.0f;
        float r_increment = 0.00f;
        float g_increment = 0.00f;
        float b_increment = 0.00f;

        va.UnBind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.setUniform4f("u_Color", r, g, b, 1.0f);
            
            /*
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

            glEnableVertexAttribArray(0);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
            */
            //GLCall(glBindVertexArray(vao));
            va.Bind();
            //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
            ib.Bind();

            //glDrawArrays(GL_TRIANGLES,0, 6);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //Draw Call
            /*
            if (r > 1.0f ) {
                r_increment = -0.05f;
            }
            else if (r < 0.0f ) {
                r_increment = 0.05f;
            }
            */
            // <RGB Square>
            r += r_increment;
            g += g_increment;
            b += b_increment;
            if (r <= 0.0f && b <= 0.0f && g <= 0.0f) {
                r_increment = 0.05f;
                g_increment = 0.00f;
                b_increment = 0.00f;
            }
            if (r >= 1.0f && g <= 0.0f && b <= 0.0f) {
                r_increment = 0.00f;
                g_increment = 0.05f;
            }
            if (g >= 1.0f && r >= 1.0f) {
                g_increment = 0.00f;
                r_increment = -0.05f;
            }
            if (r <= 0.0f && g >= 1.0f) {
                b_increment = 0.05f;
                r_increment = 0.00f;
            }
            if (b >= 1.0f && g >= 1.0f) {
                b_increment = 0.00f;
                g_increment = -0.05f;
            }
            if (b >= 1.0f && g <= 0.0f) {
                g_increment = 0.00f;
                r_increment = 0.05f;
            }
            if (b >= 1.0f && r >= 1.0f) {
                r_increment = 0.00f;
                b_increment = -0.05f;
            }
            // </RGBsquare>

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