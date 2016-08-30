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



 
#ifndef _BH3D_FPS_H_
#define _BH3D_FPS_H_

#include <SDL.h>

namespace BH3D
{

	class Fps
	{

		public:
			Fps();

			//retourne le nombre moyen d'images par second
			// (attend 1sec et regarde le nombre d'image
			inline unsigned int GetFps() const;

			//Retourne le temps écoulé en millisecond depuis le 
			//dernier appelle de getElapseTime()
			inline unsigned int GetElapseTime() const;

			//calcul du fps ( à placer dans la boucle de l'application)
			void Compute();
		private:
			unsigned int elapseTime; //temps entre deux images
			unsigned int fps; //fps moyen sur 1 sec


			//variables
			unsigned int sumTime, lastTime, count;
	};

	//inline functions 
	inline unsigned int Fps::GetFps() const
	{
		return fps;
	}
	inline unsigned int Fps::GetElapseTime() const
	{
		return elapseTime;
	}
}
#endif //
