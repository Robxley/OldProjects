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


 
 

 
 
 
#include "Fps.hpp"


namespace BH3D
{
	Fps::Fps()
	{
		//initialise toutes les variables
		elapseTime = 0;
		fps = 0;
		sumTime = 0;
		lastTime = SDL_GetTicks();
		count = 0;
	}

	void Fps::Compute()
	{

		unsigned int t = 0;
		t = SDL_GetTicks();

		elapseTime = (t - lastTime);
		lastTime = t;


		//fps moyen
		sumTime += elapseTime;
		if (sumTime > 1000)
		{
			fps = count;
			count = 0;
			sumTime = 0;
		}
		else
			count++;

	}
}
