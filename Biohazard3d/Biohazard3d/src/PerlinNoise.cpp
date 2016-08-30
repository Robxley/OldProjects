/*(
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


#include "PerlinNoise.hpp"
 

namespace BH3D
{
	PerlinNoise::PerlinNoise(unsigned int _octaves, float _frequency, float _amplitude)
	{
		octaves = _octaves;
		frequency.resize(_octaves, _frequency);
		frequency.resize(_octaves, _amplitude);
	}
	PerlinNoise::PerlinNoise(unsigned int _octaves, float * _frequency, float * _amplitude)
	{
		octaves = _octaves;
		frequency.resize(_octaves, 1.0f);
		amplitude.resize(_octaves, 1.0f);	

		if (_frequency != nullptr)
			for (unsigned int i = 0; i < _octaves; i++)
				frequency[i] = _frequency[i];
	

		if (_amplitude != nullptr)
			for (unsigned int i = 0; i < _octaves; i++)
				amplitude[i] = _amplitude[i];
		
	}
	float PerlinNoise::PerlinValue(float x, float y)
	{
		float value = 0;
		
		x += seed;
		y += seed;

		unsigned int o;

		for (o = 0; o < octaves; o++)
		{
			value += amplitude[o] * glm::perlin(frequency[o]*glm::vec2(x, y));
		}
		return value;
	}
}
 
 
 