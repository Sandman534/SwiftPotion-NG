#pragma once

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

    static RE::PlayerCharacter* GetPlayer();
    static RE::UI* GetUI();
    static RE::MenuControls* GetMenuControls();
    static void ShowNotification(std::string msg, bool messageBox = false);

    static void StopperCheck(RE::AlchemyItem* pPotion);
    static bool IsBlacklisted(RE::AlchemyItem* pPotion);

    // Player checks
    static bool PlayerIsWerewolf();
    static bool PlayerIsVampireLord();
    static bool PlayerIsLich();
    static bool IsPlayerInBrawl();
    static bool IsPlayerInDialogue();

    // Attribute Functions
    static bool PlayerIsAlive();
    static float GetPlayerAttribute(int iAttribute);
    static float GetPlayerDifference(int iAttribute);
};
