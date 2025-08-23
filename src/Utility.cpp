#include "Utility.h"
#include "Settings.h"

Utility* Utility::GetSingleton() {
    static Utility playerStatus;
    return &playerStatus;
}

RE::PlayerCharacter* Utility::GetPlayer() {
    REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{Utility::GetSingleton()->PlayerSingletonAddress};
    return singleton->get();
}

RE::UI* Utility::GetUI() {
    REL::Relocation<RE::NiPointer<RE::UI>*> singleton{Utility::GetSingleton()->UISingletonAddress};
    return singleton->get();
}

RE::MenuControls* Utility::GetMenuControls() {
    REL::Relocation<RE::NiPointer<RE::MenuControls>*> singleton{
        Utility::GetSingleton()->MenuControlsSingletonAddress};
    return singleton->get();
}

void Utility::ShowNotification(std::string msg, bool messageBox) {
    if (messageBox)
        RE::DebugMessageBox(msg.c_str());
    else
        RE::DebugNotification(msg.c_str());
}

void Utility::StopperCheck(RE::AlchemyItem* pPotion) {
    const auto settings = Settings::GetSingleton();

    for (auto& eEffect : pPotion->effects) {
		// Health
		if (stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Restore.EffectName.c_str()) == 0 && settings->Health_Restore.Stopper) {
			settings->Health_Restore.Stopper = false;
        } else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Regen.EffectName.c_str()) == 0 && settings->Health_Regen.Stopper) {
			settings->Health_Regen.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Health_Fortify.EffectName.c_str()) == 0 && settings->Health_Fortify.Stopper) {
			settings->Health_Fortify.Stopper = false;

		// Magicka
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Restore.EffectName.c_str()) == 0 && settings->Magicka_Restore.Stopper){
			settings->Magicka_Restore.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Regen.EffectName.c_str()) == 0 && settings->Magicka_Regen.Stopper) {
			settings->Magicka_Regen.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Magicka_Fortify.EffectName.c_str()) == 0 && settings->Magicka_Fortify.Stopper) {
			settings->Magicka_Fortify.Stopper = false;

		// Stamina
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Restore.EffectName.c_str()) == 0 && settings->Stamina_Restore.Stopper) {
			settings->Stamina_Restore.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Regen.EffectName.c_str()) == 0 && settings->Stamina_Regen.Stopper) {
			settings->Stamina_Regen.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Stamina_Fortify.EffectName.c_str()) == 0 && settings->Stamina_Fortify.Stopper) {
			settings->Stamina_Fortify.Stopper = false;

        // Other Effects
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Cure_Disease.EffectName.c_str()) == 0 && settings->Cure_Disease.Stopper) {
			settings->Stamina_Restore.Stopper = false;
		} else if (stricmp(eEffect->baseEffect->GetFullName(),settings->Cure_Poison.EffectName.c_str()) == 0 && settings->Cure_Poison.Stopper) {
			settings->Stamina_Regen.Stopper = false;
        }
    };
}

bool Utility::IsBlacklisted(RE::AlchemyItem* pPotion) {
    const auto settings = Settings::GetSingleton();
    int formid = pPotion->formID;

    if (settings->blacklistForms.empty())
        return false;

    auto it = settings->blacklistForms.find(formid);
    if (it == settings->blacklistForms.end())
        return false;

    return true;
}

// Player checks
bool Utility::PlayerIsWerewolf() {
    auto utility = Utility::GetSingleton();
    return utility->GetPlayer()->GetRace() == utility->raceWerewolf;
}

bool Utility::PlayerIsVampireLord() {
    auto utility = Utility::GetSingleton();
    return utility->GetPlayer()->GetRace() == utility->raceVampireLord;
}

bool Utility::PlayerIsLich() {
    auto util = Utility::GetSingleton();
    if (util->Undeath_LichPerk)
        return GetPlayer()->HasPerk(util->Undeath_LichPerk);
    else
        return false;
}

bool Utility::IsPlayerInBrawl() {
    auto util = Utility::GetSingleton();

    if (util->BrawlQuest->currentStage <= 0 || util->BrawlQuest->currentStage >= 250)
        return false;
    else
        return true;
}

bool Utility::IsPlayerInDialogue() {
    return Utility::GetSingleton()->GetUI()->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
}

// Attribute Functions
bool Utility::PlayerIsAlive() {
    return (Utility::GetSingleton()->GetPlayer()->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) > 0);
}

float Utility::GetPlayerAttribute(int iAttribute) {
    RE::ActorValue workingValue;
    if (iAttribute == 0)
        workingValue = RE::ActorValue::kHealth;
    else if (iAttribute == 1)
        workingValue = RE::ActorValue::kMagicka;
    else
        workingValue = RE::ActorValue::kStamina;
    
    return GetPlayer()->AsActorValueOwner()->GetActorValue(workingValue) / (GetPlayer()->AsActorValueOwner()->GetPermanentActorValue(workingValue) +
        GetPlayer()->As<RE::Actor>()->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, workingValue));
}

float Utility::GetPlayerDifference(int iAttribute) {
    RE::ActorValue workingValue;
    if (iAttribute == 0)
        workingValue = RE::ActorValue::kHealth;
    else if (iAttribute == 1)
        workingValue = RE::ActorValue::kMagicka;
    else
        workingValue = RE::ActorValue::kStamina;

    return (GetPlayer()->AsActorValueOwner()->GetPermanentActorValue(workingValue) +
        GetPlayer()->As<RE::Actor>()->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, workingValue)) - GetPlayer()->AsActorValueOwner()->GetActorValue(workingValue);
}