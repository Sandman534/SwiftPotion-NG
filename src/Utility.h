#pragma once

namespace {
    static void StopperCheck(RE::AlchemyItem* pPotion);
}

class Utility {
public:
    RE::TESRace* raceVampireLord;
    RE::TESRace* raceWerewolf;
    RE::BGSPerk* Undeath_LichPerk;

    RE::TESQuest* BrawlQuest;

    RE::BGSKeyword* positiveKeyword;
    RE::BGSKeyword* negativeKeyword;

    uintptr_t PlayerSingletonAddress;
    uintptr_t UISingletonAddress;
    uintptr_t MenuControlsSingletonAddress;

    static Utility* GetSingleton();

    RE::PlayerCharacter* GetPlayer();
    RE::UI* GetUI();
    RE::MenuControls* GetMenuControls();
    void ShowNotification(std::string msg, bool messageBox = false);

    // Player checks
    bool PlayerIsWerewolf();
    bool PlayerIsVampireLord();
    bool PlayerIsLich();
    bool IsPlayerInBrawl();
    bool IsPlayerInDialogue();

    // Attribute Functions
    bool PlayerIsAlive();
    float GetPlayerAttribute(int iAttribute);
    float GetPlayerDifference(int iAttribute);
};
