#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <spdlog/spdlog.h>

namespace Engine
{
	
class Log
{
public:
	static void init();

	static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

private:
	static std::shared_ptr<spdlog::logger> s_logger;
};

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#define LOG_TRACE(...)         ::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::Log::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Log::getLogger()->critical(__VA_ARGS__)

}
