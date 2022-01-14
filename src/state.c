#include "state.h"

stove_state_t update_state(stove_state_t state, temperatures_t temps) {
    switch (state)
    {
    case OFF:
    case EXCTINCTION:
        return OFF;

    case SETTING_FIRE:
            if (temps.stove_avr >= 70) 
                return BURNING;
        return SETTING_FIRE;

    case BURNING:
        if (temps.stove_avr < 60) {
            return EXCTINCTION;
        }

        if (temps.stove_avr >= 86) {
            return COOLING;
        }
        return BURNING;

     case COOLING:
        if (temps.stove_avr <= 74) {
            return BURNING;
        }
        if (temps.stove_avr <= 84) {
            return PASSIVE;
        }

        return COOLING;

    case PASSIVE:
        if (temps.stove_avr <= 74) {
            return BURNING;
        }
        if (temps.stove_avr >= 86) {
            return COOLING;
        }

        return PASSIVE;
    
    default:
        return OFF;
    }

    return OFF;
}
