#pragma once

#include <vector>
#include <unordered_map>
#include <sstream>
#include <Windows.h>

enum class ELogPrintType
{
	Console,
	VisualOutput,
	All
};

enum class ELogLevel
{
	Debug,
	Info,
	Warning,
	Error,
	Fatal,
	End
};

#define DEBUG_LEVEL		"[DEBUG]"
#define INFO_LEVEL		"[INFO]"
#define WARNING_LEVEL	"[WARNING]"
#define ERROR_LEVEL		"[ERROR]"
#define FATAL_LEVEL		"[FATAL]"

class CLog
{
private:
	static std::vector<std::string>	mvecLog;
	static HANDLE	hConsole;
	static ELogLevel mLevel;
	static ELogPrintType mPrintType;


private:
	template<typename T, typename... Args>
	static void WriteLog(std::ostringstream& stream, const T& data, const Args& ...args)
	{
		stream << data;
		WriteLog(stream, args...);
	}

	template<typename T>
	static void WriteLog(std::ostringstream& stream, const T& data)
	{
		stream << data;
	}

	//template<typename T>
	//static void WriteLog()
	//{
	//	//nothing
	//}

	template<typename T, typename... Args>
	static void PrintLog(const char* logLevel, const T& data, const Args& ...args)
	{
		SYSTEMTIME	sysTime;

		// UTC 표준시간
		//GetSystemTime(&sysTime);

		// 지역 시간
		GetLocalTime(&sysTime);

		char	TimeText[128] = {};

		sprintf_s(TimeText, "%d.%d.%d %d:%d:%d",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		std::ostringstream stream;
		stream << TimeText;
		stream << logLevel;

		WriteLog(stream, data, args...);
		stream << '\n';

		mvecLog.emplace_back(stream.str());

		switch (mPrintType)
		{
		case ELogPrintType::Console:
		{
			DWORD	Number = 0;
			WriteConsoleA(hConsole, stream.str().c_str(),
				(DWORD)stream.str().length(), &Number, nullptr);
		}
		break;
		case ELogPrintType::VisualOutput:
			OutputDebugStringA(stream.str().c_str());
			break;
		case ELogPrintType::All:
		{
			DWORD	Number = 0;
			WriteConsoleA(hConsole, stream.str().c_str(),
				(DWORD)stream.str().length(), &Number, nullptr);
			OutputDebugStringA(stream.str().c_str());
		}
		break;
		}
	}

public:
	static bool Init(ELogLevel logLevel = ELogLevel::Debug, ELogPrintType printType = ELogPrintType::All);
	static void Destroy();
	static void PrintLog(const std::string& Text,
		ELogPrintType PrintType = ELogPrintType::All);
	static void SaveLog();

	template<typename T, typename... Args>
	static void Debug(const T& data, const Args& ...args)
	{
		if (mLevel > ELogLevel::Debug)
			return;

		PrintLog(DEBUG_LEVEL, data, args...);
	}

	template<typename T, typename... Args>
	static void Info(const T& data, const Args& ...args)
	{
		if (mLevel > ELogLevel::Info)
			return;

		PrintLog(INFO_LEVEL, data, args...);
	}

	template<typename T, typename... Args>
	static void Warning(const T& data, const Args& ...args)
	{
		if (mLevel > ELogLevel::Warning)
			return;

		PrintLog(WARNING_LEVEL, data, args...);
	}

	template<typename T, typename... Args>
	static void Error(const T& data, const Args& ...args)
	{
		if (mLevel > ELogLevel::Error)
			return;

		PrintLog(ERROR_LEVEL, data, args...);
	}

	template<typename T, typename... Args>
	static void Fatal(const T& data, const Args& ...args)
	{
		if (mLevel > ELogLevel::Fatal)
			return;

		PrintLog(FATAL_LEVEL, data, args...);
	}

	template<typename T, typename... Args>
	static void Logging(ELogLevel level, const T& data, const Args& ...args)
	{
		if (mLevel > level)
			return;

		switch (level)
		{
		case ELogLevel::Debug: Debug(data, args...); break;
		case ELogLevel::Info: Info(data, args...); break;
		case ELogLevel::Warning: Warning(data, args...); break;
		case ELogLevel::Error: Error(data, args...); break;
		case ELogLevel::Fatal: Fatal(data, args...); break;
		default:
			break;
		}
	}
};

