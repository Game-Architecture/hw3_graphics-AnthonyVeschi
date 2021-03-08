/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_program.h"

#include "ga_texture.h"

#include "math/ga_mat4f.h"
#include "math/ga_vec3f.h"

#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>
#include <stb_image.h>

GLuint ga_uniform::texture_from_file(const char* path, GLuint textureUnit)
{
	/* first up, resolve the path according to the executable */
	extern char g_root_path[256];
	std::string fullpath = g_root_path;
	fullpath += path;

	/* next, load the image from the file using stb_image */
	int width, height, channels_in_file;
	uint8_t* data = stbi_load(fullpath.c_str(), &width, &height, &channels_in_file, 4);

	if (!data)
	{
		printf("texture load error\n");
		exit(EXIT_FAILURE);
	}

	/* finally generate a texture, bind it, describe it, and load the image into it */
	GLuint handle;

	glActiveTexture(GL_TEXTURE0 + textureUnit);

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	/* no longer need the stb_image */
	stbi_image_free(data);

	return handle;
}

void ga_uniform::set(const ga_mat4f& mat)
{
	// TODO: Homework 3 - Set the value of the uniform variable at location _location to mat
	GLuint mvpLoc;
	mvpLoc = glGetUniformLocation(_location, "u_mvp"); //handle???
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (GLfloat*)mat.data);
}

void ga_uniform::set(const ga_texture& tex, uint32_t unit)
{
	// TODO: Homework 3 - Set the value of the uniform variable at location _location to the
	// texture unit holding tex
	//GLuint tLoc;
	//tLoc = glGetUniformLocation(_location, "u_texture");
	glUniform1i(_location, 0); // 0: GL_TEXTURE0 <- texture unit #0, GPU has at least one!

	///*
	GLuint tHandle;
	//tHandle = texture_from_file("data/textures/rpi.png", 0);
	tHandle = tex._handle;
	glActiveTexture(GL_ZERO + unit);
	glBindTexture(GL_TEXTURE_2D, tHandle); // partial hint #3 for textures...
	//*/
}

ga_uniform::ga_uniform(int32_t location) : _location(location) {}

ga_shader::ga_shader(const char* source, GLenum type)
{
	_handle = glCreateShader(type);
	glShaderSource(_handle, 1, &source, 0);
}

ga_shader::~ga_shader()
{
	glDeleteShader(_handle);
}

bool ga_shader::compile()
{
	glCompileShader(_handle);

	int32_t compile_status = GL_FALSE;
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &compile_status);
	return compile_status == GL_TRUE;
}

std::string ga_shader::get_compile_log() const
{
	int32_t length;
	glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &length);

	std::string log(length, '\0');
	glGetShaderInfoLog(_handle, length, &length, &log[0]);
	return log;
}

ga_program::ga_program()
{
	_handle = glCreateProgram();
}

ga_program::~ga_program()
{
	glDeleteProgram(_handle);
}

void ga_program::attach(const ga_shader& shader)
{
	glAttachShader(_handle, shader._handle);
}

void ga_program::detach(const ga_shader& shader)
{
	glDetachShader(_handle, shader._handle);
}

bool ga_program::link()
{
	glLinkProgram(_handle);

	int32_t link_status = GL_FALSE;
	glGetProgramiv(_handle, GL_LINK_STATUS, &link_status);
	return link_status == GL_TRUE;
}

std::string ga_program::get_link_log() const
{
	int32_t length;
	glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &length);

	std::string log(length, '\0');
	glGetProgramInfoLog(_handle, length, &length, &log[0]);
	return log;
}

ga_uniform ga_program::get_uniform(const char* name)
{
	int32_t location = glGetUniformLocation(_handle, name);
	return ga_uniform(location);
}

void ga_program::use()
{
	glUseProgram(_handle);
}
