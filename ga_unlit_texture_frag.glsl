#version 400

uniform sampler2D u_texture;

// TODO: Homework 3 - Add any other necessary uniforms or input/output variables here
layout(location = 0) out vec4 myColor;
in vec2 fragmentUV;

void main()
{
	// TODO: Homework 3
    myColor = texture(u_texture, fragmentUV); // this version pulls color from texture
}