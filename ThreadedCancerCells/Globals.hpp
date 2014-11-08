#pragma once

#include <glm/fwd.hpp>
#include <vector>
#include <atomic>
#include <array>

namespace TCC
{
	class Display;
	class GridBuffer;
	class WorkerThread;
	class MedecineManager;

	//////////
	/// VERY DIRTY GLOBALS - Shame on me :(

	static unsigned int windowWidth = 1024;
	static unsigned int windowHeight = 768;
	static Display *displayBuffer;
	static GLuint fontTex;
	double mouse_x, mouse_y;
	bool rMouse, lMouse;
	int injectionRadius = 2;
	int injectionThickness = 4;

	int cancerPercent = 45;
	int healthyPercent = 30;

	bool running;

	float zoom = 1.0f;

	GridBuffer *buffer;

	MedecineManager *medecineManager;

	typedef glm::uvec2 Position;
	typedef glm::uvec3 Color;

	int grainSize = 10000;

	bool count = false;

	static std::array<unsigned int, 4> Counter;
}