#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>


// 讀取整個檔案內容
char* readEntireFile(const char* source) 
{
	std::ifstream file;
	file.open(source);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << source << std::endl;
		return nullptr;
	}

	GLint size = 0;
	file.seekg(0, file.end);
	size = file.tellg();
	file.seekg(0, file.beg);

	char* fileContent = new char[size + 1] {};

	file.read(fileContent, size);

	file.close();
	
	return fileContent;

}

bool Shader::loadShaderProgramFromFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	char* vertexShaderData = readEntireFile(vertexShaderPath);
	char* fragmentShaderData = readEntireFile(fragmentShaderPath);

	if (vertexShaderData == nullptr || fragmentShaderData == nullptr)
	{
		delete[] vertexShaderData;
		delete[] fragmentShaderData;

		return 0;
	}


	bool result = loadShaderProgramFromData(vertexShaderData, fragmentShaderData);//以免出現記憶體洩漏，先將讀取的檔案內容傳入loadShaderProgramFromData函式，然後再刪除讀取的檔案內容
	
	delete[] vertexShaderData;
	delete[] fragmentShaderData;
	return result;
}

GLint createShaderFromData(const char* data, GLenum shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &data, nullptr);
	glCompileShader(shaderId);

	GLint result = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		char* message = 0;
		int l = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &l);

		if (l)
		{
			message = new char[l];

			glGetShaderInfoLog(shaderId, l, &l, message);

			message[l - 1] = 0;

			std::cout << data << ":\n" << message << std::endl;

			delete[] message;
		}
		else
		{
			std::cout << data << ":\n" << "Unknown error" << std::endl;
		}

		glDeleteShader(shaderId);

		shaderId = 0;
		return shaderId;

	}
	return shaderId;
}

bool Shader::loadShaderProgramFromData(const char* vertexShaderData, const char* fragmentShaderData)
{
	auto vertexShaderId = createShaderFromData(vertexShaderData, GL_VERTEX_SHADER);

	if (vertexShaderId == 0)
	{
		return 0;
	}

	auto fragmentShaderId = createShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);

	if (fragmentShaderId == 0)
	{
		glDeleteShader(vertexShaderId);
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);

	glLinkProgram(id);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char* message = 0;
		int l = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);
		if (l)
		{
			message = new char[l];
			glGetProgramInfoLog(id, l, &l, message);
			message[l - 1] = 0;
			std::cout << "Link error:\n" << message << std::endl;
			delete[] message;
		}
		else
		{
			std::cout << "Link error:\n" << "Unknown error" << std::endl;
		}
		glDeleteProgram(id);
		id = 0;
		return 0;
	}

}

void Shader::bind()
{
	glUseProgram(id);
}

void Shader::clear()
{
		glDeleteProgram(id);
		*this = {};

}

GLint Shader::getUniformLocation(const char* name)
{
	GLint rez = glGetUniformLocation(id, name);

	if (rez == -1)
	{
		std::cout << "Uniform " << name << " not found in shader program" << std::endl;
	}

	return rez;
}