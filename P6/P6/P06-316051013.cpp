/*
Pr�ctica 6: Texturizado
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
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture dado10Texture;

//Dado
Model Dado_M;

//Partes de Coche
Model Cabina_M;
Model Cofre_M;
Model Llanta_derecha_M;
Model Llanta_izquierda_M;
Model Articulacion_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//c�lculo del promedio de las normales para sombreado de Phong
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
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

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};


	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	0.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	0.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	0.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	0.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.5f,  0.34f,		0.0f,	0.0f,	0.0f,	//4
		0.5f, -0.5f,  -0.5f,	0.74f,	0.34f,		0.0f,	0.0f,	0.0f,	//5
		0.5f,  0.5f,  -0.5f,	0.74f,	0.66f,		0.0f,	0.0f,	0.0f,	//6
		0.5f,  0.5f,  0.5f,	    0.5f,	0.66f,		0.0f,	0.0f,	0.0f,	//7

		// back
		-0.5f, -0.5f, -0.5f,	0.99f,  0.34f,		0.0f,	0.0f,	0.0f,	//8
		0.5f, -0.5f, -0.5f,		0.75f,	0.34f,		0.0f,	0.0f,	0.0f,	//9
		0.5f,  0.5f, -0.5f,		0.75f,	0.66f,		0.0f,	0.0f,	0.0f,	//10	
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	0.0f,	//11

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.5f,  0.01f,		0.0f,	0.0f,	0.0f,	//12
		-0.5f, -0.5f,  0.5f,	0.74f,	0.01f,		0.0f,	0.0f,	0.0f,	//13
		-0.5f,  0.5f,  0.5f,	0.74f,	0.33f,		0.0f,	0.0f,	0.0f,	//14
		-0.5f,  0.5f,  -0.5f,	0.5f,	0.33f,		0.0f,	0.0f,	0.0f,	//15

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.01f,  0.66f,		0.0f,	0.0f,	0.0f,	//16
		0.5f,  -0.5f,  0.5f,	0.24f,	0.66f,		0.0f,	0.0f,	0.0f,	//17
		 0.5f,  -0.5f,  -0.5f,	0.24f,	0.34f,		0.0f,	0.0f,	0.0f,	//18
		-0.5f, -0.5f,  -0.5f,	0.01f,	0.34f,		0.0f,	0.0f,	0.0f,	//19

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.51f,	0.67f,		0.0f,	0.0f,	0.0f,	//20
		 0.5f,  0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	0.0f,	0.0f,	//21
		  0.5f, 0.5f,  -0.5f,	0.74f,	1.0f,		0.0f,	0.0f,	0.0f,	//22
		 -0.5f, 0.5f,  -0.5f,	0.51f,	1.0f,		0.0f,	0.0f,	0.0f,	//23

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
}


void CrearDecagono()
{
	unsigned int indices[] = {

		//1
		0, 1, 2,
		2, 3, 0,


		//2
		4, 5, 6,
		6, 7, 4,

		//3
		8, 9, 10,
		10, 11, 8,

		//4
		12, 13, 14,
		14, 15, 12,

		//5
		16, 17, 18,
		18, 19, 16,

		//6
		20, 21, 22,
		22, 23, 20,

		//7
		24, 25, 26,
		26, 27, 24,

		//8
		28, 29, 30,
		30, 31, 28,

		//9
		32, 33, 34,
		34, 35, 32,

		//10
		36, 37, 38,
		38, 39, 36,

		//*/
	};

	GLfloat vertices[] = {
		//	x		y		z
		//1 - 1
		-0.35f,	0.25f,	-0.5f,		0.75f,	0.5f,		0.0f,	0.0f,	0.0f,		//A-0
		-0.56f,	-0.4f,	0.43f,		0.742f,	0.031f,		0.0f,	0.0f,	0.0f,		//D-3 <- se invierten las caras C
		-0.9f,	-0.15f,	0.69f,		0.85f,	0.031f,		0.0f,	0.0f,	0.0f,		//B-1
		-1.03f,	0.25f,	0.43f,		0.92f,	0.22f,		0.0f,	0.0f,	0.0f,		//C-2 <- se invierten las caras D
		/*
		*
		* Por las mediciones estas son las coordenadas que deber�a tener la cara //1 pero
		* al ingresarlas no las reconoce
		* //Solucionado con la modificacion anexando una letra "A" al archivo ".tga"
		* .75 .5
		* .92 .22
		* .85 0.031
		* .742 0.031
		*/

		//2 - 3
		-0.35f,	0.25f,  -0.5f,		0.744f, 0.527f,		0.0f,	0.0f,	0.0f,	//A-4
		-1.03f,	0.25f,  0.43f,		0.923f,	0.234f,		0.0f,	0.0f,	0.0f,	//C-5 <- se invierten las caras F
		-0.9f,	0.65f,  0.69f,		0.996f, 0.427f,		0.0f,	0.0f,	0.0f,	//E-6
		-0.56f,	0.9f,  0.43f,		0.96f,	0.658f,		0.0f,	0.0f,	0.0f,	//F-7 <- se invierten las caras C

		/* MEDIDAS
		* .744 0.527
		* .96 0.658
		* .996 0.427
		* .923 0.234
		*/

		//3 - 9
		-0.35f,	0.25f,  -0.5f,		0.744f, 0.539f,		0.0f,	0.0f,	0.0f,	//A-8
		-0.56f,	0.9f,  0.43f,		0.96f,	0.671f,		0.0f,	0.0f,	0.0f,	//F-9 <- se invierten las caras H
		-0.13f,	0.9f,  0.69f,		0.929f,	0.906f,		0.0f,	0.0f,	0.0f,	//G-10
		0.21f,  0.65f,  0.43f,		0.826f,	0.99f,		0.0f,	0.0f,	0.0f,	//H-11 <- se invierten las caras F

		/* MEDIDAS
		* .744 0.539
		* .826 0.99
		* .929 0.906
		* .96 0.671
		*/

		//4 - 5
		-0.35f,	0.25f,  -0.5f,		0.732f, 0.525f,		0.0f,	0.0f,	0.0f,	//A-12
		0.21f,  0.65f,  0.43f,		0.513f,	0.66f,		0.0f,	0.0f,	0.0f,	//H-13 <- se invierten las caras J
		0.34f,	0.25f,  0.69f,		0.48f,  0.427f,		0.0f,	0.0f,	0.0f,	//I-14
		0.21f,	-0.15f,  0.43f,		0.548f,	0.236f,		0.0f,	0.0f,	0.0f,	//J-15 <- se invierten las caras H

		/* MEDIDAS
		* .732 0.525
		* .548 0.236
		* .48 0.427
		* .513 0.66
		*/

		//5 - 7
		-0.35f,	0.25f,  -0.5f,		0.736f, 0.517f,		0.0f,	0.0f,	0.0f,	//A-16
		0.21f,	-0.15f,  0.43f,		0.552f,	0.226f,		0.0f,	0.0f,	0.0f,	//J-17 <- se invierten las caras D
		-0.13f,	-0.4f,  0.69f,		0.619f,	0.027f,		0.0f,	0.0f,	0.0f,	//K-18
		-0.56f,  -0.4f,  0.43f,		0.734f,	0.027f,		0.0f,	0.0f,	0.0f,	//D-19 <- se invierten las caras J

		/* MEDIDAS
		* .736 0.517
		* .734 0.027
		* .619 0.027
		* .552 0.226
		*/

		//6 - 6
		-0.35f,  0.25f,  1.61f,		0.242f,	0.562f,		0.0f,	0.0f,	0.0f,	//L-20
		-0.9f,	-0.15f,  0.69f,		0.125f,	0.152f,		0.0f,	0.0f,	0.0f,	//B-21 
		-1.03f,	0.25f,  0.43f,		0.027f,	0.277f,		0.0f,	0.0f,	0.0f,	//C-22
		-0.9f,	0.65f,  0.69f,		0.017f,	0.517f,		0.0f,	0.0f,	0.0f,	//E-23 

		/* MEDIDAS
		* 0.242 0.562
		* 0.125 0.152
		* 0.027 0.277
		* 0.017 0.517
		*/

		//7  - 4
		-0.35f,  0.25f,  1.61f,		0.244f,	0.57f,		0.0f,	0.0f,	0.0f,	//L-24
		-0.9f,	0.65f,  0.69f,		0.015f, 0.531f,		0.0f,	0.0f,	0.0f,	//E-25
		-0.56f,	0.9f,  0.43f,		0.005f,	0.775f,		0.0f,	0.0f,	0.0f,	//F-26
		-0.13f,	0.9f,  0.69f,		0.091f,	0.933f,		0.0f,	0.0f,	0.0f,	//G-27

		/* MEDIDAS
		* 0.244 0.57
		* 0.015 0.531
		* 0.005 0.775
		* 0.091 0.933
		*/

		//8 - 10
		-0.35f,  0.25f,  1.61f,		0.255f,	0.57f,		0.0f,	0.0f,	0.0f,	//L-28
		-0.13f,	0.9f,  0.69f,		0.439f,	0.857f,		0.0f,	0.0f,	0.0f,	//G-29
		0.21f,  0.65f,  0.43f,		0.503f,	0.673f,		0.0f,	0.0f,	0.0f,	//H-30
		0.34f,	0.25f,  0.69f,		0.472f, 0.439f,		0.0f,	0.0f,	0.0f,	//I-31

		/* MEDIDAS
		* 0.255 0.57
		* 0.439 0.857
		* 0.503 0.673
		* 0.472 0.439
		*/

		//9 -8
		-0.35f,  0.25f,  1.61f,		0.255f,	0.558f,		0.0f,	0.0f,	0.0f,	//L-32
		0.34f,	0.25f,  0.69f,		0.468f, 0.421f,		0.0f,	0.0f,	0.0f,	//I-33
		0.21f,	-0.15f,  0.43f,		0.439f,	0.187f,		0.0f,	0.0f,	0.0f,	//J-34
		-0.13f,	-0.4f,  0.69f,		0.333f,	0.107f,		0.0f,	0.0f,	0.0f,	//K-35


		/* MEDIDAS
		* 0.255 0.558
		* 0.468 0.421
		* 0.439 0.187
		* 0.333 0.107
		*/

		//10 - 2
		-0.35f,  0.25f,  1.61f,		0.248f,	0.554f,		0.0f,	0.0f,	0.0f,	//L-36
		-0.13f,	-0.4f,  0.69f,		0.328f,	0.099f,		0.0f,	0.0f,	0.0f,	//K-37
		-0.56f,  -0.4f,  0.43f,		0.224f,	0.011f,		0.0f,	0.0f,	0.0f,	//D-38
		-0.9f,	-0.15f,  0.69f,		0.126f,	0.146f,		0.0f,	0.0f,	0.0f,	//B-39
		//*/

		/* MEDIDAS
		* 0.248 0.554
		* 0.328 0.099
		* 0.224 0.011
		* 0.126 0.146
		*/
	};

	// Crear el objeto de malla para el dec�gono
	Mesh* decagono = new Mesh();
	decagono->CreateMesh(vertices, indices, 42*8, 22*3); // 
	meshList.push_back(decagono);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearDecagono();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales_recorte.tga");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	
	dado10Texture = Texture("Textures/dado-10caras.tga");
	dado10Texture.LoadTextureA();
	
	//----------------DADO-----------------------------
	Dado_M = Model();
	Dado_M.LoadModel("Models/dado_animales.obj");

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



	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelcar (1.0);
	glm::mat4 modelart_1(1.0);
	glm::mat4 modelart_2(1.0);
	glm::mat4 modelart_3(1.0);
	glm::mat4 modelart_4(1.0);

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		
		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();
		

		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes
		
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
	
		
		//*/

		/*Reporte de pr�ctica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de c�digo
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y 
		texturizar el logo de la Facultad de ingenier�a en el cofre de su propio modelo de coche
		*/
		
		//DADO de OpenGL - 10 CARAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 6.5f, -2.0f));
		model = glm::scale(model, glm::vec3(5.0, 5.0, 5.0));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Texture.UseTexture();
		//glBindTexture(GL_TEXTURE_2D, 0); // Desactivar textura
		meshList[5]->RenderMesh();
		//*/
		
		

		//------------*INICIA DIBUJO DE COCHE-------------------*
		//Nodo Padre
		// Cabina 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 2.5f, 20.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmover()));
		modelcar = model;
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		color = glm::vec3(0.7f, 0.7f, 0.7f); //modelo de coche de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabina_M.RenderModel();


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
		model = glm::translate(model, glm::vec3(-0.8f, -1.0f, 9.0f));
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
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_M.RenderModel();

		
		//Llanta_2
		model = modelart_1;
		model = glm::translate(model, glm::vec3(7.3f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_M.RenderModel();
		

		//cilindro 2
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.8f, -1.0f, -6.7f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		modelart_2 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11f, 0.1, 0.1));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		//Llanta_3
		model = modelart_2;
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_M.RenderModel();

		//Llanta_4
		model = modelart_2;
		model = glm::translate(model, glm::vec3(7.3f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_M.RenderModel();

		//*/
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/