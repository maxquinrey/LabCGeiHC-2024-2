//Práctica 2 semestre 2024-2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
/*/
Se va a tener una lista de cada mesh
Se crea un objeto y se mandan los datos
*/
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { //se estan reutilizando los vértices que se tienen abajo
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1); //para guardar el dato en nuestra lista
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = { //cada cara esta conformada de 3 triángulos
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = { //se declaran los 8 vérticies correspondientes
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36); //se multiplican 8 vertices x 3 columnas =24
													  
meshList.push_back(cubo);
}

void CrearLetrasyFiguras() //se agregan las letras creadas en la práctica 1
{
	//rojo casa
	GLfloat vertices_cuadrorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadrorojo = new MeshColor();
	cuadrorojo->CreateMeshColor(vertices_cuadrorojo, 36);
	meshColorList.push_back(cuadrorojo);

	//verde ventana
	GLfloat vertices_ventanaverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* ventanaverde = new MeshColor();
	ventanaverde->CreateMeshColor(vertices_ventanaverde, 36);
	meshColorList.push_back(ventanaverde);

	// Cuadrado Café
	GLfloat vertices_cuadrado_cafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
	};

	MeshColor* cuadrado_cafe = new MeshColor();
	cuadrado_cafe->CreateMeshColor(vertices_cuadrado_cafe, 36); 
	meshColorList.push_back(cuadrado_cafe);

	//verde arbol
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.0f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.0f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.0f,			0.0f,	0.5f,	0.0f,
	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	//azul techo
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	/*
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*//*
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);


	*/


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//CrearCubo(); //índice 0 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{


		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


        // Renderizar los cuadros verdes
        shaderList[1].useShader(); // Usar el shader adecuado
        glUniformMatrix4fv(shaderList[1].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));

        // Renderizar cuadro verde 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.25f, -0.05f, -3.0f));
        model = glm::scale(model, glm::vec3(0.30f, 0.40f, 0.0f));
        glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[1]->RenderMeshColor(); // Renderizar el cuadro verde 1

        // Renderizar cuadro verde 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.25f, -0.05f, -3.0f));
        model = glm::scale(model, glm::vec3(0.30f, 0.40f, 0.0f));
        glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[1]->RenderMeshColor(); // Renderizar el cuadro verde 2

        // Renderizar cuadro verde 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.79f, -3.0f));
        model = glm::scale(model, glm::vec3(0.30f, 0.40f, 0.0f));
        glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        meshColorList[1]->RenderMeshColor(); // Renderizar el cuadro verde 3

		// Renderizar el cuadro rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, -3.0f)); // Ajusta la posición del cuadro rojo
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, -2.0f)); // Ajusta el tamaño del cuadro rojo
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor(); // Renderizar el cuadro rojo

		// Renderizar el triángulo azul
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.605f, -3.0f)); // Ajusta la posición del triángulo azul
		model = glm::scale(model, glm::vec3(0.6f, 0.35f, 0.0f)); // Ajusta el tamaño del triángulo azul
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor(); // Renderizar el triángulo azul


		// Renderizar el triangulo_verde (hojas_1)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.78f, -0.48f, -3.0f)); // Ajusta la posición del triángulo verde
		model = glm::scale(model, glm::vec3(0.2f, 0.28f, -0.3f)); // Ajusta el tamaño del triángulo verde
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor(); // Renderizar el triángulo verde

		// Renderizar el triangulo_verde (hojas_2)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.78f, -0.48f, -3.0f)); // Ajusta la posición del triángulo verde
		model = glm::scale(model, glm::vec3(0.2f, 0.28f, -0.3f)); // Ajusta el tamaño del triángulo verde
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor(); // Renderizar el triángulo verde

		// Renderizar el cuadro_cafe (tronco_1)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.78f, -0.88f, -3.0f)); // Ajusta la posición del cuadro_cafe 
		model = glm::scale(model, glm::vec3(0.18f, 0.23f, -0.2f)); // Ajusta el tamaño del cuadro_cafe 
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor(); // Renderizar el cuadro_cafe 


		// Renderizar el cuadro_cafe (tronco_2)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.78f, -0.88f, -3.0f)); // Ajusta la posición del cuadro_cafe 
		model = glm::scale(model, glm::vec3(0.18f, 0.23f, -0.2f)); // Ajusta el tamaño del cuadro_cafe 
		glUniformMatrix4fv(shaderList[1].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor(); // Renderizar el cuadro_cafe 

		//glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/