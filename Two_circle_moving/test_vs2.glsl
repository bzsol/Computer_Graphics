#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 matrix2;

out vec3 colour;

void main() {
	colour = vertex_colour;
	gl_Position = matrix2 * vec4(vertex_position, 1.0);
}