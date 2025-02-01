#version 410
out vec4 frag_colour;
in vec3 ourColor;
void main () {
frag_colour = vec4(ourColor,1.0);
}