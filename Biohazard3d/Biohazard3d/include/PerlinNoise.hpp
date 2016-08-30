/*
 * The MIT License
 *
 * Copyright 2014 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



#ifndef _BH3D_PERLINNOISE_H_
#define _BH3D_PERLINNOISE_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace BH3D
{
	class PerlinNoise
	{	
		unsigned int octaves = 4;
		std::vector<float> frequency;
		std::vector<float> amplitude;

		unsigned int seed = 0;

		PerlinNoise(unsigned int _octaves = 4, float _frequency = 1.0f, float _amplitude = 1.0f);
		PerlinNoise(unsigned int _octaves, float * _frequency, float * _amplitude);

		float PerlinValue(float x, float y);
		inline float PerlinValue(glm::vec2 p) { PerlinValue(p.x, p.y); }

	};
}
#endif //
