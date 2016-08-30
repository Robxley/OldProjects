#include "RxLogger.h"


RxLogger::RxLogger() : m_File(RX_LOGGER_FILE)
{
	 // On vérifie que le fichier est bien ouvert
	if (!m_File.is_open())
	{
        return;
	}
    // Inscription de l'en-tête du fichier
    m_File << "  ========================================" << std::endl;
    m_File << "   RxEngine - Logger File - OPEN " << std::endl;
    m_File << "  ========================================" << std::endl << std::endl;
}
RxLogger::~RxLogger()
{
	if(m_File.is_open())
	{
		m_File << std::endl;
		m_File << "  ========================================" << std::endl;
		m_File << "   RxEngine - Logger File - CLOSE " << std::endl;
		m_File << "  ========================================" << std::endl << std::endl;
	}
}

RxLogger &RxLogger::Instance()
{
	static RxLogger instance;
	return instance;
}

