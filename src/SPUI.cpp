#include "SPUI.h"

void SPUI::Register() {

    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }

	// Render Menu
    SKSEMenuFramework::SetSection("Swift Potion NG");
	SKSEMenuFramework::AddSectionItem("General", RenderSettings);
    SKSEMenuFramework::AddSectionItem("Auto Health", RenderAutoHealth);
	SKSEMenuFramework::AddSectionItem("Auto Magicka", RenderAutoMagicka);
	SKSEMenuFramework::AddSectionItem("Auto Stamina", RenderAutoStamina);
    SKSEMenuFramework::AddSectionItem("Effect Hotkeys", RenderEffectHotkeys);
}

void __stdcall SPUI::RenderSettings()
{
	Settings* spSettings = Settings::GetSingleton();
	PotionData& pdCureDisease = Settings::GetSingleton()->Cure_Disease;
	PotionData& pdCurePoison = Settings::GetSingleton()->Cure_Poison;

	if (ImGui::CollapsingHeader("General Options", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Enable Auto System", &spSettings->SPNG_Enabled);
		ImGui::Checkbox("Hotkey Notifications", &spSettings->SPNG_Notifications);
		ImGui::Checkbox("Persistant Effect Hotkeys", &spSettings->SPNG_HotkeyFile);
	}

	if (ImGui::CollapsingHeader("Additional Auto Options", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Cure Disease", &pdCureDisease.Enabled);
		ImGui::Checkbox("Cure Poison", &pdCurePoison.Enabled);
	}

	if (ImGui::CollapsingHeader("Modifier Keys", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawHotkeyUI("Modifier Key 1", spSettings->SPNG_Modifier1, 0);
		DrawHotkeyUI("Modifier Key 2", spSettings->SPNG_Modifier2, 1);
		DrawHotkeyUI("Modifier Key 3", spSettings->SPNG_Modifier3, 2);
	}
}

void __stdcall SPUI::RenderAutoHealth() {
	PotionData& pdRestore = Settings::GetSingleton()->Health_Restore;
	PotionData& pdRegen = Settings::GetSingleton()->Health_Regen;
	PotionData& pdFortify = Settings::GetSingleton()->Health_Fortify;

	if (ImGui::CollapsingHeader("Hotkey Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawHotkeyUI("System Hotkey", pdRestore.Hotkey, 3);
		ImGui::Checkbox("Modifier 1", &pdRestore.Modifier1);
		ImGui::Checkbox("Modifier 2", &pdRestore.Modifier2);
		ImGui::Checkbox("Modifier 3", &pdRestore.Modifier3);
	}

    // Restore Effect
	if (ImGui::CollapsingHeader("Restore Effects")) {
		SPUI::AutoSystemEntry(pdRestore, effectTypes);
	}

    // Regenerate Effect
	if (ImGui::CollapsingHeader("Regenerate Effects")) {
		SPUI::AutoSystemEntry(pdRegen, hotkeyTypes);
	}

    // Foritfy Effect
	if (ImGui::CollapsingHeader("Fortify Effects")) {
		SPUI::AutoSystemEntry(pdFortify, hotkeyTypes);
	}
}

void __stdcall SPUI::RenderAutoMagicka() {
	PotionData& pdRestore = Settings::GetSingleton()->Magicka_Restore;
	PotionData& pdRegen = Settings::GetSingleton()->Magicka_Regen;
	PotionData& pdFortify = Settings::GetSingleton()->Magicka_Fortify;

	if (ImGui::CollapsingHeader("Hotkey Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawHotkeyUI("System Hotkey", pdRestore.Hotkey, 4);
		ImGui::Checkbox("Modifier 1", &pdRestore.Modifier1);
		ImGui::Checkbox("Modifier 2", &pdRestore.Modifier2);
		ImGui::Checkbox("Modifier 3", &pdRestore.Modifier3);
	}

    // Restore Effect
	if (ImGui::CollapsingHeader("Restore Effects")) {
		SPUI::AutoSystemEntry(pdRestore, effectTypes);
	}

    // Regenerate Effect
	if (ImGui::CollapsingHeader("Regenerate Effects")) {
		SPUI::AutoSystemEntry(pdRegen, hotkeyTypes);
	}

    // Foritfy Effect
	if (ImGui::CollapsingHeader("Fortify Effects")) {
		SPUI::AutoSystemEntry(pdFortify, hotkeyTypes);
	}
}

void __stdcall SPUI::RenderAutoStamina() {
	PotionData& pdRestore = Settings::GetSingleton()->Stamina_Restore;
	PotionData& pdRegen = Settings::GetSingleton()->Stamina_Regen;
	PotionData& pdFortify = Settings::GetSingleton()->Stamina_Fortify;

	if (ImGui::CollapsingHeader("Hotkey Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		DrawHotkeyUI("System Hotkey", pdRestore.Hotkey, 5);
		ImGui::Checkbox("Modifier 1", &pdRestore.Modifier1);
		ImGui::Checkbox("Modifier 2", &pdRestore.Modifier2);
		ImGui::Checkbox("Modifier 3", &pdRestore.Modifier3);
	}

    // Restore Effect
	if (ImGui::CollapsingHeader("Restore Effects")) {
		SPUI::AutoSystemEntry(pdRestore, effectTypes);
	}

    // Regenerate Effect
	if (ImGui::CollapsingHeader("Regenerate Effects")) {
		SPUI::AutoSystemEntry(pdRegen, hotkeyTypes);
	}

    // Foritfy Effect
	if (ImGui::CollapsingHeader("Fortify Effects")) {
		SPUI::AutoSystemEntry(pdFortify, hotkeyTypes);
	}
}

void __stdcall SPUI::RenderEffectHotkeys() {
	std::vector<PotionData>& effectHotkeys = Settings::GetSingleton()->HotkeyRecords; 
	std::vector<std::string> effectList;
	ImGui::Columns(2);

	if (ImGui::BeginListBox("Hotkeys", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing()))) {
		for (int i = 0; i < effectHotkeys.size(); i++)
		{
			const bool isSelected = (i == hotkeyIndex);
			std::string name = "Hotkey " + std::format("{:02}", i + 1) + ": " + effectHotkeys[i].EffectName;
			if (ImGui::Selectable(name.c_str(), isSelected))
				hotkeyIndex = i;

			if (isSelected)
				ImGui::SetItemDefaultFocus(); // ensures selected item is focused
		}
		ImGui::EndListBox();
	}
	ImGui::NextColumn();

	// The Hotkey Modifications
	if (hotkeyIndex >= 0 && hotkeyIndex < effectHotkeys.size()) {
		PotionData& pData = effectHotkeys[hotkeyIndex];

		// Set the effects based on the Poison variables
		if (pData.Poison)
			effectList = Settings::GetSingleton()->Negative_Effects;
		else
			effectList = Settings::GetSingleton()->Positive_Effects;

		// Effect settings
		if (ImGui::CollapsingHeader("Effect Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			// Effect Selection
			auto it = std::find(effectList.begin(), effectList.end(), pData.EffectName);
			int index = std::distance(effectList.begin(), it);
			if (ImGui::BeginCombo("Magic Effect", pData.EffectName.c_str())) {
				for (int i = 0; i < effectList.size(); i++) {
					bool is_selected = (index == i);
					if (ImGui::Selectable(effectList[i].c_str(), is_selected))
						pData.EffectName = effectList[i];
				}
				ImGui::EndCombo();
			}

			// Use Type
			if (ImGui::BeginCombo("Use Type", hotkeyTypes[pData.BestValue].c_str())) {
				for (int i = 0; i < hotkeyTypes.size(); i++) {
					bool is_selected = (pData.BestValue == i);
					if (ImGui::Selectable(hotkeyTypes[i].c_str(), is_selected))
						pData.BestValue = i;
				}
				ImGui::EndCombo();
			}
		}

		// Food/Combat Checkboxes
		if (ImGui::Checkbox("Poison", &pData.Poison)) {
			pData.EffectName = "";
		}
		ImGui::Checkbox("Use Food", &pData.UseFood);

		// Hotkey Selection
		if (ImGui::CollapsingHeader("Hotkey Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawHotkeyUI("System Hotkey", pData.Hotkey, 6);
			ImGui::Checkbox("Modifier 1", &pData.Modifier1);
			ImGui::Checkbox("Modifier 2", &pData.Modifier2);
			ImGui::Checkbox("Modifier 3", &pData.Modifier3);
		}
	}

	//ImGui::ListBox
}

void SPUI::AutoSystemEntry(PotionData& pData, std::vector<std::string>& eTypes) {
	std::vector<std::string> effectsPositive = Settings::GetSingleton()->Positive_Effects;

	ImGui::Checkbox("Enabled", &pData.Enabled);

	// Effect Selection
	auto it = std::find(effectsPositive.begin(), effectsPositive.end(), pData.EffectName);
	int index = std::distance(effectsPositive.begin(), it);
	if (ImGui::BeginCombo("Magic Effect", pData.EffectName.c_str())) {
		for (int i = 0; i < effectsPositive.size(); i++) {
			bool is_selected = (index == i);
			if (ImGui::Selectable(effectsPositive[i].c_str(), is_selected))
				pData.EffectName = effectsPositive[i];
		}
		ImGui::EndCombo();
	}

	// Food/Combat Checkboxes
	ImGui::Checkbox("Use Food", &pData.UseFood);
	ImGui::Checkbox("Only in Combat", &pData.CombatOnly);

	// Attribute Threshold
    int fortifyThreshold = static_cast<int>(pData.Threshold* 100);
	if (ImGui::SliderInt("Threshold", &fortifyThreshold, 0, 100, "%d%%"))
		pData.Threshold = fortifyThreshold / 100.0f;

	// Use Type
	if (ImGui::BeginCombo("Use Type", eTypes[pData.BestValue].c_str())) {
		for (int i = 0; i < eTypes.size(); i++) {
			bool is_selected = (pData.BestValue == i);
			if (ImGui::Selectable(eTypes[i].c_str(), is_selected))
				pData.BestValue = i;
		}
		ImGui::EndCombo();
	}
}

int SPUI::modifierKeySwap(int idCode) {
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

void SPUI::DrawHotkeyUI(const char* header, int& hotkey, int waitIndex) {
    ImGui::Text(header);
    ImGui::SameLine();

    // Show the button with current hotkey name
	if (ImGui::Button(waitKeys[waitIndex] ? "Press a key..." : ImGui::GetKeyName((ImGuiKey)IDCodeToImGuiKey(hotkey)))) {

		// Clear out all of the waiting keys and enable the working one
		std::fill(waitKeys.begin(), waitKeys.end(), false);
        waitKeys[waitIndex] = true;
        hotkey = -1;
    }

    // If waiting, check all keys
    if (waitKeys[waitIndex]) {

		// Look for key input
        for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key++) {
            if (ImGui::IsKeyPressed((ImGuiKey)key)) {
				hotkey = ImGuiKeyToIDCode((ImGuiKey)key);
                waitKeys[waitIndex] = false;
                break;
            }
        }
    }
	// Show a Swap Button
	if (std::find(modifierKeys.begin(), modifierKeys.end(), hotkey) != modifierKeys.end()) {
		ImGui::SameLine();
		if (ImGui::Button("Swap"))
			hotkey = modifierKeySwap(hotkey);
	}

	// Show a Clear Button
	if (hotkey >= 0) {
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
			hotkey = -1;
	}
}

ImGuiKey SPUI::IDCodeToImGuiKey(int idCode)
{
	switch (idCode) {
	case RE::BSKeyboardDevice::Key::kA: return ImGuiKey_A;
	case RE::BSKeyboardDevice::Key::kB: return ImGuiKey_B;
	case RE::BSKeyboardDevice::Key::kC: return ImGuiKey_C;
	case RE::BSKeyboardDevice::Key::kD: return ImGuiKey_D;
	case RE::BSKeyboardDevice::Key::kE: return ImGuiKey_E;
	case RE::BSKeyboardDevice::Key::kF: return ImGuiKey_F;
	case RE::BSKeyboardDevice::Key::kG: return ImGuiKey_G;
	case RE::BSKeyboardDevice::Key::kH: return ImGuiKey_H;
	case RE::BSKeyboardDevice::Key::kI: return ImGuiKey_I;
	case RE::BSKeyboardDevice::Key::kJ: return ImGuiKey_J;
	case RE::BSKeyboardDevice::Key::kK: return ImGuiKey_K;
	case RE::BSKeyboardDevice::Key::kL: return ImGuiKey_L;
	case RE::BSKeyboardDevice::Key::kM: return ImGuiKey_M;
	case RE::BSKeyboardDevice::Key::kN: return ImGuiKey_N;
	case RE::BSKeyboardDevice::Key::kO: return ImGuiKey_O;
	case RE::BSKeyboardDevice::Key::kP: return ImGuiKey_P;
	case RE::BSKeyboardDevice::Key::kQ: return ImGuiKey_Q;
	case RE::BSKeyboardDevice::Key::kR: return ImGuiKey_R;
	case RE::BSKeyboardDevice::Key::kS: return ImGuiKey_S;
	case RE::BSKeyboardDevice::Key::kT: return ImGuiKey_T;
	case RE::BSKeyboardDevice::Key::kU: return ImGuiKey_U;
	case RE::BSKeyboardDevice::Key::kV: return ImGuiKey_V;
	case RE::BSKeyboardDevice::Key::kW: return ImGuiKey_W;
	case RE::BSKeyboardDevice::Key::kX: return ImGuiKey_X;
	case RE::BSKeyboardDevice::Key::kY: return ImGuiKey_Y;
	case RE::BSKeyboardDevice::Key::kZ: return ImGuiKey_Z;

	case RE::BSKeyboardDevice::Key::kNum0: return ImGuiKey_0;
	case RE::BSKeyboardDevice::Key::kNum1: return ImGuiKey_1;
	case RE::BSKeyboardDevice::Key::kNum2: return ImGuiKey_2;
	case RE::BSKeyboardDevice::Key::kNum3: return ImGuiKey_3;
	case RE::BSKeyboardDevice::Key::kNum4: return ImGuiKey_4;
	case RE::BSKeyboardDevice::Key::kNum5: return ImGuiKey_5;
	case RE::BSKeyboardDevice::Key::kNum6: return ImGuiKey_6;
	case RE::BSKeyboardDevice::Key::kNum7: return ImGuiKey_7;
	case RE::BSKeyboardDevice::Key::kNum8: return ImGuiKey_8;
	case RE::BSKeyboardDevice::Key::kNum9: return ImGuiKey_9;

	case RE::BSKeyboardDevice::Key::kF1:  return ImGuiKey_F1;
	case RE::BSKeyboardDevice::Key::kF2:  return ImGuiKey_F2;
	case RE::BSKeyboardDevice::Key::kF3:  return ImGuiKey_F3;
	case RE::BSKeyboardDevice::Key::kF4:  return ImGuiKey_F4;
	case RE::BSKeyboardDevice::Key::kF5:  return ImGuiKey_F5;
	case RE::BSKeyboardDevice::Key::kF6:  return ImGuiKey_F6;
	case RE::BSKeyboardDevice::Key::kF7:  return ImGuiKey_F7;
	case RE::BSKeyboardDevice::Key::kF8:  return ImGuiKey_F8;
	case RE::BSKeyboardDevice::Key::kF9:  return ImGuiKey_F9;
	case RE::BSKeyboardDevice::Key::kF10: return ImGuiKey_F10;
	case RE::BSKeyboardDevice::Key::kF11: return ImGuiKey_F11;
	case RE::BSKeyboardDevice::Key::kF12: return ImGuiKey_F12;

	case RE::BSKeyboardDevice::Key::kUp:	return ImGuiKey_UpArrow;
	case RE::BSKeyboardDevice::Key::kDown:	return ImGuiKey_DownArrow;
	case RE::BSKeyboardDevice::Key::kLeft:	return ImGuiKey_LeftArrow;
	case RE::BSKeyboardDevice::Key::kRight: return ImGuiKey_RightArrow;

	case RE::BSKeyboardDevice::Key::kHome:	   return ImGuiKey_Home;
	case RE::BSKeyboardDevice::Key::kEnd:	   return ImGuiKey_End;
	case RE::BSKeyboardDevice::Key::kInsert:   return ImGuiKey_Insert;
	case RE::BSKeyboardDevice::Key::kDelete:   return ImGuiKey_Delete;
	case RE::BSKeyboardDevice::Key::kPageUp:   return ImGuiKey_PageUp;
	case RE::BSKeyboardDevice::Key::kPageDown: return ImGuiKey_PageDown;

	case RE::BSKeyboardDevice::Key::kEscape:    return ImGuiKey_Escape;
	case RE::BSKeyboardDevice::Key::kSpacebar:  return ImGuiKey_Space;
	case RE::BSKeyboardDevice::Key::kEnter:	    return ImGuiKey_Enter;
	case RE::BSKeyboardDevice::Key::kTab:	    return ImGuiKey_Tab;
	case RE::BSKeyboardDevice::Key::kBackspace: return ImGuiKey_Backspace;

	// Implentation for Left/Right modifiers
	case RE::BSKeyboardDevice::Key::kLeftShift:	   return ImGuiKey_LeftShift;
	case RE::BSKeyboardDevice::Key::kRightShift:   return ImGuiKey_RightShift;
	case RE::BSKeyboardDevice::Key::kLeftControl:  return ImGuiKey_LeftCtrl;
	case RE::BSKeyboardDevice::Key::kRightControl: return ImGuiKey_RightCtrl;
	case RE::BSKeyboardDevice::Key::kLeftAlt:      return ImGuiKey_LeftAlt;
	case RE::BSKeyboardDevice::Key::kRightAlt:     return ImGuiKey_RightAlt;

	case RE::BSKeyboardDevice::Key::kEquals: return ImGuiKey_Equal;
	case RE::BSKeyboardDevice::Key::kMinus:  return ImGuiKey_Minus;

	case RE::BSKeyboardDevice::Key::kBracketLeft:  return ImGuiKey_LeftBracket;
	case RE::BSKeyboardDevice::Key::kBracketRight: return ImGuiKey_RightBracket;
	case RE::BSKeyboardDevice::Key::kBackslash:	   return ImGuiKey_Backslash;
	case RE::BSKeyboardDevice::Key::kSemicolon:    return ImGuiKey_Semicolon;
	case RE::BSKeyboardDevice::Key::kApostrophe:   return ImGuiKey_Apostrophe;
	case RE::BSKeyboardDevice::Key::kComma:		   return ImGuiKey_Comma;
	case RE::BSKeyboardDevice::Key::kPeriod:	   return ImGuiKey_Period;
	case RE::BSKeyboardDevice::Key::kSlash:		   return ImGuiKey_Slash;
	case RE::BSKeyboardDevice::Key::kTilde:		   return ImGuiKey_GraveAccent;

	case RE::BSKeyboardDevice::Key::kCapsLock:	 return ImGuiKey_CapsLock;
	case RE::BSKeyboardDevice::Key::kNumLock:	 return ImGuiKey_NumLock;
	case RE::BSKeyboardDevice::Key::kScrollLock: return ImGuiKey_ScrollLock;
	case RE::BSKeyboardDevice::Key::kPause:		 return ImGuiKey_Pause;

	case RE::BSKeyboardDevice::Key::kKP_0: return ImGuiKey_Keypad0;
	case RE::BSKeyboardDevice::Key::kKP_1: return ImGuiKey_Keypad1;
	case RE::BSKeyboardDevice::Key::kKP_2: return ImGuiKey_Keypad2;
	case RE::BSKeyboardDevice::Key::kKP_3: return ImGuiKey_Keypad3;
	case RE::BSKeyboardDevice::Key::kKP_4: return ImGuiKey_Keypad4;
	case RE::BSKeyboardDevice::Key::kKP_5: return ImGuiKey_Keypad5;
	case RE::BSKeyboardDevice::Key::kKP_6: return ImGuiKey_Keypad6;
	case RE::BSKeyboardDevice::Key::kKP_7: return ImGuiKey_Keypad7;
	case RE::BSKeyboardDevice::Key::kKP_8: return ImGuiKey_Keypad8;
	case RE::BSKeyboardDevice::Key::kKP_9: return ImGuiKey_Keypad9;	

	case RE::BSKeyboardDevice::Key::kKP_Divide:   return ImGuiKey_KeypadDivide;
	case RE::BSKeyboardDevice::Key::kKP_Multiply: return ImGuiKey_KeypadMultiply;
	case RE::BSKeyboardDevice::Key::kKP_Subtract: return ImGuiKey_KeypadSubtract;
	case RE::BSKeyboardDevice::Key::kKP_Plus:     return ImGuiKey_KeypadAdd;
	case RE::BSKeyboardDevice::Key::kKP_Enter:    return ImGuiKey_KeypadEnter;
	case RE::BSKeyboardDevice::Key::kKP_Decimal:  return ImGuiKey_KeypadDecimal;

	case 256: return ImGuiKey_MouseLeft;
	case 257: return ImGuiKey_MouseRight;
	case 258: return ImGuiKey_MouseMiddle;
	case 259: return ImGuiKey_MouseX1;
	case 260: return ImGuiKey_MouseX2;

	case 266: return ImGuiKey_GamepadDpadUp;
	case 267: return ImGuiKey_GamepadDpadDown;
	case 268: return ImGuiKey_GamepadDpadLeft;
	case 269: return ImGuiKey_GamepadDpadRight;
	case 270: return ImGuiKey_GamepadStart;
	case 271: return ImGuiKey_GamepadBack;
	case 272: return ImGuiKey_GamepadL3;
	case 273: return ImGuiKey_GamepadR3;
	case 274: return ImGuiKey_GamepadL1;
	case 275: return ImGuiKey_GamepadR1;
	case 276: return ImGuiKey_GamepadFaceDown;
	case 277: return ImGuiKey_GamepadFaceRight;
	case 278: return ImGuiKey_GamepadFaceLeft;
	case 279: return ImGuiKey_GamepadFaceUp;
	case 280: return ImGuiKey_GamepadL2;
	case 281: return ImGuiKey_GamepadR2;

	default: return ImGuiKey_None;
	}
}

int SPUI::ImGuiKeyToIDCode(ImGuiKey key)
{
	switch (key) {
    case ImGuiKey_A: return RE::BSKeyboardDevice::Key::kA;
    case ImGuiKey_B: return RE::BSKeyboardDevice::Key::kB;
    case ImGuiKey_C: return RE::BSKeyboardDevice::Key::kC;
    case ImGuiKey_D: return RE::BSKeyboardDevice::Key::kD;
    case ImGuiKey_E: return RE::BSKeyboardDevice::Key::kE;
    case ImGuiKey_F: return RE::BSKeyboardDevice::Key::kF;
    case ImGuiKey_G: return RE::BSKeyboardDevice::Key::kG;
    case ImGuiKey_H: return RE::BSKeyboardDevice::Key::kH;
    case ImGuiKey_I: return RE::BSKeyboardDevice::Key::kI;
    case ImGuiKey_J: return RE::BSKeyboardDevice::Key::kJ;
    case ImGuiKey_K: return RE::BSKeyboardDevice::Key::kK;
    case ImGuiKey_L: return RE::BSKeyboardDevice::Key::kL;
    case ImGuiKey_M: return RE::BSKeyboardDevice::Key::kM;
    case ImGuiKey_N: return RE::BSKeyboardDevice::Key::kN;
    case ImGuiKey_O: return RE::BSKeyboardDevice::Key::kO;
    case ImGuiKey_P: return RE::BSKeyboardDevice::Key::kP;
    case ImGuiKey_Q: return RE::BSKeyboardDevice::Key::kQ;
    case ImGuiKey_R: return RE::BSKeyboardDevice::Key::kR;
    case ImGuiKey_S: return RE::BSKeyboardDevice::Key::kS;
    case ImGuiKey_T: return RE::BSKeyboardDevice::Key::kT;
    case ImGuiKey_U: return RE::BSKeyboardDevice::Key::kU;
    case ImGuiKey_V: return RE::BSKeyboardDevice::Key::kV;
    case ImGuiKey_W: return RE::BSKeyboardDevice::Key::kW;
    case ImGuiKey_X: return RE::BSKeyboardDevice::Key::kX;
    case ImGuiKey_Y: return RE::BSKeyboardDevice::Key::kY;
    case ImGuiKey_Z: return RE::BSKeyboardDevice::Key::kZ;

    case ImGuiKey_0: return RE::BSKeyboardDevice::Key::kNum0;
    case ImGuiKey_1: return RE::BSKeyboardDevice::Key::kNum1;
    case ImGuiKey_2: return RE::BSKeyboardDevice::Key::kNum2;
    case ImGuiKey_3: return RE::BSKeyboardDevice::Key::kNum3;
    case ImGuiKey_4: return RE::BSKeyboardDevice::Key::kNum4;
    case ImGuiKey_5: return RE::BSKeyboardDevice::Key::kNum5;
    case ImGuiKey_6: return RE::BSKeyboardDevice::Key::kNum6;
    case ImGuiKey_7: return RE::BSKeyboardDevice::Key::kNum7;
    case ImGuiKey_8: return RE::BSKeyboardDevice::Key::kNum8;
    case ImGuiKey_9: return RE::BSKeyboardDevice::Key::kNum9;

    case ImGuiKey_F1:  return RE::BSKeyboardDevice::Key::kF1;
    case ImGuiKey_F2:  return RE::BSKeyboardDevice::Key::kF2;
    case ImGuiKey_F3:  return RE::BSKeyboardDevice::Key::kF3;
    case ImGuiKey_F4:  return RE::BSKeyboardDevice::Key::kF4;
    case ImGuiKey_F5:  return RE::BSKeyboardDevice::Key::kF5;
    case ImGuiKey_F6:  return RE::BSKeyboardDevice::Key::kF6;
    case ImGuiKey_F7:  return RE::BSKeyboardDevice::Key::kF7;
    case ImGuiKey_F8:  return RE::BSKeyboardDevice::Key::kF8;
    case ImGuiKey_F9:  return RE::BSKeyboardDevice::Key::kF9;
    case ImGuiKey_F10: return RE::BSKeyboardDevice::Key::kF10;
    case ImGuiKey_F11: return RE::BSKeyboardDevice::Key::kF11;
    case ImGuiKey_F12: return RE::BSKeyboardDevice::Key::kF12;

    case ImGuiKey_UpArrow:    return RE::BSKeyboardDevice::Key::kUp;
    case ImGuiKey_DownArrow:  return RE::BSKeyboardDevice::Key::kDown;
    case ImGuiKey_LeftArrow:  return RE::BSKeyboardDevice::Key::kLeft;
    case ImGuiKey_RightArrow: return RE::BSKeyboardDevice::Key::kRight;

    case ImGuiKey_Home:    return RE::BSKeyboardDevice::Key::kHome;
    case ImGuiKey_End:     return RE::BSKeyboardDevice::Key::kEnd;
    case ImGuiKey_Insert:  return RE::BSKeyboardDevice::Key::kInsert;
    case ImGuiKey_Delete:  return RE::BSKeyboardDevice::Key::kDelete;
    case ImGuiKey_PageUp:  return RE::BSKeyboardDevice::Key::kPageUp;
    case ImGuiKey_PageDown:return RE::BSKeyboardDevice::Key::kPageDown;

    case ImGuiKey_Escape:    return RE::BSKeyboardDevice::Key::kEscape;
    case ImGuiKey_Space:     return RE::BSKeyboardDevice::Key::kSpacebar;
    case ImGuiKey_Enter:     return RE::BSKeyboardDevice::Key::kEnter;
    case ImGuiKey_Tab:       return RE::BSKeyboardDevice::Key::kTab;
    case ImGuiKey_Backspace: return RE::BSKeyboardDevice::Key::kBackspace;

    case ImGuiKey_LeftShift:  return RE::BSKeyboardDevice::Key::kLeftShift;
    case ImGuiKey_RightShift: return RE::BSKeyboardDevice::Key::kRightShift;
    case ImGuiKey_LeftCtrl:   return RE::BSKeyboardDevice::Key::kLeftControl;
    case ImGuiKey_RightCtrl:  return RE::BSKeyboardDevice::Key::kRightControl;
    case ImGuiKey_LeftAlt:    return RE::BSKeyboardDevice::Key::kLeftAlt;
    case ImGuiKey_RightAlt:   return RE::BSKeyboardDevice::Key::kRightAlt;

	case ImGuiKey_ReservedForModShift: return RE::BSKeyboardDevice::Key::kLeftShift;
	case ImGuiKey_ReservedForModCtrl: return RE::BSKeyboardDevice::Key::kLeftControl;
	case ImGuiKey_ReservedForModAlt: return RE::BSKeyboardDevice::Key::kLeftAlt;

    case ImGuiKey_Equal:        return RE::BSKeyboardDevice::Key::kEquals;
    case ImGuiKey_Minus:        return RE::BSKeyboardDevice::Key::kMinus;
    case ImGuiKey_LeftBracket:  return RE::BSKeyboardDevice::Key::kBracketLeft;
    case ImGuiKey_RightBracket: return RE::BSKeyboardDevice::Key::kBracketRight;
    case ImGuiKey_Backslash:    return RE::BSKeyboardDevice::Key::kBackslash;
    case ImGuiKey_Semicolon:    return RE::BSKeyboardDevice::Key::kSemicolon;
    case ImGuiKey_Apostrophe:   return RE::BSKeyboardDevice::Key::kApostrophe;
    case ImGuiKey_Comma:        return RE::BSKeyboardDevice::Key::kComma;
    case ImGuiKey_Period:       return RE::BSKeyboardDevice::Key::kPeriod;
    case ImGuiKey_Slash:        return RE::BSKeyboardDevice::Key::kSlash;
    case ImGuiKey_GraveAccent:  return RE::BSKeyboardDevice::Key::kTilde;

    case ImGuiKey_CapsLock:    return RE::BSKeyboardDevice::Key::kCapsLock;
    case ImGuiKey_NumLock:     return RE::BSKeyboardDevice::Key::kNumLock;
    case ImGuiKey_ScrollLock:  return RE::BSKeyboardDevice::Key::kScrollLock;
    case ImGuiKey_Pause:       return RE::BSKeyboardDevice::Key::kPause;

    case ImGuiKey_Keypad0: return RE::BSKeyboardDevice::Key::kKP_0;
    case ImGuiKey_Keypad1: return RE::BSKeyboardDevice::Key::kKP_1;
    case ImGuiKey_Keypad2: return RE::BSKeyboardDevice::Key::kKP_2;
    case ImGuiKey_Keypad3: return RE::BSKeyboardDevice::Key::kKP_3;
    case ImGuiKey_Keypad4: return RE::BSKeyboardDevice::Key::kKP_4;
    case ImGuiKey_Keypad5: return RE::BSKeyboardDevice::Key::kKP_5;
    case ImGuiKey_Keypad6: return RE::BSKeyboardDevice::Key::kKP_6;
    case ImGuiKey_Keypad7: return RE::BSKeyboardDevice::Key::kKP_7;
    case ImGuiKey_Keypad8: return RE::BSKeyboardDevice::Key::kKP_8;
    case ImGuiKey_Keypad9: return RE::BSKeyboardDevice::Key::kKP_9;

    case ImGuiKey_KeypadDivide:		return RE::BSKeyboardDevice::Key::kKP_Divide;
    case ImGuiKey_KeypadMultiply:	return RE::BSKeyboardDevice::Key::kKP_Multiply;
    case ImGuiKey_KeypadSubtract:	return RE::BSKeyboardDevice::Key::kKP_Subtract;
    case ImGuiKey_KeypadAdd:		return RE::BSKeyboardDevice::Key::kKP_Plus;
	case ImGuiKey_KeypadEnter:		return RE::BSKeyboardDevice::Key::kKP_Enter;
	case ImGuiKey_KeypadDecimal:	return RE::BSKeyboardDevice::Key::kKP_Decimal;

	case ImGuiKey_MouseLeft:		return 256;
	case ImGuiKey_MouseRight:		return 257;
	case ImGuiKey_MouseMiddle:		return 258;
	case ImGuiKey_MouseX1:			return 259;
	case ImGuiKey_MouseX2:			return 260;

	case ImGuiKey_GamepadDpadUp:	return 266;
	case ImGuiKey_GamepadDpadDown:  return 267;
	case ImGuiKey_GamepadDpadLeft:  return 268;
	case ImGuiKey_GamepadDpadRight: return 269;
	case ImGuiKey_GamepadStart:     return 270;
	case ImGuiKey_GamepadBack:      return 271;
	case ImGuiKey_GamepadL3:		return 272;
	case ImGuiKey_GamepadR3:		return 273;
	case ImGuiKey_GamepadL1:		return 274;
	case ImGuiKey_GamepadR1:		return 275;
	case ImGuiKey_GamepadFaceDown:  return 276;
	case ImGuiKey_GamepadFaceRight: return 277;
	case ImGuiKey_GamepadFaceLeft:  return 278;
	case ImGuiKey_GamepadFaceUp:    return 279;
	case ImGuiKey_GamepadL2:		return 280;
	case ImGuiKey_GamepadR2:		return 281;

	default: return -1;
	}
}