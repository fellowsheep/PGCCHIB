
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h> // Carregamento dos ponteiros para funções OpenGL

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const GLint WIDTH = 800, HEIGHT = 600;
glm::mat4 matrix = glm::mat4(1);

bool load_texture (const char* file_name, GLuint* tex) {
    int x, y, n;
    int force_channels = 4;
    glEnable(GL_TEXTURE_2D);
    unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        fprintf (stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // NPOT check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        fprintf (
                 stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
                 );
    }
//    int width_in_bytes = x * 4;
//    unsigned char *top = NULL;
//    unsigned char *bottom = NULL;
//    unsigned char temp = 0;
//    int half_height = y / 2;
//    for (int row = 0; row < half_height; row++) {
//        top = image_data + row * width_in_bytes;
//        bottom = image_data + (y - row - 1) * width_in_bytes;
//        for (int col = 0; col < width_in_bytes; col++) {
//            temp = *top;
//            *top = *bottom;
//            *bottom = temp;
//            top++;
//            bottom++;
//        }
//    }
    
    glGenTextures (1, tex);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, *tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap (GL_TEXTURE_2D);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    GLfloat max_aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
    // set the maximum!
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
    return true;
}




void mouse(double mx, double my) {
    double dx = mx - WIDTH / 2;
    double dy = my - HEIGHT / 2;
    
    matrix = glm::translate(glm::mat4(1), glm::vec3(dx, dy, 0));
}


int main() {
    
    glfwInit();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#pragma region Basic Setup
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ORTHO + MOUSE + TEXTURE", nullptr, nullptr);
    
    
    
    if (window == nullptr) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    // GLAD: carrega todos os ponteiros d funções da OpenGL
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
        "layout ( location = 2 ) in vec2 vTexture;"
        "uniform mat4 proj;"
        "uniform mat4 matrix;"
        "out vec2 text_map;"
        "out vec3 color;"
        "void main() {"
        "    color = vColor;"
        "    text_map = vTexture;"
        "    gl_Position = proj * matrix * vec4 ( vPosition, 1.0);"
        "}";
    
    const char* fragment_shader =
        "#version 410\n"
        "in vec2 text_map;"
        "in vec3 color;"
        "uniform sampler2D basic_texture;"
        "out vec4 frag_color;"
        "void main(){"
        "   frag_color = texture(basic_texture, text_map);"
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
        // Positions                             // Colors           // texture map
        WIDTH * 0.35f, HEIGHT * 0.65f, +0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        WIDTH * 0.50f, HEIGHT * 0.35f, +0.0f,    1.0f, 0.0f, 0.0f,   0.5f, 0.0f,
        WIDTH * 0.65f, HEIGHT * 0.65f, +0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    };

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    
    
    GLuint VAO, VBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid *)(3*sizeof(float)) );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid *)(6*sizeof(float)) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );

    GLuint tex;
    load_texture ("../src/ExemplosMoodle/M4_material/icon-unisinos.png", &tex);

    
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
        
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        
        glUseProgram (shader_programme);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));

        glBindVertexArray( VAO );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);
        glDrawArrays( GL_TRIANGLES, 0, 3);
        glBindVertexArray( 0 );
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
