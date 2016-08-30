//==========================================================
// File.cpp
//
// Implantation de la classe RxFile
//
//==========================================================


//==========================================================
// En-têtes
//==========================================================
#include "RxFile.h"
#include <algorithm>
#include <fstream>

RxFile::RxFile(const std::string& name) :
fullName(name)
{
	std::replace(fullName.begin(), fullName.end(), '\\', '/');
}



RxFile::RxFile(const char* name) :
fullName(name)
{
	std::replace(fullName.begin(), fullName.end(), '\\', '/');
}

//----------------------------------------------------------
// Constructeur à partir d'un std::string
//----------------------------------------------------------
void RxFile::set(const std::string& name)
{
	fullName = name;
}

////////////////////////////////////////////////////////////
// Indique si le fichier existe ou non
//
// [retval] True si le fichier existe
//
////////////////////////////////////////////////////////////
bool RxFile::Exists() const
{
	std::ifstream file(fullName.c_str());

	return file.is_open();
}


////////////////////////////////////////////////////////////
// Renvoie le nom du fichier avec son chemin complet
//
// [retval] Chemin complet du fichier
//
////////////////////////////////////////////////////////////
const std::string& RxFile::Fullname() const
{
	return fullName;
}


////////////////////////////////////////////////////////////
// Renvoie le nom du fichier sans son chemin
//
// [retval] Nom du fichier
//
////////////////////////////////////////////////////////////
std::string RxFile::Filename() const
{
	std::string::size_type Pos = fullName.find_last_of("\\/");

	if (Pos != std::string::npos)
		return fullName.substr(Pos + 1, std::string::npos);
	else
		return fullName;
}


////////////////////////////////////////////////////////////
// Renvoie le nom du fichier sans extension ni chemin
//
// [retval] Nom du fichier
//
////////////////////////////////////////////////////////////
std::string RxFile::ShortFilename() const
{
	return Filename().substr(0, Filename().find_last_of("."));
}


////////////////////////////////////////////////////////////
// Renvoie l'extension du fichier
//
// [retval] Extension du fichier
//
////////////////////////////////////////////////////////////
std::string RxFile::Extension() const
{
	std::string::size_type Pos = fullName.find_last_of(".");
	if (Pos != std::string::npos)
		return fullName.substr(Pos + 1, std::string::npos);
	else
		return "";
}

std::string RxFile::Dir() const
{
	std::string::size_type Pos = fullName.find_last_of("\\/");
	if (Pos != std::string::npos)
		return fullName.substr(0, Pos);
	else
		return "";
}
