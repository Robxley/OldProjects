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


 
#ifndef _BH3D_STUFFMATH_H_
#define _BH3D_STUFFMATH_H_


#define BH3D_EPSILON			0.000001f
#define BH3D_PI					3.141592653589793238462643383279f
#define BH3D_PIDIV2				1.570796326794896619231321691639f		// PI / 2
#define BH3D_2PI				6.283185307179586476925286766559f		// 2 * PI
#define BH3D_PI2				9.869604401089358618834490999876f		// PI au carre
#define BH3D_PIDIV180			0.01745329251994329576923690768488f		// PI / 180
#define BH3D_180DIVPI			57.295779513082320876798154814105f		// 180 / PI


#define BH3D_STACK_MAX_SIZE 50 //taille par défaut du tas

#include <vector>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#define MatStack Stack<glm::mat4>

namespace BH3D
{
	template <typename T>
	class Stack
	{
	public:
		Stack(std::size_t reserve = BH3D_STACK_MAX_SIZE);
		Stack(const T& m, std::size_t reserve = BH3D_STACK_MAX_SIZE);
		Stack(const Stack& m);

		//sauvegarde
		inline void Push(); // ~ glPushMatrix

		//restitution de la matrice
		inline void Pop(); // ~ glPopMatrix

		//charge la matrice identitée sur la matrice active
		inline void LoadIdentity(); // ~ glLoadIdentity

		//charge une matrice
		inline void Load(const T & m); // ~ glLoadMatrix

		//multiplication
		inline T& Mult(const T & m); // ~ glMultMatrix

		//retour la objet en haut de pile (matrice active)
		inline T& Top();

		inline Stack<T>& operator = (const T & m);
		inline Stack<T>& operator = (const Stack<T> & m);

		inline void Reset(const T & m);
		inline void Reset();

		std::vector<T> tStack;
	};

	template <typename T>
	Stack<T>::Stack(std::size_t reserve )
	{
		tStack.reserve(reserve);
		tStack.resize(1);
		LoadIdentity();
	}

	template <typename T>
	Stack<T>::Stack(const T& m, std::size_t reserve)
	{
		tStack.reserve(reserve);
		tStack.push_back(m)
	}

	template <typename T>
	Stack<T>::Stack(const Stack& m)
	{
		tStack = m.tStack();
	}

	//inline functions
	template <typename T>
	inline void Stack<T>::Stack::Push()
	{
		tStack.push_back(tStack.back());
	}
	template <typename T>
	inline void Stack<T>::Pop()
	{
		if (tStack.size())
			tStack.pop_back();
	}
	template <typename T>
	inline void Stack<T>::LoadIdentity()
	{
		tStack.back() = T();
	}

	template <typename T>
	inline void Stack<T>::Load(const T & m)
	{
		tStack.back() = m;
	}

	template <typename T>
	inline void Stack<T>::Reset(const T & m)
	{
		tStack.clear();
		tStack.push_back(m);
	}

	template <typename T>
	inline void Stack<T>::Reset()
	{
		tStack.clear();
		tStack.resize(1);
		LoadIdentity();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return tStack.back();
	}

	template <typename T>
	inline T& Stack<T>::Mult(const T& m)
	{
		return (tStack.back() *= m);
	}

	template <typename T>
	inline Stack<T>& Stack<T>::operator = (const T& m)
	{
		tStack.back() = m;
		return *this;
	}

	template <typename T>
	inline Stack<T>& Stack<T>::operator = (const Stack<T>& m)
	{
		tStack.back() = m.tStack.back();
		return *this;
	}
/*
	class MatStack
	{

	public:
		MatStack(const glm::mat4& m);
		MatStack(const MatStack& m);
		MatStack();

		//sauvegarde la matrice
		inline void PushMatrix(); // ~ glPushMatrix

		//restitution de la matrice
		inline void PopMatrix(); // ~ glPopMatrix

		//charge la matrice identitée sur la matrice active
		inline void LoadIdentity(); // ~ glLoadIdentity

		//charge une matrice
		inline void LoadMatrix(const glm::mat4 & m); // ~ glLoadMatrix

		//multiplication
		inline glm::mat4& MultMatrix(const glm::mat4 & m); // ~ glMultMatrix

		//retour la matrice sauvegardée en haut de pile (matrice active)
		inline glm::mat4& Top();

		inline MatStack& operator = (const glm::mat4 & m);
		inline MatStack& operator = (const MatStack & m);

		inline void Reset(const glm::mat4 & m);
		inline void Reset();

		std::vector<glm::mat4> tStack;
	};

	//inline functions

	//sauvegarde la matrice
	inline void MatStack::PushMatrix()
	{
		tStack.push_back(tStack.back());
	}

	//restitution de la matrice
	inline void MatStack::PopMatrix()
	{
		if (tStack.size())
			tStack.pop_back();
	}
	inline void MatStack::LoadIdentity()
	{
		tStack.back() = glm::mat4();
	}

	inline void MatStack::LoadMatrix(const glm::mat4 & m)
	{
		tStack.back() = m;
	}
	inline void MatStack::Reset(const glm::mat4 & m)
	{
		tStack.clear();
		tStack.push_back(m);
	}
	inline void MatStack::Reset()
	{
		tStack.clear();
		tStack.push_back(glm::mat4());
	}

	inline glm::mat4& MatStack::Top()
	{

		return tStack.back();

	}

	inline glm::mat4& MatStack::MultMatrix(const glm::mat4& m)
	{
		return (tStack.back() *= m);
	}


	inline MatStack& MatStack::operator = (const glm::mat4& m)
	{
		tStack.back() = m;
		return *this;
	}

	inline MatStack& MatStack::operator = (const MatStack& m)
	{
		tStack.back() = m.tStack.back();
		return *this;
	}*/

}
#endif //
