/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture logofiTexture;

//Partes de Coche
Model Cabina_M;
Model Cofre_M;
Model Llanta_derecha_M;
Model Llanta_izquierda_M;
Model Articulacion_M;


//helicoptero
Model Blackhawk_M;
Model helice_M;
Model helice_corta_M;

//lampara
Model lampara_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_POINT_LIGHTS];


//las había declarado directamente pero no lo reconocía bien
/*
PointLight pointLights[5];
SpotLight spotLights[5];

*/

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,  //0,0,0
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();

	//----------------COCHE----------------------------
	Cabina_M = Model();
	Cabina_M.LoadModel("Models/jeep_cabina.obj");

	Cofre_M = Model();
	Cofre_M.LoadModel("Models/jeep_cofre.obj");

	Llanta_derecha_M = Model();
	Llanta_derecha_M.LoadModel("Models/jeep_llanta_derecha.obj");


	Llanta_izquierda_M = Model();
	Llanta_izquierda_M.LoadModel("Models/jeep_llanta_izquierda.obj");

	Articulacion_M = Model();
	Articulacion_M.LoadModel("Models/jeep_articulacion.obj");


	//----------------HELICOPTERO----------------------------

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/prueba.obj");

	helice_M = Model();
	helice_M.LoadModel("Models/prueba-2.obj");

	helice_corta_M = Model();
	helice_corta_M.LoadModel("Models/prueba-4.obj");
	
	//----------------LAMPARA----------------------------
	lampara_M = Model();
	lampara_M.LoadModel("Models/lampara.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);   //0,0,-1

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Ligada al modelo de lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	
	//se crean mas luces puntuales y spotlight 
	//...

	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,  //ligado al coche
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		0.5f, 0.01f, 0.001f,
		15.0f);  //angulo de apertura del cono
	spotLightCount++;

	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,  //ligado al coche
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		0.5f, 0.01f, 0.001f,
		15.0f);  //angulo de apertura del cono
	spotLightCount++;

	// Inicialización de la luz spotlight indexada en 4
	// Ligada al modelo del helicóptero
	spotLights[4] = SpotLight( 1.0f, 1.0f, 0.0f, 		// Color de la luz (RGB)
		0.5f, 0.5f, //Intensidad Ambienteal, Intensidad Difusa
		5.0f, 10.0f, 0.0f, //Posicion
		0.0f, -5.0f, 0.0f, //Direccion
		0.5f, 0.01f, 0.001f, //Atenuacion: constante, lineal, exponencial
		// Ángulo de apertura del cono de la luz spotlight
		15.0f);
	spotLightCount++;

	//Esta luz no se emplea pero tambien se tiene un resultado satisfactorio para la lámpara
	/*
	// Ligada al modelo de lampara
	spotLights[5] = SpotLight(1.0f, 1.0f, 1.0f, 		// Color de la luz (RGB)
		1.0f, 1.0f, //Intensidad Ambienteal, Intensidad Difusa
		5.0f, 10.0f, 6.0f, //Posicion
		0.0f, -1.0f, 0.1f, //Direccion
		0.5f, 0.01f, 0.001f, //Atenuacion: constante, lineal, exponencial
		// Ángulo de apertura del cono de la luz spotlight
		50.0f);
	spotLightCount++;
	*/
	//*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelcar(1.0);
	glm::mat4 modelart_1(1.0);
	glm::mat4 modelart_2(1.0);
	glm::mat4 modelart_3(1.0);
	glm::mat4 modelart_4(1.0);
	glm::mat4 model_helicopter(1.0);
	glm::mat4 model_aspa(1.0);
	glm::mat4 model_lamp(1.0);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		/*
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());   //comentar y ver el resultado
		*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//glm::mat4 model(1.0);
		//glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		

		//------------*INICIA DIBUJO DE COCHE-------------------*
		//Nodo Padre
		// Cabina 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 3.0f, 20.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmover()));
		modelcar = model;
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		color = glm::vec3(0.7f, 0.7f, 0.7f); //modelo de coche de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabina_M.RenderModel();

		// Obtener la dirección de la cabina después de renderizarla
		glm::vec3 cabinaDirection = glm::vec3(0.0f, 0.0f, 1.0f); // Por ejemplo, dirección hacia adelante (0,0,1)

		//LUZ 1 ----- FARO 1
		// Obtener la posición de la cabina después de renderizarla
		glm::vec3 faro_1 = glm::vec3(model[3][0]+3.2f, model[3][1]+2.3f, model[3][2]+5.0f); // <- con esto hacemos que la luz siga al modelo
		// Configurar la posición y la dirección de la luz
		spotLights[2].SetFlash(faro_1, cabinaDirection);

		//LUZ 2 ----- FARO 2
		// Obtener la posición de la cabina después de renderizarla
		glm::vec3 faro_2 = glm::vec3(model[3][0] - 3.2f, model[3][1] + 2.3f, model[3][2] + 5.0f); // <- con esto hacemos que la luz siga al modelo
		// Configurar la posición y la dirección de la luz
		spotLights[3].SetFlash(faro_2, cabinaDirection);


		//Cofre
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.5f, 4.5f, 4.12f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); //color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//cilindro 1
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.8f, -0.2f, 9.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		modelart_1 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11, 0.1, 0.1));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		glm::vec3 colorLlanta = glm::vec3(0.8f, 0.8f, 0.8f);

		//Llanta_1
		model = modelart_1;
		model = glm::translate(model, glm::vec3(-6.5f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_M.RenderModel();


		//Llanta_2
		model = modelart_1;
		model = glm::translate(model, glm::vec3(7.3f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_M.RenderModel();


		//cilindro 2
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.8f, -0.2f, -6.7f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		modelart_2 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11f, 0.1, 0.1));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		//Llanta_3
		model = modelart_2;
		model = glm::translate(model, glm::vec3(-6.5f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_M.RenderModel();

		//Llanta_4
		model = modelart_2;
		model = glm::translate(model, glm::vec3(7.3f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_M.RenderModel();
		
		
	
		//------------*INICIA DIBUJO DE HELICOPTERO-------------------*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, mainWindow.getelevar_helicoptero(), mainWindow.getmover_helicoptero()));
		model_helicopter = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Obtener la dirección de la cabina después de renderizarla
		glm::vec3 HelicopteroDirection = glm::vec3(0.0f, -1.0f, 0.0f); // Por ejemplo, dirección hacia abajo (0,-1,0)

		// Obtener la posición de la cabina después de renderizarla
		glm::vec3 centro = glm::vec3(model[3][0] + 0.0f, model[3][1] + 0.0f, model[3][2] + 0.0f); // <- con esto hacemos que la luz siga al modelo
		// Configurar la posición y la dirección de la luz
		spotLights[4].SetFlash(centro, HelicopteroDirection);
		
		//Aspa Principal
		model = model_helicopter;
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionaspa()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helice_M.RenderModel();

		//Aspa Corta
		model = model_helicopter;
		model = glm::translate(model, glm::vec3(0.35f, -10.5f, 2.3));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionaspa_corta()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helice_corta_M.RenderModel();
		
		
		//----------------LAMPARA----------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.0f, 6.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model_lamp = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara_M.RenderModel();
		
		// Obtener la dirección de la cabina después de renderizarla
		//glm::vec3 lampDir = glm::vec3(0.0f, -1.0f, 0.0f); // Por ejemplo, dirección hacia abajo (0,1,0)
		// Obtener la posición de la cabina después de renderizarla
		glm::vec3 lampPos = glm::vec3(model[3][0] + 0.0f, model[3][1] + 22.0f, model[3][2] + 0.0f); // <- con esto hacemos que la luz siga al modelo
		// Configurar la posición y la dirección de la luz
		pointLights[1].SetPosition(lampPos);


		//------------*INICIA DIBUJO DE Agave-------------------*
		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		//*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
