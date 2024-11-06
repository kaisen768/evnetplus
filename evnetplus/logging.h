#pragma once

#ifdef __cplusplus

#include <fstream>

// #define H_EVNETPLUS_DETAILED_DEBUG

#if defined (H_EVNETPLUS_DETAILED_DEBUG)

#define LOG_TRACE std::cout << std::endl << __FILE__ << ":" << __LINE__ << " " 
#define LOG_DEBUG std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO  std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define LOG_WARN  std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define LOG_ERROR std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define LOG_FATAL std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define DLOG_TRACE std::cout << std::endl << __FILE__ << ":" << __LINE__ << " "
#define DLOG_WARN std::cout << std::endl << __PRETTY_FUNCTION__ << " this=" << this << " "

#else

#define LOG_TRACE std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define LOG_DEBUG std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO  std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define LOG_WARN  std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define LOG_ERROR std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define LOG_FATAL std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define DLOG_TRACE std::ofstream("/dev/null") << __FILE__ << ":" << __LINE__ << " "
#define DLOG_WARN std::ofstream("/dev/null") << __PRETTY_FUNCTION__ << " this=" << this << " "

#endif

#define CHECK_NOTnullptr(val) LOG_ERROR << "'" #val "' Must be non nullptr";

#endif // end of define __cplusplus

//#ifdef _DEBUG
//#ifdef assert
//#undef assert
//#endif
//#define assert(expr)  { if (!(expr)) { LOG_FATAL << #expr ;} }
//#endif
