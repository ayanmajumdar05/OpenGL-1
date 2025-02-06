#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	//caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set Uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, int value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	ShaderProgramSource parseShader(const std::string& filepath);
	//unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};