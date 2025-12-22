#ifndef PIQUET_MANUAL_STATE_H
#define PIQUET_MANUAL_STATE_H

#include <Servo.h>
#include "pin_mappings.h"
#include "../../controllers/PiquetController/piquet_controller.h"
#include "../../state_machines/state.h"

class PiquetController;

class PiquetManualState : public State
{
public:
    PiquetManualState(PiquetController *controller) : _controller(controller) {};

    void onEnter() override;
    void onExecute() override;
    void onExit() override;

    void handleRotator();
    void handleStopper();

private:
    PiquetController *_controller;
    uint32_t previousMillis = 0;
    uint32_t previousRotatorMillis = 0;
    uint32_t previousStopperMillis = 0;

    bool _isTowardsPince = false;
    bool _isStopperClosed = false;
};

#endif
