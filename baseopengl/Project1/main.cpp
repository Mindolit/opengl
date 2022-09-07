#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//사용자정의 Header
#include"SHADER.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int widht, int height);
void processInput(GLFWwindow* window);
float vertices[] = {
	// positions         // colors           //Texture 좌표
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
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//pointer 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	Shader TriangleShader("Triangle.vert", "Triangle.frag");

	//Texture Generating start
	unsigned int texture;
	glGenTextures(1, &texture); //gl 텍스쳐 생성
	glBindTexture(GL_TEXTURE_2D, texture);//텍스쳐 바인딩, 이제부터 Texture관련은 바인딩된 texture의 적용된다.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S축 랩핑 하는데 REPEAT형식으로 랩핑
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T축 랩핑 하는데 REPEAT형식으로 랩핑
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//MIP MAP 필터링 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("resource/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)//데이터 불러오기 성공했다면
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "텍스쳐 데이터 불러오기 실패\n";
	}

	stbi_image_free(data);//로드한 데이터를 적용했기 때문에 메모리 해제




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


		//텍스쳐 사용
		glBindTexture(GL_TEXTURE_2D, texture);


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