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
//OpenAL includes
#include "al.h" 
#include "alc.h" 
#include "AL/alut.h"
//Canciones
#define DANCIN "media/Aaron Smith - Dancin (KRONO Remix).wav"


#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

/* Camara primera persona */
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());
/* Camara tercera persona (para juegos) */
std::shared_ptr<FirstPersonCamera> camera3P(new FirstPersonCamera());


const float toRadians = 3.14159265f / 180.0f;

Sphere sphere(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphere4(20, 20);
Sphere sphere5(20, 20);

Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
Cylinder cylinder4(20, 20, 0.5, 0.5);
Cylinder cylinder5(20, 20, 0.5, 0.5);

Box box,box1,box2,box3,box4,box5,box6,box7;
/* Iluminación */
Sphere sphereLuz(0.1, 0.1);
/* Carros Chocones */
Box baseCCH;
Cylinder columnasCCH(20, 20, 0.5, 0.5);
/* Ambiente*/
Box Suelo, way,water,hielo;


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
Model fencel;
Model gate;
Model Lampara;
Model Lucario;
Model Charizard;
/*Tazas*/
Model taza1;
Model baseT;
Model carpaT;
/*Cafeteria*/
Model coffeeShop;
Model bancaMadera;
/* Autos chocones */
Model Carro;
Model Carro2;
Model Carro3;
/* Rueda de la fortuna */
Model Wheel;
/* Nave  */
Model NaveSW;
/*Modelo montaña rusa*/
Model   montana;
Model   pingu;

GLuint textureID1, textureID2, textureID3,textureID4,
textureID5,textureID6, textureID7, textureID8, 
textureID9, textureID10,textureID11, textureID12, 
textureID13, textureID14, textureID15, 
textureID16, textureID17, textureID18, 
textureID19, textureID20, textureID21,
textureID22, textureID23, textureID24,
textureID25, textureID26, textureID27, textureID28,
textureID29, textureID30, textureID31, textureCubeTexture;
GLuint cubeTextureID;
/* Texturas ambiente */
GLuint textureCespedID, Camino,textureWater,textureHielo;
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
bool animation3 = false;


/*Animacion tazas*/
float girot = 0.0f;

//OpenAL variables
ALCcontext *context;
ALCdevice *device;
ALuint buffer;
ALint state;
ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	//Fuentes de sonido
	ALuint source[4];


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
bool mueve = false;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;
float rot1 = 0.0f, rot2 = 0.0f, rot3 = 0.0f,
rot4 = 0.0f , rot5 = 0.0f, rot6 = 0.0f, rot7 = 0.0f ;
float i = 0.0f;
int j = 0.0f;

double deltaTime;



float movX, movZ,rotationP;

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
	sphere3.init();
	sphere4.init();
	sphere5.init();
	cylinder.init();
	cylinder2.init();
	cylinder3.init();
	cylinder4.init();
	cylinder5.init();
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
	water.init();
	water.scaleUVS(glm::vec2(2.0, 100.0));
	hielo.init();
	hielo.scaleUVS(glm::vec2(2.0, 100.0));


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
	/*Tazas*/
	baseT.loadModel("../../models/BaseT/Base.obj");
	taza1.loadModel("../../models/taza1/taza17.obj");
	carpaT.loadModel("../../models/carpaT/carpa8.obj");

	/*cafeteria*/
	coffeeShop.loadModel("../../models/coffeeShop/coffeeShop.obj"); //CHECAR
	bancaMadera.loadModel("../../models/BancaMad/wood_bench.obj");

	

	



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


	/* Textura agua para el show acuático */
	texture = Texture("../../Textures/water2.jpg");
	bitmap = texture.loadImage(false);
    data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureWater);
	glBindTexture(GL_TEXTURE_2D, textureWater);
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


	//textura hielo
	texture = Texture("../../Textures/piso.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureHielo);
	glBindTexture(GL_TEXTURE_2D, textureHielo);
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

	
	//textura piel de pingüino negra
	texture = Texture("../../Textures/piel.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID27);
	glBindTexture(GL_TEXTURE_2D, textureID27);
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


	//textura piel de pingüino blanca
	texture = Texture("../../Textures/peloBlanco.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID28);
	glBindTexture(GL_TEXTURE_2D, textureID28);
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


	//textura pico pingüino
	texture = Texture("../../Textures/pico.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID30);
	glBindTexture(GL_TEXTURE_2D, textureID30);
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

	//textura ojo pingüino
	texture = Texture("../../Textures/ojo.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID31);
	glBindTexture(GL_TEXTURE_2D, textureID31);
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
	sphere2.destroy();
	sphere3.destroy();
	sphere4.destroy();
	sphere5.destroy();
	cylinder.destroy();
	cylinder2.destroy();
	cylinder3.destroy();
	cylinder4.destroy();
	cylinder5.destroy();
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
	water.destroy();

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
	//movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);

	//Up - Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
		camera->moveUpCamera (false, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->moveUpCamera (true, deltaTime);

	//camera
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0f, -1.0f, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0f, +1.0f, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(-1.0f, 0.0f, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(1.0f, 0.0f, deltaTime);

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

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
			rot4 += 0.01;
	}

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

		
		//Listener update OpenAL
		glm::vec3 pos = camera->getPosition();
		glm::vec3 front = camera->getFront();
		std::cout << "Posicion: " << pos[0] << "," << pos[1] << "," << pos[2] << std::endl;
		alListener3i(AL_POSITION, pos[0], pos[1], pos[2]);
		listenerOri[0] = front[0];
		listenerOri[1] = front[1];
		listenerOri[2] = front[2];
		alListenerfv(AL_ORIENTATION, listenerOri);

		

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
		/									MOVIMINETOS JUEGO  COHETE
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

		/*
		pINGüino*/

		//**************************************************************************


		//PINGUINO 1
		//Baase 2
		glm::mat4 matrixpbase = glm::mat4(1.0f);
		matrixpbase = glm::translate(matrixpbase, glm::vec3(-11.4f,2.0f, -5.4f));
		glm::mat4 matrixp0= glm::translate(matrixpbase, glm::vec3(0.0f, 2.0f, 0.20f));
		matrixp0 = glm::rotate(matrixpbase, rot4, glm::vec3(0.1f, 0.0f, 0.00f));
		//se coloca el   torso en la coordenada 0,0,-1
		//matrixp0 = glm::translate(matrixp0,glm::vec3(-10.4f, 2.0f, -5.0f));
		//con msts1 se  dibuja la primer esfera , se coloca -0.5 unidade en el eje y debajo del torso      
		glm::mat4 matrixsp1 = glm::translate(matrixp0, glm::vec3(0.0f, -0.05f, 0.0f));
		//se escala el cilindro del torso , es decir en el eje 0.6x se hace  mas pequeño mas angosto , pero largo
		glm::mat4 matrixsp5 = glm::translate(matrixp0, glm::vec3(0.0f, 0.05f, 0.0f));
		glm::mat4 matrixsp6 = glm::translate(matrixsp5, glm::vec3(0.03f, 0.0f, 0.0f));
		glm::mat4 matrixp7 = glm::rotate(matrixsp6, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixsp8 = glm::translate(matrixsp5, glm::vec3(-0.01f, 0.0f, 0.0f));//esfera superior torso
		glm::mat4 matrixp9 = glm::rotate(matrixsp8, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		// se coloca -.5 en x ya que  el brazo esta en -.25 
		glm::mat4 matrixsp15 = glm::translate(matrixp9, glm::vec3(-.5f, 0.0f, 0.0f));
		// posicón codo derecho 
		glm::mat4 matrixsp16 = glm::translate(matrixp7, glm::vec3(.5f, 0.0f, 0.0f));
		glm::mat4 matrixp17 = glm::rotate(matrixsp15, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixp18 = glm::rotate(matrixsp16, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixp19 = glm::rotate(matrixsp5, 0.0f, glm::vec3(0.0f, 1.0f, -1.0f));
		//base
		glBindTexture(GL_TEXTURE_2D, textureID1);
		matrixpbase = glm::scale(matrixpbase, glm::vec3(0.01f, 0.01, 0.01f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixpbase);
		//torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixp0 = glm::scale(matrixp0, glm::vec3(0.5f, 0.6f, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixp0);

		//cabeza
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixp19 = glm::translate(matrixp0, glm::vec3(0.0f, 0.6f, 0.0f));
		matrixp19 = glm::scale(matrixp19, glm::vec3(0.4f, 0.6, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixp19);
		//pico
		glm::mat4 matrixpico = glm::rotate(matrixp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpico = glm::translate(matrixp19, glm::vec3(0.0f, -0.1f, 0.4f));
		matrixpico = glm::scale(matrixpico, glm::vec3(0.2f, 0.3, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpico);
		//ojo 1 pinguino 
		glm::mat4 matrixojo1 = glm::rotate(matrixp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo1 = glm::translate(matrixp19, glm::vec3(-0.2f, 0.3f, 0.3f));
		matrixojo1 = glm::scale(matrixojo1, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo1);
		


		//ojo 1  interior pinguino 
		glm::mat4 matrixojoi = glm::rotate(matrixojo1, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoi = glm::translate(matrixojo1, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoi = glm::scale(matrixojoi, glm::vec3(0.1f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoi);
		

		//ojo 2 pinguino 
		glm::mat4 matrixojo2 = glm::rotate(matrixp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo2 = glm::translate(matrixp19, glm::vec3(0.2f, 0.3f, 0.3f));
		matrixojo2 = glm::scale(matrixojo2, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo2);
		//ojo 2  interior pinguino 
		glm::mat4 matrixojoi2 = glm::rotate(matrixojo2, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoi2 = glm::translate(matrixojo2, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoi2 = glm::scale(matrixojoi2, glm::vec3(0.10f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoi2);
		//brazo izquierdo
		//matrix9
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixp9 = glm::translate(matrixp9, glm::vec3(-0.25f, 0.1f, 0.0f));
		matrixp9 = glm::rotate(matrixp9, 1.5708f, glm::vec3(0.2f, 0.1f, 0.0f));
		matrixp9 = glm::scale(matrixp9, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixp9);
		//esfera superior izquierda torso 
		matrixsp8 = glm::scale(matrixsp8, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixsp8);
		//brazo derecho 
		//matrix7
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixp7 = glm::translate(matrixp7, glm::vec3(0.25f, 0.1f, 0.0f));
		matrixp7 = glm::rotate(matrixp7, 1.5708f, glm::vec3(-0.2f, 0.1f, 0.0f));
		matrixp7 = glm::scale(matrixp7, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixp7);
		//matrixs6
		//esfera superior derecha 
		matrixsp6 = glm::scale(matrixsp6, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixsp6);
		//hemos escalado las esferas a 0.1
		// esfera superior centro 
		matrixsp5 = glm::scale(matrixsp5, glm::vec3(0.1, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixsp5);

	

	
		//Estomago Blanco
		glm::mat4 matrixpanza = glm::rotate(matrixp0, 0.0f, glm::vec3(0.5f, 0.5f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, textureID28);
		matrixpanza = glm::translate(matrixp0, glm::vec3(0.0f, -0.1f, 0.3f));
		matrixpanza = glm::scale(matrixpanza, glm::vec3(0.6f, 0.6, 0.6f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpanza);
		


		glm::mat4 matrixsp2 = glm::translate(matrixsp1, glm::vec3(-0.0225f, 0.0f, 0.0f));
		glm::mat4 matrixsp3 = glm::translate(matrixsp1, glm::vec3(0.0225f, 0.0f, 0.0f));

		//esfera inferior centro torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixsp1 = glm::scale(matrixsp1, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixsp1);

		glm::mat4 matrixp1 = glm::rotate(matrixsp2, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixp1 = glm::translate(matrixp1, glm::vec3(0.0, -0.04, 0.0));

		glm::mat4 matrixp10 = glm::rotate(matrixsp3, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixp10 = glm::translate(matrixp10, glm::vec3(0.0, -0.04, 0.0));

		// movemos hacia la posición que queremos 
		//rod izq
		glm::mat4 matrixsp4 = glm::translate(matrixp1, glm::vec3(0.0f, -0.4f, 0.0f));
		//rod der 
		glm::mat4 matrixsp11 = glm::translate(matrixp10, glm::vec3(0.0f, -0.4f, 0.0f));
		//posiciona pierna izquierda 
		glm::mat4 matrixp2 = glm::rotate(matrixsp4, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixp2 = glm::translate(matrixsp4, glm::vec3(0.0f, -0.3f, 0.0f));
		//posiciona pierna derecha 
		glm::mat4 matrixp12 = glm::rotate(matrixsp11, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixp12 = glm::translate(matrixsp11, glm::vec3(0.0f, -0.3f, 0.0f));
	
		//rodilla derecha
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixsp11 = glm::rotate(matrixsp11, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixsp11 = glm::scale(matrixsp11, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixsp11);
		//rodilla izquierda
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixsp4 = glm::rotate(matrixsp4, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixsp4 = glm::scale(matrixsp4, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixsp4);


		//parte 1 pierna derecha
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixp10 = glm::scale(matrixp10, glm::vec3(0.2f, 0.4f, 0.3f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixp10);

		//parte 1 pierna izquierda 
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixp1 = glm::scale(matrixp1, glm::vec3(0.2f, 0.4f, 0.2f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixp1);
		//Esfera izquierda torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixsp2 = glm::scale(matrixsp2, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixsp2);
		//Esfera derecha inferior torso 
		//Esto permite hacer las esferas mas grandes , es decir escalarlas 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixsp3 = glm::scale(matrixsp3, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixsp3);
		/*
		
		-------------------------------pinguino 2
		
		
		*/


		//PINGUINO 2
		//Baase 2
		glm::mat4 matrixppbase = glm::mat4(1.0f);
		matrixppbase = glm::translate(matrixppbase, glm::vec3(-8.4f, 1.8f, -5.4f));
		glm::mat4 matrixpp0 = glm::translate(matrixppbase, glm::vec3(0.0f, 2.0f, 0.20f));
		matrixpp0 = glm::rotate(matrixppbase, rot4, glm::vec3(0.1f, 0.0f, 0.00f));
		//se coloca el   torso en la coordenada 0,0,-1
		//matrixp0 = glm::translate(matrixp0,glm::vec3(-10.4f, 2.0f, -5.0f));
		//con msts1 se  dibuja la primer esfera , se coloca -0.5 unidade en el eje y debajo del torso      
		glm::mat4 matrixspp1 = glm::translate(matrixpp0, glm::vec3(0.0f, -0.05f, 0.0f));
		//se escala el cilindro del torso , es decir en el eje 0.6x se hace  mas pequeño mas angosto , pero largo
		glm::mat4 matrixspp5 = glm::translate(matrixpp0, glm::vec3(0.0f, 0.05f, 0.0f));
		glm::mat4 matrixspp6 = glm::translate(matrixspp5, glm::vec3(0.03f, 0.0f, 0.0f));
		glm::mat4 matrixpp7 = glm::rotate(matrixspp6, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixspp8 = glm::translate(matrixspp5, glm::vec3(-0.01f, 0.0f, 0.0f));//esfera superior torso
		glm::mat4 matrixpp9 = glm::rotate(matrixspp8, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		// se coloca -.5 en x ya que  el brazo esta en -.25 
		glm::mat4 matrixspp15 = glm::translate(matrixpp9, glm::vec3(-.5f, 0.0f, 0.0f));
		// posicón codo derecho 
		glm::mat4 matrixspp16 = glm::translate(matrixpp7, glm::vec3(.5f, 0.0f, 0.0f));
		glm::mat4 matrixpp17 = glm::rotate(matrixspp15, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixpp18 = glm::rotate(matrixspp16, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixpp19 = glm::rotate(matrixspp5, 0.0f, glm::vec3(0.0f, 1.0f, -1.0f));
		//base
		glBindTexture(GL_TEXTURE_2D, textureID1);
		matrixppbase = glm::scale(matrixppbase, glm::vec3(0.01f, 0.01, 0.01f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixppbase);
		//torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp0 = glm::scale(matrixpp0, glm::vec3(0.5f, 0.6f, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpp0);

		//cabeza
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp19 = glm::translate(matrixpp0, glm::vec3(0.0f, 0.6f, 0.0f));
		matrixpp19 = glm::scale(matrixpp19, glm::vec3(0.4f, 0.6, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpp19);
		//pico
		glm::mat4 matrixpicop = glm::rotate(matrixpp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpicop = glm::translate(matrixpp19, glm::vec3(0.0f, -0.1f, 0.4f));
		matrixpicop = glm::scale(matrixpicop, glm::vec3(0.2f, 0.3, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpicop);
		//ojo 1 pinguino 
		glm::mat4 matrixojo1p = glm::rotate(matrixpp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo1p = glm::translate(matrixpp19, glm::vec3(-0.2f, 0.3f, 0.3f));
		matrixojo1p = glm::scale(matrixojo1p, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo1p);



		//ojo 1  interior pinguino 
		glm::mat4 matrixojoip = glm::rotate(matrixojo1p, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoip = glm::translate(matrixojo1p, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoip = glm::scale(matrixojoip, glm::vec3(0.1f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoip);


		//ojo 2 pinguino 
		glm::mat4 matrixojo2p = glm::rotate(matrixpp19, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo2p = glm::translate(matrixpp19, glm::vec3(0.2f, 0.3f, 0.3f));
		matrixojo2p = glm::scale(matrixojo2p, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo2p);
		//ojo 2  interior pinguino 
		glm::mat4 matrixojoi2p = glm::rotate(matrixojo2p, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoi2p = glm::translate(matrixojo2p, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoi2p = glm::scale(matrixojoi2p, glm::vec3(0.10f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoi2p);
		//brazo izquierdo
		//matrix9
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp9 = glm::translate(matrixpp9, glm::vec3(-0.25f, 0.1f, 0.0f));
		matrixpp9 = glm::rotate(matrixpp9, 1.5708f, glm::vec3(0.2f, 0.1f, 0.0f));
		matrixpp9 = glm::scale(matrixpp9, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixpp9);
		//esfera superior izquierda torso 
		matrixspp8 = glm::scale(matrixspp8, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp8);
		//brazo derecho 
		//matrix7
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp7 = glm::translate(matrixpp7, glm::vec3(0.25f, 0.1f, 0.0f));
		matrixpp7 = glm::rotate(matrixpp7, 1.5708f, glm::vec3(-0.2f, 0.1f, 0.0f));
		matrixpp7 = glm::scale(matrixpp7, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixpp7);
		//matrixs6
		//esfera superior derecha 
		matrixspp6 = glm::scale(matrixspp6, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp6);
		//hemos escalado las esferas a 0.1
		// esfera superior centro 
		matrixspp5 = glm::scale(matrixspp5, glm::vec3(0.1, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp5);




		//Estomago Blanco
		glm::mat4 matrixpanzap = glm::rotate(matrixpp0, 0.0f, glm::vec3(0.5f, 0.5f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, textureID28);
		matrixpanzap = glm::translate(matrixpp0, glm::vec3(0.0f, -0.1f, 0.3f));
		matrixpanzap = glm::scale(matrixpanzap, glm::vec3(0.6f, 0.6, 0.6f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpanzap);



		glm::mat4 matrixspp2 = glm::translate(matrixspp1, glm::vec3(-0.0225f, 0.0f, 0.0f));
		glm::mat4 matrixspp3 = glm::translate(matrixspp1, glm::vec3(0.0225f, 0.0f, 0.0f));

		//esfera inferior centro torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp1 = glm::scale(matrixspp1, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp1);

		glm::mat4 matrixpp1 = glm::rotate(matrixspp2, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp1 = glm::translate(matrixpp1, glm::vec3(0.0, -0.04, 0.0));

		glm::mat4 matrixpp10 = glm::rotate(matrixspp3, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp10 = glm::translate(matrixpp10, glm::vec3(0.0, -0.04, 0.0));

		// movemos hacia la posición que queremos 
		//rod izq
		glm::mat4 matrixspp4 = glm::translate(matrixpp1, glm::vec3(0.0f, -0.4f, 0.0f));
		//rod der 
		glm::mat4 matrixspp11 = glm::translate(matrixpp10, glm::vec3(0.0f, -0.4f, 0.0f));
		//posiciona pierna izquierda 
		glm::mat4 matrixpp2 = glm::rotate(matrixspp4, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp2 = glm::translate(matrixspp4, glm::vec3(0.0f, -0.3f, 0.0f));
		//posiciona pierna derecha 
		glm::mat4 matrixpp12 = glm::rotate(matrixspp11, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp12 = glm::translate(matrixspp11, glm::vec3(0.0f, -0.3f, 0.0f));

		//rodilla derecha
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp11 = glm::rotate(matrixspp11, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixspp11 = glm::scale(matrixspp11, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp11);
		//rodilla izquierda
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp4 = glm::rotate(matrixspp4, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixspp4 = glm::scale(matrixspp4, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp4);


		//parte 1 pierna derecha
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpp10 = glm::scale(matrixpp10, glm::vec3(0.2f, 0.4f, 0.3f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixpp10);

		//parte 1 pierna izquierda 
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpp1 = glm::scale(matrixpp1, glm::vec3(0.2f, 0.4f, 0.2f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixpp1);
		//Esfera izquierda torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp2 = glm::scale(matrixspp2, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp2);
		//Esfera derecha inferior torso 
		//Esto permite hacer las esferas mas grandes , es decir escalarlas 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp3 = glm::scale(matrixspp3, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp3);


		/*
		---------------------------Pingu 3
		
		*/

		//PINGUINO 2
		//Baase 2
		glm::mat4 matrixppbasei = glm::mat4(1.0f);
		matrixppbasei = glm::translate(matrixppbasei, glm::vec3(-6.4f, 1.8f, -5.2f));
		glm::mat4 matrixpp0i = glm::translate(matrixppbasei, glm::vec3(0.0f, 2.0f, 0.20f));
		matrixpp0i = glm::rotate(matrixppbasei, rot4, glm::vec3(0.1f, 0.0f, 0.00f));
		//se coloca el   torso en la coordenada 0,0,-1
		//matrixp0 = glm::translate(matrixp0,glm::vec3(-10.4f, 2.0f, -5.0f));
		//con msts1 se  dibuja la primer esfera , se coloca -0.5 unidade en el eje y debajo del torso      
		glm::mat4 matrixspp1i = glm::translate(matrixpp0i, glm::vec3(0.0f, -0.05f, 0.0f));
		//se escala el cilindro del torso , es decir en el eje 0.6x se hace  mas pequeño mas angosto , pero largo
		glm::mat4 matrixspp5i = glm::translate(matrixpp0i, glm::vec3(0.0f, 0.05f, 0.0f));
		glm::mat4 matrixspp6i = glm::translate(matrixspp5i, glm::vec3(0.03f, 0.0f, 0.0f));
		glm::mat4 matrixpp7i = glm::rotate(matrixspp6i, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixspp8i = glm::translate(matrixspp5i, glm::vec3(-0.01f, 0.0f, 0.0f));//esfera superior torso
		glm::mat4 matrixpp9i = glm::rotate(matrixspp8i, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		// se coloca -.5 en x ya que  el brazo esta en -.25 
		glm::mat4 matrixspp15i = glm::translate(matrixpp9i, glm::vec3(-.5f, 0.0f, 0.0f));
		// posicón codo derecho 
		glm::mat4 matrixspp16i = glm::translate(matrixpp7i, glm::vec3(.5f, 0.0f, 0.0f));
		glm::mat4 matrixpp17i = glm::rotate(matrixspp15i, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixpp18i = glm::rotate(matrixspp16i, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixpp19i = glm::rotate(matrixspp5i, 0.0f, glm::vec3(0.0f, 1.0f, -1.0f));
		//base
		glBindTexture(GL_TEXTURE_2D, textureID1);
		matrixppbasei = glm::scale(matrixppbasei, glm::vec3(0.01f, 0.01, 0.01f));
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.render(matrixppbasei);
		//torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp0i = glm::scale(matrixpp0i, glm::vec3(0.5f, 0.6f, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpp0i);

		//cabeza
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp19i = glm::translate(matrixpp0i, glm::vec3(0.0f, 0.6f, 0.0f));
		matrixpp19i = glm::scale(matrixpp19i, glm::vec3(0.4f, 0.6, 0.4f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpp19i);
		//pico
		glm::mat4 matrixpicopi = glm::rotate(matrixpp19i, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpicopi = glm::translate(matrixpp19i, glm::vec3(0.0f, -0.1f, 0.4f));
		matrixpicopi = glm::scale(matrixpicopi, glm::vec3(0.2f, 0.3, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpicopi);
		//ojo 1 pinguino 
		glm::mat4 matrixojo1pi = glm::rotate(matrixpp19i, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo1pi = glm::translate(matrixpp19i, glm::vec3(-0.2f, 0.3f, 0.3f));
		matrixojo1pi = glm::scale(matrixojo1pi, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo1pi);



		//ojo 1  interior pinguino 
		glm::mat4 matrixojoipi = glm::rotate(matrixojo1pi, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoipi = glm::translate(matrixojo1pi, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoipi = glm::scale(matrixojoipi, glm::vec3(0.1f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoipi);


		//ojo 2 pinguino 
		glm::mat4 matrixojo2pi = glm::rotate(matrixpp19i, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		matrixojo2pi = glm::translate(matrixpp19i, glm::vec3(0.2f, 0.3f, 0.3f));
		matrixojo2pi = glm::scale(matrixojo2pi, glm::vec3(0.2f, 0.2, 0.2f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojo2pi);
		//ojo 2  interior pinguino 
		glm::mat4 matrixojoi2pi = glm::rotate(matrixojo2pi, 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixojoi2pi = glm::translate(matrixojo2pi, glm::vec3(0.0f, 0.1f, 0.4f));
		matrixojoi2pi = glm::scale(matrixojoi2pi, glm::vec3(0.10f, 0.1, 0.1f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixojoi2pi);
		//brazo izquierdo
		//matrix9
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp9i = glm::translate(matrixpp9i, glm::vec3(-0.25f, 0.1f, 0.0f));
		matrixpp9i = glm::rotate(matrixpp9i, 1.5708f, glm::vec3(0.2f, 0.1f, 0.0f));
		matrixpp9i = glm::scale(matrixpp9i, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixpp9i);
		//esfera superior izquierda torso 
		matrixspp8i = glm::scale(matrixspp8i, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp8i);
		//brazo derecho 
		//matrix7
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixpp7i = glm::translate(matrixpp7i, glm::vec3(0.25f, 0.1f, 0.0f));
		matrixpp7i = glm::rotate(matrixpp7i, 1.5708f, glm::vec3(-0.2f, 0.1f, 0.0f));
		matrixpp7i = glm::scale(matrixpp7i, glm::vec3(0.5f, 0.2f, 0.1f));
		sphere4.setShader(&shaderLighting);
		sphere4.setProjectionMatrix(projection);
		sphere4.setViewMatrix(view);
		sphere4.render(matrixpp7i);
		//matrixs6
		//esfera superior derecha 
		matrixspp6i = glm::scale(matrixspp6i, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp6i);
		//hemos escalado las esferas a 0.1
		// esfera superior centro 
		matrixspp5i = glm::scale(matrixspp5i, glm::vec3(0.1, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp5i);




		//Estomago Blanco
		glm::mat4 matrixpanzapi = glm::rotate(matrixpp0i, 0.0f, glm::vec3(0.5f, 0.5f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, textureID28);
		matrixpanzapi = glm::translate(matrixpp0i, glm::vec3(0.0f, -0.1f, 0.3f));
		matrixpanzapi = glm::scale(matrixpanzapi, glm::vec3(0.6f, 0.6, 0.6f));
		sphere5.setShader(&shaderLighting);
		sphere5.setProjectionMatrix(projection);
		sphere5.setViewMatrix(view);
		sphere5.render(matrixpanzapi);



		glm::mat4 matrixspp2i = glm::translate(matrixspp1i, glm::vec3(-0.0225f, 0.0f, 0.0f));
		glm::mat4 matrixspp3i = glm::translate(matrixspp1i, glm::vec3(0.0225f, 0.0f, 0.0f));

		//esfera inferior centro torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp1i = glm::scale(matrixspp1i, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixspp1i);

		glm::mat4 matrixpp1i = glm::rotate(matrixspp2i, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp1i = glm::translate(matrixpp1i, glm::vec3(0.0, -0.04, 0.0));

		glm::mat4 matrixpp10i = glm::rotate(matrixspp3i, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp10i = glm::translate(matrixpp10i, glm::vec3(0.0, -0.04, 0.0));

		// movemos hacia la posición que queremos 
		//rod izq
		glm::mat4 matrixspp4i = glm::translate(matrixpp1i, glm::vec3(0.0f, -0.4f, 0.0f));
		//rod der 
		glm::mat4 matrixspp11i = glm::translate(matrixpp10i, glm::vec3(0.0f, -0.4f, 0.0f));
		//posiciona pierna izquierda 
		glm::mat4 matrixpp2i = glm::rotate(matrixspp4i, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp2i = glm::translate(matrixspp4i, glm::vec3(0.0f, -0.3f, 0.0f));
		//posiciona pierna derecha 
		glm::mat4 matrixpp12i = glm::rotate(matrixspp11i, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrixpp12i = glm::translate(matrixspp11i, glm::vec3(0.0f, -0.3f, 0.0f));

		//rodilla derecha
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp11i = glm::rotate(matrixspp11i, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixspp11i = glm::scale(matrixspp11i, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp11i);
		//rodilla izquierda
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp4i = glm::rotate(matrixspp4i, 1.5708f, glm::vec3(0.0f, 0.0f, 0.0f));
		matrixspp4i = glm::scale(matrixspp4i, glm::vec3(0.2f, 0.3f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp4i);


		//parte 1 pierna derecha
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpp10i = glm::scale(matrixpp10i, glm::vec3(0.2f, 0.4f, 0.3f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixpp10i);

		//parte 1 pierna izquierda 
		glBindTexture(GL_TEXTURE_2D, textureID30);
		matrixpp1i = glm::scale(matrixpp1i, glm::vec3(0.2f, 0.4f, 0.2f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.render(matrixpp1i);
		//Esfera izquierda torso 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp2i = glm::scale(matrixspp2i, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp2i);
		//Esfera derecha inferior torso 
		//Esto permite hacer las esferas mas grandes , es decir escalarlas 
		glBindTexture(GL_TEXTURE_2D, textureID29);
		matrixspp3i = glm::scale(matrixspp3i, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixspp3i);

		 

		//**********************************************************************
	

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

		/* Agua */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWater);
		water.setShader(&shaderLighting);
		water.setProjectionMatrix(projection);
		water.setViewMatrix(view);
		water.setPosition(glm::vec3(-10.4f, 0.5f, -8.4f));
		/* (x, y, z)*/
		water.setScale(glm::vec3(8.0f, 2.0f, 8.0f));
		/* Offset de textura */
		water.offsetUVS(glm::vec2(0.01, 0.01));
		water.render();

		if (angle > 2 * M_PI) 
			angle = 0.0;
		else
			angle += 0.001;


		/* Hielo */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHielo);
		hielo.setShader(&shaderLighting);
		hielo.setProjectionMatrix(projection);
		hielo.setViewMatrix(view);
		hielo.setPosition(glm::vec3(-10.4f, 2.7f, -8.4f));
		/* (x, y, z)*/
		hielo.setScale(glm::vec3(3.0f, 0.2f, 3.0f));
		/* Offset de textura */
		hielo.offsetUVS(glm::vec2(0.001, 0.001));
		hielo.render();

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
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();


		fence.setPosition(glm::vec3(7.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(10.0f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(12.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(15.0f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(17.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();


		fence.setPosition(glm::vec3(-2.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-5.0f, -0.1f, 18.0));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-7.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-10.0f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-12.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-15.0f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-17.5f, -0.1f, 18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();

		/* Detras*/
		fence.setPosition(glm::vec3(1.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(2.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(5.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(7.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(10.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(12.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(15.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(17.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
	
		fence.setPosition(glm::vec3(-1.0f, -0.1f, -18.0));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-2.0f, -0.1f, -18.0));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-5.0f, -0.1f, -18.0));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-7.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-10.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-12.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-15.0f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();
		fence.setPosition(glm::vec3(-17.5f, -0.1f, -18.0f));
		fence.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		fence.render();

		/*Laterales izquierdo */
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 1.0));
		fence.setOrientation(glm::vec3(180.0f,90.0f,180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 2.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 5.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 7.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 10.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 12.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 15.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 17.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();


		fence.setPosition(glm::vec3(-18.0f, -0.1f, -1.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -2.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -5.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -7.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -10.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -12.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, 15.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(-18.0f, -0.1f, -17.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();

		/*Laterales derecho */
		/*Laterales izquierdo */
		fence.setPosition(glm::vec3(19.0f, -0.1f, 2.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 3.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 5.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 7.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 10.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 12.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 15.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 17.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();


		fence.setPosition(glm::vec3(19.0f, -0.1f, -2.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -3.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -5.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -7.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -10.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -12.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, 15.0));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
		fence.render();
		fence.setPosition(glm::vec3(19.0f, -0.1f, -17.5));
		fence.setOrientation(glm::vec3(180.0f, 90.0f, 180.0f));
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

		/*Tazas*/

		/*baseT.setShader(&shaderLighting);
		baseT.setProjectionMatrix(projection);
		baseT.setViewMatrix(view);
		baseT.setPosition(glm::vec3(12.0f, -0.698f, 5.0f));
		baseT.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
		baseT.render();*/

		/*taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);
		taza1.setPosition(glm::vec3(12.0f, -0.698f, 5.0f));
		taza1.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
		taza1.render();*/
		//Animacion taza//



		//glm::mat4 = model(1.0f);
		glm::mat4 model(1.0);
		glm::mat4 model2(1.0);
		glm::mat4 model3(1.0);
		girot += 5.0f * deltaTime;

		carpaT.setShader(&shaderLighting);
		carpaT.setProjectionMatrix(projection);
		carpaT.setViewMatrix(view);
		//carpaT.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, -0.698f, 5.0f));
		model2 = model;
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		carpaT.render(model);



		baseT.setShader(&shaderLighting);
		baseT.setProjectionMatrix(projection);
		baseT.setViewMatrix(view);
		//baseT.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::mat4(1.0);
		model = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
		model3 = model;
		model3 = model = glm::rotate(model, girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		baseT.render(model);

		//Taza1
		taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model3, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 9 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		taza1.render(model);

		//Taza2
		taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model3, glm::vec3(2.0f, 0.0f, -0.75f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 9 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		taza1.render(model);

		//Taza3
		taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model3, glm::vec3(0.5f, 0.0f, 1.75f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 9 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		taza1.render(model);

		//Taza4
		taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model3, glm::vec3(-0.5f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 13 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		taza1.render(model);

		//Taza5
		taza1.setShader(&shaderLighting);
		taza1.setProjectionMatrix(projection);
		taza1.setViewMatrix(view);;
		model = glm::mat4(1.0);
		model = glm::translate(model3, glm::vec3(-1.75f, 0.0f, 0.25f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 14 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		taza1.render(model);
		
		//coffee shop
		coffeeShop.setShader(&shaderLighting);
		coffeeShop.setProjectionMatrix(projection);
		coffeeShop.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f, -0.698f, 14.5f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		//model = glm::rotate(model, 14 * girot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		coffeeShop.render(model);

		//banca1
		bancaMadera.setShader(&shaderLighting);
		bancaMadera.setProjectionMatrix(projection);
		bancaMadera.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, -0.698f, 13.5f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		bancaMadera.render(model);

		//banca1
		bancaMadera.setShader(&shaderLighting);
		bancaMadera.setProjectionMatrix(projection);
		bancaMadera.setViewMatrix(view);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(16.0f, -0.698f, 15.7f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		bancaMadera.render(model);
		
	


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

	// Initialize the environment
	//alutInit(0, NULL);
	alutInitWithoutContext(NULL, NULL);
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alListener3i(AL_POSITION, 0, 0, 0);
	alListener3i(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	alGenSources((ALuint)4, source);
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1);
	alSource3f(source[0], AL_POSITION, 0, 0, 0);
	alSource3f(source[0], AL_VELOCITY, 0, 0, 0);
	alSourcei(source[0], AL_LOOPING, AL_FALSE);

	alDistanceModel(AL_LINEAR_DISTANCE);
	alSourcei(source[0], AL_MAX_DISTANCE, 100);
	alSourcei(source[0], AL_REFERENCE_DISTANCE, 20);


	alGenBuffers(1, &buffer);

	buffer = alutCreateBufferFromFile(DANCIN);

	alSourcei(source[0], AL_BUFFER, buffer);
	alSourcePlay(source[0]);


	applicationLoop();
	destroy();

	//OpenAL close
	alDeleteSources(1, source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	alutExit();

	return 1;
}


// para point light comenatmos linea 371y 372.