#include "piquet_reloading_state.h"

void PiquetReloadingState::onEnter()
{
    Serial.println("> Positioning piquet towards pince...");
    this->_controller->setReachedReloading(false);

    this->_controller->openStopper();

    previousMillis = millis();
}

void PiquetReloadingState::onExecute()
{
    if (_controller->getCurrentAngle() >= PIQUET_RELOADING_TARGET_ROTATION)
    {
        this->_controller->setReachedReloading(true);
        return;
    }

    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= PIQUET_SPEED)
    {
        previousMillis = currentMillis;
        _controller->setCurrentAngle(_controller->getCurrentAngle() + 1);
    }
}

void PiquetReloadingState::onExit()
{
    this->_controller->setReachedReloading(false);
}