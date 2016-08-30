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

#include "Logger.hpp"

#ifdef BH3D_USE_FILE_LOGGER

#include "Assertion.hpp"

namespace BH3D
{
	Logger::Logger() : logFile(BH3D_LOGGER_FILE)
	{
		// On vérifie que le fichier est bien ouvert
		if (!logFile.is_open())
		{
			BH3D_ASSERT(0);
			return;
		}
		// Inscription de l'en-tête du fichier
		logFile << "  ========================================" << std::endl;
		logFile << "   Biohazard3d - Logger File - OPEN " << std::endl;
		logFile << "  ========================================" << std::endl << std::endl;
	}
	Logger::~Logger()
	{
		if (logFile.is_open())
		{
			logFile << std::endl;
			logFile << "  ========================================" << std::endl;
			logFile << "   Biohazard3d - Logger File - CLOSE " << std::endl;
			logFile << "  ========================================" << std::endl << std::endl;
		}
	}

	Logger &Logger::Instance()
	{
		static Logger instance;
		return instance;
	}

	std::ofstream& Logger::getFileLogger()
	{ 
		BH3D_ASSERT(logFile.is_open());
		return logFile;
	}

}

#endif