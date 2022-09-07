#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
//��������� Header
#include"SHADER.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int widht, int height);
void processInput(GLFWwindow* window);
float vertices[] = {
	// positions         // colors           //Texture ��ǥ
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	  // x ,y,z  , r,g,b
};

float mix_value = 0;

int main()
{
	float screenWidth=800, screenHeight=600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MINDOL", NULL, NULL);
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

	unsigned int indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};
	unsigned int VAO, VBO,EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//pointer ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	Shader TriangleShader("Triangle.vert", "Triangle.frag");

	//Texture Generating start pepe
	unsigned int pepeTexture;
	glGenTextures(1, &pepeTexture); //gl �ؽ��� ����
	glBindTexture(GL_TEXTURE_2D, pepeTexture);//�ؽ��� ���ε�, �������� Texture������ ���ε��� texture�� ����ȴ�.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S�� ���� �ϴµ� REPEAT�������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//T�� ���� �ϴµ� REPEAT�������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//MIP MAP ���͸� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("resource/pepe.jfif", &width, &height, &nrChannels, 0);
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
	
	//Texture Genrating start wall;
	unsigned int  wallTexture;
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// S-AXIS WRAP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T-AXIS WRAP
	//filetering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load("resource/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "�ؽ��� 2��° ������ �ҷ����� ����\n";
	}
	stbi_image_free(data);

	TriangleShader.use();
	TriangleShader.setInt("pepeTexture", 0);
	TriangleShader.setInt("wallTexture", 1);
	TriangleShader.setFloat("mix_value", mix_value);


	

	//���� ����
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//�ﰢ�� �׸������� ���̴��� ������� ����ϰڴ�.

		//�ﰢ���� �׸������� ���̴��� fragment shader�� Uniform ����
		//float timeValue = glfwGetTime();//glfwGetTime��  GLFW�� �ʱ�ȭ�� ���ĺ����� �ð�  '��'������ ��ȯ
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//TriangleShader.setFloat4("unicolor", 0.0f, greenValue, 0.0f, 1.0f);
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
		

		unsigned int modelloc = glGetUniformLocation(TriangleShader.ID, "model");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

		modelloc = glGetUniformLocation(TriangleShader.ID, "view");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(view));

		modelloc = glGetUniformLocation(TriangleShader.ID, "projection");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(projection));


		//�ؽ��� ���
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pepeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		

		//�������κ�
		TriangleShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		TriangleShader.setFloat("mix_value", mix_value);
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
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if(mix_value<=1.0)
		mix_value+=0.001;
		std::cout << mix_value << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if(mix_value>=0.0)
		mix_value -= 0.001;
		std::cout << mix_value << std::endl;
	}
}