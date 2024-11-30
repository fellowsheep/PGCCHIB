#version 460

uniform vec4 inputColor;
in vec4 finalColor;
out vec4 color;

void main() {
    color = finalColor * inputColor;
}