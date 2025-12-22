#include "piquet_manual_state.h"

void PiquetManualState::onEnter()
{
    Serial.println("> Positioning piquet towards canon...");
    this->_controller->setReachedIdle(false);

    this->_controller->closeStopper();

    previousMillis = millis();
    previousRotatorMillis = millis();
    previousStopperMillis = millis();
}

void PiquetManualState::onExecute()
{
    this->handleRotator();
    this->handleStopper();

    // Going to idle
    if (!this->_controller->hasReachedIdle())
    {
        if (this->_controller->getCurrentAngle() <= PIQUET_IDLE_TARGET_ROTATION)
        {
            Serial.println("> Positioned piquet towards canon.");
            this->_controller->closeStopper();
            this->_controller->setReachedIdle(true);
            this->_controller->setReachedReloading(false);
        }
    }

    // Going to pince
    if (!this->_controller->getReachedReloading())
    {
        if (this->_controller->getCurrentAngle() >= PIQUET_RELOADING_TARGET_ROTATION)
        {
            Serial.println("> Positioned piquet towards pince.");
            this->_controller->openStopper();
            this->_controller->setReachedReloading(true);
            this->_controller->setReachedIdle(false);
        }
    }
}

void PiquetManualState::handleRotator()
{
    int8_t yButton = CrcLib::ReadDigitalChannel(CONTROLLER_PIQUET_TOGGLE_ROTATOR);

    uint32_t currentMillis = millis();
    if (currentMillis - previousRotatorMillis >= PIQUET_MANUAL_ROTATOR_DELAY)
    {
        if (yButton == HIGH)
        {
            this->_isTowardsPince = !this->_isTowardsPince;
            this->_controller->setCurrentAngle(this->_isTowardsPince ? PIQUET_RELOADING_TARGET_ROTATION : PIQUET_IDLE_TARGET_ROTATION);
            
            this->_isStopperClosed = true;
            this->_controller->closeStopper();
            previousRotatorMillis = currentMillis;
        }
    }
}

void PiquetManualState::handleStopper()
{
    int8_t bButton = CrcLib::ReadDigitalChannel(CONTROLLER_PIQUET_TOGGLE_STOPPER);

    uint32_t currentMillis = millis();
    if (currentMillis - previousStopperMillis >= PIQUET_MANUAL_STOPPER_DELAY)
    {
        if (bButton == HIGH)
        {
            this->_isStopperClosed = !this->_isStopperClosed;

            if (this->_isStopperClosed)
            {
                this->_controller->closeStopper();
            }
            else
            {
                this->_controller->openStopper();
            }

            previousStopperMillis = currentMillis;
        }
    }
}

void PiquetManualState::onExit()
{
    this->_controller->setReachedIdle(false);
}