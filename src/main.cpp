#include "Events.h"
#include "FormLoader.h"
#include "Hooks.h"
#include "Serialization.h"
#include "SPUI.h"
#include "Translation.h"
#include <stddef.h>

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
	switch (message->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		Translation::Install();
		FormLoader::GetSingleton()->CacheGameAddresses();
		FormLoader::GetSingleton()->LoadAllForms();
		Settings::GetSingleton()->LoadSettings();
		Hooks::Install();
		Events::Register();
		SPUI::Register();
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    REL::Module::reset();

    // Message Listener
    auto g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));

    if (!g_messaging) {
        logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");
        return false;
    }

    logger::info("{} v{} is loading..."sv, Plugin::NAME, Plugin::VERSION.string());

    SKSE::Init(a_skse);
    SKSE::AllocTrampoline(42);

    g_messaging->RegisterListener("SKSE", SKSEMessageHandler);

    // Serialization to save/load information
	if (auto serialization = SKSE::GetSerializationInterface()) {
		serialization->SetUniqueID(Serialization::ID);
		serialization->SetSaveCallback(&Serialization::SaveCallback);
		serialization->SetLoadCallback(&Serialization::LoadCallback);
		serialization->SetRevertCallback(&Serialization::RevertCallback);
	}

	// Register the SKSE Menu
	logger::info("{} has finished loading.", Plugin::NAME);

    return true;
}
