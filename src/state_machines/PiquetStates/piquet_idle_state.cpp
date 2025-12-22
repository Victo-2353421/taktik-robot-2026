#include "piquet_idle_state.h"

void PiquetIdleState::onEnter()
{
    Serial.println("> Positioning piquet towards canon...");
    this->_controller->setReachedIdle(false);
    
    this->_controller->closeStopper();

    previousMillis = millis();
}

void PiquetIdleState::onExecute()
{
    // TODO: Fix this
    if (!_controller->hasReachedIdle())
    {
        if (_controller->getCurrentAngle() <= PIQUET_IDLE_TARGET_ROTATION)
        {
            Serial.println("> Positioned piquet towards canon.");
            this->_controller->setReachedIdle(true);
            this->_controller->openStopper();
        } else {
            return;
        }
    }
    
    if (_controller->getCurrentAngle() >= PIQUET_RELOADING_TARGET_ROTATION)
    {
        Serial.println("> Positioned piquet towards canon.");
        this->_controller->setReachedIdle(true);
        this->_controller->closeStopper();
        return;
    }

    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= PIQUET_SPEED)
    {
        previousMillis = currentMillis;
        //_controller->setCurrentAngle(_controller->getCurrentAngle() - 1);
    }
}

void PiquetIdleState::onExit()
{
    this->_controller->setReachedIdle(false);
}