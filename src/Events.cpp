#include "Utility.h"
#include "SwiftPotion.h"

// Look for changes in player inventory
class OnContainerChangedEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
public:
    std::mutex container_mutex;
        
    static OnContainerChangedEventHandler* GetSingleton() {
        static OnContainerChangedEventHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override {
        auto utility = Utility::GetSingleton();
            
        if (a_event->newContainer != utility->GetPlayer()->formID)
            return RE::BSEventNotifyControl::kContinue;

        const std::lock_guard<std::mutex> lock(container_mutex);

        auto alchemyItem = RE::TESForm::LookupByID<RE::AlchemyItem>(a_event->baseObj);

        if (alchemyItem && !alchemyItem->IsFood())
            Utility::StopperCheck(alchemyItem);

        return RE::BSEventNotifyControl::kContinue;
    }

    static void Register() {
        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventHolder->AddEventSink(OnContainerChangedEventHandler::GetSingleton());
    }
};

class MagicEffectApplyEventHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent> {
public:
    static MagicEffectApplyEventHandler* GetSingleton() {
        static MagicEffectApplyEventHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_source) override {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;
            
        auto* effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);
        auto* target = a_event->target.get();

        if (target == RE::PlayerCharacter::GetSingleton() && effect)
            SwiftPotion::ResistCheck(effect->data.resistVariable);

        return RE::BSEventNotifyControl::kContinue;
    }

    static void Register() {
        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventHolder->AddEventSink(MagicEffectApplyEventHandler::GetSingleton());
    }
};

// Look for key inputs
class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*> {
private:
    bool isModifier1{false};
    bool isModifier2{false};
    bool isModifier3{false};

public:
    static InputEventHandler* GetSingleton() {
        static InputEventHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {
        if (a_event) {
            const auto ui = RE::UI::GetSingleton();
            const auto settings = Settings::GetSingleton();

            if (ui && !ui->GameIsPaused() && !ui->IsApplicationMenuOpen() && !ui->IsItemMenuOpen() && !ui->IsMenuOpen(RE::InterfaceStrings::GetSingleton()->dialogueMenu)) {
                const auto controlMap = RE::ControlMap::GetSingleton();
                const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
                const auto playerControls = RE::PlayerControls::GetSingleton();

                if (controlMap && playerCharacter && playerControls) {
                    for (auto event = *a_event; event; event = event->next) {
                        if (event->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                            const auto button = static_cast<RE::ButtonEvent*>(event);
                            if (!button || (button->IsPressed() && !button->IsDown()))
                                continue;

                            // Get an adjusted scan code
                            auto scan_code = HelperFunctions::FixCode(button->device.get(), button->GetIDCode());

                            // Modifier Key
                            if (scan_code == settings->SPNG_Modifier1)
                                isModifier1 = button->IsPressed();
                            else if (scan_code == settings->SPNG_Modifier2)
                                isModifier2 = button->IsPressed();
                            else if (scan_code == settings->SPNG_Modifier3)
                                isModifier3 = button->IsPressed();

                            // Dont activate on button up
                            if (!button->IsUp())
                                SwiftPotion::ProcessHotkey(scan_code, isModifier1, isModifier2, isModifier3);
                                
                        }
                    }
                }
            }
        }
            
        return RE::BSEventNotifyControl::kContinue;
    }

    static void Register() {
        RE::BSInputDeviceManager* inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
        inputDeviceManager->AddEventSink(InputEventHandler::GetSingleton());
    }
};

namespace Events {
    void Register() {
        OnContainerChangedEventHandler::Register();
        MagicEffectApplyEventHandler::Register();
        InputEventHandler::Register();
        
    }
}