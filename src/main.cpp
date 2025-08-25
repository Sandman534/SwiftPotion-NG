#include "Events.h"
#include "FormLoader.h"
#include "Hooks.h"
#include "Serialization.h"
#include "SPUI.h"
#include <stddef.h>

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

void SetupLog()
{
	auto logsFolder = SKSE::log::log_directory();
	if (!logsFolder)
		SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
	auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
	auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
	auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
	auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
	spdlog::set_default_logger(std::move(loggerPtr));
	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::trace);
}

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	auto settings = Settings::GetSingleton();

	switch (a_msg->type) {
	case SKSE::MessagingInterface::kInputLoaded:
		Events::RegisterInput();
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		FormLoader::GetSingleton()->LoadAllForms();
		settings->LoadSettings();
		break;
	}
}

SKSEPluginLoad(const LoadInterface* skse)
{
	SetupLog();
	logger::info("{} {} is loading...", Plugin::NAME, Plugin::Version.string());

	Init(skse);
	FormLoader::GetSingleton()->CacheGameAddresses();
	SKSE::AllocTrampoline(42);
	Hooks::Install();
	Events::Register();

	// Form Loader
	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener)) {
		return false;
	}

	// Load Saved Hotkey Data
	if (auto serialization = SKSE::GetSerializationInterface()) {
		serialization->SetUniqueID(Serialization::ID);
		serialization->SetSaveCallback(&Serialization::SaveCallback);
		serialization->SetLoadCallback(&Serialization::LoadCallback);
		serialization->SetRevertCallback(&Serialization::RevertCallback);
	}

	logger::info("{} has finished loading.", Plugin::NAME);
	SPUI::Register();
	return true;
}
