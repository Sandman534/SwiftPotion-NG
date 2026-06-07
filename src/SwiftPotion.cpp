#include "Hooks.h"
#include "SwiftPotion.h"
#include "Utility.h"
#include "Settings.h"

std::int32_t SwiftPotion::OnUpdate() {
    if (!Utility::GetUI()->GameIsPaused()) {
        if (g_deltaTime > 0) {
            lastTime += g_deltaTime;
            if (lastTime >= 1.0f) {
                SwiftPotionLoopUpdate();
                lastTime = 0;
            }
        }
    }

    return _OnUpdate();
}

bool SwiftPotion::InstallUpdateHook() {
    auto& trampoline = SKSE::GetTrampoline();
    _OnUpdate = trampoline.write_call<5>(Hooks::On_Update_Hook.address(), OnUpdate);
    logger::info("Installed update hook");
    return true;
}

void SwiftPotion::SwiftPotionLoopUpdate() {
    auto utility = Utility::GetSingleton();
    auto settings = Settings::GetSingleton();

    if (settings->SPNG_Enabled && !utility->PlayerIsWerewolf() && !utility->PlayerIsVampireLord() && utility->PlayerIsAlive() && !utility->IsPlayerInBrawl()) {
        // Auto Health System
        AutoSystemCheck(settings->Health_Restore);
        AutoSystemCheck(settings->Health_Regen);
        AutoSystemCheck(settings->Health_Fortify);

        // Auto Magicka System
        AutoSystemCheck(settings->Magicka_Restore);
        AutoSystemCheck(settings->Magicka_Regen);
        AutoSystemCheck(settings->Magicka_Fortify);

        // Auto Stamina System
        AutoSystemCheck(settings->Stamina_Restore);
        AutoSystemCheck(settings->Stamina_Regen);
        AutoSystemCheck(settings->Stamina_Fortify);

        // Specific Monitoring
		CureSystemCheck(settings->Cure_Disease);
        CureSystemCheck(settings->Cure_Poison);
    }
}

std::unordered_set<RE::FormID> SwiftPotion::GetActiveEffects(std::string sEffect) {
    auto utility = Utility::GetSingleton();
    RE::Actor* aPlayer = Utility::GetPlayer();
    std::unordered_set<RE::FormID> activeForms;

    // Check to see if the player has the effect listed
    for (auto& eEffect : *aPlayer->AsMagicTarget()->GetActiveEffectList()) {
		//eEffect->GetBaseObject()->GetFormID();
        auto* baseEffect = eEffect->GetBaseObject();

        if (baseEffect && strcmpi(baseEffect->GetFullName(),sEffect.c_str()) == 0 && baseEffect->HasKeyword(utility->positiveKeyword))
             activeForms.insert(baseEffect->GetFormID());
    }

    return activeForms;
}

void SwiftPotion::AutoSystemCheck(PotionData &SystemData) {
    auto utility = Utility::GetSingleton();

    // Conditional Checks
    if (!SystemData.Enabled || SystemData.Stopper) return;
    if (SystemData.Attribute == 2 && utility->GetPlayer()->IsOverEncumbered()) return;
    if (utility->GetPlayerAttribute(SystemData.Attribute) > SystemData.Threshold) return;
    if (SystemData.CombatOnly && !utility->GetPlayer()->IsInCombat()) return;

    // Get the active forms associated to the effect and send them along for additional checks
    auto activeForms = GetActiveEffects(SystemData.EffectName);
	UsePotion(utility->GetPlayer(), SystemData, false, activeForms);
}

void SwiftPotion::CureSystemCheck(PotionData &cureData) {
    auto utility = Utility::GetSingleton();
    RE::Actor* aPlayer = Utility::GetPlayer();

    // Return if no data found
    if (!cureData.Enabled || cureData.Stopper) return;
  
    // Check to see if the player has the effect listed
    for (auto& eEffect : *aPlayer->AsMagicTarget()->GetActiveEffectList()) {
		if (cureData.Attribute == 0) {
			if (eEffect->spell && eEffect->spell->GetSpellType() == RE::MagicSystem::SpellType::kDisease) {
				UsePotion(utility->GetPlayer(), cureData, false, {});
				break;
			}
		} else if (cureData.Attribute == 1) {
			if (eEffect->spell && eEffect->spell->GetSpellType() == RE::MagicSystem::SpellType::kPoison) {
				UsePotion(utility->GetPlayer(), cureData, false, {});
				break;
			}
		}
    }
}

void SwiftPotion::ResistSystemCheck(PotionData &resistData) {
    auto utility = Utility::GetSingleton();
    RE::Actor* aPlayer = Utility::GetPlayer();

    // Conditional Checks
    if (!resistData.Enabled || resistData.Stopper) return;
    if (utility->GetPlayerAttribute(resistData.Attribute) > resistData.Threshold) return;
    if (resistData.CombatOnly && !utility->GetPlayer()->IsInCombat()) return;

    // Get the active forms associated to the effect and send them along for additional checks
    auto activeForms = GetActiveEffects(resistData.EffectName);
    UsePotion(utility->GetPlayer(), resistData, false, activeForms);
}

void SwiftPotion::ResistCheck(RE::ActorValue resistVariable) {
    auto utility = Utility::GetSingleton();
    auto settings = Settings::GetSingleton();

    if (resistVariable == RE::ActorValue::kResistFire)
        ResistSystemCheck(settings->Resist_Fire);
    else if (resistVariable == RE::ActorValue::kResistShock)
        ResistSystemCheck(settings->Resist_Shock);
    else if (resistVariable == RE::ActorValue::kResistFrost)
        ResistSystemCheck(settings->Resist_Frost);
    else if (resistVariable == RE::ActorValue::kResistMagic)
        ResistSystemCheck(settings->Resist_Magic);
}

void SwiftPotion::ProcessHotkey(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3) {
    auto utility = Utility::GetSingleton();
    auto settings = Settings::GetSingleton();

    if (settings->Health_Restore.Hotkey == _code && settings->Health_Restore.Modifier1 == _modifier1 && settings->Health_Restore.Modifier2 == _modifier2 && settings->Health_Restore.Modifier3 == _modifier3)
        UsePotionAutoHotkey(utility->GetPlayer(), settings->Health_Restore, settings->Health_Regen, settings->Health_Fortify, "Health");
    else if (settings->Magicka_Restore.Hotkey == _code && settings->Magicka_Restore.Modifier1 == _modifier1 && settings->Magicka_Restore.Modifier2 == _modifier2 && settings->Magicka_Restore.Modifier3 == _modifier3)
        UsePotionAutoHotkey(utility->GetPlayer(), settings->Magicka_Restore, settings->Magicka_Regen, settings->Magicka_Fortify, "Magicka");
    else if (settings->Stamina_Restore.Hotkey == _code && settings->Stamina_Restore.Modifier1 == _modifier1 && settings->Stamina_Restore.Modifier2 == _modifier2 && settings->Stamina_Restore.Modifier3 == _modifier3)
        UsePotionAutoHotkey(utility->GetPlayer(), settings->Stamina_Restore, settings->Stamina_Regen, settings->Stamina_Fortify, "Stamina");
    else {
        for (PotionData hotkeyData : settings->HotkeyRecords) {
            if (hotkeyData.Hotkey == _code && hotkeyData.Modifier1 == _modifier1 && hotkeyData.Modifier2 == _modifier2 && hotkeyData.Modifier3 == _modifier3) {
				UsePotion(utility->GetPlayer(), hotkeyData, true, {});
            }
        }
    }
}

void SwiftPotion::UsePotion(RE::Actor* aPlayer, PotionData &SystemData, bool bHotkey, const std::unordered_set<RE::FormID> &activeForms) {
	// Setup constants
    auto utility = Utility::GetSingleton();
    auto settings = Settings::GetSingleton();   

    // Get the correct effect list based on type
	foundPotionData pPotion = GetPotion(aPlayer, SystemData, activeForms);

	// Use the potion, or stop the Auto system from running until another enteres the user inventory
	if (pPotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, pPotion.potion, nullptr, 1, nullptr, true);
    else if (!pPotion.potion && !pPotion.foundPotion && !bHotkey)
        SystemData.Stopper = true;

    // Display notification if turned on
    if (!pPotion.potion && !pPotion.foundPotion && settings->SPNG_Notifications && !SystemData.Poison)
    	utility->ShowNotification("You have no " + SystemData.EffectName + " potions");
    else if (!pPotion.potion && !pPotion.foundPotion && settings->SPNG_Notifications && SystemData.Poison)
    	utility->ShowNotification("You have no " + SystemData.EffectName + " poisons");
}

void SwiftPotion::UsePotionAutoHotkey(RE::Actor* aPlayer, PotionData &RestoreData, PotionData &RegenData, PotionData &FortifyData, std::string EffectName) {
	// Setup constants
    auto utility = Utility::GetSingleton();
    auto settings = Settings::GetSingleton();

    // Get the correct effect list based on type
	foundPotionData restorePotion = GetPotion(aPlayer, RestoreData, {});
    foundPotionData regenPotion = GetPotion(aPlayer, RegenData, {});
    foundPotionData fortifyPotion = GetPotion(aPlayer, FortifyData, {});

    // Display notification if turned on
    if (!restorePotion.potion && !regenPotion.potion && !fortifyPotion.potion && settings->SPNG_Notifications) {
    	utility->ShowNotification("You have no " + EffectName + " effect potions");
        return;
    }

    // Auto System Hotkey 
    if (FortifyData.Enabled && utility->GetPlayerAttribute(FortifyData.Attribute) <= FortifyData.Threshold && fortifyPotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, fortifyPotion.potion, nullptr, 1, nullptr, true);
    else if (RestoreData.Enabled && utility->GetPlayerAttribute(RestoreData.Attribute) <= RestoreData.Threshold && restorePotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, restorePotion.potion, nullptr, 1, nullptr, true);
    else if (RegenData.Enabled && utility->GetPlayerAttribute(RegenData.Attribute) <= RegenData.Threshold && regenPotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, regenPotion.potion, nullptr, 1, nullptr, true);
    else if (RestoreData.Enabled && restorePotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, restorePotion.potion, nullptr, 1, nullptr, true);
    else if (RegenData.Enabled && regenPotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, regenPotion.potion, nullptr, 1, nullptr, true);
    else if (FortifyData.Enabled && fortifyPotion.potion)
        RE::ActorEquipManager::GetSingleton()->EquipObject(aPlayer, fortifyPotion.potion, nullptr, 1, nullptr, true);

}

SwiftPotion::foundPotionData SwiftPotion::GetPotion(RE::Actor* aPlayer, PotionData &SystemData, const std::unordered_set<RE::FormID> &activeForms) {
    auto utility = Utility::GetSingleton();

    // Set up local variables
    RE::AlchemyItem* pFinalPotion = nullptr;
	foundPotionData foundPotion;

    // Magnitude needs to be set based on High/Low Option
    float iMagnitude = NULL;

    // Get reference Inventory
    auto inv = aPlayer->GetInventory();
    for (const auto& [item, data] : inv) {
        if (!item->Is(RE::FormType::AlchemyItem))
            continue;

        const auto& [count, entry] = data;
        if (count > 0 && entry) {
            // Found Potion
            RE::AlchemyItem* pPotion = item->As<RE::AlchemyItem>();

            // If the potion doesnt meet our requirements
            if ((pPotion->IsFood() && !SystemData.UseFood) || (pPotion->IsPoison() && !SystemData.Poison) || utility->IsBlacklisted(pPotion))
                continue;

            // Loop through all of the magic effects on the potion
            for (auto& eEffect : pPotion->effects) {
                RE::FormID effectFormID = eEffect->baseEffect->GetFormID();

                // The forms name matches
                if (stricmp(eEffect->baseEffect->GetFullName(),SystemData.EffectName.c_str()) == 0) {

                    // We have found both the effect and the assoicated form
                    if (activeForms.contains(effectFormID)) {
						foundPotion.foundPotion = true;
						break;
					}
                    // Optimal Value if selected
                    if (SystemData.BestValue == 2) {
                        // If the potion has no duration, set it to 1
                        float potionDuration = NULL;
                        if (eEffect->effectItem.duration <= 0)
                            potionDuration = 1.0;
                        else
                            potionDuration = eEffect->effectItem.duration;

                        float healthGap = abs(utility->GetPlayerDifference(SystemData.Attribute) - (eEffect->effectItem.magnitude * potionDuration));
                        if (!iMagnitude || healthGap < iMagnitude) {
                            iMagnitude = healthGap;
                            foundPotion.potion = pPotion;
                        }
                    
                    // Higher vs Lower Poition
                    } else if (!iMagnitude || (SystemData.BestValue == 0 && eEffect->effectItem.magnitude > iMagnitude) || (SystemData.BestValue == 1 && eEffect->effectItem.magnitude < iMagnitude)) {
                        iMagnitude = eEffect->effectItem.magnitude;
                        foundPotion.potion = pPotion;
                    }
                }
            }
        }
    }

    return foundPotion;
}