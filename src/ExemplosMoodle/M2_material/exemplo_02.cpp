#include <iostream>
#include <cstdio>
#include <string>


#include <glad/glad.h> // Carregamento dos ponteiros para funções OpenGL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
glm::mat4 matrix = glm::mat4(1);

void mouse(double mx, double my) {
    double dx = mx - WIDTH / 2;
    double dy = my - HEIGHT / 2;
    
    matrix = glm::translate(glm::mat4(1), glm::vec3(dx, dy, 0));
}


int main() {
    
    glfwInit();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#pragma region Basic Setup
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ORTHO + MOUSE", nullptr, nullptr);
    
    
    
    if (window == nullptr) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    // 3 - GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}
    
#pragma endregion
    
    const char* vertex_shader =
        "#version 410\n"
        "layout ( location = 0 ) in vec3 vPosition;"
        "layout ( location = 1 ) in vec3 vColor;"
        "uniform mat4 proj;"
        "uniform mat4 matrix;"
        "out vec3 color;"
        "void main() {"
        "    color = vColor;"
        "    gl_Position = proj * matrix * vec4 ( vPosition, 1.0);"
        "}";
    
    const char* fragment_shader =
        "#version 410\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main(){"
        "  frag_color = vec4(color, 1.0f);"
        "}";
    
    int vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    int fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    
    int shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);
    glLinkProgram (shader_programme);
    
    GLfloat vertices[] = {
        // Positions                             // Colors
        WIDTH * 0.25f, HEIGHT * 0.75f, +0.0f,    0.0f, 0.0f, 1.0f,
        WIDTH * 0.50f, HEIGHT * 0.25f, +0.0f,    1.0f, 0.0f, 0.0f,
        WIDTH * 0.75f, HEIGHT * 0.75f, +0.0f,    0.0f, 1.0f, 0.0f,
    };

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    
    
    GLuint VAO, VBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (GLvoid *)(3*sizeof(float)) );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        const int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS) {
            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            mouse(mx, my);
        }

#pragma region Input Handling
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        
#pragma endregion
        
        glClearColor(0.4f, 0.65f, 0.8f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        
        // PASSAGEM DE PARÂMETROS PRA SHADERS
        glUseProgram (shader_programme);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));

        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3);
        glBindVertexArray( 0 );
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
