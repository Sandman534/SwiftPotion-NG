#pragma once
#include "Settings.h"

namespace SwiftPotionExtension {
    bool RegisterSwiftPotion(RE::BSScript::IVirtualMachine* vm);

    std::string stringToUpper(std::string oString);
    void SaveSettings(RE::StaticFunctionTag*);

    std::vector<std::string> GetEffectList(RE::StaticFunctionTag*, bool isNegative);
    PotionData& GetPotionData(std::string sSystem, int iHotkey);
    int Get_PotionDataInt(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter);
    std::string Get_PotionDataString(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter);
    float Get_PotionDataFloat(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter);
    bool Get_PotionDataBool(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter);
    int HotkeyCount(RE::StaticFunctionTag*);
    bool Get_SystemBool(RE::StaticFunctionTag*, std::string sParameter);
    int Get_SystemInt(RE::StaticFunctionTag*, std::string sParameter);

    void Set_PotionDataInt(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter, int aValue);
    void Set_PotionDataString(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter, std::string aValue);
    void Set_PotionDataFloat(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter, float aValue);
    void Set_PotionDataBool(RE::StaticFunctionTag*, std::string sSystem, int iHotkey, std::string sParameter, bool aValue);
    void Set_SystemBool(RE::StaticFunctionTag*, std::string sParameter, bool aValue);
    void Set_SystemInt(RE::StaticFunctionTag*, std::string sParameter, int aValue);
}