#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//사용자정의 Header
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
		std::cout << "Window 생성 실패\n";
			return -1;
	}

	//callback 함수를 통해 window의 viewport 설정
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD설정
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD 초기화 실패\n";
		return -1;
	}


	unsigned int VAO, VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//pointer 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader TriangleShader("Triangle.vert", "Triangle.frag");



	//렌더 루프
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//삼각형 그리기위한 쉐이더를 현재부터 사용하겠다.
		TriangleShader.use();

		//삼각형을 그리기위한 쉐이더중 fragment shader의 Uniform 설정
		float timeValue = glfwGetTime();//glfwGetTime은  GLFW가 초기화된 이후부터의 시간  '초'단위로 반환
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		TriangleShader.setFloat4("unicolor", 0.0f, greenValue, 0.0f, 1.0f);


		//렌더링부분
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//버퍼 스왑하고 io이벤트 체크
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
		glfwSetWindowShouldClose(window, true); //glfw window를 꺼라 
	}
}