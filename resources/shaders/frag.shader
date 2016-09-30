#version 150

out vec4 out_color;
in float ypos;

void main() {
	gl_FragColor = vec4(cos(ypos), sin(ypos), ypos*ypos, 1.0);
}
