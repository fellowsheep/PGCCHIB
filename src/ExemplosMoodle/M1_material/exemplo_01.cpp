#include <glad/glad.h> // Carregamento dos ponteiros para funções OpenGL
#include <GLFW/glfw3.h> // GLFW biblioteca para interface com SO (janela, mouse, teclado, ...)
#include <iostream>      // biblioteca padrão C para I/O

using namespace std;

int main () {
  // 1 - Inicialização da GLFW
  if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }
  // 1.1 - Necessário para Apple OS X
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 2 - Criação do contexto gráfico (window)
  GLFWwindow* window = glfwCreateWindow (640, 480, "Buffers - Dados e Layout", NULL, NULL);
  if (!window) {
    fprintf (stderr, "*** ERRO: não foi possível abrir janela com a GLFW\n");
    // 2.0 - Se não foi possível iniciar GLFW, então termina / remove lib GLFW da memória.
    glfwTerminate();
    return 1;
  }
  // 2.1 - Torna janela atual como o contexto gráfico atual para desenho
  glfwMakeContextCurrent (window);

  // 3 - GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

  // 4 - Objetivo do exemplo: detectar a versão atual do OpenGL e motor de renderização.
  const GLubyte* renderer = glGetString (GL_RENDERER);
  const GLubyte* version = glGetString (GL_VERSION);
  printf ("\n** Verificação da OpenGL ************\n");
  printf ("\tRenderer: %s\n", renderer);
  printf ("\tVersão suportada da OpenGL %s\n\n", version);

  /************** 
   * 5 - AQUI VEM TODO O CÓDIGO DA APP GRÁFICA, PRINCIPALMENTE, O MAIN LOOP
   **************/

  // 5.1 - Geometria da cena
  GLfloat vertices[] = {
    -0.9,  0.9, 0.0, // primeiro triângulo // +-----+
     0.9,  0.9, 0.0,                       // | \ 1 |
     0.9, -0.9, 0.0,                       // |  \  |
     0.9, -0.9, 0.0, // segundo triângulo  // | 2 \ |
    -0.9, -0.9, 0.0,                       // +-----+
    -0.9,  0.9, 0.0,                      
  };

  GLuint vboVert; // identificador de vértices
  glGenBuffers(1, &vboVert);
  glBindBuffer(GL_ARRAY_BUFFER, vboVert);
  // 5.1.1 - Comando que passa dados de vértices da memória da CPU -> memória da GPU 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLfloat colors[] = { // cores como floats, valores entre [0..1]
     0.0,  0.0, 1.0, // primeiro triângulo // +-----+
     0.0,  1.0, 1.0,                       // | \ 1 |
     0.0,  1.0, 0.0,                       // |  \  |
     0.0,  1.0, 0.0, // segundo triângulo  // | 2 \ |
     1.0,  0.0, 0.0,                       // +-----+
     0.0,  0.0, 1.0,                      
  };

  GLuint vboCor; // identificador de vértices
  glGenBuffers(1, &vboCor);
  glBindBuffer(GL_ARRAY_BUFFER, vboCor);
  // 5.1.2 - Comando que passa dados de cores da memória da CPU -> memória da GPU 
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  // 5.2 - Objeto de propriedades / layout
  // como variável foi criada dentro do método main, este é o seu escopo!
  GLuint vao; 
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 5.2.1 - Vínculo entre áreas de dados e layout de leitura de memória na GPU
  // 5.2.1.1 - Vinculando layout de leitura dos vértices
  glBindBuffer(GL_ARRAY_BUFFER, vboVert); // identifica vbo atual
  //   habilitado primeiro atributo do vbo (bind atual)
  glEnableVertexAttribArray(0);
  //   associação do vbo atual com primeiro atributo
  //   0 (primeiro 0) identifica que o primeiro atributo está sendo definido
  //   3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float, 
  //   começando no primeiro byte (último 0).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // 5.2.1.2 - Vinculando layout de leitura das cores
  glBindBuffer(GL_ARRAY_BUFFER, vboCor); // identifica vbo atual
  glEnableVertexAttribArray(1);          // agora utilizamos o próx layout disponível, 1
  // 1 indica que estamos definindo o segundo atributo de layout de memória
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // 5.3 - Definição dos shaders para renderização
  // 5.3.1 - Vertex shader é o estagio de entrada, é ele quem recebe os dados da GPU:
  const char* vertex_shader =
      "#version 410\n"
      "layout(location=0) in vec3 vp;"    // layout 0 foi utilizado para receber vec3 do vértice
      "layout(location=1) in vec3 vc;"    // layout 1 foi utilizado para receber vec3 da cor
      "out vec3 color;"                   // todos os estágios tem: entra -> proc -> saída, color é a saída para ao FS
      "void main () {"
      "   color = vc;"                    // a cor vc associada ao respectivo vértice vp é propagada para o FS
      "	  gl_Position = vec4 (vp, 1.0);"  //objetivo do VS, posicionar o vértice vp no sistema de referência do universo
      "}";

  // 5.3.2 - é o estágio de saída do pipeline que podemos intervir
  const char* fragment_shader =
      "#version 410\n"
      "in vec3 color;"                    // recebe a cor saída do estágio anterior VS
      "out vec4 frag_color;"
      "void main () {"
      "	 frag_color = vec4 (color, 1.0);" // objetivo do FS, determinar a cor do fragmento
      "}";

  // 5.4 - Compilação e "linkagem" dos shaders num shader programm
  // identifica vs e o associa com vertex_shader
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  int params = -1;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		// print_shader_info_log(vs);
    glfwTerminate();
		return 1; // or exit or something
	}

  // identifica fs e o associa com fragment_shader
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		// print_shader_info_log(fs);
    glfwTerminate();
		return 1; // or exit or something
	}

  // identifica do programa, adiciona partes e faz "linkagem"
  GLuint shader_programm = glCreateProgram ();
  glAttachShader(shader_programm, fs);
  glAttachShader(shader_programm, vs);
  glLinkProgram(shader_programm);
  
  glGetProgramiv(shader_programm, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
				shader_programm);
				// print_programme_info_log( shader_programm);
    glfwTerminate();
		return 1;
	}

  // 5.4 - Finalmente, loop de desenho. Note que até o momento pipeline não foi utilizado!
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  while (!glfwWindowShouldClose (window)) {
    glClear (GL_COLOR_BUFFER_BIT);

    // Define shader_programme como o shader a ser utilizado
    glUseProgram (shader_programm);

    // Define vao como verte array atual 
    glBindVertexArray (vao);
    // 5.4.1 - Este comando dispara a execução do pipeline na GPU
    // desenha pontos a partir do vértice 0 até 6
    glDrawArrays (GL_TRIANGLES, 0, 6);

    // 5.4.2 - Este comando faz controle double buffering, obtendo imagem do framebuffer 
    //         gerado pela OpenGL para renderização no contexto gráfico (window).
    glfwSwapBuffers (window);

    // Processa eventos da GLFW
    glfwPollEvents();
  }

  // 6 - Ao final, terminar / remover GLFW da memória.
  glfwTerminate();
  return 0;
}
