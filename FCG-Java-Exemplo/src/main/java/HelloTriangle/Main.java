package HelloTriangle;

import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryStack;

import java.nio.IntBuffer;

import static HelloTriangle.Geometry.*;
import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30.glBindVertexArray;
import static org.lwjgl.opengl.GL30.glDeleteVertexArrays;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Main {

    public static void main(String[] args) {
        new Main().run();
    }

    private long window;

    public void run() {
        init();
        loop();

        //free the window callbacks and destroy the window
        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);

        //terminate GLFW and free the error callback
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void init() {
        //error callback
        GLFWErrorCallback.createPrint(System.err).set();

        //init GLFW
        if (!glfwInit())
            throw new IllegalStateException("Unable to initialize GLFW");

        //configure GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //create window
        window = glfwCreateWindow(600, 600, "Hello Triangle!", NULL, NULL);
        if (window == NULL)
            throw new RuntimeException("Failed to create the GLFW window");

        //input callback
        glfwSetKeyCallback(window, (window, key, scancode, action, mods) -> {
            //mark the window to close
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
        });

        //make the OpenGL context current
        glfwMakeContextCurrent(window);

        //finishes the initializing process
        GL.createCapabilities();

        // Get the thread stack and push a new frame
        try (MemoryStack stack = stackPush()) {
            IntBuffer pWidth = stack.mallocInt(1);
            IntBuffer pHeight = stack.mallocInt(1);

            glfwGetFramebufferSize(window, pWidth, pHeight);
            glViewport(0, 0, pWidth.get(0), pHeight.get(0));
        } //the stack frame is popped automatically

        //make the window visible
        glfwShowWindow(window);
    }

    private void loop() {
        Shader shader = new Shader("HelloTriangle.vsh", "HelloTriangle.fsh");

        //VAOs
        int triangles = setupGeometry(mergeVertices(triangle(0.5f, 0.5f, 0.25f), triangle(0.5f, -0.5f, 0.25f)));
        int points = setupGeometry(mergeVertices(triangle(-0.5f, 0.5f, 0.25f), triangle(0.5f, -0.5f, 0.25f)));
        int lines = setupGeometry(mergeVertices(triangle(-0.5f, -0.5f, 0.25f), triangle(0.5f, -0.5f, 0.25f)));

        int circleCount = 17;
        int circle = setupGeometry(circle(0.0f, 0.0f, 0.25f, circleCount, 0.0f));
        int octagon = setupGeometry(circle(0.75f, 0.0f, 0.25f, 10, 22.5f));
        int pentagon = setupGeometry(circle(-0.75f, 0.0f, 0.25f, 7, 90f - 360f / 5f));

        int pacman = setupGeometry(circle(0.0f, 0.75f, 0.2f, 17, 360f / 15f));
        int pizza = setupGeometry(circle(0.1f, 0.75f, 0.2f, 17, 360f / -15f));

        int star = setupGeometry(star(-0.5f, 0.5f, 0.25f));

        int spiralCount = 100;
        int spiral = setupGeometry(spiral(0.0f, -0.66f, spiralCount, 3f, 0.25f, 0f));

        shader.use();

        //render loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            //clear the framebuffer
            glClearColor(0.3f, 0.2f, 0.3f, 1f);
            glClear(GL_COLOR_BUFFER_BIT);

            //rendering properties
            glLineWidth(10);
            glPointSize(20);

            //reset shader colour
            shader.setVec4("inputColor", 1.0f, 1.0f, 1.0f, 1.0f);

            glBindVertexArray(points);
            glDrawArrays(GL_POINTS, 0, 6);

            glBindVertexArray(triangles);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(lines);
            glDrawArrays(GL_LINE_LOOP, 0, 3);

            shader.setVec4("inputColor", 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_LINE_LOOP, 3, 3);

            //reset shader colour
            shader.setVec4("inputColor", 1.0f, 1.0f, 1.0f, 1.0f);

            glBindVertexArray(circle);
            glDrawArrays(GL_TRIANGLE_FAN, 0, circleCount);

            glBindVertexArray(octagon);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

            glBindVertexArray(pentagon);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 7);

            glBindVertexArray(pacman);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 15);

            glBindVertexArray(pizza);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            glBindVertexArray(star);
            glDrawArrays(GL_LINE_LOOP, 0, 10);

            glBindVertexArray(spiral);
            glDrawArrays(GL_LINE_STRIP, 0, spiralCount);

            //unbind VAO
            glBindVertexArray(0);

            //end render
            glfwSwapBuffers(window);
        }

        glDeleteVertexArrays(points);
        glDeleteVertexArrays(triangles);
        glDeleteVertexArrays(lines);
    }
}
