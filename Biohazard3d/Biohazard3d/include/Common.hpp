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




/*!
\file Common.hpp
\author Robxley (A.CAILLY)
\version 0.1
\date 09/12/2014
\~english
\brief Common define
\details  Common objects and macros definition
\~french
\brief Quelques definitions
\details  Décription des macros communes à l'ensemble du projet Biohazard3d
*/


#ifndef _BH3D_COMMON_H_
#define _BH3D_COMMON_H_

/*! Everything is fine ( return function )  - Tout va bien (retour fonction) */
#define BH3D_OK         1
/*! Everything is not fine ( return function )  - Tout ne va pas bien (retour fonction) */
#define BH3D_ERROR      0

#define BH3D_EXIT		0

//buffer offset in bytes
//#define BH3D_BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define BH3D_BUFFER_OFFSET(i) ((void*)(i))

#endif