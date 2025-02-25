package HelloTriangle;

import java.nio.file.Files;
import java.nio.file.Path;

import static org.lwjgl.opengl.GL30.*;

public class Shader {

    public final int ID;

    public Shader(String vertex, String fragment) {
        String vertexSource, fragmentSource;
        try {
            vertexSource = Files.readString(Path.of("src/main/resources/shaders/" + vertex));
            fragmentSource = Files.readString(Path.of("src/main/resources/shaders/" + fragment));
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, vertexSource);
        glCompileShader(vertexShader);
        //checkCompileErrors(vertex, "VERTEX");

        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, fragmentSource);
        glCompileShader(fragmentShader);
        //checkCompileErrors(fragment, "FRAGMENT");

        int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        //checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        this.ID = program;
    }

    public void use() {
        glUseProgram(ID);
    }

    public void setBool(String name, boolean value) {
        glUniform1i(glGetUniformLocation(ID, name), value ? 1 : 0);
    }

    void setInt(String name, int value) {
        glUniform1i(glGetUniformLocation(ID, name), value);
    }

    void setFloat(String name, float value) {
        glUniform1f(glGetUniformLocation(ID, name), value);
    }

    void setVec3(String name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(ID, name), x, y, z);
    }

    void setVec4(String name, float x, float y, float z, float w) {
        glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
    }
}
