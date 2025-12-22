#include "base_controller.h"

void BaseController::initialize(BaseController *controller)
{
    Serial.println("BaseController::initialize() not implemented.");
    exit(1);
}

void BaseController::update()
{
    Serial.println("BaseController::update() not implemented.");
    exit(1);
}

void BaseController::changeState(State *newState)
{
    if (currentState)
    {
        currentState->onExit();
    }
    currentState = newState;
    currentState->onEnter();
}