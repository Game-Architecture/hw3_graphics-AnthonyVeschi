#version 400

uniform mat4 u_mvp;

// TODO: Homework 3 - Add any other necessary uniforms or input/output variables here
layout(location = 0) in vec3 coord3d;
layout(location = 1) in vec2 vertexUV;
out vec2 fragmentUV;

void main()
{
	// TODO: Homework 3
	fragmentUV = vertexUV;
	gl_Position = u_mvp * vec4(coord3d, 1.0);
}