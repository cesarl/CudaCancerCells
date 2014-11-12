#pragma once

#include <GL\glew.h>
#include <string>

namespace OpenGLTools
{
	class Shader
	{
	private:
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;

		Shader();
		void compileShader(GLuint shaderId,const std::string &file) const;
		void linkProgram() const;
		GLuint addShader(const std::string &path, GLenum type);

	public:
		Shader(const std::string &compute);
		Shader(const std::string &vertex, const std::string &fragment);
		Shader(const std::string &vertex, const std::string &fragment, const std::string &geometry);
		~Shader(void);
		Shader(Shader &&Shader);
		Shader(Shader const &Shader);
		Shader &operator=(Shader const &shader);
		Shader &operator=(Shader &&shader);

		void	use();	
		GLuint	getId() const;
	};

}