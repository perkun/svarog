
#ifdef SV_DEBUG

	#define CORE_TRACE(...) Log::get_core_logger()->trace(__VA_ARGS__)
	#define CORE_INFO(...)  Log::get_core_logger()->info(__VA_ARGS__)
	#define CORE_WARN(...)  Log::get_core_logger()->warn(__VA_ARGS__)
	#define CORE_ERROR(...) Log::get_core_logger()->error(__VA_ARGS__)
	#define CORE_CRITICAL(...) Log::get_core_logger()->critical(__VA_ARGS__)

	#define TRACE(...) Log::get_client_logger()->trace(__VA_ARGS__)
	#define INFO(...)  Log::get_client_logger()->info(__VA_ARGS__)
	#define WARN(...)  Log::get_client_logger()->warn(__VA_ARGS__)
	#define ERROR(...) Log::get_client_logger()->error(__VA_ARGS__)
	#define CRITICAL(...) Log::get_client_logger()->critical(__VA_ARGS__)

	#define ASSERT(x, msg) if (!(x)) { CRITICAL(msg); } assert(x);
	#define CORE_ASSERT(x, msg) if (!(x)) { CORE_CRITICAL(msg); } assert(x);
#else

	#define CORE_TRACE(...)
	#define CORE_INFO(...)
	#define CORE_WARN(...)
	#define CORE_ERROR(...)
	#define CORE_FATAL(...)

	#define TRACE(...)
	#define INFO(...)
	#define WARN(...)
	#define ERROR(...)
	#define FATAL(...)

	#define ASSERT(x, msg)
	#define CORE_ASSERT(x, msg)
#endif
