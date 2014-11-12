#pragma once

#include <SDL.h>
#include <memory>
#include <glm/glm.hpp>
#include <gl/glew.h>

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

class App
{
public:
	App();
	~App();
	App(const App &) = delete;
	App(App &&) = delete;
	App &operator=(const App &) = delete;
	App &operator=(App &&) = delete;
	void init();
	bool run();
	bool deactivate();
private:
	SDL_Window *_window;
	SDL_GLContext _context;
	bool _updateInput();
	unsigned int _pastTime;
	float _deltaTime;
	GLuint _glTexture;
	GLuint _glPixelBuffer;
	cudaGraphicsResource *_cudaGr;
	std::size_t _width;
	std::size_t _height;

	void _resetBuffer();
};