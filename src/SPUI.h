#pragma once
#include "Settings.h"
#include "include\SKSEMenuFramework.h"

using namespace ImGuiMCP;

namespace SPUI {
    void Register();

    // Static Variables
    static std::vector<bool> waitKeys(30, false);
    static std::vector<std::string> effectTypes = { "Highest", "Lowest", "Optimal" };
    static std::vector<std::string> hotkeyTypes = { "Highest", "Lowest" };
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

    // Additional Functions
    bool AutoSystemEntry(PotionData& pData, std::vector<std::string>& eTypes, const char* category);
    bool DrawHotkeyUI(const char* header, int& hotkey, int waitIndex);
    bool DrawEffectSelect(PotionData& pData, std::vector<std::string>& effectsPositive, const char* header);
    int modifierKeySwap(int idCode);
    int ImGuiKeyToIDCode(ImGuiKey key);
	ImGuiKey IDCodeToImGuiKey(int idCode);
    std::string MakeLabel(const char* label, const char* category);

};