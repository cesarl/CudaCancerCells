#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/gl.h>
#include "Globals.hpp"
#include <cmath>

namespace TCC
{
	class GridBuffer;
	class Display
	{
	public:
		Display(unsigned int width, unsigned int height)
			: _width(width)
			, _height(height)
			, _buffer(nullptr)
		{
			_buffer = new unsigned char[_width * _height * 3]();
			memset((void*)(_buffer), 0, sizeof(unsigned char) * _width * height * 3);
		}

		~Display()
		{
			if (_buffer)
				delete[] _buffer;
		}

		void fill(const glm::uvec3& color)
		{
			auto so = sizeof(unsigned char) * 3;
			unsigned char c[3] = { color.x, color.y, color.z };
			auto t = _width * _height;
			for (unsigned int i = 0; i < _width; ++i)
			{
				memcpy((void*)(&_buffer[i * 3]), (void*)(&c), so);
			}
			for (unsigned int i = 1; i < _height; ++i)
			{
				memcpy((void*)(&_buffer[i * _width * 3]), (void*)(&_buffer[0]), _width * so);
			}
		}

		void randomFill()
		{
			for (unsigned int i = 0; i < _width; ++i)
			{
				for (unsigned int j = 0; j < _height; ++j)
				{
					drawPixel(glm::uvec2(i, j), glm::uvec3(rand() % 255, rand() % 255, rand() % 255));
				}
			}
		}

		void drawPixel(const glm::uvec2& position, const glm::uvec3& color)
		{
			if (position.x >= _width || position.y >= _height)
				return;
			_buffer[(position.y * (_width * 3)) + position.x * 3] = color.x;
			_buffer[(position.y * (_width * 3)) + position.x * 3 + 1] = color.y;
			_buffer[(position.y * (_width * 3)) + position.x * 3 + 2] = color.z;
		}

		void drawLine(const glm::uvec2& _from, const glm::uvec2& _to, const glm::uvec3& color)
		{
			auto from = _from;
			auto to = _to;
			const bool steep = (fabs((float)to.y - from.y) > fabs((float)to.x - from.y));
			if (steep)
			{
				std::swap(from.x, from.y);
				std::swap(to.x, to.y);
			}
 
			if (from.x > to.x)
			{
				std::swap(from.x, to.x);
				std::swap(from.y, to.y);
			}
 
			const float dx = (float)(to.x - from.y);
			const float dy = (float)(fabs((float)to.y - from.y));
 
			float error = dx / 2.0f;
			const int ystep = (from.y < to.y) ? 1 : -1;
			int y = from.y;
 
			const int maxX = to.x;
 
			for (int x = from.x; x < maxX; x++)
			{
				if (steep)
				{
					this->drawPixel(glm::uvec2(y, x), color);
				}
				else
				{
					this->drawPixel(glm::uvec2(x, y), color);
				}
 
				error -= dy;
				if (error < 0)
				{
					y += ystep;
					error += dx;
				}
			}
		}

		void drawCircle(const glm::uvec2& center, unsigned int radius, const glm::uvec3& color)
		{
			drawPixel(glm::uvec2(center.x, center.y + radius), color);
			drawPixel(glm::uvec2(center.x, center.y - radius), color);
			drawPixel(glm::uvec2(center.x + radius, center.y), color);
			drawPixel(glm::uvec2(center.x - radius, center.y), color);

			int f = 1 - radius;
			int fx = 0;
			int fy = -2 * radius;
			int x = 0;
			int y = radius;

			while (x < y)
			{
				if (f >= 0)
				{
					--y;
					fy += 2;
					f += fy;
				}
				++x;
				fx += 2;
				f += fx + 1;
				drawPixel(Position(center.x + x, center.y + y), color);
				drawPixel(Position(center.x - x, center.y + y), color);
				drawPixel(Position(center.x + x, center.y - y), color);
				drawPixel(Position(center.x - x, center.y - y), color);
				drawPixel(Position(center.x + y, center.y + x), color);
				drawPixel(Position(center.x - y, center.y + x), color);
				drawPixel(Position(center.x + y, center.y - x), color);
				drawPixel(Position(center.x - y, center.y - x), color);
			}
		}

		void render()
		{
			glPixelZoom(TCC::zoom, TCC::zoom);
			glDrawPixels(_width, _height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)_buffer);
		}

		Display() = delete;
		Display(const Display&) = delete;
		Display(Display&&) = delete;
		Display& operator=(const Display&) = delete;
		Display& operator=(Display&&) = delete;
	private:
		unsigned int _width;
		unsigned int _height;
		unsigned char* _buffer;
		friend class GridBuffer;
	};
}