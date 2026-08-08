#include "Utility.h"
#include "Settings.h"

void StopperCheck(RE::AlchemyItem* pPotion) {
    const auto settings = Settings::GetSingleton();

    for (auto& eEffect : pPotion->effects) {
		// Health
		if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Restore.EffectName.c_str()) == 0 && settings->Health_Restore.Stopper) {
			settings->Health_Restore.Stopper = false;
        } else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Regen.EffectName.c_str()) == 0 && settings->Health_Regen.Stopper) {
			settings->Health_Regen.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Fortify.EffectName.c_str()) == 0 && settings->Health_Fortify.Stopper) {
			settings->Health_Fortify.Stopper = false;

		// Magicka
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Restore.EffectName.c_str()) == 0 && settings->Magicka_Restore.Stopper){
			settings->Magicka_Restore.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Regen.EffectName.c_str()) == 0 && settings->Magicka_Regen.Stopper) {
			settings->Magicka_Regen.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Fortify.EffectName.c_str()) == 0 && settings->Magicka_Fortify.Stopper) {
			settings->Magicka_Fortify.Stopper = false;

		// Stamina
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Restore.EffectName.c_str()) == 0 && settings->Stamina_Restore.Stopper) {
			settings->Stamina_Restore.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Regen.EffectName.c_str()) == 0 && settings->Stamina_Regen.Stopper) {
			settings->Stamina_Regen.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Fortify.EffectName.c_str()) == 0 && settings->Stamina_Fortify.Stopper) {
			settings->Stamina_Fortify.Stopper = false;

        // Other Effects
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Cure_Disease.EffectName.c_str()) == 0 && settings->Cure_Disease.Stopper) {
			settings->Cure_Disease.Stopper = false;
		} else if (_stricmp(eEffect->baseEffect->GetFullName(),settings->Cure_Poison.EffectName.c_str()) == 0 && settings->Cure_Poison.Stopper) {
			settings->Cure_Poison.Stopper = false;
        }
    };
}

Utility* Utility::GetSingleton() {
    static Utility playerStatus;
    return &playerStatus;
}

RE::PlayerCharacter* Utility::GetPlayer() {
    REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
    return singleton->get();
}

RE::UI* Utility::GetUI() {
    REL::Relocation<RE::NiPointer<RE::UI>*> singleton{ UISingletonAddress };
    return singleton->get();
}

RE::MenuControls* Utility::GetMenuControls() {
    REL::Relocation<RE::NiPointer<RE::MenuControls>*> singleton{ MenuControlsSingletonAddress };
    return singleton->get();
}

void Utility::ShowNotification(std::string msg, bool messageBox) {
    if (messageBox)
        RE::DebugMessageBox(msg.c_str());
    else
        RE::DebugNotification(msg.c_str());
}

// Player checks
bool Utility::PlayerIsWerewolf() {
    return GetPlayer()->GetRace() == raceWerewolf;
}

bool Utility::PlayerIsVampireLord() {
    return GetPlayer()->GetRace() == raceVampireLord;
}

bool Utility::PlayerIsLich() {
    if (Undeath_LichPerk)
        return GetPlayer()->HasPerk(Undeath_LichPerk);
    else
        return false;
}

bool Utility::IsPlayerInBrawl() {
    if (BrawlQuest->currentStage <= 0 || BrawlQuest->currentStage >= 250)
        return false;
    else
        return true;
}

bool Utility::IsPlayerInDialogue() {
    return GetUI()->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
}

// Attribute Functions
bool Utility::PlayerIsAlive() {
    return (GetPlayer()->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) > 0);
}

float Utility::GetPlayerAttribute(int iAttribute) {
    const RE::ActorValue actorValues[] = {
        RE::ActorValue::kHealth,
        RE::ActorValue::kMagicka,
        RE::ActorValue::kStamina
    };
    RE::ActorValue workingValue = (iAttribute >= 0 && iAttribute < 3) ? actorValues[iAttribute] : RE::ActorValue::kHealth;
    
    return GetPlayer()->AsActorValueOwner()->GetActorValue(workingValue) / (GetPlayer()->AsActorValueOwner()->GetPermanentActorValue(workingValue) +
        GetPlayer()->As<RE::Actor>()->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, workingValue));
}

float Utility::GetPlayerDifference(int iAttribute) {
    const RE::ActorValue actorValues[] = {
        RE::ActorValue::kHealth,
        RE::ActorValue::kMagicka,
        RE::ActorValue::kStamina
    };
    RE::ActorValue workingValue = (iAttribute >= 0 && iAttribute < 3) ? actorValues[iAttribute] : RE::ActorValue::kHealth;

    return (GetPlayer()->AsActorValueOwner()->GetPermanentActorValue(workingValue) + 
        GetPlayer()->As<RE::Actor>()->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, workingValue)) - GetPlayer()->AsActorValueOwner()->GetActorValue(workingValue);
}