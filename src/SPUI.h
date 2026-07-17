#pragma once
#include "Settings.h"
#include "SKSEMenuFramework.h"
#include <nlohmann/json.hpp>

using namespace ImGuiMCP;

namespace SPUI {
    void Register();

    // Static Variables
    static std::vector<bool> waitKeys(30, false);
    static std::vector<std::string> effectTypes = { };
    static std::vector<std::string> hotkeyTypes = { };
    static std::vector<RE::BSKeyboardDevice::Key> modifierKeys = { 
        RE::BSKeyboardDevice::Key::kRightShift,
        RE::BSKeyboardDevice::Key::kLeftShift,
        RE::BSKeyboardDevice::Key::kRightControl,
        RE::BSKeyboardDevice::Key::kLeftControl,
        RE::BSKeyboardDevice::Key::kRightAlt,
        RE::BSKeyboardDevice::Key::kLeftAlt
    };
    static int hotkeyIndex = 0;

    // Render Functions
    void __stdcall RenderSettings();
	void __stdcall RenderAutoHealth();
	void __stdcall RenderAutoStamina();
	void __stdcall RenderAutoMagicka();
    void __stdcall RenderAutoExtra();
    void __stdcall RenderEffectHotkeys();

    inline constexpr const char* translationsFolder = "Data\\SKSE\\Plugins\\SwiftPotionNG_Translation.json";
    inline const char* defaultTranslation = "Missing Translation";
    static inline std::map<std::string, const char*> translations;

    // Translations
    void InstallTranslation();
    const char* Translate(std::string key);

    // Additional Functions
    bool AutoSystemEntry(PotionData& pData, std::vector<std::string>& eTypes, const char* category);
    bool DrawHotkeyUI(const char* header, int& hotkey, int waitIndex);
    bool DrawEffectSelect(PotionData& pData, std::vector<std::string>& effectsPositive, const char* header);
    int modifierKeySwap(int idCode);
    int ImGuiKeyToIDCode(ImGuiKey key);
	ImGuiKey IDCodeToImGuiKey(int idCode);
    std::string MakeLabel(const char* label, const char* category);

};