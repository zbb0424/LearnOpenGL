#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		VertexArray VAO;
		VertexBuffer VBO(positions, 4 * 2 * sizeof(float));/*复制顶点数组到缓冲中供OpengGL使用*/

		VertexBufferLayout layout;
		layout.Push<float>(2);
		VAO.AddBuffer(VBO, layout);

		IndexBuffer IBO(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		/*uniform是全局的，是有别于顶点属性的另一种从cpu传递数据到gpu上的着色器的方式*/
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		/*清理所有绑定*/
		VAO.Unbind();
		shader.Unbind();
		VBO.Unbind();
		IBO.Unbind();

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();//重新启用shader
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			VAO.Bind();
			//IBO.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}