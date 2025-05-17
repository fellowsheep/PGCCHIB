// Leonardo Meinerz Ramos

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Exercicio Modulo 2 - Triangulos - Leonardo Ramos"

GLuint createShader(GLchar *shaderSource, GLenum shaderType);
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2);
GLuint createSmallTriangle(float x, float y);
int createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

void setupGlConfiguration();
void setupGlad();
void setViewportDimensions(GLFWwindow *window);
void assertProgramLinkingStatus(GLuint shaderProgram);
void assertShaderCompilationStatus(GLuint shader);
void cursorMoveCallback(GLFWwindow *window, double xpos, double ypos);
void cursorClickCallback(GLFWwindow *window, int button, int action, int mods);

int createShaderProgram();
GLFWwindow *makeWindow(int width, int height, const char *title);

float mouseX, mouseY;

struct Triangle
{
    int vertexNumber = 0;
    float xVertices[3];
    float yVertices[3];
    bool isReady = false;
};
void drawTriangle(GLuint VAO, GLint colorLoc, float r, float g, float b);

Triangle currentTriangle = Triangle();
int currentTriangleVaoIndex = 0;
GLuint triangleVAOs[5];

void addVertices(Triangle &triangle, float x, float y)
{
    if (triangle.vertexNumber < 3)
    {
        triangle.xVertices[triangle.vertexNumber] = x;
        triangle.yVertices[triangle.vertexNumber] = y;
        triangle.vertexNumber++;
    }
    if (triangle.vertexNumber == 3)
    {
        triangle.isReady = true;
    }
}

void addToTrianglesIfReady(Triangle &triangle)
{
    if (triangle.isReady)
    {
        triangleVAOs[currentTriangleVaoIndex] = createTriangle(currentTriangle.xVertices[0], currentTriangle.yVertices[0],
                                                               currentTriangle.xVertices[1], currentTriangle.yVertices[1],
                                                               currentTriangle.xVertices[2], currentTriangle.yVertices[2]);
        currentTriangleVaoIndex++;
        if (currentTriangleVaoIndex >= 5)
        {
            currentTriangleVaoIndex = 0;
        }
        currentTriangle = Triangle();

    }
}

int main()
{
    glfwInit();

    setupGlConfiguration();

    GLFWwindow *window = makeWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorMoveCallback);
    glfwSetMouseButtonCallback(window, cursorClickCallback);

    setupGlad();
    setViewportDimensions(window);

    GLuint shaderId = createShaderProgram();
    GLint colorLoc = glGetUniformLocation(shaderId, "inputColor");
    glUseProgram(shaderId);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        for (int i = 0; i < 5; i++)
        {
            // std::cout << "Triangle VAO " << i << ": " << triangleVAOs[i] << std::endl;
            drawTriangle(triangleVAOs[i], colorLoc, 1.0f / i, 0.4f / i, 1.0f);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void drawTriangle(GLuint VAO, GLint colorLoc, float r, float g, float b)
{
    glBindVertexArray(VAO);
    glUniform4f(colorLoc, r, g, b, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2)
{
    GLfloat vertices[] = {
        x0, y0, 0.0f,
        x1, y1, 0.0f,
        x2, y2, 0.0f};

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

GLuint createShader(GLchar *shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    assertShaderCompilationStatus(shader);
    return shader;
}

void assertShaderCompilationStatus(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;

        exit(1);
    }
}

int createShaderProgram()
{
    GLuint vertexShader = createShader(R"(
        #version 400
        layout (location = 0) in vec3 position;
        void main()
        {
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
        )",
                                       GL_VERTEX_SHADER);

    GLuint fragmentShader = createShader(R"(
        #version 400
        uniform vec4 inputColor;
        out vec4 color;
        void main()
        {
            color = inputColor;
        }
        )",
                                         GL_FRAGMENT_SHADER);

    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    assertProgramLinkingStatus(shaderProgram);
    return shaderProgram;
}

void assertProgramLinkingStatus(GLuint shaderProgram)
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
}

void setViewportDimensions(GLFWwindow *window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

GLFWwindow *makeWindow(int width, int height, const char *title)
{
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return window;
}
void setupGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void setupGlConfiguration()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
}

void cursorMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
}

void cursorClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Left mouse button clicked!" << std::endl;
        std::cout << "Mouse position: (" << mouseX << ", " << mouseY << ")" << std::endl;
        addVertices(currentTriangle, (mouseX/800 *2 -1), -(mouseY/600 * 2 -1));
        // print current triangle position
        std::cout << "Current triangle position: (" << currentTriangle.xVertices[0] << ", " << currentTriangle.yVertices[0] << ", "
                  << currentTriangle.xVertices[1] << ", " << currentTriangle.yVertices[1] << ", "
                  << currentTriangle.xVertices[2] << ", " << currentTriangle.yVertices[2] << ")" << std::endl;
        addToTrianglesIfReady(currentTriangle);
        for(int i = 0; i < 5; i++)
        {
            std::cout << "Triangle VAO " << i << ": " << triangleVAOs[i] << std::endl;
        }
    }
}
