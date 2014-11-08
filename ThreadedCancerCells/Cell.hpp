#pragma once

#include "Globals.hpp"
#include <glm/glm.hpp>

namespace TCC
{
	enum CellType : unsigned char
	{
		Healthy = 0
		, Cancer = 1
		, Medecine = 2
		, None = 3
	};

	static const unsigned char Style[12] = {
		0, 255, 0 // Healthy
		, 255, 0, 0 // Cancer
		, 255, 255, 0 // Medecine
	    , 0, 0, 0}; // None
}