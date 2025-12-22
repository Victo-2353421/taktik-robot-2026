#ifndef PIQUET_IDLE_STATE_H
#define PIQUET_IDLE_STATE_H

#include <Servo.h>
#include "pin_mappings.h"
#include "../../controllers/PiquetController/piquet_controller.h"
#include "../../state_machines/state.h"
#include "variables.h"

class PiquetController;

class PiquetIdleState : public State
{
public:
    PiquetIdleState(PiquetController *controller) : _controller(controller) {};

    void onEnter() override;
    void onExecute() override;
    void onExit() override;

private:
    PiquetController *_controller;
    uint32_t previousMillis = 0;
};

#endif
