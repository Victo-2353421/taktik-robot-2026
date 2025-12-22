#include "pince_manual_state.h"

void PinceManualState::onEnter()
{
    Serial.println("> Entering manual pince state...");

    this->_controller->setCurrentFlipAngle(PINCE_FLIP_MIN_ANGLE);
    this->_controller->setReachedReloading(true);

    this->previousMillis = millis();
    this->previousManualFlipMillis = millis();
    this->previousDoigtMillis = millis();
    this->previousPoignetAngleMillis = millis();
}

void PinceManualState::onExecute()
{
    if (_controller->getReachedReloading())
    {
        return;
    }

    this->controlVerticalMovement();
    this->controlHorizontalMovement();
    this->controlPoignet();
    this->controlOpening();
    this->handleManualFlipAngle();
}

void PinceManualState::controlVerticalMovement()
{
    float upButton = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_UP_MOVEMENT);
    float downButton = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_DOWN_MOVEMENT);

    if (upButton == HIGH)
    {
        CrcLib::SetPwmOutput(PINCE_VERTICAL_DRIVE_PIN, 128);
    }
    else if (downButton == HIGH)
    {
        CrcLib::SetPwmOutput(PINCE_VERTICAL_DRIVE_PIN, 127);
    }
    else
    {
        CrcLib::SetPwmOutput(PINCE_VERTICAL_DRIVE_PIN, 0);
    }
}

void PinceManualState::controlHorizontalMovement()
{
    float leftButton = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_LEFT_MOVEMENT);
    float rightButton = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_RIGHT_MOVEMENT);

    if (leftButton == HIGH)
    {
        CrcLib::SetPwmOutput(PINCE_HORIZONTAL_DRIVE_PIN, 128);
    }
    else if (rightButton == HIGH)
    {
        CrcLib::SetPwmOutput(PINCE_HORIZONTAL_DRIVE_PIN, 127);
    }
    else
    {
        CrcLib::SetPwmOutput(PINCE_HORIZONTAL_DRIVE_PIN, 0);
    }
}

void PinceManualState::controlOpening()
{
    int32_t doigt_val = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_DOIGTS);

    uint32_t currentMillis = millis();
    if (currentMillis - this->previousDoigtMillis >= PINCE_POIGNET_DEBOUNCE_TIME)
    {
        if (doigt_val == HIGH)
        {
            if (this->_controller->areDoigtsOpen())
            {
                this->_controller->closePince();
            }
            else
            {
                this->_controller->openPince();
            }
            this->previousDoigtMillis = currentMillis;
        }
    }
}

void PinceManualState::controlPoignet()
{
    float poignetLeft = CrcLib::ReadAnalogChannel(CONTROLLER_PINCE_POIGNET_LEFT);
    float poignetRight = CrcLib::ReadAnalogChannel(CONTROLLER_PINCE_POIGNET_RIGHT);

    uint32_t currentMillis = millis();
    if (currentMillis - this->previousPoignetAngleMillis >= PINCE_POIGNET_DEBOUNCE_TIME)
    {
        if (poignetLeft > 0)
        {
            this->changePoignetRotation(-90);
            this->previousPoignetAngleMillis = currentMillis;
        }
        else if (poignetRight > 0)
        {
            this->changePoignetRotation(90);
            this->previousPoignetAngleMillis = currentMillis;
        }
    }
}

void PinceManualState::handleManualFlipAngle()
{
    uint8_t isFlipped = CrcLib::ReadDigitalChannel(CONTROLLER_PINCE_MANUAL_FLIP);

    uint32_t currentMillis = millis();
    if (currentMillis - previousManualFlipMillis >= PINCE_FLIP_MANUAL_DELAY)
    {
        if (isFlipped == HIGH)
        {
            switch (this->_flippedState)
            {
            case 0:
                this->_controller->setCurrentFlipAngle(PINCE_FLIP_IDLE_ANGLE);
                this->_flippedState = 1;
                break;
            case 1:
                this->_controller->setCurrentFlipAngle(PINCE_FLIP_MAX_ANGLE);
                this->_flippedState = 2;
                break;
            case 2:
                this->_controller->setCurrentFlipAngle(PINCE_FLIP_MIN_ANGLE);
                this->_flippedState = 0;
                break;
            }

            previousManualFlipMillis = currentMillis;
        }
    }
}

void PinceManualState::changePoignetRotation(int16_t value)
{
    int16_t newValue = this->_currentPoignetAngle + value;
    Serial.println(value);

    if (newValue > PINCE_POIGNET_MAX_ANGLE)
    {
        newValue = PINCE_POIGNET_MAX_ANGLE;
    }
    else if (newValue < PINCE_POIGNET_MIN_ANGLE)
    {
        newValue = PINCE_POIGNET_MIN_ANGLE;
    }

    this->_currentPoignetAngle = newValue;
    this->_controller->setCurrentPoignetAngle(this->_currentPoignetAngle);

    Serial.print("> Poignet angle: ");
    Serial.println(this->_currentPoignetAngle);
}

void PinceManualState::onExit()
{
}