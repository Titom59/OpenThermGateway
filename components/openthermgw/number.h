#pragma once

#include "esphome/components/number/number.h"
#include "esphome/core/hal.h"
#include "input.h"

namespace esphome {
namespace OpenThermGateway {

class OpenThermGatewayNumber : public number::Number, public Component, public OpenThermGatewayInput {
protected:
    void setup() override;
    
    uint32_t m_msLastSet{0};

    void control(float value) override {
        this->m_msLastSet = millis();
        this->publish_state(value);
    }

public:
    void set_min_value(float min_value) override { this->traits.set_min_value(min_value); }
    void set_max_value(float max_value) override { this->traits.set_max_value(max_value); }
    void set_step(float step) override { this->traits.set_step(step); }

    // Override value to apply, or 0 when disabled or not refreshed by Home Assistant within timeout_ms
    float active_override(uint32_t timeout_ms) {
        if (!this->has_state() || this->state == 0.f || this->m_msLastSet == 0)
            return 0.f;
        if (millis() - this->m_msLastSet > timeout_ms)
            return 0.f;
        return this->state;
    }
};

} // namespace OpenThermGateway
} // namespace esphome
