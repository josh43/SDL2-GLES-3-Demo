#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include "glm/glm.hpp"
#include <vector>
class Shader

{
public:
    Shader(){
        
    }
	Shader(const std::string &fileName);
    virtual void findUniformsAndAttribs(){
        
    }
	~Shader();
	void bind();
	//void update(Transform &transform, const Camera & camera);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	void updateShadowMatrix(glm::mat4 &shadowM);
	GLuint CreateShader(const std::string &fullText, GLenum typeShader);
	inline void setLightDirection(glm::vec3 lightDirection) {
		GLuint lightDir = glGetUniformLocation(m_program, "lightDirection");
		glUniform3fv(lightDir, 1, &lightDirection[0]);
	}
		inline void setLightColor(glm::vec3 lightColor) {
			GLuint lightColorPosition = glGetUniformLocation(m_program, "lightColor");
			glUniform3fv(lightColorPosition, 1, &lightColor[0]);

		}
	inline GLuint getProgramID() {
		return m_program;
	}

	enum {
		MODEL_U,
		VIEWPROJECTION_U,
		MODEL_VIEW,
		SHADOW_MATRIX,
		NUM_UNIFORMS
	};
	
	const static int NUM_SHADERS = 2;
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
  
    
};

