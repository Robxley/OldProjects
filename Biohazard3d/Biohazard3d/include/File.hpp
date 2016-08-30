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


#ifndef _BH3D_FILE_H_
#define _BH3D_FILE_H_


#include <string>

namespace BH3D
{	

	class File
	{
	public:

		//constructeurs
		File(const File & file);
		File(const std::string & file = "");
		File(const char * file);

		//quelque opérateur utile
		operator std::string() const;
		operator std::string&();

		//File(const File & file);
		virtual bool ReadAndSaveIn(std::string & buffer) const;

		//écris dans le fichier, supprime le contenu du fichier si existant
		virtual void WriteIn(const std::string & data, bool erase = true);


		//accesseur
		void set(const std::string & Name);

		//----------------------------------------------------------
		// Indique si le fichier existe ou non
		//----------------------------------------------------------
		bool Exists() const;

		//----------------------------------------------------------
		// Renvoie le nom du fichier avec son chemin complet 
		// (Dir+filename) ex : data/maison/porte.jpg
		//----------------------------------------------------------
		const std::string& Fullname() const;

		//----------------------------------------------------------
		// Renvoie le nom du fichier sans son chemin
		// ex : porte.jpg
		//----------------------------------------------------------
		std::string Filename() const;

		//----------------------------------------------------------
		// Renvoie le nom du fichier sans extension ni chemin
		// ex : porte
		//----------------------------------------------------------
		std::string ShortFilename() const;

		//----------------------------------------------------------
		// Renvoie l'extension du fichier
		// ex : jpg
		//----------------------------------------------------------
		std::string Extension() const;

		//----------------------------------------------------------
		// Renvoie l'extension du fichier
		// ex : data/maison
		//----------------------------------------------------------
		std::string Dir() const;
	private:

		//----------------------------------------------------------
		// Données membres
		//----------------------------------------------------------
		std::string fullName; // Chemin complet du fichier
	};


}
#endif //
