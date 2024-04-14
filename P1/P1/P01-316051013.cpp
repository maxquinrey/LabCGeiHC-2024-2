/*
E01-316051013
Maximiliano Quiñones Reyes

*/

#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
#include <random>


//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader; //variables de tipo entero sin signo, variables que va a usar el GPU


// Variables para el color con el que comienza la pantalla
float rojo = 0.0f, verde = 0.0f, azul = 0;

// Variables para el cambio de color
int colorActual = 0; // Índice del color actual (rojo)
float tiempoAnterior = 0.0f; // Tiempo anterior para el cambio de color
float tiempoIntervalo = 2.0f; // Intervalo de tiempo en segundos para cambiar de color


//VAO - datos
//VBO -  memoria de video
//shader - cuanto shader vamos a utilizar


//Vertex Shader GLSL 
/*
El shader necesario de posicion xyz
*/
//recibir color, salida Vcolor
/*
mínimo a recibir siempre variable de entrada no necesita de salida
versión
VARIABLE tipo vec3 = "pos"
se convierte en matriz de 4x1
gl_position = vec4(x y z 1 ) -> todo se usa en matrices de 4
*/

static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//FRAGMENT SHADER - procesa toda la información de color, iluminacion y textura
//recibir Vcolor y dar de salida color
//no siempre tiene una variable de entrada pero si de salida

static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";

/*
Hasta aquí sabemos que lo que dibujemos se va a ver de color rojo
*/

//Función para cambiar el color entre diferentes colores RGB
void CambiarColorVentana(int colorAleatorio) {
	switch (colorAleatorio) {
	case 0:
		rojo = 1.0f; // Rojo
		verde = 0.0f;
		azul = 0.0f;
		break;
	case 1:
		rojo = 0.0f;
		verde = 1.0f; // Verde
		azul = 0.0f;
		break;
	case 2:
		rojo = 0.0f;
		verde = 0.0f;
		azul = 1.0f; // Azul
		break;
	case 3:
		rojo = 1.0f;
		verde = 1.0f; // Amarillo
		azul = 0.0f;
		break;
	case 4:
		rojo = 0.0f;
		verde = 1.0f;
		azul = 1.0f; // Cian
		break;
	case 5:
		rojo = 0.5f;
		verde = 0.5f;
		azul = 0.5f; // Gris
		break;
	case 6:
		rojo = 0.0f;
		verde = 0.0f;
		azul = 0.0f; // Negro
		break;
	case 7:
		rojo = 1.0f;
		verde = 0.0f;
		azul = 1.0f; // Magenta
		break;
	case 8:
		rojo = 0.8f;
		verde = 0.8f;
		azul = 0.8f; // Gris claro (anteriormente blanco)
		break;
	case 9:
		rojo = 0.5f;
		verde = 1.0f;
		azul = 0.5f; // Verde claro
		break;
	case 10:
		rojo = 0.2f;
		verde = 0.7f;
		azul = 0.9f; // Azul claro
		break;
	}
}




void CrearTriangulo()
{
	//datos de vértices, importa el acomodo, renglones y columnas
	GLfloat vertices[] = {
		//LETRA M

		//1era PARTE
		-0.5f, 0.0f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.6f,0.4f,0.0f,
		-0.6f,0.0f,0.0f,

		-0.6f, 0.4f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.5f, 0.4f, 0.0f,
		-0.5f,0.0f,0.0f,

		//2da PARTE
		-0.5f, 0.4f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.4f,0.35f,0.0f,
		-0.5f,0.3f,0.0f,

		-0.3f, 0.4f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.4f, 0.35f, 0.0f,
		-0.3f,0.3f,0.0f,

		-0.4f, 0.35f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.5f,0.3f,0.0f,
		-0.3f,0.3f,0.0f,

		-0.5f, 0.3f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.4f, 0.2f, 0.0f,
		-0.3f,0.3f,0.0f,

		//3era PARTE

		-0.2f, 0.0f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.3f,0.0f,0.0f,
		-0.2f,0.4f,0.0f,

		-0.3f, 0.0f,0.0f, //conjuntos de tres datos, coordenadas xyz
		-0.3f, 0.4f, 0.0f,
		-0.2f,0.4f,0.0f,

		//---------------------------------------------------------

		//LETRA Q

		//1era parte
		0.0f, 0.0f, 0.0f,
		-0.1f, 0.0f, 0.0f,
		-0.1f, 0.4f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.4f, 0.0f,
		-0.1f, 0.4f, 0.0f,

		//2da parte
		0.1f, 0.3f, 0.0f,
		0.1f, 0.4f, 0.0f,
		0.0f, 0.3f, 0.0f,

		0.0f, 0.3f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.1f, 0.4f, 0.0f,

		//3era parte
		0.1f, 0.0f, 0.0f,
		0.1f, 0.1f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.1f, 0.1f, 0.0f,


		//4ta parte
		0.2f, 0.0f, 0.0f,
		0.1f, 0.0f, 0.0f,
		0.1f, 0.4f, 0.0f,

		0.2f, 0.0f, 0.0f,
		0.2f, 0.4f, 0.0f,
		0.1f, 0.4f, 0.0f,

		//5ta parte
		0.25f, -0.05f, 0.0f,
		0.2f, 0.1f, 0.0f,
		0.1f, 0.0f, 0.0f,


		//----------------------------------------------------
		//Letra R
		
		//1era parte
		0.3f, 0.0f, 0.0f,
		0.3f, 0.4f, 0.0f,
		0.4f, 0.0f, 0.0f,

		//2da parte
		0.4f, 0.0f, 0.0f,
		0.4f, 0.4f, 0.0f,
		0.3f, 0.4f, 0.0f,
		
		//3era parte
		0.4f, 0.4f, 0.0f,
		0.4f, 0.35f, 0.0f,
		0.55f, 0.4f, 0.0f, 

		//4ta parte
		0.4f, 0.35f, 0.0f,
		0.55f, 0.35f, 0.0f,
		0.55f, 0.4f, 0.0f,

		//5ta parte	
		0.4f, 0.25f, 0.0f,
		0.4f, 0.20f, 0.0f,
		0.55f, 0.25f, 0.0f,

		//6ta parte
		0.4f, 0.2f, 0.0f,
		0.55f, 0.25f, 0.0f,
		0.55f, 0.2f, 0.0f,

		//7pta parte
		0.5f, 0.35f, 0.0f,
		0.5f, 0.25f, 0.0f,
		0.55f, 0.35f, 0.0f,

		//8tva parte
		0.5f, 0.25f, 0.0f,
		0.55f, 0.35f, 0.0f,
		0.55f, 0.25f, 0.0f,

		//9na parte
		0.4f, 0.15f, 0.0f,
		0.45f, 0.2f, 0.0f,
		0.5f, 0.0f, 0.0f,

		//10ma parte
		0.45f, 0.2f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,

		//11va parte
		0.4f, 0.15f, 0.0f,
		0.4f, 0.2f, 0.0f,
		0.45f, 0.2f, 0.0f

	};
	/*
	Ahorita solo son flotantes cuando los reciba SHADER lo va a convertir
	*/
	//Nota para cada VAO necesitamos un VBO
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	//por cada vertexatrribpointer necesitamos un enable
	//Los datos los vamos a ir leyendo de 3 en 3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	//...3 * sizeof(GL_FLOAT)... <- el 3 son el número de columnas
	//...(GLvoid*)0)... <- el 0 es la cantidad de datos a ignorar en este caso si tuvieramos mas columnas es decir 4
	//                     se pondria un 1 para que almacene 3 datos e ignore 1
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //se pone en 0 para el índice 
	glBindVertexArray(0);

}

//NOTA - ADDSHDAER siempre la vamos a tener y no va a cambiar
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

//NOTA - COMPILESHADER siempre la vamos a tener y no va a cambiar
void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1: Introduccion a OpenGL", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tríangulo
	CrearTriangulo();
	CompileShaders(); //vertexshader y fragmentshader

	// Imprimir información de OpenGL
	printf("Version de Opengl: %s \n", glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//control del tiempo en que cambia el color
		float tiempoActual = glfwGetTime();
		float tiempoDelta = tiempoActual - tiempoAnterior;

		if (tiempoDelta >= tiempoIntervalo) {
			std::random_device rd;
			std::uniform_int_distribution<int> dist(0, 10);
			int colorAleatorio = dist(rd);
			CambiarColorVentana(colorAleatorio);
			tiempoAnterior = tiempoActual;
		}

		//Limpiar la ventana
		glClearColor(rojo, verde, azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
			

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 300); //empieza en el vértice "0" y utiliza "12" datos

		//limpiar una vez que se dibujo
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}

	return 0;
}