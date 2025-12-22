#ifndef PINCE_MANUAL_STATE_H
#define PINCE_MANUAL_STATE_H

#include <Servo.h>
#include "pin_mappings.h"
#include "controllers/PinceController/pince_controller.h"
#include "state_machines/state.h"

class PinceController;

class PinceManualState : public State
{
public:
    PinceManualState(PinceController *controller) : _controller(controller) {}

    void onEnter() override;
    void onExecute() override;
    void onExit() override;

private:
    PinceController *_controller;
    uint32_t previousMillis = 0;
    uint32_t previousDoigtMillis = 0;
    uint32_t previousManualFlipMillis = 0;
    uint32_t previousPoignetAngleMillis = 0;

    uint16_t _currentPoignetAngle = PINCE_POIGNET_IDLE_ANGLE;
    uint8_t _flippedState = 1;

    /**
     * Control the vertical position of the pince
     */
    void controlVerticalMovement();

    /**
     * Control the horizontal position of the pince
     */
    void controlHorizontalMovement();
    
    /**
     * Control the rotation of the poignet
     */
    void controlPoignet();
    
    void handleManualFlipAngle();

    /**
     * Control the opening of the pince
     */
    void controlOpening();
    
    /**
     * Change the rotation of the poignet
     */
    void changePoignetRotation(int16_t value);
};

#endif
