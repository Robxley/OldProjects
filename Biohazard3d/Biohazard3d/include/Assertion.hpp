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
\file Assertion.hpp
\author Robxley (A.CAILLY)
\version 0.1
\date 09/12/2014
\~english
\brief Biohazard3d assertion macro
\details  Describe the Assertion routines and macros
\~french
\brief Macro pour les assertions
\details  Décription des routines et macros utilisées par Biohazard3d
*/



#ifndef _BH3D_ASSERTION_H_
#define _BH3D_ASSERTION_H_
#include <cassert>
#include "Logger.hpp"

/**
*\~english
*\brief		Biohazard assertion macro
*\param[in]	Expression to be tested
*\~french
*\brief		Macro assertion utilisée par biohazard
*\param[in]	Expression à tester
*/
#define BH3D_ASSERT(w) assert(w);

#ifdef _DEBUG
#define BH3D_ASSERT_MSG(w,msg,r) if(!(w)){ BH3D_LOGGERERROR(msg) ; return r;}
#else
#define BH3D_ASSERT_MSG(w,msg,r)
#endif

#endif 
