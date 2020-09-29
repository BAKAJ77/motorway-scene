#pragma once
#include <string>

#define OutputLog(msg, severity) Logging::HandleLogOutput(msg, severity)

namespace Logging
{
	enum class Severity
	{
		NOTIFICATION,
		WARNING,
		FATAL
	};

	extern void HandleLogOutput(const std::string& log_msg, Severity severity_level); // Outputs log to console/file (depending on build config)
}