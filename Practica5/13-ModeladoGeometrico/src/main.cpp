//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
Box box;

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader shader;

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
float camaraX = 0.3f;
float camaraY = 0.0f;
float camaraZ = 0.5f;
int lastMousePosX;
int lastMousePosY;

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

	shader.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	sphere.init();
	sphere.setShader(&shader);
	sphere.setColor(glm::vec3(0.3, 0.3, 1.0));

	cylinder.init();
	cylinder.setShader(&shader);
	cylinder.setColor(glm::vec3(3.0, 2.0, 1.0));


	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec3(2.2, 1.0, 1.5));


	cylinder3.init();
	cylinder3.setShader(&shader);
	cylinder3.setColor(glm::vec3(2.2 ,1.0, 0.5));

	box.init();
	box.setShader(&shader);
	box.setColor(glm::vec3(3.2, 3.8, 3.4));

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();
	sphere.destroy();
	cylinder.destroy();
	cylinder2.destroy();
	cylinder3.destroy();
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

		case GLFW_KEY_A:
			camaraX = camaraX-1;
			break;

		case GLFW_KEY_D:
			camaraX = camaraX + 1;
			break;

		case GLFW_KEY_W:
			camaraY = camaraY + 1;
			break;

		case GLFW_KEY_S:
			camaraY = camaraY - 1;
			break;


		case GLFW_KEY_N:
			camaraZ = camaraY +6;
			break;

		case GLFW_KEY_F:
			camaraZ= camaraY -5;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	
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
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//matriz de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		//matriz de vista 
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f+camaraX, 0.0f+camaraY, -8.0f+camaraZ));
		//matriz con diagonal cero , la mat0 se translada 
		//matriz0 del cilindro del torso
		glm::mat4 matrix0 = glm::mat4(1.0f);
		//se coloca el   torso en la coordenada 0,0,-1
		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, -1.0f));
		//con msts1 se  dibuja la primer esfera , se coloca -0.5 unidade en el eje y debajo del torso      
		glm::mat4 matrixs1 = glm::translate(matrix0, glm::vec3(0.0f, -0.5f, 0.0f));
		//se escala el cilindro del torso , es decir en el eje 0.6x se hace  mas pequeño mas angosto , pero largo
		glm::mat4 matrixs5 = glm::translate(matrix0, glm::vec3(0.0f, 0.5f, 0.0f));
		glm::mat4 matrixs6 = glm::translate(matrixs5, glm::vec3(0.3f, 0.0f, 0.0f));
		glm::mat4 matrix7 = glm::rotate(matrixs6, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixs8 = glm::translate(matrixs5, glm::vec3(-0.3f, 0.0f, 0.0f));
		glm::mat4 matrix9 = glm::rotate(matrixs8, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		// se coloca -.5 en x ya que  el brazo esta en -.25 
		glm::mat4 matrixs15 = glm::translate(matrix9, glm::vec3(-.5f, 0.0f, 0.0f));
		// posicón codo derecho 
		glm::mat4 matrixs16 = glm::translate(matrix7, glm::vec3(.5f, 0.0f, 0.0f));
		glm::mat4 matrix17 = glm::rotate(matrixs15, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrix18= glm::rotate(matrixs16, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrix19 = glm::rotate(matrixs5, 0.0f, glm::vec3(0.0f, 1.0f, -1.0f));

	
	
		//cabeza
		matrix19 = glm::translate(matrix19, glm::vec3(0.0f, 0.5f, 0.0f));
		matrix19 = glm::scale(matrix19, glm::vec3(0.4f, 0.9, 0.3f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix19);
		// segunda parte del brazo derecho
		matrix18 = glm::translate(matrix18, glm::vec3(0.25f, 0.0f, 0.0f));
		matrix18 = glm::scale(matrix18, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix18);
		// segunda parte del brazo izquierdo
		matrix17 = glm::translate(matrix17, glm::vec3(-0.25f, 0.0f, 0.0f));
		matrix17 = glm::scale(matrix17, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix17);
		//codo der
		matrixs16 = glm::scale(matrixs16, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs16);
		//codo izq
		matrixs15 = glm::scale(matrixs15, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs15);


		//brazo izquierdo
		//matrix9
		matrix9 = glm::translate(matrix9, glm::vec3(-0.25f, 0.0f, 0.0f));
		matrix9 = glm::scale(matrix9, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix9);
		//esfera superior izquierda torso 
		matrixs8 = glm::scale(matrixs8, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs8);
		//brazo derecho 
		//matrix7
		matrix7 = glm::translate(matrix7, glm::vec3(0.25f, 0.0f, 0.0f));
		matrix7 = glm::scale(matrix7, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix7);
		//matrixs6
		//esfera superior derecha 
		matrixs6 = glm::scale(matrixs6, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs6);
		//hemos escalado las esferas a 0.1
		// esfera superior centro 
		matrixs5 = glm::scale(matrixs5, glm::vec3(0.1, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs5);

		//torso 
		matrix0 = glm::scale(matrix0, glm::vec3(0.8f, 1.0f, 0.6f));
		cylinder2.setProjectionMatrix(projection);
		cylinder2.setViewMatrix(view);
		cylinder2.enableWireMode();
		cylinder2.render(matrix0);



		glm::mat4 matrixs2 = glm::translate(matrixs1, glm::vec3(-0.225f, 0.0f, 0.0f));
		glm::mat4 matrixs3 = glm::translate(matrixs1, glm::vec3(0.225f, 0.0f, 0.0f));

		//esfera inferior centro torso 
		matrixs1 = glm::scale(matrixs1, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs1);

		glm::mat4 matrix1 = glm::rotate(matrixs2, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix1 = glm::translate(matrix1, glm::vec3(0.0, -0.4, 0.0));

		glm::mat4 matrix10 = glm::rotate(matrixs3, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix10 = glm::translate(matrix10, glm::vec3(0.0, -0.4, 0.0));
		
		// movemos hacia la posición que queremos 
		//rod izq
		glm::mat4 matrixs4 = glm::translate(matrix1, glm::vec3(0.0f, -0.4f, 0.0f));
		//rod der 
		glm::mat4 matrixs11 = glm::translate(matrix10, glm::vec3(0.0f, -0.4f, 0.0f));
		//posiciona pierna izquierda 
		glm::mat4 matrix2 = glm::rotate(matrixs4, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix2 = glm::translate(matrixs4, glm::vec3(0.0f, -0.3f, 0.0f));
		//posiciona pierna derecha 
		glm::mat4 matrix12 = glm::rotate(matrixs11, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix12 = glm::translate(matrixs11, glm::vec3(0.0f, -0.3f, 0.0f));
		// los pies se realizan recibiendo  la parte anterior en este caso la parte inferior
		// de la pierna 
		//pie izq 
		glm::mat4 matrixs13 = glm::translate(matrix2, glm::vec3(0.0f, -0.4f, 0.0f));
		//pie izquierdo 
		glm::mat4 matrixs14 = glm::translate(matrix12, glm::vec3(0.0f, -0.4f, 0.0f));



		//pie der
		matrixs14 = glm::scale(matrixs14, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs14);
		//pie izq 
		matrixs13 = glm::scale(matrixs13, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs13);
		// hicmos la segunda parte de la pierna derecha
		matrix12 = glm::scale(matrix12, glm::vec3(0.15f, 0.6f, 0.1f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix12);
		// hicmos la segunda parte de la pierna  izquierda
		matrix2 = glm::scale(matrix2, glm::vec3(0.15f, 0.6f, 0.1f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix2);
		//rodilla derecha
		matrixs11 = glm::scale(matrixs11, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs11);
		//rodilla izquierda
		matrixs4 = glm::scale(matrixs4, glm::vec3(0.2f, 0.2f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs4);


		//parte 1 pierna derecha
		matrix10 = glm::scale(matrix10, glm::vec3(0.15f, 0.8f, 0.15f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.enableWireMode();
		cylinder3.render(matrix10);

		//parte 1 pierna izquierda 
		matrix1 = glm::scale(matrix1, glm::vec3(0.20f, 0.8f, 0.20f));
		cylinder3.setProjectionMatrix(projection);
		cylinder3.setViewMatrix(view);
		cylinder3.enableWireMode();
		cylinder3.render(matrix1);
		//Esfera izquierda torso 
		matrixs2 = glm::scale(matrixs2, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs2);
		//Esfera derecha inferior torso 
		//Esto permite hacer las esferas mas grandes , es decir escalarlas 
		matrixs3 = glm::scale(matrixs3, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs3);

		

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}