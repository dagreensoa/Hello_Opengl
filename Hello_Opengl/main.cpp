#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "openglErrorReporting.h"
#include "shader.h" 


#include <iostream>

float triangleData[] = {
	//positions        // colors
	//
	0.5,0.5,0,		 1,0,0,
	-0.5,0.5,0,	 0,1,0,
	-0.5,-0.5,0,		 0,0,1,
	0.5,-0.5,0,		 0,0,1
};

unsigned short indices[] = {
	0,1,2, //first triangle
	0,2,3 //second triangle
};

int main() {

	//初始化GLFW
	if (!glfwInit()) 
	{
		std::cout << "Failed to initialize GLFW";
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL); //創建視窗
	if(!window)
	{
		std::cout << "Failed to create GLFW window";
		return 1;
	}

	glfwMakeContextCurrent(window); //設定當前上下文
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return 1;
	}

	enableReportGlErrors();

#pragma region vertex array object
	//創建Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);//生成VAO
	glBindVertexArray(vao);//綁定VAO

#pragma region buffer
	//創建Buffer
	GLuint buffer;
	glGenBuffers(1, &buffer);//生成Buffer

	glBindBuffer(GL_ARRAY_BUFFER, buffer);//綁定Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);//將資料傳入Buffer

	glEnableVertexAttribArray(0);//啟用頂點屬性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);//設定頂點屬性指針 第一位表示頂點屬性索引，第二位表示每個頂點的維度，第三位表示資料類型，第四位表示是否正規化，第五位表示每個頂點的間距，第六位表示偏移量

	glEnableVertexAttribArray(1);//啟用頂點屬性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));//設定頂點屬性指針 第一位表示頂點屬性索引，第二位表示每個頂點的維度，第三位表示資料類型，第四位表示是否正規化，第五位表示每個頂點的間距，第六位表示偏移量

#pragma endregion

#pragma region index buffer
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);//生成Buffer


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);//綁定Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//將資料傳入Buffer
#pragma endregion

	glBindVertexArray(0);//解除綁定VAO

#pragma region shader loading
	Shader shader;
	
	shader.loadShaderProgramFromFile("resources/myshader.vert", "resources/myshader.frag");

	shader.bind();

	GLint time = shader.getUniformLocation("time");

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);


		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();//綁定shader

		glUniform1f(time, (float)(clock())/100.f);
		


		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	//glDeleteBuffers(1, &buffer); //刪除Buffer不一定需要windows自己會刪除
	//關閉視窗和庫
	glfwDestroyWindow(window);
	glfwTerminate();

}