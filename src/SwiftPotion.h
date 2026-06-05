#pragma once
#include "Settings.h"

static float& g_deltaTime = (*(float*)RELOCATION_ID(523660, 410199).address());
static float lastTime;

class SwiftPotion {
    public:
        static bool InstallUpdateHook();
        static void ProcessHotkey(const uint32_t& _code, bool _modifier1, bool _modifier2, bool _modifier3);

    protected:
        inline static std::int32_t OnUpdate();
        inline static REL::Relocation<decltype(OnUpdate)> _OnUpdate;
    
    private:
        struct foundPotionData
        {
            RE::AlchemyItem* potion = nullptr;
            bool foundPotion = false;
        };

        static void SwiftPotionLoopUpdate();
        static void AutoSystemCheck(PotionData &SystemData);
        static std::unordered_set<RE::FormID> AutoSystemEffectCheck(std::string sEffect);
        static void ExtraEffectCheck(PotionData &optionalData);
        static void UsePotion(RE::Actor* aPlayer, PotionData &SystemData, bool bHotkey, const std::unordered_set<RE::FormID> &activeForms);
        static void UsePotionAutoHotkey(RE::Actor* aPlayer, PotionData &RestoreData, PotionData &RegenData, PotionData &FortifyData, std::string EffectName);
        static foundPotionData GetPotion(RE::Actor* aPlayer, PotionData &SystemData, const std::unordered_set<RE::FormID> &activeForms);
};