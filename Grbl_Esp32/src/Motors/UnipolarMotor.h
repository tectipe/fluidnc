#pragma once

#include "Motor.h"

namespace Motors {
    class UnipolarMotor : public Motor {
    public:
        UnipolarMotor() = default;

        // Overrides for inherited methods
        void init() override;
        bool set_homing_mode(bool isHoming) override { return true; }
        void set_disable(bool disable) override;
        void set_direction(bool) override;
        void step() override;

        // Configuration handlers:
        void validate() const override {
            Assert(!_pin_phase0.undefined(), "Phase 0 pin should be configured.");
            Assert(!_pin_phase1.undefined(), "Phase 1 pin should be configured.");
            Assert(!_pin_phase2.undefined(), "Phase 2 pin should be configured.");
            Assert(!_pin_phase3.undefined(), "Phase 3 pin should be configured.");
        }

        void handle(Configuration::HandlerBase& handler) override {
            handler.handle("phase0", _pin_phase0);
            handler.handle("phase1", _pin_phase1);
            handler.handle("phase2", _pin_phase2);
            handler.handle("phase3", _pin_phase3);
            handler.handle("half_step", _half_step);
        }

        // Name of the configurable. Must match the name registered in the cpp file.
        const char* name() const override { return "unipolar"; }

    private:
        Pin     _pin_phase0;
        Pin     _pin_phase1;
        Pin     _pin_phase2;
        Pin     _pin_phase3;
        uint8_t _current_phase = 0;
        bool    _half_step = true;
        bool    _enabled = false;
        bool    _dir = true;

    protected:
        void config_message() override;
    };
}
