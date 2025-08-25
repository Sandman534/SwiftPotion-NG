#pragma once

#include "SKSEMenuFramework.h"
#include "Settings.h"

namespace SPUI {
    void Register();

    // Static Variables
    static std::vector<bool> waitKeys(30, false);
    //static std::vector<std::string> effectsPositive = Settings::GetSingleton()->Positive_Effects;
    //static std::vector<std::string> effectsNegative = Settings::GetSingleton()->Negative_Effects;
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
    void __stdcall RenderEffectHotkeys();

    // Additional Functions
    void AutoSystemEntry(PotionData& pData, std::vector<std::string>& eTypes);
    void DrawHotkeyUI(const char* header, int& hotkey, int waitIndex);
    int modifierKeySwap(int idCode);
    int ImGuiKeyToIDCode(ImGuiKey key);
	ImGuiKey IDCodeToImGuiKey(int idCode);

};