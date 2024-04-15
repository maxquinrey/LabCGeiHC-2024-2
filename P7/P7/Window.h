#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	//Coche
	GLfloat getrotacionLlantas() { return rotacionLlantas; }
	GLfloat getmover() { return mover; }
	GLfloat getrotacionCofre() { return rotacionCofre; }

	//Helicoptero
	GLfloat getrotacionaspa() { return rotacionAspa; }
	GLfloat getmover_helicoptero() { return mover_helicoptero; }
	GLfloat getrotacionaspa_corta() { return rotacionAspa_corta; }
	GLfloat getelevar_helicoptero() { return elevar_helicoptero; }
	
	

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	//Coche
	GLfloat rotacionLlantas, mover, rotacionCofre;
	//Helicoptero
	GLfloat rotacionAspa, rotacionAspa_corta, mover_helicoptero, elevar_helicoptero;
};

