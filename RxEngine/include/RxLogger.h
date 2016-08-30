//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once



#ifndef _RXLOGGER_H_
#define _RXLOGGER_H_

#define RX_LOGGER_FILE "Rxlogger.log"

#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>


//#define RXLOGGER RxLogger::Instance().getFileLogger()<<"--------"
#define RXLOGGER(msg) RxLogger::Instance().getFileLogger()			<<"------------"<<msg<<std::endl
#ifdef _DEBUG
#define RXLOGGERERROR(msg) RxLogger::Instance().getFileLogger()		<<"<ERROR>-----"<<msg<<" -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl
#define RXLOGGERWARNING(msg) RxLogger::Instance().getFileLogger()	<<"<WARNING>---"<<msg<<" -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl
#else
#define RXLOGGERERROR(msg) RxLogger::Instance().getFileLogger()		<<"<ERROR>-----"<<msg<<std::endl
#define RXLOGGERWARNING(msg) RxLogger::Instance().getFileLogger()	<<"<WARNING>---"<<msg<<std::endl
#endif


class RxLogger
{
	private:
		std::ofstream      m_File;
		RxLogger();
		~RxLogger();
		
	public:
		static RxLogger &Instance();

		std::ofstream& getFileLogger(){return m_File;}


};

#endif