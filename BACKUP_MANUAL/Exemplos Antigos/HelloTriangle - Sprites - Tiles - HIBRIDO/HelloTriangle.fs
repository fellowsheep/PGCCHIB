//Código fonte do Fragment Shader (em GLSL)
#version 400

in vec3 vertexColor;
in vec2 texCoord;
out vec4 color;

uniform sampler2D texBuffer;

void main()
{
	color = texture(texBuffer, texCoord);//vec4(vertexColor,1.0);
}