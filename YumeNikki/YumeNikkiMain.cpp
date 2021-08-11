#pragma comment(lib, "winmm.lib")

// Std. Includes
#include <string>
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <windows.h>
#include <mmsystem.h>
using namespace std;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
int main();
void DoMovement();
void animacion();
void DibujarCabeza(glm::mat4 view, glm::mat4 model, GLint modelLoc, Model cabeza, Shader lightingShader);
void LimpiarKeyFrames();
//void PrintearCosas();
void PrepAnim();
void InputDelay();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Camera
Camera camera(glm::vec3(0.0f, 30.0f, 15.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
float rot = 0.0f;

// Light attributes
glm::vec3 PosIni(0.0f, 26.38f, 8.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;
float rotBraDerX = 0, rotBraDerY = 0, rotBraDerZ = 0;
float rotBraIzqX = 0, rotBraIzqY = 0, rotBraIzqZ = 0;

float rotPieDerX = 0, rotPieDerY = 0, rotPieDerZ = 0;
float rotPieIzqX = 0, rotPieIzqY = 0, rotPieIzqZ = 0;

float cuchilloX = -0.4f, cuchilloY = -0.05f, cuchilloZ = 0.0f;
float flautaX = 0.4f, flautaY = 0.1f, flautaZ = -0.1f;

float rotFlautaX = 0.0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

	float rotBraDerX;
	float rotBraIzqX;
	float rotBraDerY;
	float rotBraIzqY;
	float rotBraDerZ;
	float rotBraIzqZ;
	float rotIncBraDerX;
	float rotIncBraIzqX;
	float rotIncBraDerY;
	float rotIncBraIzqY;
	float rotIncBraDerZ;
	float rotIncBraIzqZ;

	float rotPieDerX;
	float rotPieIzqX;
	float rotPieDerY;
	float rotPieIzqY;
	float rotPieDerZ;
	float rotPieIzqZ;
	float rotIncPieDerX;
	float rotIncPieIzqX;
	float rotIncPieDerY;
	float rotIncPieIzqY;
	float rotIncPieDerZ;
	float rotIncPieIzqZ;

	float cuchilloX;
	float cuchilloY;
	float cuchilloZ;
	float incCuchilloX;
	float incCuchilloY;
	float incCuchilloZ;

	float flautaX;
	float flautaY;
	float flautaZ;
	float incFlautaX;
	float incFlautaY;
	float incFlautaZ;

	float rotFlautaX;
	float rotIncFlautaX;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(4.4f, 27.8f, 1.0f),
	glm::vec3(posX,posY + 0.52f,posZ),
	glm::vec3(0,1.0f,0),
	glm::vec3(0,0,1.0f)
};

void saveFrame(void)
{

	//printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotBraDerX = rotBraDerX;
	KeyFrame[FrameIndex].rotBraDerY = rotBraDerY;
	KeyFrame[FrameIndex].rotBraDerZ = rotBraDerZ;

	KeyFrame[FrameIndex].rotBraIzqX = rotBraIzqX;
	KeyFrame[FrameIndex].rotBraIzqY = rotBraIzqY;
	KeyFrame[FrameIndex].rotBraIzqZ = rotBraIzqZ;

	KeyFrame[FrameIndex].rotPieDerX = rotPieDerX;
	KeyFrame[FrameIndex].rotPieDerY = rotPieDerY;
	KeyFrame[FrameIndex].rotPieDerZ = rotPieDerZ;

	KeyFrame[FrameIndex].rotPieDerX = rotPieIzqX;
	KeyFrame[FrameIndex].rotPieDerY = rotPieIzqY;
	KeyFrame[FrameIndex].rotPieDerZ = rotPieIzqZ;

	KeyFrame[FrameIndex].cuchilloX = cuchilloX;
	KeyFrame[FrameIndex].cuchilloY = cuchilloY;
	KeyFrame[FrameIndex].cuchilloZ = cuchilloZ;

	KeyFrame[FrameIndex].flautaX = flautaX;
	KeyFrame[FrameIndex].flautaY = flautaY;
	KeyFrame[FrameIndex].flautaZ = flautaZ;

	KeyFrame[FrameIndex].rotFlautaX = rotFlautaX;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotBraDerX = KeyFrame[0].rotBraDerX;
	rotBraDerY = KeyFrame[0].rotBraDerY;
	rotBraDerZ = KeyFrame[0].rotBraDerZ;
	rotBraIzqX = KeyFrame[0].rotBraIzqX;
	rotBraIzqY = KeyFrame[0].rotBraIzqY;
	rotBraIzqZ = KeyFrame[0].rotBraIzqZ;

	rotPieDerX = KeyFrame[0].rotPieDerX;
	rotPieDerY = KeyFrame[0].rotPieDerY;
	rotPieDerZ = KeyFrame[0].rotPieDerZ;
	rotPieIzqX = KeyFrame[0].rotPieIzqX;
	rotPieIzqY = KeyFrame[0].rotPieIzqY;
	rotPieIzqZ = KeyFrame[0].rotPieIzqZ;

	cuchilloX = KeyFrame[0].cuchilloX;
	cuchilloY = KeyFrame[0].cuchilloY;
	cuchilloZ = KeyFrame[0].cuchilloZ;

	flautaX = KeyFrame[0].flautaX;
	flautaY = KeyFrame[0].flautaY;
	flautaZ = KeyFrame[0].flautaZ;

	rotFlautaX = KeyFrame[0].rotFlautaX;
}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotIncBraDerX = (KeyFrame[playIndex + 1].rotBraDerX - KeyFrame[playIndex].rotBraDerX) / i_max_steps;
	KeyFrame[playIndex].rotIncBraDerY = (KeyFrame[playIndex + 1].rotBraDerY - KeyFrame[playIndex].rotBraDerY) / i_max_steps;
	KeyFrame[playIndex].rotIncBraDerZ = (KeyFrame[playIndex + 1].rotBraDerZ - KeyFrame[playIndex].rotBraDerZ) / i_max_steps;

	KeyFrame[playIndex].rotIncBraIzqX = (KeyFrame[playIndex + 1].rotBraIzqX - KeyFrame[playIndex].rotBraIzqX) / i_max_steps;
	KeyFrame[playIndex].rotIncBraIzqY = (KeyFrame[playIndex + 1].rotBraIzqY - KeyFrame[playIndex].rotBraIzqY) / i_max_steps;
	KeyFrame[playIndex].rotIncBraIzqZ = (KeyFrame[playIndex + 1].rotBraIzqZ - KeyFrame[playIndex].rotBraIzqZ) / i_max_steps;

	KeyFrame[playIndex].rotIncPieDerX = (KeyFrame[playIndex + 1].rotPieDerX - KeyFrame[playIndex].rotPieDerX) / i_max_steps;
	KeyFrame[playIndex].rotIncPieDerY = (KeyFrame[playIndex + 1].rotPieDerY - KeyFrame[playIndex].rotPieDerY) / i_max_steps;
	KeyFrame[playIndex].rotIncPieDerZ = (KeyFrame[playIndex + 1].rotPieDerZ - KeyFrame[playIndex].rotPieDerZ) / i_max_steps;

	KeyFrame[playIndex].rotIncPieIzqX = (KeyFrame[playIndex + 1].rotPieIzqX - KeyFrame[playIndex].rotPieIzqX) / i_max_steps;
	KeyFrame[playIndex].rotIncPieIzqY = (KeyFrame[playIndex + 1].rotPieIzqY - KeyFrame[playIndex].rotPieIzqY) / i_max_steps;
	KeyFrame[playIndex].rotIncPieIzqZ = (KeyFrame[playIndex + 1].rotPieIzqZ - KeyFrame[playIndex].rotPieIzqZ) / i_max_steps;

	KeyFrame[playIndex].incCuchilloX = (KeyFrame[playIndex + 1].cuchilloX - KeyFrame[playIndex].cuchilloX) / i_max_steps;
	KeyFrame[playIndex].incCuchilloY = (KeyFrame[playIndex + 1].cuchilloY - KeyFrame[playIndex].cuchilloY) / i_max_steps;
	KeyFrame[playIndex].incCuchilloZ = (KeyFrame[playIndex + 1].cuchilloZ - KeyFrame[playIndex].cuchilloZ) / i_max_steps;

	KeyFrame[playIndex].incFlautaX = (KeyFrame[playIndex + 1].flautaX - KeyFrame[playIndex].flautaX) / i_max_steps;
	KeyFrame[playIndex].incFlautaY = (KeyFrame[playIndex + 1].flautaY - KeyFrame[playIndex].flautaY) / i_max_steps;
	KeyFrame[playIndex].incFlautaZ = (KeyFrame[playIndex + 1].flautaZ - KeyFrame[playIndex].flautaZ) / i_max_steps;

	KeyFrame[playIndex].rotIncFlautaX = (KeyFrame[playIndex + 1].rotFlautaX - KeyFrame[playIndex].rotFlautaX) / i_max_steps;
}

//Seleccion de efecto
enum Efecto {
	normal,
	lamp,
	neko,
	knife,
	flute,
};

Efecto tipo(normal);

//Variables para las animaciones
bool animSlideDoor = false;
bool open = true;
float xSlideDoor = -2.0f;
bool canInput = true;
bool on = false;
int fluteSound = 1;
bool flutePlay = true;

int main() {
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Yume Nikki", nullptr, nullptr);
	
	if (nullptr == window)     {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}
	
	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())     {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup and compile our shaders
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	//Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Model Madotsuki's bedroom
	Model piso((char*)"Models/Room/Piso.obj");
	Model puertaDeslizanteIzq((char*)"Models/Room/SlideDoor.obj");
	Model puertaDeslizanteDer((char*)"Models/Room/SlideDoor.obj");

	//Madotsuki
	Model cabeza((char*)"Models/Personaje/Cabeza.obj");
	Model cuerpo((char*)"Models/Personaje/Cuerpo.obj");
	Model brazoIzq((char*)"Models/Personaje/BrazoIzq.obj");
	Model brazoDer((char*)"Models/Personaje/BrazoDer.obj");
	Model pieIzq((char*)"Models/Personaje/PieIzquierdo.obj");
	Model pieDer((char*)"Models/Personaje/PieDerecho.obj");
	

	//Poderes
	Model cuchillo((char*)"Models/Efectos/Cuchillo.obj");
	Model flauta((char*)"Models/Efectos/Flauta.obj");
	Model lampara((char*)"Models/Efectos/Lampara.obj");
	Model gato((char*)"Models/Personaje/Neko.obj");

	//Inicialización de KeyFrames

	LimpiarKeyFrames();

	// Set up vertex data (and buffer(s)) and attribute pointers //Mediante estas coordenadas se forma un cubo que ayuda para visualizar donde se encuentra la iluminacion
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	//Skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Musica
	PlaySound(TEXT("Music/ADreamAlwaysBeginsintheVeranda.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	
	while (!glfwWindowShouldClose(window)) {
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1 HeadLamp light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), posX, posY + 0.52f, posZ);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		if (tipo == lamp && on) 	{
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		} else {
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		}
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 8.0f, -10.0f, 8.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.9f, 0.9f, 0.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.9f, 0.9f, 0.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.8f, 0.8f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		//Draw models
		//Piso
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		piso.Draw(lightingShader);

		//Puerta deslizante izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.15f, 27.65f, 12.9f));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaDeslizanteIzq.Draw(lightingShader);

		//Puerta deslizante derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xSlideDoor, 27.65f, 12.7f));
		model = glm::scale(model, glm::vec3(1.1f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaDeslizanteIzq.Draw(lightingShader);

		
		//Madotsuki
		// Cuerpo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		tmp = model = glm::translate(model, glm::vec3(posX, posY, posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.Draw(lightingShader);

		//Brazo derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-0.1f, 0.27f, -0.05));
		model = glm::rotate(model, glm::radians(rotBraDerX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraDerY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraDerZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer.Draw(lightingShader);

		////Brazo izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0.1f, 0.27f, -0.05));
		model = glm::rotate(model, glm::radians(rotBraIzqX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraIzqY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBraIzqZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzq.Draw(lightingShader);

		//// Pie Izq
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(tmp, glm::vec3(0.17f, -0.16f, -0.05f));
		model = glm::rotate(model, glm::radians(rotPieIzqX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieIzqY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieIzqZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pieIzq.Draw(lightingShader);

		//// Pie Der
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(tmp, glm::vec3(-0.17f, -0.16f, -0.05f));
		model = glm::rotate(model, glm::radians(rotPieDerX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieDerY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieDerZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pieDer.Draw(lightingShader);

		switch (tipo) {
			case normal:
				DibujarCabeza(view, model, modelLoc, cabeza, lightingShader);
				break;
			case lamp:
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::translate(model, glm::vec3(0.0f, 0.50f, -0.055f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				lampara.Draw(lightingShader);
				break;
			case neko:
				view = camera.GetViewMatrix();
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::translate(model, glm::vec3(0.0f, 0.7f, -0.07f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				gato.Draw(lightingShader);
				break;
			case knife:
				DibujarCabeza(view, model, modelLoc, cabeza, lightingShader);
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::translate(model, glm::vec3(cuchilloX, cuchilloY, cuchilloZ));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				cuchillo.Draw(lightingShader);
				break;
			case flute:
				DibujarCabeza(view, model, modelLoc, cabeza, lightingShader);
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotFlautaX), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::translate(model, glm::vec3(flautaX, flautaY, flautaZ));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				flauta.Draw(lightingShader);
				break;
			default:
				break;
		}

		//Draw cube for lights
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//glBindVertexArray(lightVAO);
		//for (GLuint i = 0; i < 4; i++) 		{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	//// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void DoMovement() {
	// Camera controls
	if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard( FORWARD, deltaTime);
	}

	if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard( BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (canInput) {
		//Selección de efectos
		if (keys[GLFW_KEY_1]) { //Normal
			tipo = normal;
			LimpiarKeyFrames();
			PrepAnim();
		}

		if (keys[GLFW_KEY_2]) { //Lampara
			tipo = lamp;
			LimpiarKeyFrames();
			PrepAnim();
		}

		if (keys[GLFW_KEY_3]) { //Neko
			tipo = neko;
			LimpiarKeyFrames();
			PrepAnim();
		}

		if (keys[GLFW_KEY_4]) { //cuchillo
			tipo = knife;
			LimpiarKeyFrames();
			PrepAnim();
		}

		if (keys[GLFW_KEY_5]) { //flauta
			tipo = flute;
			LimpiarKeyFrames();
			PrepAnim();
		}

		//Animaciones
		if (keys[GLFW_KEY_I]) {
			animSlideDoor = true;
			canInput = false;
			InputDelay();
		}

		if (keys[GLFW_KEY_Q] && play == false) {
			if (play == false && (FrameIndex > 1)) 		{

				resetElements();
				//First Interpolation				
				interpolation();

				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			} 		else 		{
				play = false;
			}

			canInput = false;
			InputDelay();
		}
	}

	/*if (keys[GLFW_KEY_T]) {
		flautaX += 0.01f;
		PrintearCosas();
	}

	if (keys[GLFW_KEY_Y]) {
		flautaY += 0.01f;
		PrintearCosas();
	}

	if (keys[GLFW_KEY_U]) {
		flautaZ += 0.01f;
		PrintearCosas();
	}

	if (keys[GLFW_KEY_G]) {
		flautaX -= 0.01f;
		PrintearCosas();
	}

	if (keys[GLFW_KEY_H]) {
		flautaY -= 0.01f;
		PrintearCosas();
	}

	if (keys[GLFW_KEY_J]) {
		flautaZ -= 0.01f;
		PrintearCosas();
	}*/
}

void animacion() {
	//slide door
	if (animSlideDoor) {
		if (open) 	{
			xSlideDoor += 0.05f;
			if (xSlideDoor >= 0) 	{
				animSlideDoor = false;
				open = false;
			}
		} else {
			xSlideDoor -= 0.05f;
			if (xSlideDoor <= -2) {
				animSlideDoor = false;
				open = true;
			}
		}
	}

	if (play) 		{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
				if (tipo == lamp) {
					on = !on;
				}
				flutePlay = true;
			} 				else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		} 			else 			{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotBraDerX += KeyFrame[playIndex].rotIncBraDerX;
			rotBraDerY += KeyFrame[playIndex].rotIncBraDerY;
			rotBraDerZ += KeyFrame[playIndex].rotIncBraDerZ;

			rotBraIzqX += KeyFrame[playIndex].rotIncBraIzqX;
			rotBraIzqY += KeyFrame[playIndex].rotIncBraIzqY;
			rotBraIzqZ += KeyFrame[playIndex].rotIncBraIzqZ;

			rotPieDerX += KeyFrame[playIndex].rotIncPieDerX;
			rotPieDerY += KeyFrame[playIndex].rotIncPieDerY;
			rotPieDerZ += KeyFrame[playIndex].rotIncPieDerZ;

			rotPieIzqX += KeyFrame[playIndex].rotIncPieIzqX;
			rotPieIzqY += KeyFrame[playIndex].rotIncPieIzqY;
			rotPieIzqZ += KeyFrame[playIndex].rotIncPieIzqZ;

			cuchilloX += KeyFrame[playIndex].incCuchilloX;
			cuchilloY += KeyFrame[playIndex].incCuchilloY;
			cuchilloZ += KeyFrame[playIndex].incCuchilloZ;

			flautaX += KeyFrame[playIndex].incFlautaX;
			flautaY += KeyFrame[playIndex].incFlautaY;
			flautaZ += KeyFrame[playIndex].incFlautaZ;

			rotFlautaX += KeyFrame[playIndex].rotIncFlautaX;

			if (playIndex == 3 && tipo == flute && flutePlay) {
				flutePlay = false;
				if (fluteSound == 1) 	{
					PlaySound(TEXT("Music/Flute1.wav"), NULL, SND_FILENAME);
				}

				if (fluteSound == 2) 	{
					PlaySound(TEXT("Music/Flute2.wav"), NULL, SND_FILENAME);
				}

				if (fluteSound == 3) {
					PlaySound(TEXT("Music/Flute3.wav"), NULL, SND_FILENAME);
				}

				if (fluteSound == 4) {
					PlaySound(TEXT("Music/Flute4.wav"), NULL, SND_FILENAME);
				}

				if (fluteSound == 5) {
					PlaySound(TEXT("Music/Flute5.wav"), NULL, SND_FILENAME);
					fluteSound = 0;
				}
				PlaySound(TEXT("Music/ADreamAlwaysBeginsintheVeranda.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
				fluteSound++;

			}


			i_curr_steps++;
		}

	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}         else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void DibujarCabeza(glm::mat4 view, glm::mat4 model, GLint modelLoc, Model cabeza, Shader lightingShader) {
	view = camera.GetViewMatrix();
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::translate(model, glm::vec3(0.0f, 0.7f, -0.07f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	cabeza.Draw(lightingShader);
}

void LimpiarKeyFrames() {
	for (int i = 0; i < MAX_FRAMES; i++) {
		KeyFrame[i].posX = posX;		//Variable para PosicionX
		KeyFrame[i].posY = posY;		//Variable para PosicionY
		KeyFrame[i].posZ = posZ;		//Variable para PosicionZ
		KeyFrame[i].incX = 0;		//Variable para IncrementoX
		KeyFrame[i].incY = 0;		//Variable para IncrementoY
		KeyFrame[i].incZ = 0;		//Variable para IncrementoZ

		KeyFrame[i].rotBraDerX = 0;
		KeyFrame[i].rotBraIzqX = 0;
		KeyFrame[i].rotBraDerY = 0;
		KeyFrame[i].rotBraIzqY = 0;
		KeyFrame[i].rotBraDerZ = 0;
		KeyFrame[i].rotBraIzqZ = 0;
		KeyFrame[i].rotIncBraDerX = 0;
		KeyFrame[i].rotIncBraIzqX = 0;
		KeyFrame[i].rotIncBraDerY = 0;
		KeyFrame[i].rotIncBraIzqY = 0;
		KeyFrame[i].rotIncBraDerZ = 0;
		KeyFrame[i].rotIncBraIzqZ = 0;

		KeyFrame[i].rotPieDerX = 0;
		KeyFrame[i].rotPieIzqX = 0;
		KeyFrame[i].rotPieDerY = 0;
		KeyFrame[i].rotPieIzqY = 0;
		KeyFrame[i].rotPieDerZ = 0;
		KeyFrame[i].rotPieIzqZ = 0;
		KeyFrame[i].rotIncPieDerX = 0;
		KeyFrame[i].rotIncPieIzqX = 0;
		KeyFrame[i].rotIncPieDerY = 0;
		KeyFrame[i].rotIncPieIzqY = 0;
		KeyFrame[i].rotIncPieDerZ = 0;
		KeyFrame[i].rotIncPieIzqZ = 0;

		KeyFrame[i].cuchilloX = -0.4f;
		KeyFrame[i].cuchilloY = 0.0f;
		KeyFrame[i].cuchilloZ = 0.0f;
		KeyFrame[i].incCuchilloX = 0.0f;
		KeyFrame[i].incCuchilloY = 0.0f;
		KeyFrame[i].incCuchilloZ = 0.0f;

		KeyFrame[i].flautaX = 0.4f;
		KeyFrame[i].flautaY = 0.1f;
		KeyFrame[i].flautaZ = -0.1f;
		KeyFrame[i].incFlautaX = 0.0f;
		KeyFrame[i].incFlautaY = 0.0f;
		KeyFrame[i].incFlautaZ = 0.0f;
		
		KeyFrame[i].rotFlautaX = 0.0f;
		KeyFrame[i].rotIncFlautaX = 0.0f;

	}
}

//Preparar animación
void PrepAnim() {
	FrameIndex = 0;
	switch (tipo) 	{
	case normal:
		break;
	case lamp:
		saveFrame();
		rotBraIzqZ = 128.0f;
		saveFrame();
		rotBraIzqZ = 0.0f;
		saveFrame();
		break;
	case neko:
		saveFrame();
		rotBraIzqZ = 100.0f;
		saveFrame();
		rotBraIzqY = -40.0f;
		saveFrame();
		rotBraIzqY = 0.0f;
		saveFrame();
		rotBraIzqZ = 0.0f;
		saveFrame();
		break;
	case knife:
		saveFrame();
		rotBraIzqY = -100.0f;
		rotBraDerY = 100.0f;
		cuchilloX = 0.0f;
		cuchilloY = -0.05f;
		cuchilloZ = 0.4f;
		saveFrame();
		rotBraIzqY = 0.0f;
		rotBraDerY = 0.0f;
		cuchilloX = 0.4f;
		cuchilloY = -0.05f;
		cuchilloZ = 0.0f;
		saveFrame();
		rotBraIzqY = -100.0f;
		rotBraDerY = 100.0f;
		cuchilloX = 0.0f;
		cuchilloY = -0.05f;
		cuchilloZ = 0.4f;
		saveFrame();
		rotBraIzqY = 0.0f;
		rotBraDerY = 0.0f;
		cuchilloX = -0.4f;
		cuchilloY = -0.05f;
		cuchilloZ = 0.0f;
		saveFrame();
		break;
	case flute:
		saveFrame();
		rotBraIzqY = -100.0f;
		rotBraDerY = 100.0f;
		flautaX = 0.0f;
		flautaY = 0.16f;
		flautaZ = 0.28f;
		saveFrame();
		rotBraDerX = -20.0f;
		rotBraIzqX = -40.0f;
		rotFlautaX = -10.0f;
		saveFrame();
		rotBraDerX = -40.0f;
		rotBraIzqX = -20.0f;
		saveFrame();
		rotBraDerX = -10.0f;
		rotBraIzqX = -30.0f;
		saveFrame();
		rotBraDerX = -60.0f;
		rotBraIzqX = -50.0f;
		saveFrame();
		rotBraDerX = 0.0f;
		rotBraIzqX = 0.0f;
		rotFlautaX = 0.0f;
		saveFrame();
		rotBraIzqY = 0.0f;
		rotBraDerY = 0.0f;
		flautaX = 0.4f;
		flautaY = 0.1f;
		flautaZ = -0.1f;
		saveFrame();
		break;
	default:
		break;
	}
	resetElements();
	canInput = false;
	InputDelay();
}

//Función auxiliar

//void PrintearCosas() {
//	/*printf("X: %f\t", flautaX);
//	printf("Y: %f\t", flautaY);
//	printf("Z: %f\n", flautaZ);*/
//}

void InputDelay() {
	Sleep(200);
	canInput = true;
}