#include "LoggingManager.h"

#include <glad/glad.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

namespace Logging
{
	void HandleLogOutput(const std::string& log_msg, Severity severity_level)
	{
	#ifdef _DEBUG
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (severity_level)
		{
		case Severity::NOTIFICATION:
			SetConsoleTextAttribute(console_handle, 11);
			std::cout << "Notification: " << log_msg << std::endl;
			break;
		case Severity::WARNING:
			SetConsoleTextAttribute(console_handle, 14);
			std::cout << "Warning: " << log_msg << std::endl;
			break;
		case Severity::FATAL:
			SetConsoleTextAttribute(console_handle, 12);
			std::cout << "Notification: " << log_msg << std::endl;
			assert(false);
			break;
		}
	#endif
	}
}