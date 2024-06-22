#version 410

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_mapping;

out vec2 texture_coords;

void main () {
	texture_coords = texture_mapping;
	gl_Position = vec4 (vertex_position, 0.0, 1.0);
}
