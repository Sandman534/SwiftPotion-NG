#pragma once
#include "SimpleIni.h"
#include <format>
#include <unordered_map>
#include <unordered_set>

struct PotionData {
	std::string EffectName = "";
    bool UseFood = false;
	bool Poison = false;
	int BestValue = 0;

	// Auto System Values
	bool Enabled = false;
	bool CombatOnly = false;
	float Threshold = 0;
	int Attribute = 0;
	bool Stopper = false;

	// Hotkey Values
	int Hotkey = 0;
	bool Modifier1 = false;
	bool Modifier2 = false;
	bool Modifier3 = false;

	bool solve(std::string s);
	std::string Serialize();
	void Deserialize(std::string sData);
};

class Settings {
public:
	bool SPNG_Enabled { true };
	bool SPNG_Notifications { true };
	bool SPNG_HotkeyFile { false };

	int SPNG_Modifier1 { 0 };
	int SPNG_Modifier2 { 0 };
	int SPNG_Modifier3 { 0 };

	// Health Effect Names
	std::string HealthRestoreName;
    std::string HealthRegenName;
    std::string HealthFortifyName;

    // Magicka Effect Names
    std::string MagickaRestoreName;
    std::string MagickaRegenName;
    std::string MagickaFortifyName;

    // Stamina Effect Names
    std::string StaminaRestoreName;
    std::string StaminaRegenName;
    std::string StaminaFortifyName;

	// Additional Effect Names
	std::string CureDiseaseName;
	std::string CurePoisonName;

	// Potion Data
	PotionData Health_Restore;
	PotionData Health_Regen;
	PotionData Health_Fortify;

	PotionData Magicka_Restore;
	PotionData Magicka_Regen;
	PotionData Magicka_Fortify;;

	PotionData Stamina_Restore;
	PotionData Stamina_Regen;
	PotionData Stamina_Fortify;

	std::vector<PotionData> HotkeyRecords;

	// Additional Data
	PotionData Cure_Disease;
	PotionData Cure_Poison;

	// Effect Name Lists
	std::vector<std::string> Positive_Effects;
	std::vector<std::string> Negative_Effects;

	// Additional Name Lists
	std::vector<std::string> Disease_Effects;

	// Blacklist
	std::unordered_set<int> blacklistForms;

	static Settings* GetSingleton();

	void LoadSettings();
	void LoadINI();
	void SaveINI();
	void LoadHotkeys();
	void LoadHotkeyINI();
	void SaveHotkeyINI();
	void LoadBlacklistINI(std::unordered_set<int> *set);

	std::string HotkeyString(int iNum, std::string sName);
	void SetupEffects();
	void DefaultAutoData(PotionData &SystemData, std::string sType, std::string sEffectName);

private:
	struct detail {
		// Get Values
		static void get_value(CSimpleIniA& a_ini, bool& a_value, const char* a_section, const char* a_key);
		static void get_value(CSimpleIniA& a_ini, std::string& a_value, const char* a_section, const char* a_key);
		static void get_value(CSimpleIniA& a_ini, int& a_value, const char* a_section, const char* a_key);
		static void get_value(CSimpleIniA& a_ini, float& a_value, const char* a_section, const char* a_key);

		// Set Values
		static void set_value(CSimpleIniA& a_ini, bool a_value, const char* a_section, const char* a_key);
		static void set_value(CSimpleIniA& a_ini, std::string a_value, const char* a_section, const char* a_key);
		static void set_value(CSimpleIniA& a_ini, int a_value, const char* a_section, const char* a_key);
		static void set_value(CSimpleIniA& a_ini, float a_value, const char* a_section, const char* a_key);

	};
};