

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//������ɫ��
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Ƭ����ɫ��
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	// ��������shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��һ������ɫ�����󣬵ڶ����Ǵ��ݵ�Դ���ַ��������������������Ƕ�����ɫ��������Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//����
	glCompileShader(vertexShader);
	// check for shader compile errors
	//����Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	//����Ƭ����ɫ������
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//ָ��Ƭ����ɫ������
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//����
	glCompileShader(fragmentShader);
	// check for shader compile errors
	//���������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//���ӱ���õ���ɫ������
	glLinkProgram(shaderProgram);
	// check for linking errors
	//����Ƿ�����ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//������ɺ�ɾ����ɫ�������Ѿ�����Ҫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// ���嶥������
	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};
	unsigned int indices[] = {  // note that we start from 0!
		   0, 1, 3,  // first Triangle
		   1, 2, 3   // second Triangle
	};
	unsigned int VBO[2], VAO[2], EBO;
	//����������������κ����Ķ������Ե��ö��ᴢ�������VAO�У�
	//�����ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ�
	//��ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ��ո����õ�����״̬�����洢��VAO��
	//��һ�������Ǵ������������ڶ����ǽ��յĵ�ַ
	glGenVertexArrays(2, VAO);
	//�������㻺�����ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�
	glGenBuffers(2, VBO);

	//========================================================

	//�����������������������������OpenGL������Щ����������������û����ĸ����㡣
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and  then configure vertex attributes(s).
	//��VAO���Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬֮����VAO��֮��ʹ�á��Ժ����Ҫ����һ�������ʱ��ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ����ˡ�
	glBindVertexArray(VAO[0]);
	//�󶨻��壬����ΪGL_ARRAY_BUFFER
	//OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡�
	//����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	//״̬���ú���
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//�Ѷ������ݸ��Ƶ����㻺����
	//1��Ŀ�껺�������
	//2��ָ���������ݵĴ�С(���ֽ�Ϊ��λ)
	//3��ָ��ʵ��ʵ�����ݵ�ָ��
	//4��ϣ���Կ���ι������������
	//* GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	//* GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	//* GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	//��EBO
	//�����������鵽һ������������
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//�趨��������ָ��
	//1��ָ��Ҫ���õĶ������ԣ���Ӧ�ڶ���shader�����layout(location = 0)
	//2��ָ���������Դ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3
	//3��ָ���������ͣ�
	//4���Ƿ�ϣ�����ݱ���׼��(Normalize)
	//5������(Stride)�����������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)
	//6��λ�������ڻ�������ʼλ�õ�ƫ����(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//�Զ�������λ��ֵ��Ϊ���������ö������ԡ���������Ĭ���ǽ��õġ�
	glEnableVertexAttribArray(0);

	//========================================================

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as  the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//���ʱ���ǿ��Խ��VBO�ģ���Ϊ��ʱ�Ѿ��趨�궥������ָ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element  buffer object IS stored in the VAO; keep the EBO bound.
	//��VAO�󶨵�ʱ�򣬲����Խ��EBO��VAO��ʱ���ڰ󶨵������������ᱻ����ΪVAO��Ԫ�ػ�����󡣰�VAO��ͬʱҲ���Զ���EBO��ȡ��ע��������仰���ᱨ��
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify  this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind  VAOs (nor VBOs) when it's not directly necessary.
	//��ʱ���Խ��VAO�ˣ�
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//�����߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no  need to bind it every time, but we'll do so to keep things a bit more organized
		//���û��ʹ��EBO����Ҫ��������仰������������
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse  moved etc.)
		//  -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and  react accordingly
//  ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function  executes
//  ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
