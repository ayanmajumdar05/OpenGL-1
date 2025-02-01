#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath) 
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1,VERTEX = 0,FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream,line)) {
        if (line.find("#shader") != std::string::npos) 
        {
            if (line.find("vertex") != std::string::npos) 
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) 
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


static unsigned int compileShader(unsigned int type, const std::string& source) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src,nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to Compile"<< (type == GL_VERTEX_SHADER? "Vertex": "fragment") << " Shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    
    }

    return id;
}
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);


    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;

}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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
    //send VERTEX BUFFER DATA TO GPU
    unsigned int buffer;   
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6* 2* sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
  //glVertexAttribPointer(index,size,type     ,normalized,stride            ,pointer)
    glVertexAttribPointer(0    ,2   , GL_FLOAT, GL_FALSE , sizeof(float) * 2, 0);


    //send INDEX BUFFER DATA TO GPU
    unsigned int ibo; // IndexBufferObject
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    //Creating shader
    
    ShaderProgramSource source = parseShader("res/shaders/basic.shader");
    /*
    std::cout << "VERTEX" <<std:: endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    */


    //BIND SHADER
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    //Retrieving location of Uniform in shader Program

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    //GLCall(glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f));

    float r = 0.0f;
    float b = 0.0f;
    float g = 0.0f;
    float r_increment = 0.00f;
    float g_increment = 0.00f;
    float b_increment = 0.00f;

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(location, r, g, b, 1.0f));
        //glDrawArrays(GL_TRIANGLES,0, 6);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
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
            b_increment = 0.00f;}
        if (r >= 1.0f && g <= 0.0f && b<=0.0f) {
            r_increment = 0.00f;
            g_increment = 0.05f;}
        if (g >= 1.0f && r >=1.0f) {
            g_increment = 0.00f;
            r_increment = -0.05f;}
        if (r <= 0.0f && g >= 1.0f) {
            b_increment = 0.05f;
            r_increment = 0.00f;}
        if (b >= 1.0f && g >= 1.0f) {
            b_increment = 0.00f;
            g_increment = -0.05f;}
        if (b >= 1.0f && g <= 0.0f) {
            g_increment = 0.00f;
            r_increment = 0.05f;}
        if (b >= 1.0f && r >= 1.0f) {
            r_increment = 0.00f;
            b_increment = -0.05f;}
        // </RGBsquare>
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    //glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}