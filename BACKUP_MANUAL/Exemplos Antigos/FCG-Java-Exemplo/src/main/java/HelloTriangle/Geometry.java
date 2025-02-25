package HelloTriangle;

import org.joml.Vector3f;

import static org.lwjgl.opengl.GL11.GL_FLOAT;
import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glVertexAttribPointer;
import static org.lwjgl.opengl.GL30.glBindVertexArray;
import static org.lwjgl.opengl.GL30.glGenVertexArrays;

public class Geometry {

    public static final int FLOAT_SIZE = 4;

    public static int setupGeometry(float[] vertices) {
        int VBO = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

        int VAO = glGenVertexArrays();
        glBindVertexArray(VAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * FLOAT_SIZE, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * FLOAT_SIZE, 3 * FLOAT_SIZE);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        return VAO;
    }

    public static void addPoint(float[] vertices, int i, float x, float y, Vector3f color) {
        vertices[i++] = x; //x
        vertices[i++] = y; //y
        vertices[i++] = 0.0f; //z
        vertices[i++] = color.x; //r
        vertices[i++] = color.y; //g
        vertices[i] = color.z; //b
    }

    public static float[] mergeVertices(float[] v0, float[] v1) {
        float[] vertices = new float[v0.length + v1.length];
        System.arraycopy(v0, 0, vertices, 0, v0.length);
        System.arraycopy(v1, 0, vertices, v0.length, v1.length);
        return vertices;
    }

    public static float[] triangle(float centerX, float centerY, float radius) {
        float[] vertices = new float[3 * 6];

        //angle between points
        float angle = 360.0f / 3;

        for (int i = 0; i < 3; i++) {
            //current angle
            float theta = angle * i + 90f;

            //calculate xy
            float x = centerX + radius * (float) Math.cos(Math.toRadians(theta));
            float y = centerY + radius * (float) Math.sin(Math.toRadians(theta));

            //add point
            addPoint(vertices, i * 6, x, y, Colors.get());
        }

        return vertices;
    }

    public static float[] circle(float centerX, float centerY, float radius, int points, float rotation) {
        float[] vertices = new float[points * 6];
        addPoint(vertices, 0, centerX, centerY, Colors.get());

        //angle between points
        float angle = 360.0f / (points - 2);

        for (int i = 0; i < points - 1; i++) {
            //current angle
            float theta = angle * i + rotation;

            //calculate xy
            float x = centerX + radius * (float) Math.cos(Math.toRadians(theta));
            float y = centerY + radius * (float) Math.sin(Math.toRadians(theta));

            //add point
            addPoint(vertices, (i + 1) * 6, x, y, Colors.get());
        }

        return vertices;
    }

    public static float[] star(float centerX, float centerY, float radius) {
        float[] vertices = new float[10 * 6];

        for (int i = 0; i < 10; i++) {
            float ratio = ((i % 2) + 0.618f) / 1.618f; //golden ratio
            float angle = (float) (2 * Math.PI * i - Math.PI) / 10f;

            //calculate xy
            float x = centerX + (float) Math.cos(angle) * ratio * radius;
            float y = centerY + (float) Math.sin(angle) * ratio * radius;

            //add point
            addPoint(vertices, i * 6, x, y, Colors.get());
        }

        return vertices;
    }

    public static float[] spiral(float centerX, float centerY, int points, float loops, float radius, float rotation) {
        float[] vertices = new float[points * 6];
        addPoint(vertices, 0, centerX, centerY, Colors.get());

        //angle between points
        float ratio = loops / (points - 1);

        for (int i = 1; i < points; i++) {
            //current angle
            float j = i * ratio;
            float angle = j * (float) (Math.PI * 2 + Math.toRadians(rotation));
            float magnitude = j * radius / loops;

            //calculate xy
            float x = centerX + (float) Math.cos(angle) * magnitude;
            float y = centerY + (float) Math.sin(angle) * magnitude;

            //add point
            addPoint(vertices, i * 6, x, y, Colors.get());
        }

        return vertices;
    }

    public enum Colors {
        PINK(0xFF72AD),
        PURPLE(0xAD72FF),
        BLUE(0x72ADFF),
        CYAN(0x72FFAD),
        GREEN(0x72FF72),
        YELLOW(0xFFFF72),
        ORANGE(0xFFAD72),
        RED(0xFF7272);

        private static int last = (int) (Math.random() * values().length);

        private final Vector3f color;

        Colors(int hex) {
            float r = ((hex >> 16) & 0xFF) / 255f;
            float g = ((hex >> 8) & 0xFF) / 255f;
            float b = (hex & 0xFF) / 255f;
            this.color = new Vector3f(r, g, b);
        }

        public static Vector3f get() {
            Colors[] colors = values();
            last++;
            last %= colors.length;
            return colors[last].color;
        }

        public static Vector3f random() {
            Colors[] colors = values();
            return colors[(int) (Math.random() * colors.length)].color;
        }
    }
}
