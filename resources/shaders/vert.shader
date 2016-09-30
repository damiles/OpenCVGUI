#version 150

in vec4 position;
uniform mat4 MVP;
out float ypos;

void main() {
    ypos= position[1];
	gl_Position = MVP*position;
}
