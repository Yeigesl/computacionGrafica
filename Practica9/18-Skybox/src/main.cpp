//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

//glfw include
#include <GLFW/glfw3.h>

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
//Texture includes
#include "Headers/Texture.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere(20, 20);
Sphere sphere2(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
Box box,box1,box2,box3,box4,box5,box6,box7;

Shader shaderColor;
Shader shaderTexture;
Shader shaderCubeTexture;
Shader shaderMateriales;
Shader shaderDirectionLight;
Shader shaderPointLight;
Shader shaderSpotLight;

GLuint textureID1, textureID2, textureID3,textureID4,
textureID5,textureID6, textureID7, textureID8, 
textureID9, textureID10,textureID11, textureID12, 
textureID13, textureID14, textureID15, 
textureID16, textureID17, textureID18, 
textureID19, textureID20, textureID21,
textureID22, textureID23, textureID24, textureCubeTexture;
GLuint cubeTextureID;

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

std::string fileNames[6] = { 
	"../../Textures/mp_mercury/mercury_ft.tga",
	"../../Textures/mp_mercury/mercury_bk.tga",
	"../../Textures/mp_mercury/mercury_up.tga",
	"../../Textures/mp_mercury/mercury_dn.tga",
	"../../Textures/mp_mercury/mercury_rt.tga",
	"../../Textures/mp_mercury/mercury_lf.tga"
};

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
bool ilumina = true;
bool ilumina2 = true;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
			exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shaderColor.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	shaderTexture.initialize("../../Shaders/texturizado_res.vs", "../../Shaders/texturizado_res.fs");
	shaderCubeTexture.initialize("../../Shaders/cubeTexture.vs", "../../Shaders/cubeTexture.fs");
	shaderMateriales.initialize("../../Shaders/iluminacion_materiales_res.vs", "../../Shaders/iluminacion_materiales_res.fs");
	shaderDirectionLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/directionalLight.fs");
	shaderPointLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/pointLight.fs");
	shaderSpotLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/spotLight.fs");

	sphere.init();
	sphere2.init();
	cylinder.init();
	cylinder2.init();
	cylinder3.init();
	box.init();
	box1.init();
	box2.init();
	box3.init();
	box4.init();
	box5.init();
	box6.init();
	box7.init();

	camera->setPosition(glm::vec3(0.0f, 0.0f, 0.4f));

	// Textura Ladrillos
	int imageWidth, imageHeight;
	Texture texture("../../Textures/texturaLadrillos.jpg");
	FIBITMAP* bitmap = texture.loadImage(false);
	unsigned char * data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID1);
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Texture Goku
	texture = Texture("../../Textures/goku.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Textura cuadritos
	texture = Texture("../../Textures/test.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura pared 
	texture = Texture("../../Textures/pared.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// textura piso 
	texture = Texture("../../Textures/piso.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID5);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura sillon 
	texture = Texture("../../Textures/piel.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID6);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura mesa
	texture = Texture("../../Textures/mesa.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura lampara
	texture = Texture("../../Textures/lampara.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura espejo
	texture = Texture("../../Textures/espejo.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);



	//textura maceta
	texture = Texture("../../Textures/maceta.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID10);
	glBindTexture(GL_TEXTURE_2D, textureID10);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura cactus
	texture = Texture("../../Textures/cactus.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura PUERTA 
	texture = Texture("../../Textures/puerta.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura cama
	texture = Texture("../../Textures/seda.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);



	//textura almohada 
	texture = Texture("../../Textures/almohada.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura ropero
	texture = Texture("../../Textures/ropero.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura metal
	texture = Texture("../../Textures/metal.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID16);
	glBindTexture(GL_TEXTURE_2D, textureID16);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);



	//textura compu
	texture = Texture("../../Textures/compu.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID17);
	glBindTexture(GL_TEXTURE_2D, textureID17);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura  fondo compu
	texture = Texture("../../Textures/fondo.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura  teclado compu
	texture = Texture("../../Textures/teclado.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura banco de compu
	texture = Texture("../../Textures/banco.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura cocina
	texture = Texture("../../Textures/cocina.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID21);
	glBindTexture(GL_TEXTURE_2D, textureID21);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura baño
	texture = Texture("../../Textures/baño.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	//textura pared baño
	texture = Texture("../../Textures/paredbaño.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	//textura pared cristañ 
	texture = Texture("../../Textures/cristal.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	glGenTextures(1, &cubeTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		texture = Texture(fileNames[i]);
		FIBITMAP* bitmap = texture.loadImage(true);
		data = texture.convertToData(bitmap, imageWidth, imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		texture.freeImage(bitmap);
	}

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shaderColor.destroy();
	shaderTexture.destroy();
	shaderCubeTexture.destroy();
	shaderMateriales.destroy();
	shaderDirectionLight.destroy();
	shaderPointLight.destroy();
	shaderSpotLight.destroy();
	sphere.destroy();
	cylinder.destroy();
	cylinder2.destroy();
	cylinder3.destroy();
	box.destroy();
	box1.destroy();
	box2.destroy();
	box3.destroy();
	box4.destroy();
	box5.destroy();
	box6.destroy();
	box7.destroy();



}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		case GLFW_KEY_1:
			ilumina= true;
			break;
		case GLFW_KEY_2:
			ilumina= false;
			break;
		case GLFW_KEY_3:
			ilumina2 =false;
			break;

		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	glm::vec3 objPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	float angle = 0.0;
	float ratio = 20.0;

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Matrix de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		// matrix de vista
		glm::mat4 view = camera->getViewMatrix();

		glm::mat4 cubeModelMatrix = glm::translate(glm::mat4(1.0f), objPosition);

		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setShader(&shaderTexture);
		cylinder.render(0, cylinder.getSlices() * cylinder.getStacks() * 2 * 3, cubeModelMatrix);

		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3, cylinder.getSlices() * 3, cubeModelMatrix);
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3 + cylinder.getSlices() * 3, cylinder.getSlices() * 3, cubeModelMatrix);
		
		
		glBindTexture(GL_TEXTURE_2D, textureID2);
		sphere2.setShader(&shaderSpotLight);
		sphere2.setProjectionMatrix(projection);
		sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(0.0, 0.0, 0.0));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder2.setShader(&shaderPointLight);
		cylinder2.setProjectionMatrix(projection);
		cylinder2.setViewMatrix(view);
		cylinder2.setPosition(glm::vec3(0.0, 0.0, 0.0));
		cylinder2.setScale(glm::vec3(1.0, 1.0, 1.0));
		cylinder2.render();
		//pared 1 INFERIOR 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box.setShader(&shaderSpotLight);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(15.0, 0.0, -3.0));
		box.setScale(glm::vec3(60.0, 30.0, 2.0));
		box.render();
		//pared 1 posterior 
		glBindTexture(GL_TEXTURE_2D, textureID24);
		box.setShader(&shaderSpotLight);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(15.0, 0.0, 25.0));
		box.setScale(glm::vec3(60.0, 30.0, 2.0));
		box.render();



		//piso
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box1.setShader(&shaderSpotLight);
		box1.setProjectionMatrix(projection);
		box1.setViewMatrix(view);
		box1.setPosition(glm::vec3(15.0, -15.0, 0.0));
		box1.setScale(glm::vec3(60.0, 2.0, 50.0));
		box1.render();
		//techo
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box1.setShader(&shaderSpotLight);
		box1.setProjectionMatrix(projection);
		box1.setViewMatrix(view);
		box1.setPosition(glm::vec3(15.0, 15.0, 0.0));
		box1.setScale(glm::vec3(60.0, 2.0, 50.0));
		box1.render();



		//pared lateral 1
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-15.0, 0.0, 0.0));
		box2.setScale(glm::vec3(2.0, 30.0, 50.0));
		box2.render();
		
		//pared lateral 2
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box3.setShader(&shaderSpotLight);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.setPosition(glm::vec3(45.0, 0.0, 0.0));
		box3.setScale(glm::vec3(2.0, 30.0, 50.0));
		box3.render();



		//_---------------------------------------
		//SILLON 1
		
		// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box4.setShader(&shaderSpotLight);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(0.0,-11.0, 0.0));
		box4.setScale(glm::vec3(6.0, 4.0, 4.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box5.setShader(&shaderSpotLight);
		box5.setProjectionMatrix(projection);
		box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(0.0, -13.0, 0.0));
		box5.setScale(glm::vec3(6.0,2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box6.setShader(&shaderSpotLight);
		box6.setProjectionMatrix(projection);
		box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(-2.5, -12.0, 0.0));
		box6.setScale(glm::vec3(1.0, 1.5, 8.0));
		box6.render();
		// sillon codo derecho
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box7.setShader(&shaderSpotLight);
		box7.setProjectionMatrix(projection);
		box7.setViewMatrix(view);
		box7.setPosition(glm::vec3(2.5, -12.0, 0.0));
		box7.setScale(glm::vec3(1.0, 1.5, 8.0));
		box7.render();



		//_---------------------------------------
		//SILLON 2
	
		// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box4.setShader(&shaderSpotLight);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(-8.0, -11.0, 8.0));
		box4.setScale(glm::vec3(1.0, 2.0, 8.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box5.setShader(&shaderSpotLight);
		box5.setProjectionMatrix(projection);
		box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(-7.0, -13.0, 8.0));
		box5.setScale(glm::vec3(3.0, 2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box6.setShader(&shaderSpotLight);
		box6.setProjectionMatrix(projection);
		box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(-7.0, -11.0, 11.5));
		box6.setScale(glm::vec3(3.0, 1.0, 1.0));
		box6.render();




		//_---------------------------------------
		//SILLON 3

		// sillon respaldo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box4.setShader(&shaderSpotLight);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(8.0, -11.0, 8.0));
		box4.setScale(glm::vec3(1.0, 2.0, 8.0));
		box4.render();
		// sillon asiento
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box5.setShader(&shaderSpotLight);
		box5.setProjectionMatrix(projection);
		box5.setViewMatrix(view);
		box5.setPosition(glm::vec3(7.0, -13.0, 8.0));
		box5.setScale(glm::vec3(3.0, 2.0, 8.0));
		box5.render();
		// sillon codo izquierdo 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box6.setShader(&shaderSpotLight);
		box6.setProjectionMatrix(projection);
		box6.setViewMatrix(view);
		box6.setPosition(glm::vec3(7.0, -11.0, 11.5));
		box6.setScale(glm::vec3(3.0, 1.0, 1.0));
		box6.render();
		
		// sillon codo derecho
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box7.setShader(&shaderSpotLight);
		box7.setProjectionMatrix(projection);
		box7.setViewMatrix(view);
		box7.setPosition(glm::vec3(7.0, -11.0, 5.0));
		box7.setScale(glm::vec3(3.0, 1.0, 1.0));
		box7.render();


		//--------------------------- centro de mesa
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID7);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(0.0, -13.0, 8.0));
		cylinder3.setScale(glm::vec3(3.0, 3.0, 3.0));
		cylinder3.render();

		//----------------------- lampara de mesa
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(0.0, -10.0, 8.0));
		cylinder3.setScale(glm::vec3(1.0, 3.0, 1.0));
		cylinder3.render();

		//-----------------------espejo
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box4.setShader(&shaderSpotLight);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setPosition(glm::vec3(0.0, -5.0, 0.0));
		box4.setScale(glm::vec3(2.0, 5.0, 0.5));
		box4.render();


		//----------------------- maceta 
		//cilindro
		glBindTexture(GL_TEXTURE_2D, textureID10);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(6.0, -12.0, 14.0));
		cylinder3.setScale(glm::vec3(1.0, 3.0, 1.0));
		cylinder3.render();

		//cactus
		glBindTexture(GL_TEXTURE_2D, textureID11);
		sphere2.setShader(&shaderSpotLight);
		sphere2.setProjectionMatrix(projection);
		sphere2.setViewMatrix(view);
		sphere2.setPosition(glm::vec3(6.0, -10.0, 14.0));
		sphere2.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere2.render();





		//----------------PLANTA ALTA 
			//segundo piso
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box1.setShader(&shaderSpotLight);
		box1.setProjectionMatrix(projection);
		box1.setViewMatrix(view);
		box1.setPosition(glm::vec3(5.0, 0.0, 0.0));
		box1.setScale(glm::vec3(40.0, 2.0, 50.0));
		box1.render();

		//pared lateral  superior 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(5.0, 7.0, 0.0));
		box2.setScale(glm::vec3(2.0, 15.0, 30.0));
		box2.render();

		//PUERTA
		glBindTexture(GL_TEXTURE_2D, textureID12);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(5.0, 7.0, 15.0));
		box2.setScale(glm::vec3(2.0, 12.0, 5.0));
		box2.render();

		//´parte superior PUERTA
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(5.0, 13.0, 15.0));
		box2.setScale(glm::vec3(2.0, 3.0, 5.0));
		box2.render();

		//pared lateral  superior 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(5.0, 7.0, 20.0));
		box2.setScale(glm::vec3(2.0, 15.0, 5.0));
		box2.render();

		//_________________cama
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-5.0,3.0, 10.0));
		box2.setScale(glm::vec3(8.0, 2.0, 5.0));
		box2.render();

		//_________________base cama
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-5.0, 2.0, 10.0));
		box2.setScale(glm::vec3(8.0, 2.0, 5.0));
		box2.render();
	
		//_________________cabecera cama
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-8.0, 4.0, 10.0));
		box2.setScale(glm::vec3(2.0, 2.0, 5.0));
		box2.render();
		//-------almohada
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-6.0,4.0, 10.0));
		cylinder3.setScale(glm::vec3(1.0, 0.5, 3.0));
		cylinder3.render();

		//-------almohada
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-6.0, 4.0, 9.0));
		cylinder3.setScale(glm::vec3(1.0, 0.5, 3.0));
		cylinder3.render();

		//-----------------ropero
		glBindTexture(GL_TEXTURE_2D, textureID15);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(2.0,5.0, 6.0));
		box2.setScale(glm::vec3(4.0, 6.0, 5.0));
		box2.render();

		//-----------------mesa 
		glBindTexture(GL_TEXTURE_2D, textureID16);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.0, 5.0, 14.0));
		box2.setScale(glm::vec3(4.0, 1.0, 5.0));
		box2.render();
		//base mesa 
		glBindTexture(GL_TEXTURE_2D, textureID16);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-12.0, 2.0, 14.0));
		cylinder3.setScale(glm::vec3(1.0, 5.0, 3.0));
		cylinder3.render();

		 //----- base computadora
		glBindTexture(GL_TEXTURE_2D, textureID17);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.0, 5.5, 14.0));
		box2.setScale(glm::vec3(1.0,0.5, 2.0));
		box2.render();
		//----- tapa computadora
		glBindTexture(GL_TEXTURE_2D, textureID17);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.4, 5.5, 14.0));
		box2.setScale(glm::vec3(0.2, 4.0, 2.0));
		box2.render();

		//----- fondo computadora
		glBindTexture(GL_TEXTURE_2D, textureID18);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.3, 5.5, 14.0));
		box2.setScale(glm::vec3(0.1, 3.8, 1.8));
		box2.render();

		//----- teclado computadora
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.0, 5.8, 14.0));
		box2.setScale(glm::vec3(0.9, 0.1, 1.8));
		box2.render();

		//banco para computadora
		glBindTexture(GL_TEXTURE_2D, textureID20);
		cylinder3.setShader(&shaderPointLight);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.setPosition(glm::vec3(-10.0, 2.0, 14.0));
		cylinder3.setScale(glm::vec3(3.0, 3.0, 3.0));
		cylinder3.render();

		//----------------------BAÑO
		//----- teclado computadora
		glBindTexture(GL_TEXTURE_2D, textureID22);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(12.0, 8.0, -1.0));
		box2.setScale(glm::vec3(15.0, 15.0, 0.1));
		box2.render();

		//----------------PAREDES BAÑO

		glBindTexture(GL_TEXTURE_2D, textureID23);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(12.0, 8.0, 10.0));
		box2.setScale(glm::vec3(15.0, 15.0, 0.1));
		box2.render();

		glBindTexture(GL_TEXTURE_2D, textureID23);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(20.0, 8.0, 0.0));
		box2.setScale(glm::vec3(0.5, 15.0, 5.0));
		box2.render();




		//.--------------------------escaleras

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(40.0, -13.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(38.0, -12.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(37.0, -10.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(36.0, -9.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(35.0, -8.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();


		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(34.0, -7.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(33.0, -6.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(32.0, -5.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(31.0, -4.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();

		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(28.0, -3.0, 20.0));
		box2.setScale(glm::vec3(3.0, 2.0, 5.0));
		box2.render();
		//escalon
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(24.0, -1.0, 20.0));
		box2.setScale(glm::vec3(8.0, 2.0, 5.0));
		box2.render();



		// pared separa sala de  cocina
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(10.0, -8.0, 0.0));
		box2.setScale(glm::vec3(2.0, 15.0, 30.0));
		box2.render();


		// pared final de la cocina 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(25.0, -8.0, 0.0));
		box2.setScale(glm::vec3(2.0, 15.0, 20.0));
		box2.render();
		
		// cocina integral 
		// pared final de la cocina 
		glBindTexture(GL_TEXTURE_2D, textureID21);
		box2.setShader(&shaderSpotLight);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(17.0, -8.0, 0.0));
		box2.setScale(glm::vec3(8.0, 13.0, 1.0));
		box2.render();




	



		// matriz de luz 
		glm::mat4 lightModelmatrix = glm::rotate(cubeModelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix, glm::vec3(0.0f, 0.0f, -ratio));
		if (ilumina) {
			// Iluminación direccional 

			shaderDirectionLight.turnOn();
			//se comentan las sigyuientes lineas para spotlight
			glUniform3fv(shaderDirectionLight.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(2.0f,0.0f,5.0)));
			glUniform3fv(shaderDirectionLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-0.3f, -1.0f, -0.2f)));
			//glUniform3fv(shaderDirectionLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition()));
			//se comenta para direccional  y se comenta nuevamente para spot
			glUniform3fv(shaderDirectionLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront())); //direcional
			glUniform3fv(shaderDirectionLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderDirectionLight.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
			glUniform3f(shaderDirectionLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderDirectionLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			glUniform3f(shaderDirectionLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			glUniform3f(shaderDirectionLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			glUniform3f(shaderDirectionLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			//definimos ángulos para el spotlight se descomentan 
			//glUniform1f(shaderDirectionLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(5.0f))); 
			// disminuimos valores el cono se hace maspeque
			//glUniform1f(shaderDirectionLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(7.5f)));
			//modificamos los  valores de point ligth, mas pequeño el termino  cuadratico    
			//permite alejarse y que no se vea tanto la diferencia  de que se obscurece

			//glUniform1f(shaderDirectionLight.getUniformLocation("light.constant"), 1.0f); //distancias  de 400
			//glUniform1f(shaderDirectionLight.getUniformLocation("light.linear"), 0.007f);
			//glUniform1f(shaderDirectionLight.getUniformLocation("light.quadratics"), 0.0002f);
			//glUniform1f(shaderDirectionLight.getUniformLocation("material.shininess"), 32.0);
			//sphere2.render();
			//sphere2.setPosition(glm::vec3(-0.4, 0.5, 0.5));
			//sphere2.render();
			//cylinder2.setPosition(glm::vec3(-5.0, 0.0, 4.0));
			//cylinder2.render();
			//box.setPosition(glm::vec3(-5.0, 0.0, -9.0));
			//box.render();
			//PISO
			//box1.setPosition(glm::vec3(-5.0, -15.0, 0.0));
			//box1.render();
			//PARED LATERAL 1
			//box2.setPosition(glm::vec3(-20.0, 0.0, 0.0));
			//box2.render();
			//PARED LATERAL 2
			//box3.setPosition(glm::vec3(10.0, 0.0, 0.0));
			//box3.render();
			shaderDirectionLight.turnOff();
		}
		else {
			// Iluminación

			shaderSpotLight.turnOn();
			//se comentan las sigyuientes lineas para spotlight
			//glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(2.0f,0.0f,5.0)));
			//glUniform3fv(shaderSpotLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-0.3f, -1.0f, -0.2f)));
			glUniform3fv(shaderSpotLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition())); //se comenta para direccional  y se comenta nuevamente para spot
			glUniform3fv(shaderSpotLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront())); //direcional
			glUniform3fv(shaderSpotLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderSpotLight.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
			glUniform3f(shaderSpotLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderSpotLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			glUniform3f(shaderSpotLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			glUniform3f(shaderSpotLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			glUniform3f(shaderSpotLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			//definimos ángulos para el spotlight se descomentan 
			glUniform1f(shaderSpotLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(5.0f))); // disminuimos valores el cono se hace maspeque
			glUniform1f(shaderSpotLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(7.5f)));
			//modificamos los  valores de point ligth, mas pequeño el termino  cuadratico    
			//permite alejarse y que no se vea tanto la diferencia  de que se obscurece

			glUniform1f(shaderSpotLight.getUniformLocation("light.constant"), 1.0f); //distancias  de 325
			glUniform1f(shaderSpotLight.getUniformLocation("light.linear"), 0.014f);
			glUniform1f(shaderSpotLight.getUniformLocation("light.quadratics"), 0.0007f);
			glUniform1f(shaderSpotLight.getUniformLocation("material.shininess"), 32.0);
			//sphere2.render();
			//sphere2.setPosition(glm::vec3(-0.4, 0.5, 0.5));
			//sphere2.render();
			//cylinder2.setPosition(glm::vec3(-5.0, 0.0, 4.0));
			//cylinder2.render();



			shaderSpotLight.turnOff();

		}
		if (ilumina2) {
			// Iluminación

			shaderPointLight.turnOn();
			//se comentan las sigyuientes lineas para spotlight
			glUniform3fv(shaderPointLight.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(2.0f,0.0f,5.0)));
			glUniform3fv(shaderPointLight.getUniformLocation("light.direction"), 1, glm::value_ptr(glm::vec3(-0.3f, -1.0f, -0.2f)));
			glUniform3fv(shaderPointLight.getUniformLocation("light.position"), 1, glm::value_ptr(camera->getPosition())); 
			//se comenta para direccional  y se comenta nuevamente para spot
			glUniform3fv(shaderPointLight.getUniformLocation("light.direction"), 1, glm::value_ptr(camera->getFront())); 
			//direcional
			glUniform3fv(shaderPointLight.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
			glUniform3f(shaderPointLight.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
			glUniform3f(shaderPointLight.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
			glUniform3f(shaderPointLight.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
			glUniform3f(shaderPointLight.getUniformLocation("material.ambient"), 1.0, 0.2, 0.6);
			glUniform3f(shaderPointLight.getUniformLocation("material.diffuse"), 0.4, 0.5, 0.8);
			glUniform3f(shaderPointLight.getUniformLocation("material.specular"), 0.5, 0.3, 0.2);
			//definimos ángulos para el spotlight se descomentan 
			//glUniform1f(shaderPointLight.getUniformLocation("light.cutOff"), glm::cos(glm::radians(5.0f))); 
			// disminuimos valores el cono se hace maspeque
			//glUniform1f(shaderPointLight.getUniformLocation("light.outerCutOff"), glm::cos(glm::radians(7.5f)));
			//modificamos los  valores de point ligth, mas pequeño el termino  cuadratico    
			//permite alejarse y que no se vea tanto la diferencia  de que se obscurece

			glUniform1f(shaderPointLight.getUniformLocation("light.constant"), 1.0f); //distancias  de 400
			glUniform1f(shaderPointLight.getUniformLocation("light.linear"), 0.007f);
			glUniform1f(shaderPointLight.getUniformLocation("light.quadratics"), 0.0002f);
			glUniform1f(shaderPointLight.getUniformLocation("material.shininess"), 32.0);
			//sphere2.render();
			//sphere2.setPosition(glm::vec3(-0.4, 0.5, 0.5));
			//sphere2.render();
			//cylinder2.setPosition(glm::vec3(-5.0, 0.0, 4.0));
			//cylinder2.render();

			shaderPointLight.turnOff();


		}
		
		glBindTexture(GL_TEXTURE_2D, 0);

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.001;

		
		sphere.setShader(&shaderColor);
		sphere.setColor(glm::vec3(0.4f, 0.3f, 0.6f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		sphere.enableWireMode();
		sphere.render(lightModelmatrix);

	


		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderCubeTexture.turnOn();
		GLuint cubeTextureId = shaderCubeTexture.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sphere.setShader(&shaderCubeTexture);
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(glm::mat4(glm::mat3(view)));
		sphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));
		sphere.render();

		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		shaderCubeTexture.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}


// para point light comenatmos linea 371y 372.