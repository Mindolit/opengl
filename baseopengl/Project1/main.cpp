#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//��������� Header
#include"SHADER.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int widht, int height);
void processInput(GLFWwindow* window);
float vertices[] = {
	// positions         // colors           //Texture ��ǥ
	  -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,1.0f,  // bottom right
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f, // bottom left
	  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,0.0f  // top 
	  // x ,y,z  , r,g,b
};


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "MINDOL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window ���� ����\n";
		return -1;
	}

	//callback �Լ��� ���� window�� viewport ����
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD �ʱ�ȭ ����\n";
		return -1;
	}


	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//pointer ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	Shader TriangleShader("Triangle.vert", "Triangle.frag");

	//Texture Generating start
	unsigned int texture;
	glGenTextures(1, &texture); //gl �ؽ��� ����
	glBindTexture(GL_TEXTURE_2D, texture);//�ؽ��� ���ε�, �������� Texture������ ���ε��� texture�� ����ȴ�.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S�� ���� �ϴµ� REPEAT�������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T�� ���� �ϴµ� REPEAT�������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//MIP MAP ���͸� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("resource/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)//������ �ҷ����� �����ߴٸ�
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "�ؽ��� ������ �ҷ����� ����\n";
	}

	stbi_image_free(data);//�ε��� �����͸� �����߱� ������ �޸� ����




	//���� ����
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//�ﰢ�� �׸������� ���̴��� ������� ����ϰڴ�.
		TriangleShader.use();

		//�ﰢ���� �׸������� ���̴��� fragment shader�� Uniform ����
		float timeValue = glfwGetTime();//glfwGetTime��  GLFW�� �ʱ�ȭ�� ���ĺ����� �ð�  '��'������ ��ȯ
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		TriangleShader.setFloat4("unicolor", 0.0f, greenValue, 0.0f, 1.0f);


		//�ؽ��� ���
		glBindTexture(GL_TEXTURE_2D, texture);


		//�������κ�
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//���� �����ϰ� io�̺�Ʈ üũ
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true); //glfw window�� ���� 
	}
}