#include "../Include/Common.h"
#include "../Include/mesh_data.h"

//GLUT timer variable
GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

using namespace glm;
using namespace std;

// Global variables
GLuint program;			// Shader program id
GLuint vao;				// VAO uniform id
GLuint tex[2];			// Texture uniform ids
mat4 m;					// M matrix
mat4 vp;				// View and Perspective matrix

GLint um4m;				// M matrix uniform id
GLint um4vp;			// View and Perspective matrix uniform id
GLint us2dtex[2];		// Texture uniform ids

// Load shader file to program
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

// Free shader file
void freeShaderSource(char** srcp)
{
    delete srcp[0];
    delete srcp;
}

void My_LoadModels()
{
	// Setup VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// TODO 1: Setup VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_data), mesh_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (sizeof(GL_FLOAT) * 19686 * 2));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Load textures
	texture_data albedo = load_png("albedo.png");
	texture_data terracotta = load_png("terracotta.png");

	// TODO 2: Setup 2 texture and setup parameter

	glGenTextures(2, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, albedo.width, albedo.height , 0, GL_RGB, GL_UNSIGNED_BYTE, albedo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, terracotta.width, terracotta.height, 0, GL_RGB, GL_UNSIGNED_BYTE, terracotta.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

// OpenGL initialization
void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Create Shader Program
    program = glCreateProgram();

	// Create customize shader by tell openGL specify shader type 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader file
	char** vertexShaderSource = loadShaderSource("vertex.vs.glsl");
	char** fragmentShaderSource = loadShaderSource("fragment.fs.glsl");

	// Assign content of these shader files to those shaders we created before
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	// Free the shader file string(won't be used any more)
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);

	// Compile these shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

	// Logging
	shaderLog(vertexShader);
    shaderLog(fragmentShader);

	// Assign the program we created before with these shaders
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

	um4m = glGetUniformLocation(program, "um4m");
	um4vp = glGetUniformLocation(program, "um4vp");

	// TODO 3: Get sampler2D location (global variable: us2dtex[2])
	// ...
	us2dtex[0] = glGetUniformLocation(program, "us2dtex[0]");
	us2dtex[1] = glGetUniformLocation(program, "us2dtex[1]");

	// Tell OpenGL to use this shader program now
    glUseProgram(program);

	// TODO 4: Assign binding id to sampler2D (global variable: us2dtex[2])
	glUniform1i(us2dtex[0], 0);
	glUniform1i(us2dtex[1], 1);
   
	My_LoadModels();

	glUseProgram(0);
	glBindVertexArray(0);
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	// Clear display buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(vao);

	// Setup model matrix (m)
	m = rotate(mat4(1.0f), radians(glutGet(GLUT_ELAPSED_TIME) / 10.0f), vec3(0.0f, 1.0f, 0.0f));

	// TODO 5: Bind textures
	// ...
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex[1]);

	// Pass uniforms
	glUniformMatrix4fv(um4m, 1, GL_FALSE, value_ptr(m));
	glUniformMatrix4fv(um4vp, 1, GL_FALSE, value_ptr(vp));

	// TODO 6: Issue draw command
	glDrawArraysInstanced(GL_TRIANGLES, 0, 19684, 50);
  
	// Change current display buffer to another one (refresh frame) 
    glutSwapBuffers();
}

// Setup viewing matrix (vp)
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	float viewportAspect = (float)width / (float)height;
	vp = perspective(radians(80.0f), viewportAspect, 0.1f, 1000.0f) * lookAt(vec3(5.0f, 20.0f, 0.0f), vec3(-20.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void My_Timer(int val)
{
	timer_cnt++;
	glutPostRedisplay();
	if(timer_enabled)
	{
		glutTimerFunc(timer_speed, My_Timer, val);
	}
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    // Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Quiz03 Buffer and Texture"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif
	dumpInfo();
	My_Init();

	// Register GLUT callback functions.
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutTimerFunc(timer_speed, My_Timer, 0);

	// Enter main event loop.
	glutMainLoop();

	return 0;
}