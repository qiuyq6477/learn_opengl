

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器
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
	// 创建顶点shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//第一个是着色器对象，第二个是传递的源码字符串数量，第三个参数是顶点着色器真正的源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译
	glCompileShader(vertexShader);
	// check for shader compile errors
	//检查是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	//创建片段着色器对象
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//指定片段着色器代码
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译
	glCompileShader(fragmentShader);
	// check for shader compile errors
	//检查编译错误
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
	//链接编译好的着色器对象
	glLinkProgram(shaderProgram);
	// check for linking errors
	//检查是否链接失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//链接完成后，删除着色器对象，已经不需要他们了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// 定义顶点数据
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
	//创建顶点数组对象，任何随后的顶点属性调用都会储存在这个VAO中，
	//这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。
	//这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中
	//第一个参数是创建的数量，第二个是接收的地址
	glGenVertexArrays(2, VAO);
	//创建顶点缓冲对象，使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
	glGenBuffers(2, VBO);

	//========================================================

	//创建索引缓冲对象，用来储存索引，OpenGL调用这些顶点的索引来决定该绘制哪个顶点。
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and  then configure vertex attributes(s).
	//绑定VAO，从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。以后如果要绘制一个物体的时候，只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了。
	glBindVertexArray(VAO[0]);
	//绑定缓冲，类型为GL_ARRAY_BUFFER
	//OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
	//从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
	//状态设置函数
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//把顶点数据复制到顶点缓冲中
	//1，目标缓冲的类型
	//2，指定传输数据的大小(以字节为单位)
	//3，指向实际实际数据的指针
	//4，希望显卡如何管理给定的数据
	//* GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//* GL_DYNAMIC_DRAW：数据会被改变很多。
	//* GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	//绑定EBO
	//复制索引数组到一个索引缓冲中
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//设定顶点属性指针
	//1，指定要配置的顶点属性，对应于顶点shader里面的layout(location = 0)
	//2，指定顶点属性大小，顶点属性是一个vec3，它由3个值组成，所以大小是3
	//3，指定数据类型，
	//4，是否希望数据被标准化(Normalize)
	//5，步长(Stride)，设置连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
	//6，位置数据在缓冲中起始位置的偏移量(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//以顶点属性位置值作为参数，启用顶点属性。顶点属性默认是禁用的。
	glEnableVertexAttribArray(0);

	//========================================================

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as  the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//这个时候是可以解绑VBO的，因为此时已经设定完顶点属性指针了
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element  buffer object IS stored in the VAO; keep the EBO bound.
	//当VAO绑定的时候，不可以解绑EBO，VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。绑定VAO的同时也会自动绑定EBO。取消注释下面这句话，会报错
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify  this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind  VAOs (nor VBOs) when it's not directly necessary.
	//此时可以解绑VAO了，
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//开启线框模式
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
		//如果没有使用EBO，就要用下面这句话来绘制三角形
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
