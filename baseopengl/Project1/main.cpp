#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//��������� Header
#include"SHADER.h"


void framebuffer_size_callback(GLFWwindow* window, int widht, int height);
void processInput(GLFWwindow* window);
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
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
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//pointer ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader TriangleShader("Triangle.vert", "Triangle.frag");



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