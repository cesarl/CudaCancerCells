#include "App.hpp"
#include <cassert>
#include <mutex>

#include <GL\glew.h>
#include <GL/glut.h>
#include <GL\GLU.h>
#include <GL\glext.h>
#include <GL\GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

App::App()
	: _window(nullptr)
	, _pastTime(0)
	, _deltaTime(0.0f)
	, _width(1024)
	, _height(640)
{
}

App::~App()
{
	deactivate();
}

void App::init()
{
	static std::once_flag flag;
	std::call_once(flag, [this]()
	{
		assert(cudaGLSetGLDevice(0) == cudaSuccess);
		assert(SDL_Init(SDL_INIT_VIDEO) == 0);

		_window = SDL_CreateWindow("SPH DEMOSCENE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		assert(_window != nullptr);
		_context = SDL_GL_CreateContext(_window);
		glewInit();
		glGenBuffers(1, &_glPixelBuffer);
		_resetBuffer();
	});
	assert(cudaGraphicsGLRegisterBuffer(&_cudaGr, _glPixelBuffer, cudaGraphicsMapFlagsWriteDiscard) == cudaSuccess);
}

void App::_resetBuffer()
{
	std::vector<glm::vec4> t;
	t.resize(_width * _height, glm::vec4(1,0,1,1));
	//randomFill(t);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, _glPixelBuffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, _width * _height * sizeof(glm::vec4),
		t.data(), GL_STREAM_COPY);
}

bool App::run()
{
	if (!_updateInput())
		return false;

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawPixels(_width, _height, GL_RGBA, GL_FLOAT, 0);
	SDL_GL_SwapWindow(_window);

	return true;
}

bool App::deactivate()
{
	static std::once_flag flag;
	std::call_once(flag, [this]()
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		SDL_Quit();
	});
	return true;
}

bool App::_updateInput()
{
	SDL_Event event;

	auto dtNow = SDL_GetTicks();
	if (dtNow > _pastTime)
	{
		_deltaTime = (float)(dtNow - _pastTime) / 1000.0f;
		_pastTime = dtNow;
	}

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				return false;
			}
		}
	}
	return true;
}

