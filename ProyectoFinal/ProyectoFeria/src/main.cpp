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
//Model includes
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

/* Camara primera persona */
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());
/* Camara tercera persona (para juegos) */
std::shared_ptr<FirstPersonCamera> camera3P(new FirstPersonCamera());


Sphere sphere(20, 20);
Sphere sphere2(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
Cylinder cylinder4(20, 20, 0.5, 0.5);
Box box,box1,box2,box3,box4,box5,box6,box7;
/* Iluminación */
Sphere sphereLuz(0.1, 0.1);
/* Carros Chocones */
Box baseCCH;
Cylinder columnasCCH(20, 20, 0.5, 0.5);
/* Ambiente*/
Box Suelo, way;


Shader shaderColor;
Shader shaderTexture;
Shader shaderCubeTexture;
Shader shaderMateriales;
Shader shaderDirectionLight;
Shader shaderPointLight;
Shader shaderSpotLight;
Shader shaderLighting; // contiene todas las luces

/* M O D E L O S */
/* Ambiente*/
Model arbol;
Model fence; 
Model gate;
Model Lampara;
Model Lucario;
Model Charizard;
/* Autos chocones */
Model Carro;
Model Carro2;
Model Carro3;
/* Rueda de la fortuna */
Model Wheel;
/* Montaña rusa  */
Model NaveSW;

GLuint textureID1, textureID2, textureID3,textureID4,
textureID5,textureID6, textureID7, textureID8, 
textureID9, textureID10,textureID11, textureID12, 
textureID13, textureID14, textureID15, 
textureID16, textureID17, textureID18, 
textureID19, textureID20, textureID21,
textureID22, textureID23, textureID24,
textureID25, textureID26, textureCubeTexture;
GLuint cubeTextureID;
/* Texturas ambiente */
GLuint textureCespedID, Camino;
GLuint plataformaCCH, columsCCH;

/* Camara */
float posX = 1.0;
float posY = 0.0;
float posZ = 24.0;

float posX3P, posY3P, posZ3P;

bool ChangeCamera = true;

/* Animaciones */
bool animation1 = false;
bool animation2 = false;


GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

std::string fileNames[6] = { 
	"../../Textures/ely_hills/hills_ft.tga",
	"../../Textures/ely_hills/hills_bk.tga",
	"../../Textures/ely_hills/hills_up.tga",
	"../../Textures/ely_hills/hills_dn.tga",
	"../../Textures/ely_hills/hills_rt.tga",
	"../../Textures/ely_hills/hills_lf.tga"
};

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
bool ilumina = true;
bool ilumina2 = true;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;
float rot1 = 0.0f, rot2 = 0.0f, rot3 = 0.0f;
float i = 0.0f;

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
	shaderLighting.initialize("../../Shaders/typeLight.vs", "../../Shaders/multipleLights.fs");



	sphere.init();
	sphere2.init();
	cylinder.init();
	cylinder2.init();
	cylinder3.init();
	cylinder4.init();
	box.init();
	box1.init();
	box2.init();
	box3.init();
	box4.init();
	box5.init();
	box6.init();
	box7.init();

	/* Luz */
	sphereLuz.init();


	/* Ambiente */
	Suelo.init();
	Suelo.scaleUVS(glm::vec2(100.0, 100.0));
	way.init();
	way.scaleUVS(glm::vec2(2.0, 100.0));

	/* Carros chocones */
	baseCCH.init();
	columnasCCH.init();

	/* Cámaras manejadas en la ventana*/
	/* Primera persona */
	camera->setPosition(glm::vec3(posX, posY, posZ));
	/* Cámara tercera persona */
	camera3P->setPosition(glm::vec3(posX3P, posY3P, posZ3P));





	/*  M O D E L O S */
	/* Ambiente */
	arbol.loadModel("../../models/Tree/Tree.obj");
	fence.loadModel("../../models/fence01_obj/fence01.obj");
	/* Rueda de la fortuna */
	Wheel.loadModel("../../models/RuedaFortuna/RuedaFortuna.obj");
	/* Carros chocones */
	Carro.loadModel("../../models/car/future_car_6_FINAL(1).obj");
	Carro2.loadModel("../../models/ToonCar/toon_car (1).obj");
	Carro3.loadModel("../../models/Car3/Flying Charger.obj");
	gate.loadModel("../../models/FBX/untitled.obj");
	Lampara.loadModel("../../models/Lampara/untitled.obj");
	Lucario.loadModel("../../models/Lucario/FitLucario04.obj");
	Charizard.loadModel("../../models/Charizard/charizard.obj");
	NaveSW.loadModel("../../models/AirPlane/11804_Airplane_v2_l2.obj");

	/*
	//-------TEXTURAS----------------------------
	Texturas para cada uno de los juegos
	|
	|
	|
	|-------------------------------------------
	*/
	int imageWidth, imageHeight;


	/* Textura suelo de la feria */
	Texture texture = Texture("../../Textures/cesped.jpg");
	FIBITMAP* bitmap = texture.loadImage(false);
	unsigned char * data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureCespedID);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
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

	/* Camino de la feria */
	texture  = Texture("../../Textures/Camino.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &Camino);
	glBindTexture(GL_TEXTURE_2D, Camino);
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

	/* Carros chocones */
	texture = Texture("../../Textures/baseCCH.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &plataformaCCH);
	glBindTexture(GL_TEXTURE_2D, plataformaCCH);
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

	texture = Texture("../../Textures/metal.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &columsCCH);
	glBindTexture(GL_TEXTURE_2D, columsCCH);
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

	// Textura										JUEGO 1
	texture = Texture("../../Textures/azul.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
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

	//textura colores 
	texture = Texture("../../Textures/colores.jpg");
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


	//textura FLECHA
	texture = Texture("../../Textures/flecha.jpg");
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

	//textura martillo
	Texture texture3 = Texture("../../Textures/martillo.png");
	FIBITMAP * bitmap3 = texture3.loadImage(false);
	unsigned char * data3 = texture3.convertToData(bitmap3, imageWidth, imageHeight);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture3.freeImage(bitmap3);

	//textura tiras
	texture = Texture("../../Textures/tiras.jpg");
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


	//textura letrero juego del cohete
	texture = Texture("../../Textures/letrero1.png");
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

	//									TEXTURA JUEGO 1
	//textura rosa 
	texture = Texture("../../Textures/rosa.jpg");
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

	//textura gold
	texture = Texture("../../Textures/gold.jpg");
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

	//textura caballo
	texture = Texture("../../Textures/caballo.png");
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


	//textura carruaje 
	texture = Texture("../../Textures/carruaje.png");
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



	//textura cono
	texture = Texture("../../Textures/cone.png");
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


	//---------------------------------------------------------------------------

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

//------------					DESTRUYE PANTALLA


void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}


//------------					DESTRUYE OBJETOS

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
	cylinder4.destroy();
	box.destroy();
	box1.destroy();
	box2.destroy();
	box3.destroy();
	box4.destroy();
	box5.destroy();
	box6.destroy();
	box7.destroy();

	/* Luces */
	sphereLuz.destroy();

	/* Ambiente */
	Suelo.destroy();
	way.destroy();

	/* Carros chocones */
	baseCCH.destroy();
	columnasCCH.destroy();
}


/*------------					CALL BACK				-------------------

Incluye teclas para :
	Cambiar el tipo de iluminación.}
	Cmabiar la rotación de los objetos

*/

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


/*------------					MOUSE CALL BACK				-------------------

Imprime las posiciones en la pantalla.

*/


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


/*------------				PROCESS INPUT				-------------------

Se manejan los eventos para mover izquierda, dercha , acercar y alejar. 

*/

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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 += 0.03;
	
		
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		rot2 -= 0.03;
		/*float giro = glm::radians(0.0);
		camera->setPosition(glm::vec3(posX, posY, posZ));*/

	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		animation1 = true;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		animation1 = false;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		posX = 0.0;
		posY = 40.0f;
		posZ = 0.0;
		camera->setPosition(glm::vec3(posX, posY, posZ));
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		posX = 0.0;
		posY = 0.0f;
		posZ = 20.0;
		camera->setPosition(glm::vec3(posX, posY, posZ));
	}
		

	glfwPollEvents();
	return continueApplication;
}



/*------------				APLICATION		-------------------

Manejo  y construcción de cada uno de los obejtos.
*/

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	glm::vec3 objPosition = glm::vec3(0.0f, 0.0f, -3.0f);
	float angle = 0.0;
	float ratio = 20.0;

	float aircraftZ = 0.0; //0.0
	float aircraftX = 0.0; // 0.0
	bool directionAirCraft = true;
	float rotationAirCraft = 0.0;
	int finishRotation = 1;

	float aircraftZ1 = 0.0;
	float aircraftX1 = 0.0;
	float rotationAirCraft1 = 0.0;
	int finishRotation1 = 1;
	bool directionAirCraft1 = true;

	float airPlaneZ = 0.0;
	float airPlaneX = 0.0;
	float rotationAirPlane = 0.0;
	int finishRotationAirPlane = 1;
	bool directionalAirPlane = true;

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

		glm::mat4 matrix0 = glm::mat4(1.0f);
		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, -4.0f));
		glm::mat4 matrixs1 = glm::translate(matrix0, glm::vec3(0.0f, 0.9f, 0.0f)); // esfera para primer juego 
		glm::mat4 matrix1 = glm::translate(matrix0, glm::vec3(0.0f, 0.9f, 0.0f)); //caja 
		glm::mat4 matrix2 = glm::translate(matrixs1, glm::vec3(0.0f, -0.25f, 0.0f));// cilindro medio
		glm::mat4 matrixs2 = glm::translate(matrix2, glm::vec3(0.0f, 0.0f, 0.35f));//brazo posterior 
		glm::mat4 matrixs2_1 = glm::translate(matrix2, glm::vec3(0.0f, 0.0f, -0.35f));// brazo anterior 


		/*----------
		/									MOVIMINETOS JUEGO 1
		*/

		matrix2 = glm::rotate(matrixs2, rot1, glm::vec3(0.0f, 0.0f, 0.01f));
		matrix2 = glm::rotate(matrixs2, rot2, glm::vec3(0.0f, 0.0f, 0.01f));
		matrixs2 = glm::rotate(matrixs2, rot1, glm::vec3(0.0f, 0.0f, 0.01f));
		matrixs2_1 = glm::rotate(matrixs2_1, rot1, glm::vec3(0.0f, 0.0f, 0.01f));
		matrixs2 = glm::rotate(matrixs2, rot2, glm::vec3(0.0f, 0.0f, 0.01f));
		matrixs2_1 = glm::rotate(matrixs2_1, rot2, glm::vec3(0.0f, 0.0f, 0.01f));

		//						Se dibuja el cylindro principal del juego martillo 
		matrix0 = glm::scale(matrix0, glm::vec3(0.6f, 1.0f, 0.6f));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setScale(glm::vec3(0.5, 1.8, 0.5));
		cylinder.render(matrix0);
		//esfera  central del juego
		glBindTexture(GL_TEXTURE_2D, textureID2);
		matrixs1 = glm::rotate(matrixs1, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs1 = glm::scale(matrixs1, glm::vec3(0.4f, 0.4f, 0.4f));
		sphere.setShader(&shaderLighting);
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.setPosition(glm::vec3(0.0, 0.0, 0.7));
		sphere.setScale(glm::vec3(1.0, 1.0, 1.0));
		sphere.render(matrixs1);
		//caja superior del juego 
		glBindTexture(GL_TEXTURE_2D, textureID1);
		matrix1 = glm::scale(matrix1, glm::vec3(0.2f, 0.2f, 0.2f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.enableWireMode();
		box.setScale(glm::vec3(1.0, 1.0, 1.0));
		box.render(matrix1);
		// cilindro de en medio 
		glBindTexture(GL_TEXTURE_2D, textureID2);
		matrix2 = glm::rotate(matrix2, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrix2 = glm::scale(matrix2, glm::vec3(0.1f, 1.3f, 0.1f));
		cylinder2.setShader(&shaderLighting);
		cylinder2.setProjectionMatrix(projection);
		cylinder2.setViewMatrix(view);
		cylinder2.render(matrix2);
		// brazo posterior 
		glBindTexture(GL_TEXTURE_2D, textureID3);
		matrixs2 = glm::rotate(matrixs2, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs2 = glm::scale(matrixs2, glm::vec3(0.1f, 0.1f, 1.8f));
		cylinder3.setShader(&shaderLighting);
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixs2);
		//caja de pasajeros  brazo 1
		glm::mat4 matrix3 = glm::translate(matrixs2, glm::vec3(0.0f, 0.0f, 0.57f)); //caja pasajeros 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		matrix3 = glm::rotate(matrix3, 1.5708f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix3 = glm::scale(matrix3, glm::vec3(0.7, 0.7f, 10.0f));
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix3);
		//detalle brazo posterior 
		glm::mat4 matrix5 = glm::translate(matrixs2, glm::vec3(0.0f, 0.0f, -0.57f)); //caja pasajeros 
		glBindTexture(GL_TEXTURE_2D, textureID5);
		matrix5 = glm::rotate(matrix5, 1.5708f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix5 = glm::scale(matrix5, glm::vec3(0.2, 1.0f, 3.0f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix5);
		// brazo anterior
		glBindTexture(GL_TEXTURE_2D, textureID3);
		matrixs2_1 = glm::rotate(matrixs2_1, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs2_1 = glm::scale(matrixs2_1, glm::vec3(0.1f, 0.1f, 1.8f));
		cylinder4.setShader(&shaderLighting);
		cylinder4.setProjectionMatrix(projection);
		cylinder4.setViewMatrix(view);
		cylinder4.render(matrixs2_1);
		//caja de pasajeros  brazo 2
		glm::mat4 matrix4 = glm::translate(matrixs2_1, glm::vec3(0.0f, 0.0f, 0.57f)); //caja pasajeros 
		glBindTexture(GL_TEXTURE_2D, textureID4);
		matrix4 = glm::rotate(matrix4, 1.5708f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix4 = glm::scale(matrix4, glm::vec3(0.7, 0.7f, 10.0f));
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix4);
		//detalle brazo anterior
		glm::mat4 matrix6 = glm::translate(matrixs2_1, glm::vec3(0.0f, 0.0f, -0.57f)); //caja pasajeros 
		glBindTexture(GL_TEXTURE_2D, textureID5);
		matrix6 = glm::rotate(matrix6, 1.5708f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix6 = glm::scale(matrix6, glm::vec3(0.2, 1.0f, 3.0f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix6);

		//---------			letrero del juego 
		glm::mat4 matrix7 = glm::translate(matrix0, glm::vec3(0.0f, 1.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		matrix7 = glm::rotate(matrix7, 1.5708f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix7 = glm::scale(matrix7, glm::vec3(0.1f, 2.5f, 0.1f));
		cylinder4.setShader(&shaderLighting);
		cylinder4.setProjectionMatrix(projection);
		cylinder4.setViewMatrix(view);
		cylinder4.render(matrix7);

		glm::mat4 matrixs3 = glm::translate(matrix7, glm::vec3(0.0f, 0.6f, 0.0f)); //caja pasajeros 
		glBindTexture(GL_TEXTURE_2D, textureID6);
		matrixs3 = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.0f, 0.1f));
		matrixs3 = glm::scale(matrixs3, glm::vec3(4.5, 0.5f, 8.5f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs3);


		/*------------				JUEGO 2		---------------------
								CARRUSEL

								BASE DE TRES COLORES
		*/
		glm::mat4 matrixs4 = glm::mat4(1.0f);
		matrixs4 = glm::translate(matrixs4, glm::vec3(-1.3f, -0.5f, -1.6f));   //BASE INFERIOR




		//				MOVIMIENTOS JUEGO 2
		matrixs4 = glm::rotate(matrixs4, rot1, glm::vec3(0.0f, 0.1f, 0.0f));
		matrixs4 = glm::rotate(matrixs4, rot2, glm::vec3(0.0f, 0.1f, 0.0f));
		/*
		if(posY > 1.0 && posY < 4.0)
		{
			posY = 2.0;
			pos
			camera->setPosition(glm::vec3(posX, posY, posZ));
		}

		*/
		//DISCO DORADO
		matrixs4 = glm::scale(matrixs4, glm::vec3(3.5f, 0.1f, 3.5f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrixs4);
		//disco rosa
		glm::mat4  matrixs5 = glm::translate(matrixs4, glm::vec3(0.0f, 11.0f, 0.0f));  //BASE MEDIA 
		matrixs5 = glm::scale(matrixs5, glm::vec3(1.0f, 0.3f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrixs5);

		//disco dorado
		glm::mat4  matrixs6 = glm::translate(matrixs5, glm::vec3(0.0f, 0.2f, 0.0f));
		matrixs6 = glm::scale(matrixs6, glm::vec3(0.5f, 0.1f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrixs6);

		//cilindro central
		glm::mat4  matrix10 = glm::translate(matrixs4, glm::vec3(0.0f, 5.0f, 0.0f)); //TUBO 1
		matrix10 = glm::scale(matrix10, glm::vec3(0.1f, 6.5f, 0.1f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix10);

		//CILINDRO 1 PARA SOSTENER LOS CABALLOS 
		//CABALLO 1
		glm::mat4  matrix8 = glm::translate(matrixs4, glm::vec3(0.2f, 5.0f, -0.05f)); //TUBO 1
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix8 = glm::scale(matrix8, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix8);

		glm::mat4  matrixs7 = glm::translate(matrix8, glm::vec3(0.0f, 0.05f, -0.1f)); //caballo 1
		//matrixs7 = glm::rotate(matrixs7, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs7 = glm::scale(matrixs7, glm::vec3(0.3, 0.3f, 3.3f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs7);


		//CABALLO 2
		glm::mat4  matrix9 = glm::translate(matrixs4, glm::vec3(0.2f, 5.0f, 0.1f)); //TUBO 1
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix9 = glm::scale(matrix9, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix9);
		glm::mat4  matrixs8 = glm::translate(matrix9, glm::vec3(0.0f, 0.05f, -0.15f)); //caballo 1
		//matrixs7 = glm::rotate(matrixs7, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs8 = glm::scale(matrixs8, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs8);

		//CABALLO 3
		glm::mat4  matrix11 = glm::translate(matrixs4, glm::vec3(0.099f, 5.0f, 0.16f)); //TUBO 2
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix11 = glm::scale(matrix11, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix11);
		glm::mat4  matrixs9 = glm::translate(matrix11, glm::vec3(0.0f, 0.05f, -0.05f)); //caballo 1
		matrixs9 = glm::scale(matrixs9, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs9);

		//CARRUAJE 1
		glm::mat4  matrix12 = glm::translate(matrixs4, glm::vec3(0.05, 5.0f, 0.22f)); //TUBO 2
		matrix12 = glm::scale(matrix12, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix12);
		glm::mat4  matrixs10 = glm::translate(matrix12, glm::vec3(0.0f, 0.05f, -0.05f)); //caballo 1
		matrixs10 = glm::scale(matrixs10, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs10);

		//disco rosa intermedio 
		glm::mat4  matrixs11 = glm::translate(matrixs4, glm::vec3(0.0f, 3.0f, 0.0f));  //BASE MEDIA 
		matrixs11 = glm::scale(matrixs11, glm::vec3(0.5f, 0.6f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrixs11);
		//ESCALERAS
		//escalon 1
		glm::mat4  matrix13 = glm::translate(matrixs4, glm::vec3(0.0f, 1.3f, 0.19f)); //caballo 1
		//matrix13 = glm::rotate(matrix13, 0.005f, glm::vec3(0.0f, 0.0f, 0.1f));
		matrix13 = glm::scale(matrix13, glm::vec3(0.05f, 0.7f, 0.05f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix13);
		//escalon 2
		glm::mat4  matrixs13 = glm::translate(matrixs4, glm::vec3(0.0f, 2.0f, 0.16f)); //caballo 1
		matrixs13 = glm::scale(matrixs13, glm::vec3(0.05f, 0.7f, 0.05f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs13);
		//escalon 3
		glm::mat4  matrix14 = glm::translate(matrixs4, glm::vec3(0.0f, 2.7f, 0.13f)); //caballo 1
		matrix14 = glm::scale(matrix14, glm::vec3(0.05f, 0.7f, 0.05f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrix14);

		//Caballo 4 disco medio 
		glm::mat4  matrix15 = glm::translate(matrixs11, glm::vec3(0.08, 5.3f, 0.20f)); //TUBO 2
		matrix15 = glm::scale(matrix15, glm::vec3(0.03f, 7.0f, 0.03f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix15);
		glm::mat4  matrixs14 = glm::translate(matrix15, glm::vec3(0.0f, 0.03f, -0.05f)); //caballo 1
		matrixs14 = glm::scale(matrixs14, glm::vec3(1.0f, 0.4f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs14);

		//Caballo 5 disco medio 
		glm::mat4  matrix16 = glm::translate(matrixs11, glm::vec3(-0.08, 5.3f, 0.20f)); //TUBO 2
		matrix16 = glm::scale(matrix16, glm::vec3(0.03f, 7.0f, 0.03f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix16);
		glm::mat4  matrixs15 = glm::translate(matrix16, glm::vec3(0.0f, 0.03f, -0.05f)); //caballo 1
		matrixs15 = glm::scale(matrixs15, glm::vec3(1.0f, 0.4f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs15);

		//Caballo 6 disco medio 
		glm::mat4  matrix17 = glm::translate(matrixs11, glm::vec3(0.08, 5.3f, -0.20f)); //TUBO 2
		matrix17 = glm::scale(matrix17, glm::vec3(0.03f, 7.0f, 0.03f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix17);
		glm::mat4  matrixs16 = glm::translate(matrix17, glm::vec3(0.0f, 0.03f, -0.05f)); //caballo 1
		matrixs16 = glm::scale(matrixs16, glm::vec3(1.0f, 0.4f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs16);

		//Caballo 7 disco medio 
		glm::mat4  matrix18 = glm::translate(matrixs11, glm::vec3(-0.08, 5.3f, -0.20f)); //TUBO 2
		matrix18 = glm::scale(matrix18, glm::vec3(0.03f, 7.0f, 0.03f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix18);
		glm::mat4  matrixs17 = glm::translate(matrix18, glm::vec3(0.0f, 0.03f, -0.05f)); //caballo 1
		matrixs17 = glm::scale(matrixs17, glm::vec3(1.0f, 0.4f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs17);

		//CABALLO 8 
		glm::mat4  matrix19 = glm::translate(matrixs4, glm::vec3(-0.2f, 5.0f, 0.05f)); //TUBO 1
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix19 = glm::scale(matrix19, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix19);

		glm::mat4  matrixs18 = glm::translate(matrix19, glm::vec3(0.0f, 0.05f, -0.1f)); //caballo 1
		//matrixs7 = glm::rotate(matrixs7, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs18 = glm::scale(matrixs18, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs18);


		//CABALLO 2
		glm::mat4  matrix20 = glm::translate(matrixs4, glm::vec3(-0.2f, 5.0f, -0.1f)); //TUBO 1
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix20 = glm::scale(matrix20, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix20);
		glm::mat4  matrixs19 = glm::translate(matrix20, glm::vec3(0.0f, 0.05f, -0.15f)); //caballo 1
		//matrixs7 = glm::rotate(matrixs7, 1.5708f, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixs19 = glm::scale(matrixs19, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs19);

		//CABALLO 3
		glm::mat4  matrix21 = glm::translate(matrixs4, glm::vec3(-0.099f, 5.0f, -0.16f)); //TUBO 2
		//matrixs = glm::rotate(matrixs3, 0.001f, glm::vec3(0.0f, 0.1f, 0.0f));
		matrix21 = glm::scale(matrix21, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix21);
		glm::mat4  matrixs20 = glm::translate(matrix21, glm::vec3(0.0f, 0.05f, -0.05f)); //caballo 1
		matrixs20 = glm::scale(matrixs20, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs20);

		//CARRUAJE 2
		glm::mat4  matrix22 = glm::translate(matrixs4, glm::vec3(-0.05, 5.0f, -0.22f)); //TUBO 2
		matrix22 = glm::scale(matrix22, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix22);
		glm::mat4  matrixs21 = glm::translate(matrix22, glm::vec3(0.0f, 0.05f, -0.05f)); //caballo 1
		matrixs21 = glm::scale(matrixs21, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs21);


		//CARRUAJE 3
		glm::mat4  matrix23 = glm::translate(matrixs4, glm::vec3(0.03, 5.0f, -0.22f)); //TUBO 2
		matrix23 = glm::scale(matrix23, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix23);
		glm::mat4  matrixs22 = glm::translate(matrix23, glm::vec3(0.0f, 0.05f, -0.05f)); //caballo 1
		matrixs22 = glm::scale(matrixs22, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs22);


		//CARRUAJE 4
		glm::mat4  matrix24 = glm::translate(matrixs4, glm::vec3(-0.1, 5.0f, 0.15f)); //TUBO 2
		matrix24 = glm::scale(matrix24, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix24);
		glm::mat4  matrixs23 = glm::translate(matrix24, glm::vec3(0.0f, 0.05f, 0.05f)); //caballo 1
		matrixs23 = glm::scale(matrixs23, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs23);

		//Caballo 
		glm::mat4  matrix25 = glm::translate(matrixs4, glm::vec3(0.1, 5.0f, -0.2f)); //TUBO 2
		matrix25 = glm::scale(matrix25, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix25);
		glm::mat4  matrixs24 = glm::translate(matrix25, glm::vec3(0.0f, 0.05f, -0.08f)); //caballo 1
		matrixs24 = glm::scale(matrixs24, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs24);

		//Carruaje
		glm::mat4  matrix26 = glm::translate(matrixs4, glm::vec3(0.13, 5.0f, -0.1f)); //TUBO 2
		matrix26 = glm::scale(matrix26, glm::vec3(0.02f, 6.5f, 0.02f));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrix26);
		glm::mat4  matrixs25 = glm::translate(matrix26, glm::vec3(0.0f, 0.05f, -0.08f)); //caballo 1
		matrixs25 = glm::scale(matrixs25, glm::vec3(0.6f, 0.3f, 3.0f));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixs25);

		//disco de puntos superior 

		glm::mat4  matrixs26 = glm::translate(matrixs4, glm::vec3(0.0f, 14.0f, 0.0f));  //BASE MEDIA 
		matrixs26 = glm::scale(matrixs26, glm::vec3(1.0f, 1.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		cylinder.setShader(&shaderLighting);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.render(matrixs26);


		/* Carritos chocones */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plataformaCCH);
		baseCCH.setShader(&shaderLighting);
		baseCCH.setProjectionMatrix(projection);
		baseCCH.setViewMatrix(view);
		baseCCH.setPosition(glm::vec3(5.0, -0.699, 5.0));
		baseCCH.setScale(glm::vec3(5.0f, 0.5f, 5.0f));
		baseCCH.offsetUVS(glm::vec2(0.01f, 0.01f));
		baseCCH.render();
		/*Techo */
		baseCCH.setPosition(glm::vec3(5.0f, 2.699f, 5.0f));
		baseCCH.setScale(glm::vec3(6.0f, 0.5f, 6.0f));
		baseCCH.render();



		/* Escalones 
		baseCCH.setPosition(glm::vec3(12.5f, -0.699f, -10.0f));
		baseCCH.setScale(glm::vec3(1.0f, 0.2f, 4.0f));
		baseCCH.render();

		baseCCH.setPosition(glm::vec3(12.5f, 0.75f, -10.0f));
		baseCCH.setScale(glm::vec3(2.0f, 0.75f, 5.0f));
		baseCCH.render();

		baseCCH.setPosition(glm::vec3(12.5f, 0.0f, -10.0f));
		baseCCH.setScale(glm::vec3(3.0f, 0.75f, 7.0f));
		baseCCH.render(); */

		/* Columnas*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, columsCCH);
		columnasCCH.setShader(&shaderLighting);
		columnasCCH.setProjectionMatrix(projection);
		columnasCCH.setViewMatrix(view);
		columnasCCH.setPosition(glm::vec3(2.55f, 0.7f, 7.45f));
		columnasCCH.setScale(glm::vec3(0.1f, 4.0f, 0.1f));
		columnasCCH.render();
		
		columnasCCH.setPosition(glm::vec3(7.45f, 0.7f, 7.4f));
		columnasCCH.setScale(glm::vec3(0.1f, 4.0f, 0.1f));
		columnasCCH.render();

		columnasCCH.setPosition(glm::vec3(2.55f, 0.7f, 2.55f));
		columnasCCH.setScale(glm::vec3(0.1f, 4.0f, 0.1f));
		columnasCCH.render();

		columnasCCH.setPosition(glm::vec3(7.45f, 0.7f, 2.55f));
		columnasCCH.setScale(glm::vec3(0.1f, 4.0f, 0.1f));
		columnasCCH.render();


		Carro.setShader(&shaderLighting);
		Carro.setProjectionMatrix(projection);
		Carro.setViewMatrix(view);
		Carro.setScale(glm::vec3(0.15f, 0.15f, 0.15f));
		/* Movimientos del modelo. Desplazamiento en eje Z */
		glm::mat4 matrixAirCraft = glm::translate(glm::mat4(1.0f), glm::vec3(aircraftX, 0.0, aircraftZ));
		matrixAirCraft = glm::translate(matrixAirCraft, glm::vec3(3.0f, -0.45f, 7.0f));
		matrixAirCraft = glm::rotate(matrixAirCraft, rotationAirCraft, glm::vec3(0, 1, 0));
		Carro.render(matrixAirCraft);

		Carro2.setShader(&shaderLighting);
		Carro2.setProjectionMatrix(projection);
		Carro2.setViewMatrix(view);
		Carro2.setScale(glm::vec3(0.002f, 0.002f, 0.002f));
		/* Movimientos del modelo. Desplazamiento en eje Z */
		glm::mat4 matrixAirCraft1 = glm::translate(glm::mat4(1.0f), glm::vec3(aircraftX1, 0.0, aircraftZ1));
		matrixAirCraft1 = glm::translate(matrixAirCraft1, glm::vec3(5.0f, -0.45f, 3.0f));
		matrixAirCraft1 = glm::rotate(matrixAirCraft1, rotationAirCraft1, glm::vec3(0, 1, 0));
		Carro2.render(matrixAirCraft1);

		/*Carro3.setShader(&shaderLighting);
		Carro3.setProjectionMatrix(projection);
		Carro3.setViewMatrix(view);
		Carro3.setScale(glm::vec3(10.0f, 10.0f, 10.0f));
		/* Movimientos del modelo. Desplazamiento en eje Z 
		glm::mat4 matrixAirCraft2 = glm::translate(glm::mat4(1.0f), glm::vec3(aircraftX2, 0.0, aircraftZ2));
		matrixAirCraft2 = glm::translate(matrixAirCraft2, glm::vec3(5.0f, -0.4f, 3.0f));
		matrixAirCraft2 = glm::rotate(matrixAirCraft2, rotationAirCraft2, glm::vec3(0, 1, 0));
		Carro3.render(matrixAirCraft2); */

		if (animation1)
		{
			/* Animación carro1 */
			if (finishRotation == 1)
			{
				aircraftX += 0.1;
				if (finishRotation == 1 && aircraftX > 4.0)
				{
					aircraftX = 4.0;
					finishRotation = 2;
				}

			} /* :)*/
			else if (finishRotation == 2)
			{
				aircraftZ -= 0.1;
				if (finishRotation == 2 && aircraftZ < -4.0)
				{
					aircraftZ = -4.0;
					finishRotation = 3;
				}
				else
				{
					rotationAirCraft += 0.1;
					if (rotationAirCraft > glm::radians(90.0f))
					{
						rotationAirCraft = glm::radians(90.0f);
					}
				}
			}
			else if (finishRotation == 3)
			{
				aircraftX -= 0.1;
				if (finishRotation == 3 && aircraftX < 0.0)
				{
					aircraftX = 0.0;
					finishRotation = 4;
				}
				else
				{
					rotationAirCraft += 0.1;
					if (rotationAirCraft > glm::radians(180.0f))
					{
						rotationAirCraft = glm::radians(180.0f);
					}
				}
			}	
			else if (finishRotation == 4)
			{
				aircraftZ += 0.1;
				if (finishRotation == 4 && aircraftZ > 0.0)
				{
					aircraftZ = 0.0;
					finishRotation = 1;
				}
				else
				{
					rotationAirCraft += 0.1;
					if (rotationAirCraft > glm::radians(270.0f))
					{
						rotationAirCraft = glm::radians(270.0f);
					}
				}
			}

			rotationAirCraft += 0.1;
			if (rotationAirCraft > glm::radians(360.0f))
			{
				rotationAirCraft = glm::radians(360.0f);
			}
				
			camera->setPosition(glm::vec3(aircraftX + 3.0, 0.0f, aircraftZ + 7.0));

			/* Animación carro 2*/
			if (finishRotation1)
			{
				if (directionAirCraft1) 
					aircraftZ1 += 0.015;

				else 
					aircraftZ1 -= 0.015;

				if (directionAirCraft1 && aircraftZ1 > 4.0)
				{
					directionAirCraft1 = false;
					finishRotation1 = false;
					aircraftZ1 = 4.0;
				}
				if (!directionAirCraft1 && aircraftZ1 < 0.0)
				{
					directionAirCraft1 = true;
					finishRotation1 = false;
					aircraftZ1 = 0.0;
				}
			}
			else
			{
				rotationAirCraft1 += 0.1;
				if (!directionAirCraft1)
				{
					if (rotationAirCraft1 > glm::radians(180.0f))
					{
						finishRotation1 = true;
						rotationAirCraft1 = glm::radians(180.0f);
					}
				}
				else
				{
					if (rotationAirCraft1 > glm::radians(360.0f))
					{
						finishRotation1 = true;
						rotationAirCraft1 = glm::radians(0.0f);
					}
				}
			}

		}

		/* Rueda de la Fortuna */
		Wheel.setShader(&shaderLighting);
		Wheel.setProjectionMatrix(projection);
		Wheel.setViewMatrix(view);
		Wheel.setPosition(glm::vec3(-10.0f, 2.0f, 5.0f));
		Wheel.setScale(glm::vec3(0.275f, 0.275f, 0.275f));
		Wheel.render();

		/* Nave */
		NaveSW.setShader(&shaderLighting);
		NaveSW.setProjectionMatrix(projection);
		NaveSW.setViewMatrix(view);
		NaveSW.setPosition(glm::vec3(-20.0f, 10.0f, 20.0f));
		NaveSW.setScale(glm::vec3(0.001f, 0.001f, 0.001f));
		NaveSW.render(); 




		/*------------				MANEJO DE LA ILUMINACION			-------------------

		Se manejan los eventos para mover izquierda, dercha , acercar y alejar.

		*/
		shaderLighting.turnOn();
		glUniform3fv(shaderLighting.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
		//Directional light
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.ambient"), 0.025, 0.025, 0.025);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.diffuse"), 0.1, 0.1, 0.1);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.specular"), 0.15, 0.15, 0.15);
		glUniform3fv(shaderLighting.getUniformLocation("directionalLight.direction"), 1, glm::value_ptr(glm::vec3(0, -1.0, 0.0)));
		//Numero de luces spot y point
		glUniform1i(shaderLighting.getUniformLocation("pointLightCount"), 0);
		glUniform1i(shaderLighting.getUniformLocation("spotLightCount"), 1);
		// Spot light
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].position"), 1, glm::value_ptr(camera->getPosition()));
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].direction"), 1, glm::value_ptr(camera->getFront()));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].constant"), 1.0f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].linear"), 0.14f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].quadratics"), 0.07f);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.ambient"), 0.025, 0.025, 0.025);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.diffuse"), 0.7, 0.2, 0.6);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.specular"), 0.1, 0.7, 0.8);
		shaderLighting.turnOff();


		/* AMBIENTE */

		/* Suelo */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		Suelo.setShader(&shaderLighting);
		Suelo.setProjectionMatrix(projection);
		Suelo.setViewMatrix(view);
		Suelo.setPosition(glm::vec3(0.0f, -0.7f, 0.0f));
		/* (x, y, z)*/
		Suelo.setScale(glm::vec3(40.0f, 0.001f, 40.0f));
		/* Offset de textura */
		Suelo.offsetUVS(glm::vec2(0.0001, 0.0001));
		Suelo.render();

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.001;

		/* Camino */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Camino);
		way.setShader(&shaderTexture);
		way.setProjectionMatrix(projection);
		way.setViewMatrix(view);
		way.setPosition(glm::vec3(1.0f, -0.698f, 0.0f));
		way.setScale(glm::vec3(1.0f, 0.0f, 35.0f));
		way.render();

		way.setPosition(glm::vec3(0.0f, -0.698f, 0.0f));
		way.setScale(glm::vec3(30.0f, 0.0f, 1.0f));
		way.render();

		way.setPosition(glm::vec3(0.0f, -0.698f, 10.0f));
		way.setScale(glm::vec3(30.0f, 0.0f, 1.0f));
		way.render();

		/* Dibujo de arboles  */
		arbol.setShader(&shaderLighting);
		arbol.setProjectionMatrix(projection);
		arbol.setViewMatrix(view);
		arbol.setPosition(glm::vec3(-10.0f, -0.7f, 15.0f));
		arbol.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
		arbol.render();
		
		arbol.setPosition(glm::vec3(-3.0f, -0.7f, 8.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(-5.0f, -0.7f, 8.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(-16.0f, -0.7f, -12.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(-11.0f, -0.7f, 0.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(3.0f, -0.7f, 17.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(3.0f, -0.7f, -17.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(5.0f, -0.7f, 11.0f));
		arbol.render();
		arbol.setPosition(glm::vec3(8.0f, -0.7f, 2.0f));
		arbol.render();


		/* Entrada (fence) */
		fence.setShader(&shaderTexture);
		fence.setProjectionMatrix(projection);
		fence.setViewMatrix(view);
		fence.setPosition(glm::vec3(5.0f, -0.1f, 18.0f));
		fence.setScale(glm::vec3(0.0065f, 0.0065f, 0.0065f));
		fence.render(); 

		
		fence.setPosition(glm::vec3(7.5f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(10.0f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(12.5f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(15.0f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(17.5f, -0.1f, 18.0f));
		fence.render();
		

		fence.setPosition(glm::vec3(-2.5f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-5.0f, -0.1f, 18.0));
		fence.render();
		fence.setPosition(glm::vec3(-7.5f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-10.0f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-12.5f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-15.0f, -0.1f, 18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-17.5f, -0.1f, 18.0f));
		fence.render();

		/* Detras*/

		fence.setPosition(glm::vec3(5.0f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(7.5f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(10.0f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(12.5f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(15.0f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(17.5f, -0.1f, -18.0f));
		fence.render();


		fence.setPosition(glm::vec3(-5.0f, -0.1f, -18.0));
		fence.render();
		fence.setPosition(glm::vec3(-7.5f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-10.0f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-12.5f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-15.0f, -0.1f, -18.0f));
		fence.render();
		fence.setPosition(glm::vec3(-17.5f, -0.1f, -18.0f));
		fence.render();

		/* Puerta */
		gate.setShader(&shaderLighting);
		gate.setProjectionMatrix(projection);
		gate.setViewMatrix(view);
		gate.setPosition(glm::vec3(1.0f, -0.7f, 18.0f));
		gate.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
		gate.render();

		/* Lampara */
		Lampara.setShader(&shaderTexture);
		Lampara.setProjectionMatrix(projection);
		Lampara.setViewMatrix(view);
		Lampara.setPosition(glm::vec3(-3.0f, 0.0f, 9.0f));
		Lampara.setScale(glm::vec3(0.15, 0.15f, 0.15f));
		Lampara.render();
		
		Lampara.setPosition(glm::vec3(-3.0f, 0.0f, 13.0f));
		Lampara.render();
		Lampara.setPosition(glm::vec3(-3.0f, 0.0f, 5.0f));
		Lampara.render();
		Lampara.setPosition(glm::vec3(-3.0f, 0.0f, 1.0f));
		Lampara.render();
		
		Lampara.setPosition(glm::vec3(-5.0f, 0.0f, 7.0f));
		Lampara.render();
		Lampara.setPosition(glm::vec3(-5.0f, 0.0f, 3.0f));
		Lampara.render();
		Lampara.setPosition(glm::vec3(-5.0f, 0.0f, -1.0f));
		Lampara.render();


		Lucario.setShader(&shaderLighting);
		Lucario.setProjectionMatrix(projection);
		Lucario.setViewMatrix(view);
		Lucario.setPosition(glm::vec3(2.0f, -0.698f, 18.0f));
		Lucario.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		Lucario.render();

		Charizard.setShader(&shaderLighting);
		Charizard.setProjectionMatrix(projection);
		Charizard.setViewMatrix(view);
		Charizard.setPosition(glm::vec3(0.0f, -0.698f, 18.0f));
		Charizard.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
		Charizard.render();



		/* Skybox */
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




//--------------------------------------------------------------------------

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}


// para point light comenatmos linea 371y 372.