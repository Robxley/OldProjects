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


#ifndef _BH3D_RESOURCEMANAGER_H_
#define _BH3D_RESOURCEMANAGER_H_


//#include <map>
#include <unordered_map>
#include "Logger.hpp"

namespace BH3D
{
	template <typename SingleT, typename ResT>
	class ResourceManager
	{
		//singleton
		public:

			static SingleT& Instance()
			{
				static SingleT s;
				return s;
			}


		protected:
			//Private Constructor (singleton)
			SingleT& operator= (const SingleT&){}
			//ResourceManager(const ResourceManager & r){};
			~ResourceManager()
			{ 
				Clear(); 
				BH3D_LOGGER("ResourceManager Clear");
			}

		public:

			virtual ResT * Load(const char * filename, const char * name = nullptr); //Load the resource from file and a id name. if name == nullptr, name = filename by default
			virtual ResT * Load(const void * data, const char * name);		//Load the resource from raw data, and give it a id name
			inline ResT * Load(const std::string & filename) { return Load(filename.c_str()); }

			//retourne le nombre de ressouce
			std::size_t Size() const;

			//récupére la liste des ressoucres dans une string
			virtual void GetResourceList(std::string & list, const std::string splitter = "\n");

			//suppression d'une ressource par son nom
			virtual bool Erase(const char * filename);
			inline bool Erase(const std::string & filename) { return Erase(filename.c_str()); }

			//suppression par pointer/reference
			virtual bool Erase(ResT * resource);
			inline bool Erase(ResT & resource){return Erase(&resource);}

			//suppression de l'ensemble des ressource
			virtual void Clear();

			//defini une ressource par default
			//Si aucune n'est defini par SetDefault la première ressource chargée est considéré par défaut
			virtual void SetDefault(ResT * resource);

		protected:

			//si la ressource n'existe pas déjà, LoadResourceFromFile est appelée par Load
			//LoadResourceFromFile renvoie le pointeur sur la ressource
			virtual ResT* LoadResourceFromFile(const std::string & filename) = 0;
			virtual ResT* LoadResourceFromRaw(const void * data) = 0;

			//Destructeur de la ressource appelée par Clear or Erase.
			inline virtual void FreeResource(ResT* ressource)
			{
				delete ressource;
			}

			//std::map<std::string, ResT> mapResources;
		    std::unordered_map<std::string, ResT*> mapResources;

			ResT *defaultResource = nullptr;
	};

	//Declaration des fonctions
	//-------------------------------------------------------

	template <typename SingleT, typename ResT>
	ResT * ResourceManager<SingleT,ResT>::Load(const char * filename, const char * name)
	{
		if (filename == nullptr)
		{
			BH3D_LOGGERERROR("Resource filename = nullptr");
			return defaultResource;
		}

		std::string file(filename);

		//verifie que la ressource n'existe pas deja.
		auto it = mapResources.find(file);
		if (it != mapResources.end()) return it->second;

		//charge la ressource
		ResT * res = LoadResourceFromFile(filename);

		//s'il y a une erreur
		if (res == nullptr)
		{
			BH3D_LOGGERWARNING("Can't load the resource : " << file);
			return defaultResource;
		}

		//on ajoute la ressource à la liste
		if (name == nullptr)
			mapResources[file] = res;
		else
			mapResources[{name}] = res;

		BH3D_LOGGER("Resource ok : " << file);
		return res;

	}

	template <typename SingleT, typename ResT>
	ResT * ResourceManager<SingleT, ResT>::Load(const void * data, const char * name)
	{
		if ( data = nullptr)
		{
			BH3D_LOGGERERROR("Resource -> nullptr");
			return defaultResource;
		}

		if (name = nullptr)
		{
			BH3D_LOGGERERROR("Resource -> no name");
			return defaultResource;
		}


		std::string sname(name);

		//verifie que la ressource n'existe pas deja.
		auto it = mapResources.find(sname);
		if (it != mapResources.end()) return it->second;

		//charge la ressource
		ResT * res = LoadResourceFromRaw(data);

		//s'il y a une erreur
		if (res == nullptr)
		{
			BH3D_LOGGERWARNING("Can't load the resource : " << sname);
			return defaultResource;
		}

		//on ajoute la ressource à la liste
		mapResources[sname] = res;

		BH3D_LOGGER("Resource ok : " << sname);
		return res;

	}


	template <typename SingleT, typename ResT>
	inline std::size_t ResourceManager<SingleT, ResT>::Size() const
	{
		return mapResources.size();
	}

	template <typename SingleT, typename ResT>
	void ResourceManager<SingleT, ResT>::GetResourceList(std::string & list, const std::string splitter = "\n")
	{
		std::size_t fullsize = 0;
		for (auto & res : mapResources)
		{
			fullsize += res.first.size();
		}
		fullsize += splitter.size() * mapResources.size();

		list.reserve(fullsize);
		for (auto & res : mapResources)
		{
			list += splitter + res.first;
		}
	}

	template <typename SingleT, typename ResT>
	bool ResourceManager<SingleT, ResT>::Erase(const char * filename)
	{
		std::string file(filename);

		auto it = mapResources.find(file);
		if (it != mapResources.end())
		{
			FreeResource(it->second);
			mapResources.erase(it);
			BH3D_LOGGER("Resource deleted : " << file);
			return true;
		}

		return false;
	}

	template <typename SingleT, typename ResT>
	bool ResourceManager<SingleT, ResT>::Erase(ResT * resource)
	{

		for (auto & res : mapResources)
		{
			if (res.second == resource)
			{
				std::string file = res.first;
				FreeResource(res.second);
				mapResources.erase(res.first);
				BH3D_LOGGER("Resource deleted : " << file);
				return 1;
			}
		}

		return 0;
	}

	template <typename SingleT, typename ResT>
	void ResourceManager<SingleT, ResT>::Clear()
	{


		for (auto & res : mapResources)
		{
			FreeResource(res.second);
			BH3D_LOGGER("Resource deleted : " << res.first);
		}

		mapResources.clear();
	}

	template <typename SingleT, typename ResT>
	inline void ResourceManager<SingleT, ResT>::SetDefault(ResT * resource)
	{
		ResT * defaultResource = resource;
	}


}









#endif //_BH3D_RESOURCEMANAGER_H_
