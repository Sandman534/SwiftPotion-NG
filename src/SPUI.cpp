#include "SPUI.h"
#include "Translation.h"

using Translation::Translate;

namespace SPUI {
	void Register() {
		if (!SKSEMenuFramework::IsInstalled()) return;

		// Setup Display Options
		effectTypes.push_back(Translate("Setting.EffectType1"));
		effectTypes.push_back(Translate("Setting.EffectType2"));
		effectTypes.push_back(Translate("Setting.EffectType3"));

		// Setup Display Names
		hotkeyTypes.push_back(Translate("Setting.EffectType1"));
		hotkeyTypes.push_back(Translate("Setting.EffectType2"));

		// Render Menu
		SKSEMenuFramework::SetSection(Translate("Settings.ModName"));
		SKSEMenuFramework::AddSectionItem(Translate("General.Name"), RenderSettings);
		SKSEMenuFramework::AddSectionItem(Translate("Auto.Health"), RenderAutoHealth);
		SKSEMenuFramework::AddSectionItem(Translate("Auto.Magicka"), RenderAutoMagicka);
		SKSEMenuFramework::AddSectionItem(Translate("Auto.Stamina"), RenderAutoStamina);
		SKSEMenuFramework::AddSectionItem(Translate("Auto.Effects"), RenderAutoExtra);
		SKSEMenuFramework::AddSectionItem(Translate("Effect.Name"), RenderEffectHotkeys);
	}

	void __stdcall RenderSettings()
	{
		bool optionsChanges = false;
		Settings* spSettings = Settings::GetSingleton();

		if (CollapsingHeader(Translate("General.Options"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (Checkbox(Translate("General.Enable"), &spSettings->SPNG_Enabled))
				optionsChanges = true;
			if (Checkbox(Translate("General.Notifications"), &spSettings->SPNG_Notifications))
				optionsChanges = true;
			if (Checkbox(Translate("General.Persist"), &spSettings->SPNG_HotkeyFile))
				optionsChanges = true;

			// Show Save and Load buttons for Hotkey File
			SameLine();
			if (Button(Translate("General.HotkeySave"))) 
				spSettings->SaveHotkeyINI();
			SameLine();
			if (Button(Translate("General.HotkeyLoad")))
				spSettings->LoadHotkeyINI();
		}

		if (CollapsingHeader(Translate("General.Modifier"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (DrawHotkeyUI(Translate("Settings.Modifier1"), spSettings->SPNG_Modifier1, 0))
				optionsChanges = true;
			if (DrawHotkeyUI(Translate("Settings.Modifier2"), spSettings->SPNG_Modifier2, 1))
				optionsChanges = true;
			if (DrawHotkeyUI(Translate("Settings.Modifier3"), spSettings->SPNG_Modifier3, 2))
				optionsChanges = true;
		}

		// Save the settings
		if (optionsChanges) {
			Settings::GetSingleton()->SaveINI();
		}	
	}

	void __stdcall RenderAutoHealth() {
		PotionData& pdRestore = Settings::GetSingleton()->Health_Restore;
		PotionData& pdRegen = Settings::GetSingleton()->Health_Regen;
		PotionData& pdFortify = Settings::GetSingleton()->Health_Fortify;

		bool optionsChanges = false;
		if (CollapsingHeader(Translate("Auto.Settings"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (DrawHotkeyUI(Translate("Settings.Hotkey"), pdRestore.Hotkey, 3))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier1"), &pdRestore.Modifier1))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier2"), &pdRestore.Modifier2))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier3"), &pdRestore.Modifier3))
				optionsChanges = true;
		}

		// Restore Effect
		if (CollapsingHeader(Translate("Auto.Restore"))) {
			if (AutoSystemEntry(pdRestore, effectTypes, "Restore"))
				optionsChanges = true;
		}

		// Regenerate Effect
		if (CollapsingHeader(Translate("Auto.Regenerate"))) {
			if (AutoSystemEntry(pdRegen, hotkeyTypes, "Regenerate"))
				optionsChanges = true;
		}

		// Foritfy Effect
		if (CollapsingHeader(Translate("Auto.Fortify"))) {
			if (AutoSystemEntry(pdFortify, hotkeyTypes, "Fortify"))
				optionsChanges = true;
		}

		// Save the settings
		if (optionsChanges) {
			Settings::GetSingleton()->SaveINI();
		}	
	}

	void __stdcall RenderAutoMagicka() {
		PotionData& pdRestore = Settings::GetSingleton()->Magicka_Restore;
		PotionData& pdRegen = Settings::GetSingleton()->Magicka_Regen;
		PotionData& pdFortify = Settings::GetSingleton()->Magicka_Fortify;

		bool optionsChanges = false;
		if (CollapsingHeader(Translate("Auto.Settings"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (DrawHotkeyUI(Translate("Settings.Hotkey"), pdRestore.Hotkey, 4))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier1"), &pdRestore.Modifier1))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier2"), &pdRestore.Modifier2))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier3"), &pdRestore.Modifier3))
				optionsChanges = true;
		}

		// Restore Effect
		if (CollapsingHeader(Translate("Auto.Restore"))) {
			if (AutoSystemEntry(pdRestore, effectTypes, "Restore"))
				optionsChanges = true;
		}

		// Regenerate Effect
		if (CollapsingHeader(Translate("Auto.Regenerate"))) {
			if (AutoSystemEntry(pdRegen, hotkeyTypes, "Regenerate"))
				optionsChanges = true;
		}

		// Foritfy Effect
		if (CollapsingHeader(Translate("Auto.Fortify"))) {
			if (AutoSystemEntry(pdFortify, hotkeyTypes, "Fortify"))
				optionsChanges = true;
		}

		// Save the settings
		if (optionsChanges) {
			Settings::GetSingleton()->SaveINI();
		}	
	}

	void __stdcall RenderAutoStamina() {
		PotionData& pdRestore = Settings::GetSingleton()->Stamina_Restore;
		PotionData& pdRegen = Settings::GetSingleton()->Stamina_Regen;
		PotionData& pdFortify = Settings::GetSingleton()->Stamina_Fortify;

		bool optionsChanges = false;
		if (CollapsingHeader(Translate("Auto.Settings"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (DrawHotkeyUI(Translate("Settings.Hotkey"), pdRestore.Hotkey, 5))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier1"), &pdRestore.Modifier1))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier2"), &pdRestore.Modifier2))
				optionsChanges = true;
			if (Checkbox(Translate("Settings.Modifier3"), &pdRestore.Modifier3))
				optionsChanges = true;
		}

		// Restore Effect
		if (CollapsingHeader(Translate("Auto.Restore"))) {
			if (AutoSystemEntry(pdRestore, effectTypes, "Restore"))
				optionsChanges = true;
		}

		// Regenerate Effect
		if (CollapsingHeader(Translate("Auto.Regenerate"))) {
			if (AutoSystemEntry(pdRegen, hotkeyTypes, "Regenerate"))
				optionsChanges = true;
		}

		// Foritfy Effect
		if (CollapsingHeader(Translate("Auto.Fortify"))) {
			if (AutoSystemEntry(pdFortify, hotkeyTypes, "Fortify"))
				optionsChanges = true;
		}

		// Save the settings
		if (optionsChanges) {
			Settings::GetSingleton()->SaveINI();
		}	
	}

	void __stdcall RenderAutoExtra() {
		PotionData& pdFire = Settings::GetSingleton()->Resist_Fire;
		PotionData& pdShock = Settings::GetSingleton()->Resist_Shock;
		PotionData& pdFrost = Settings::GetSingleton()->Resist_Frost;
		PotionData& pdMagic = Settings::GetSingleton()->Resist_Magic;
		PotionData& pdDisease = Settings::GetSingleton()->Cure_Disease;
		PotionData& pdPoison = Settings::GetSingleton()->Cure_Poison;

		std::vector<std::string> effectsPositive = Settings::GetSingleton()->Positive_Effects;

		bool optionsChanges = false;
		if (CollapsingHeader(Translate("Auto.Cure"), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (Checkbox(Translate("Auto.CureDisease"), &pdDisease.Enabled))
				optionsChanges = true;
			if (Checkbox(Translate("Auto.CurePoison"), &pdPoison.Enabled))
				optionsChanges = true;
		}

		// Resist Effects
		if (CollapsingHeader(Translate("Auto.Resist"), ImGuiTreeNodeFlags_DefaultOpen)) {

			if (Checkbox(MakeLabel(Translate("Settings.Enabled"), "Resist").c_str(), &pdFire.Enabled)) {
				pdShock.Enabled = pdFire.Enabled;
				pdFrost.Enabled = pdFire.Enabled;
				pdMagic.Enabled = pdFire.Enabled;
				optionsChanges = true;
			}

			// Combat Checkboxes
			if (Checkbox(MakeLabel(Translate("Settings.OnlyCombat"), "Resist").c_str(), &pdFire.CombatOnly)) {
				pdShock.CombatOnly = pdFire.CombatOnly;
				pdFrost.CombatOnly = pdFire.CombatOnly;
				pdMagic.CombatOnly = pdFire.CombatOnly;
				optionsChanges = true;
			}

			// Attribute Threshold
			int resistThreshold = static_cast<int>(pdFire.Threshold* 100);
			if (SliderInt(MakeLabel(Translate("Auto.HealthThreshold"), "Resist").c_str(), &resistThreshold, 0, 100, "%d%%")) {
				pdFire.Threshold = resistThreshold / 100.0f;
				pdShock.Threshold = resistThreshold / 100.0f;
				pdFrost.Threshold = resistThreshold / 100.0f;
				pdMagic.Threshold = resistThreshold / 100.0f;
				optionsChanges = true;
			}

			// Use Type
			if (BeginCombo(MakeLabel(Translate("Settings.UseType"), "Resist").c_str(), hotkeyTypes[pdFire.BestValue].c_str())) {
				for (int i = 0; i < hotkeyTypes.size(); i++) {
					bool is_selected = (pdFire.BestValue == i);
					if (Selectable(hotkeyTypes[i].c_str(), is_selected))
						pdFire.BestValue = i;
						pdShock.BestValue = i;
						pdFrost.BestValue = i;
						pdMagic.BestValue = i;
						optionsChanges = true;
				}
				EndCombo();
			}

			// Resist Fire
			if (DrawEffectSelect(pdFire, effectsPositive, Translate("Auto.ResistFire")))
				optionsChanges = true;
			if (DrawEffectSelect(pdShock, effectsPositive, Translate("Auto.ResistShock")))
				optionsChanges = true;
			if (DrawEffectSelect(pdFrost, effectsPositive, Translate("Auto.ResistFrost")))
				optionsChanges = true;
			if (DrawEffectSelect(pdMagic, effectsPositive, Translate("Auto.ResistMagic")))
				optionsChanges = true;
		}

		// Save the settings
		if (optionsChanges) {
			Settings::GetSingleton()->SaveINI();
		}	
	}

	void __stdcall RenderEffectHotkeys() {
		bool optionsChanges = false;
		std::vector<PotionData>& effectHotkeys = Settings::GetSingleton()->HotkeyRecords; 
		std::vector<std::string> effectList;
		Columns(2);

		if (BeginListBox(Translate("Effect.Hotkey"), ImVec2(-FLT_MIN, 20 * GetTextLineHeightWithSpacing()))) {
			for (int i = 0; i < effectHotkeys.size(); i++)
			{
				const bool isSelected = (i == hotkeyIndex);
				std::string name = std::string(Translate("Effect.Hotkey")) + " " + std::format("{:02}", i + 1) + ": " + effectHotkeys[i].EffectName;
				if (Selectable(name.c_str(), isSelected))
					hotkeyIndex = i;

				if (isSelected)
					SetItemDefaultFocus(); // ensures selected item is focused
			}
			EndListBox();
		}
		NextColumn();

		// The Hotkey Modifications
		if (hotkeyIndex >= 0 && hotkeyIndex < effectHotkeys.size()) {
			PotionData& pData = effectHotkeys[hotkeyIndex];

			// Set the effects based on the Poison variables
			if (pData.Poison)
				effectList = Settings::GetSingleton()->Negative_Effects;
			else
				effectList = Settings::GetSingleton()->Positive_Effects;

			// Effect settings
			if (CollapsingHeader(Translate("Effect.Setting"), ImGuiTreeNodeFlags_DefaultOpen)) {
				// Effect Selection
				auto it = std::find(effectList.begin(), effectList.end(), pData.EffectName);
				auto index = std::distance(effectList.begin(), it);
				if (BeginCombo(Translate("Settings.MagicEffect"), pData.EffectName.c_str())) {
					for (int i = 0; i < effectList.size(); i++) {
						bool is_selected = (index == i);
						if (Selectable(effectList[i].c_str(), is_selected)) {
							pData.EffectName = effectList[i];
							optionsChanges = true;
						}
					}
					EndCombo();
				}

				// Use Type
				if (BeginCombo(Translate("Settings.UseType"), hotkeyTypes[pData.BestValue].c_str())) {
					for (int i = 0; i < hotkeyTypes.size(); i++) {
						bool is_selected = (pData.BestValue == i);
						if (Selectable(hotkeyTypes[i].c_str(), is_selected)) {
							pData.BestValue = i;
							optionsChanges = true;
						}
					}
					EndCombo();
				}
			}

			// Food/Combat Checkboxes
			if (Checkbox(Translate("Settings.Poison"), &pData.Poison)) {
				pData.EffectName = "";
				optionsChanges = true;
			}
			if (Checkbox(Translate("Settings.UseFood"), &pData.UseFood))
				optionsChanges = true;

			// Hotkey Selection
			if (CollapsingHeader(Translate("Effect.HotkeySetting"), ImGuiTreeNodeFlags_DefaultOpen)) {
				if (DrawHotkeyUI(Translate("Settings.Hotkey"), pData.Hotkey, 6))
					optionsChanges = true;
				if (Checkbox(Translate("Settings.Modifier1"), &pData.Modifier1))
					optionsChanges = true;
				if (Checkbox(Translate("Settings.Modifier2"), &pData.Modifier2))
					optionsChanges = true;
				if (Checkbox(Translate("Settings.Modifier3"), &pData.Modifier3))
					optionsChanges = true;
			}
		}

		// Save the settings
		if (optionsChanges &&  Settings::GetSingleton()->SPNG_HotkeyFile) {
			Settings::GetSingleton()->SaveHotkeyINI();
		}	
	}

	std::string MakeLabel(const char* label, const char* category) {
		return std::format("{}##{}", label, category);
	}

	bool AutoSystemEntry(PotionData& pData, std::vector<std::string>& eTypes, const char* category) {
		bool autoChanges = false;

		std::vector<std::string> effectsPositive = Settings::GetSingleton()->Positive_Effects;

		if (Checkbox(MakeLabel(Translate("Settings.Enabled"), category).c_str(), &pData.Enabled))
			autoChanges = true;

		// Effect Selection
		auto it = std::find(effectsPositive.begin(), effectsPositive.end(), pData.EffectName);
		auto index = std::distance(effectsPositive.begin(), it);
		if (BeginCombo(MakeLabel(Translate("Settings.MagicEffect"), category).c_str(), pData.EffectName.c_str())) {
			for (int i = 0; i < effectsPositive.size(); i++) {
				bool is_selected = (index == i);
				if (Selectable(effectsPositive[i].c_str(), is_selected))
					pData.EffectName = effectsPositive[i];
					autoChanges = true;
			}
			EndCombo();
		}

		// Food/Combat Checkboxes
		if (Checkbox(MakeLabel(Translate("Settings.UseFood"), category).c_str(), &pData.UseFood))
			autoChanges = true;
		if (Checkbox(MakeLabel(Translate("Settings.OnlyCombat"), category).c_str(), &pData.CombatOnly))
			autoChanges = true;

		// Attribute Threshold
		int fortifyThreshold = static_cast<int>(pData.Threshold* 100);
		if (SliderInt(MakeLabel(Translate("Auto.Threshold"), category).c_str(), &fortifyThreshold, 0, 100, "%d%%")) {
			pData.Threshold = fortifyThreshold / 100.0f;
			autoChanges = true;
		}

		// Use Type
		if (BeginCombo(MakeLabel(Translate("Settings.UseType"), category).c_str(), eTypes[pData.BestValue].c_str())) {
			for (int i = 0; i < eTypes.size(); i++) {
				bool is_selected = (pData.BestValue == i);
				if (Selectable(eTypes[i].c_str(), is_selected))
					pData.BestValue = i;
					autoChanges = true;
			}
			EndCombo();
		}

		// Save the settings
		return autoChanges;
	}

	int modifierKeySwap(int idCode) {
		switch (idCode) {
		case RE::BSKeyboardDevice::Key::kLeftShift:	   return RE::BSKeyboardDevice::Key::kRightShift;
		case RE::BSKeyboardDevice::Key::kRightShift:   return RE::BSKeyboardDevice::Key::kLeftShift;
		case RE::BSKeyboardDevice::Key::kLeftControl:  return RE::BSKeyboardDevice::Key::kRightControl;
		case RE::BSKeyboardDevice::Key::kRightControl: return RE::BSKeyboardDevice::Key::kLeftControl;
		case RE::BSKeyboardDevice::Key::kLeftAlt:      return RE::BSKeyboardDevice::Key::kRightAlt;
		case RE::BSKeyboardDevice::Key::kRightAlt:     return RE::BSKeyboardDevice::Key::kLeftAlt;
		default: return RE::BSKeyboardDevice::Key::kRightShift;
		}
	}

	bool DrawEffectSelect(PotionData& pData, std::vector<std::string>& listNames, const char* header) {
		bool optionsChanges = false;
		auto it = std::find(listNames.begin(), listNames.end(), pData.EffectName);
		auto index = std::distance(listNames.begin(), it);
		if (BeginCombo(header, pData.EffectName.c_str())) {
			for (int i = 0; i < listNames.size(); i++) {
				bool is_selected = (index == i);
				if (Selectable(listNames[i].c_str(), is_selected))
					pData.EffectName = listNames[i];
					optionsChanges = true;
			}
			EndCombo();
		}

		return optionsChanges;
	}

	bool DrawHotkeyUI(const char* header, int& hotkey, int waitIndex) {
		Text(header);
		SameLine();

		bool keyChanges = false;

		// Show the button with current hotkey name
		if (Button(waitKeys[waitIndex] ? Translate("Settings.WaitKey") : GetKeyName((ImGuiKey)HelperFunctions::IDCodeToImGuiKey(hotkey)))) {

			// Clear out all of the waiting keys and enable the working one
			std::fill(waitKeys.begin(), waitKeys.end(), false);
			waitKeys[waitIndex] = true;
			hotkey = -1;
		}

		// Wait for they key, and grab the input when pressed
		if (waitKeys[waitIndex]) {

			// Look for key input
			for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key++) {
				if (IsKeyPressed((ImGuiKey)key)) {
					hotkey = HelperFunctions::ImGuiKeyToIDCode((ImGuiKey)key);
					waitKeys[waitIndex] = false;
					keyChanges = true;
					break;
				}
			}
		}

		// Show a Swap Button
		if (std::find(modifierKeys.begin(), modifierKeys.end(), hotkey) != modifierKeys.end()) {
			SameLine();
			if (Button(Translate("Settings.Swap"))) {
				hotkey = modifierKeySwap(hotkey);
				keyChanges = true;
			}
		}

		// Show a Clear Button
		if (hotkey >= 0) {
			SameLine();
			if (Button(Translate("Settings.Reset"))) {
				hotkey = -1;
				keyChanges = true;
			}
		}

		// Save the settings
		return keyChanges;
	}

}