#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "../../Debug/Instrumentor.h"

using namespace Engine;

std::shared_ptr<spdlog::logger> Log::s_logger;

void Log::init()
{
	PROFILE_FUNC()

	std::vector<spdlog::sink_ptr> log_sinks;
	log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Debug.log", true));

	log_sinks[0]->set_pattern("%^[%T]: %v%$");
	log_sinks[1]->set_pattern("[%T] [%l]: %v");

	s_logger = make_shared<spdlog::logger>("Logger", begin(log_sinks), end(log_sinks));
	spdlog::register_logger(s_logger);
	s_logger->set_level(spdlog::level::trace);
	s_logger->flush_on(spdlog::level::trace);
}
