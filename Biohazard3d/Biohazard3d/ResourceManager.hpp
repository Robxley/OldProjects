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
			SingleT& operator= (const SingleT&){}

			~ResourceManager()
			{ 
				Clear(); 
				BH3D_LOGGER("ResourceManager Clear");
			}


		public:


			//retourne null si bug au chargement
			virtual ResT * Load(const char * filename)
			{

				if (filename == nullptr)
				{
					BH3D_LOGGERERROR("Resource filename =  NULL");
					return defaultResource;
				}

				std::string file(filename);

				//verifie que la ressource n'existe pas deja.
				auto it = mapResources.find(file);
				if (it != mapResources.end()) return it->second;

				//charge la ressource
				ResT * res = LoadResourceFromFile(filename);

				//si pas d'erreur
				if (res == nullptr)
				{
					BH3D_LOGGERERROR("Can't load the resource : "<<file);
					return defaultResource;
				}

				//on ajoute la ressource à la liste
				mapResources[file] = res;

				BH3D_LOGGER("Resource ok : "<<file);
				return res;

			}

			virtual ResT * Load(const std::string & filename)
			{
				return Load(filename.c_str());
			}

			std::size_t Size() const
			{
				return mapResources.size();
			}


			void ResourceList(std::string & list, const std::string splitter = "\n")
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
					list += splitter +res.first;
				}
			}


			virtual bool Erase(const char * filename)
			{
				std::string file(filename);

				
				auto it = mapResources.find(file);
				if (it != mapResources.end())
				{
					UnloadResource( it->second );
					mapResources.erase(it);
					return true;
				}

				return false;
			}


			inline bool Erase(const std::string & filename)
			{
				return Erase(filename.c_str());
			}


			virtual bool Erase(ResT & resource)
			{
				for (auto & res : mapResources)
				{
					
					if ((*res.second) == resource)
					{
						UnloadResource(res.second);
						mapResources.erase(res.first);
						return 1;
					}
				}

				return 0;
	
			}




			virtual void Clear()
			{
				for (auto & res : mapResources)
				{
					UnloadResource(res.second);
				}
				mapResources.clear();
			}


			void setDefault(ResT * resource)
			{
				ResT * defaultResource = resource;
			}

		protected:

			//si la ressource n'existe pas déjà. LoadResourceFromFile est appelée par Load
			//LoadResourceFromFile renvoie le pointeur sur la ressource
			virtual  ResT* LoadResourceFromFile(const std::string & filename) = 0;

			//Destructeur de la ressource appelée par Clear or Erase.
			virtual void UnloadResource(ResT* ressource)
			{
				delete ressource;
			}

			//std::map<std::string, ResT> mapResources;
		    std::unordered_map<std::string, ResT*> mapResources;

			ResT *defaultResource = nullptr;
	};
}









#endif //_BH3D_RESOURCEMANAGER_H_
