#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Constantes
const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint ROWS = 6, COLS = 4;
const GLuint QUAD_WIDTH = 200, QUAD_HEIGHT = 100;
const float dMax = sqrt(3.0f);

// Estrutura para representar um quadrado
struct Quad {
    vec3 position;
    vec3 dimensions;
    vec3 color;
    bool eliminated = false;
};

// Grid de quadrados
Quad grid[ROWS][COLS];

int iSelected = -1;

// Variáveis de controle do jogo
int roundPoints = 0;
int totalPoints = 0;
bool gameOver = false;

// Protótipos
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void eliminarSimilares(float tolerancia);
GLuint setupShader();
GLuint createQuad();

// Vertex Shader
const GLchar* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(position, 1.0);
}
)";

// Fragment Shader
const GLchar* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 inputColor;
void main() {
    FragColor = inputColor;
}
)";

int main() {
    srand(static_cast<unsigned int>(time(0)));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das Cores 🎨 - Aron", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    GLuint shaderID = setupShader();
    GLuint VAO = createQuad();

    glUseProgram(shaderID);

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint projLoc = glGetUniformLocation(shaderID, "projection");
    GLint modelLoc = glGetUniformLocation(shaderID, "model");

    mat4 projection = ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    // Inicializa grid
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Quad& quad = grid[i][j];
            quad.position = vec3((j * QUAD_WIDTH) + QUAD_WIDTH / 2.0f, (i * QUAD_HEIGHT) + QUAD_HEIGHT / 2.0f, 0.0f);
            quad.dimensions = vec3(QUAD_WIDTH, QUAD_HEIGHT, 1.0f);
            quad.color = vec3((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f);
        }
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        if (iSelected != -1) {
            eliminarSimilares(0.2f);
        }
        gameOver = true;
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                Quad& quad = grid[i][j];
                if (!quad.eliminated) {
                    gameOver = false;
                    mat4 model = mat4(1.0f);
                    model = translate(model, quad.position);
                    model = scale(model, quad.dimensions);

                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
                    glUniform4f(colorLoc, quad.color.r, quad.color.g, quad.color.b, 1.0f);

                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                }
            }
        }
        if (gameOver) {
            cout << "Fim de jogo! Pontos totais: " << totalPoints << endl;
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int x = static_cast<int>(xpos) / QUAD_WIDTH;
        int y = static_cast<int>(ypos) / QUAD_HEIGHT;

        if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
            iSelected = x + y * COLS;
        }
    }
}

GLuint setupShader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cout << "Vertex Shader Error:\n" << infoLog << endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cout << "Fragment Shader Error:\n" << infoLog << endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cout << "Shader Program Linking Error:\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint createQuad() {
    GLuint VAO, VBO;

    GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f,  // Top Left
        -0.5f, -0.5f, 0.0f,  // Bottom Left
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f   // Bottom Right
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void eliminarSimilares(float tolerancia) {
    int x = iSelected % COLS;
    int y = iSelected / COLS;

	if (grid[y][x].eliminated == true){
		cout << "Quadrado ja eliminado!" << endl;
        iSelected = -1;
		return;
	}
    vec3 C = grid[y][x].color;
    grid[y][x].eliminated = true;
    roundPoints = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            vec3 O = grid[i][j].color;
            float d = sqrt(pow(C.r - O.r, 2.0f) + pow(C.g - O.g, 2.0f) + pow(C.b - O.b, 2.0f));
            if ((d / dMax) <= tolerancia) {
                grid[i][j].eliminated = true;
                roundPoints += 5;
            }
        }
    }
    cout << "+" << roundPoints << " Pontos!" << endl;
    cout << "-10 Pontos pela rodada!" << endl;
    totalPoints += roundPoints - 10;
    cout << "Total: " << totalPoints << " Pontos!" << endl;

    iSelected = -1;
}
