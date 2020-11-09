#include "../Include/Common.h"
#include "../Assets/modelData.h"

GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

using namespace glm;
mat4 mvp(1.0f);
GLuint um4mvp;
GLuint u1iflag;
GLuint program;
GLuint vao;
bool flag = false;

char** loadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char **srcp = new char*[1];
	srcp[0] = src;
	return srcp;
}

void freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

void My_Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// TODO :
	// (1.) Generate the program with shaders binded
	program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char** vertexShaderSource = loadShaderSource("vertex.vs.glsl");
	char** fragmentShaderSource = loadShaderSource("fragment.fs.glsl");
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	shaderLog(vertexShader);
	shaderLog(fragmentShader);
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// (2.) Generate a 'uniform int u1iflag' and a uniform matrix
	// ...
	u1iflag = glGetUniformLocation(program, "u1iflag");
	um4mvp = glGetUniformLocation(program, "um4mvp");

	// (3.) Generate a vertex array object for the model 
	// ...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// (4.) Generate buffer to pass the data into GPU (see Assets/modelData.h)
	GLuint vbo;
	// ...
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelData), modelData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid *)(sizeof(GL_FLOAT) * 0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid *)(sizeof(GL_FLOAT) * 3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glBindVertexArray(vao);
	// TODO :
	// (1.) Given value to uniform matrix (mvp)
	// ...
	glUniformMatrix4fv(um4mvp, 1, GL_FALSE, value_ptr(mvp));

	// (2.) Sending a draw command
	// ...
	glDrawArrays(GL_TRIANGLES, 0, 24876);

	glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	float viewportAspect = (float)width / (float)height;
	mvp = ortho(-20 * viewportAspect, 20 * viewportAspect, -20.0f, 20.0f, 0.0f, 50.0f);
	mvp = mvp * lookAt(vec3(0.0f, 20.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 10.0f));
}

void My_Timer(int val)
{
	timer_cnt++;
	glutPostRedisplay();
	if (timer_enabled)
	{
		glutTimerFunc(timer_speed, My_Timer, val);
	}
}

void My_Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
	}
	else if (state == GLUT_UP)
	{
		printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}

void My_Keyboard(unsigned char key, int x, int y)
{
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
	//TODO:
	//New a case for passing 'uniform int u1iflag' by key 'S' to fragment shader here.
	/////////////////////////////
	if (key == 's') {
		flag = !flag;
		if (flag)
			glUniform1i(u1iflag, 1);
		else
			glUniform1i(u1iflag, 0);
	}
	
	/////////////////////////////
}

void My_SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		printf("Left arrow is pressed at (%d, %d)\n", x, y);
		break;
	default:
		printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
	// Change working directory to source code path
	chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Quiz 02"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!

#ifdef _MSC_VER
	glewInit();
#endif
	dumpInfo();
	My_Init();

	// Register GLUT callback functions.
	///////////////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0);
	///////////////////////////////

	// Enter main event loop.
	//////////////
	glutMainLoop();
	//////////////
	return 0;
}
