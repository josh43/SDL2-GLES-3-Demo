#include "Shader.h"

static std::string LoadShader(const std::string &fileName);


Shader::Shader(const std::string & fileName)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);

	}

	glBindAttribLocation(m_program, 0, "position");
	//glBindAttribLocation(m_program, 1, "normal");
	//glBindAttribLocation(m_program, 2, "textureCoord");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");


	
	/*
	uniform mat4 model;
	uniform mat4 viewProjection;

	*/
	

}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
void Shader::updateShadowMatrix(glm::mat4 &shadowM)
{
	glUniform4fv(m_uniforms[SHADOW_MATRIX], 1, &shadowM[0][0]);
}
 GLuint Shader::CreateShader(const std::string &fullText, GLenum typeShader) {
	GLuint shader = glCreateShader(typeShader);

	const GLchar * shaderSourceString[1];
	GLint shaderSourceStringLength[1];

	shaderSourceString[0] = fullText.c_str();
	shaderSourceStringLength[0] = (GLint)fullText.length();
	glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	
	return shader;


}

static std::string LoadShader(const std::string &fileName) {
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + '\n');
		}
	}
	else {
		std::cerr << "ERror reading the file ";
	}

	return output;
}
void Shader::bind()
{
	glUseProgram(m_program);
}



Shader::~Shader()
{
	for (int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}


