/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_output.h"

#include "ga_frame_params.h"

#include "graphics/ga_material.h"
#include "graphics/ga_program.h"
#include "math/ga_mat4f.h"
#include "math/ga_quatf.h"

#include <cassert>
#include <iostream>
#include <SDL.h>

#include <windows.h>

#define GLEW_STATIC
#include <GL/glew.h>

ga_output::ga_output(void* win) : _window(win)
{
	int width, height;
	SDL_GetWindowSize(static_cast<SDL_Window*>(_window), &width, &height);

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

ga_output::~ga_output()
{
}

void ga_output::draw(const ga_static_drawcall& drawcall) const
{
	// TODO: Homework 3 - Draw the geometry from the array object in drawcall
}

void ga_output::update(ga_frame_params* params)
{
	// Update viewport in case window was resized:
	int width, height;
	SDL_GetWindowSize(static_cast<SDL_Window* >(_window), &width, &height);
	glViewport(0, 0, width, height);

	// Clear viewport:
	glDepthMask(GL_TRUE);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute projection matrices:
	ga_mat4f perspective;
	perspective.make_perspective_rh(ga_degrees_to_radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
	ga_mat4f view_perspective = params->_view * perspective;

	// Draw all static geometry:
	for (auto& d : params->_static_drawcalls)
	{
		// Bind the material used by the drawcall
		d._material->bind(view_perspective, d._transform);

		draw(d);
	}

	GLenum error = glGetError();
	assert(error == GL_NONE);

	// Swap frame buffers:
	SDL_GL_SwapWindow(static_cast<SDL_Window* >(_window));
}
