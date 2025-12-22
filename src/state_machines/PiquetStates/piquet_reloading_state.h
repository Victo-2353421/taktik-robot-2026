#ifndef PIQUET_RELOADING_STATE_H
#define PIQUET_RELOADING_STATE_H

#include "../../controllers/PiquetController/piquet_controller.h"
#include "../../state_machines/state.h"
#include "variables.h"

class PiquetController;

class PiquetReloadingState : public State
{
public:
    PiquetReloadingState(PiquetController *controller) : _controller(controller) {};

    void onEnter() override;
    void onExecute() override;
    void onExit() override;

private:
    PiquetController *_controller;
    uint32_t previousMillis = 0;
};

#endif
