// careful, this is not compiled
// only sample usage
#pragma once
#include <log_helper.hpp>

#ifdef _STACK_TRACING_
#include <boost/stacktrace.hpp>
#endif

// LoggingLevel
enum class LL : int {
	Full = -1,
	Debug = 0,
	Info,
	Warning,
	Error,
	Fatal,
};

template <typename... Types>
void ConsoleLog(const LL, const Types&...);

// implements
template <typename... Types>
inline void ConsoleLog(const LL _logging_level, const Types&... _args) {
	try {
		//from program setting
		static const auto option_level = LL::Full;

		if (static_cast<int>(_logging_level) < option_level)
			return;

		// // caching
		//std::string string_buffer;
		//log_helper::Stringnizer helper(string_buffer);

		//local usage
		log_helper::Stringnizer helper;

		str_buffer += helper.ToString(_args...);

#ifdef _STACK_TRACING_
		switch (_logging_level) {
			case LL::Full:
			case LL::Error:
			case LL::Fatal:
				str_buffer += '\n';
				str_buffer += boost::stacktrace::to_string(boost::stacktrace::stacktrace());
				break;
			default:
				break;
		}
#endif

		switch (_logging_level) {
			case LL::Full:
			case LL::Debug:
				std::cout << "[DEBUG]" << str_buffer << std::endl;
				break;
			case LL::Info:
				std::cout << "[INFO]" << str_buffer << std::endl;
				break;
			case LL::Warning:
				std::cout << "[WARNING]" << str_buffer << std::endl;
				break;
			case LL::Error:
				std::cerr << "[ERROR]" << str_buffer << std::endl;
				break;
			case LL::Fatal:
				std::cerr << "[FATAL]" << str_buffer << std::endl;
				exit(1);
				break;
			default:
				break;
		}
	} catch (const std::exception& e) {
		LOG_ERROR("logging fail, %s", e.what());
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
// macro
#ifdef _USE_PRETTY_FUNCTION_
  #if defined(_MSC_VER)
    #define __FUNCTION_OUT__ __FUNCSIG__
  #else
    #define __FUNCTION_OUT__ __PRETTY_FUNCTION__	  
  #endif
#else
  #define __FUNCTION_OUT__ __func__
#endif

#define CONSOLE_LOG(LOGLEVEL, ...) \
	ConsoleLog(LOGLEVEL, __FILE__, ":", __LINE__, " ", __FUNCTION_OUT__, "\n", __VA_ARGS__)
#define CONSOLE_LOG_IF(CONDITION, LOGLEVEL, ...) \
	if (CONDITION)                                  \
	ConsoleLog(LOGLEVEL, __FILE__, __LINE__, __FUNCTION_OUT__, __VA_ARGS__)

#define TO_STRING(...) log_helper::Stringnizer s; s.ToString(__VA_ARGS__)
#define TO_C_STR(...) log_helper::Stringnizer s; s.ToString(__VA_ARGS__).c_str()

#define RANGE_TO_STRING(...) log_helper::Stringnizer s; s.RangeToString(__VA_ARGS__)


