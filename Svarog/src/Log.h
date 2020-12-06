#ifndef LOG_H_
#define LOG_H_


using namespace std;

class Log
{
public:
	static void init();
	static inline std::shared_ptr<spdlog::logger>& get_core_logger() {
		return core_logger;
	}

	static inline std::shared_ptr<spdlog::logger>& get_client_logger() {
		return client_logger;
	}

private:
	static std::shared_ptr<spdlog::logger> core_logger;
	static std::shared_ptr<spdlog::logger> client_logger;

};

#endif /* LOG_H_ */
