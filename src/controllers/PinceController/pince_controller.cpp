#include "pince_controller.h"

void PinceController::initialize(BaseController *controller)
{
    this->_mainController = (MainController *)controller;

    this->_pinceDoigtServo1.attach(PINCE_DOIGT_SERVO_1_PIN);
    this->_pinceDoigtServo2.attach(PINCE_DOIGT_SERVO_2_PIN);
    this->_pincePoignetServo.attach(PINCE_POIGNET_SERVO_PIN);
    this->_pinceFlipServo.attach(PINCE_FLIP_SERVO_PIN);

    this->closePince();

    this->currentState = new PinceManualState(this);

    Serial.println("> Contrôleur de pince initialisé.");
}

void PinceController::update()
{
    if (this->currentState != nullptr)
    {
        this->currentState->onExecute();
    }

    this->handleDoigts();

    this->_pincePoignetServo.write(this->_currentPincePoignetAngle);
    this->_pinceFlipServo.write(this->_currentPinceFlipAngle);
    this->_pinceDoigtServo1.write(this->_currentPinceDoigtAngle1);
    this->_pinceDoigtServo2.write(this->_currentPinceDoigtAngle2);
}

void PinceController::translateVertical(int32_t value)
{
    int32_t mappedValue = map(value, -1, 1, 0, 255);

    CrcLib::SetPwmOutput(PINCE_VERTICAL_DRIVE_PIN, mappedValue);
}

void PinceController::translateHorizontal(int32_t value)
{
    int32_t mappedValue = map(value, -1, 1, 0, 255);

    CrcLib::SetPwmOutput(PINCE_HORIZONTAL_DRIVE_PIN, mappedValue);
}

void PinceController::handleDoigts()
{
    if (this->_areDoigtsOpen)
    {
        this->_currentPinceDoigtAngle1 = 180 - PINCE_DOIGT_MAX_ANGLE;
        this->_currentPinceDoigtAngle2 = PINCE_DOIGT_MAX_ANGLE;
    }
    else
    {
        this->_currentPinceDoigtAngle1 = 180 - PINCE_DOIGT_IDLE_ANGLE;
        this->_currentPinceDoigtAngle2 = PINCE_DOIGT_IDLE_ANGLE;
    }
}

bool PinceController::areDoigtsOpen()
{
    return this->_areDoigtsOpen;
}

void PinceController::startReloadingSequence()
{
    this->changeState(new PinceReloadingState(this));
}

void PinceController::setReachedReloading(bool value)
{
    if (value == true)
    {
        this->_mainController->onPinceReachedReloading();
    }

    this->_hasReachedReloading = value;
}

bool PinceController::getReachedReloading()
{
    return this->_hasReachedReloading;
}

void PinceController::openPince()
{
    Serial.println("> Pince ouverte.");

    this->_areDoigtsOpen = true;
}

void PinceController::closePince()
{
    Serial.println("> Pince fermée.");

    this->_areDoigtsOpen = false;
}

bool PinceController::upperLimitReached()
{
    return CrcLib::GetDigitalInput(PINCE_UPPER_LIMIT_SWITCH_PIN) == HIGH;
}

bool PinceController::lowerLimitReached()
{
    return CrcLib::GetDigitalInput(PINCE_LOWER_LIMIT_SWITCH_PIN) == HIGH;
}

int32_t PinceController::getCurrentPoignetAngle()
{
    return this->_currentPincePoignetAngle;
}

void PinceController::setCurrentPoignetAngle(int32_t angle)
{
    this->_currentPincePoignetAngle = angle;
}

int32_t PinceController::getCurrentFlipAngle()
{
    return this->_currentPinceFlipAngle;
}

void PinceController::setCurrentFlipAngle(int32_t angle)
{
    this->_currentPinceFlipAngle = angle;
}

void PinceController::setDroppedGamePieces(bool value)
{
    if (value == true)
    {
        this->_mainController->onPinceDroppedGamePieces();
    }

    this->_hasDroppedGamePieces = value;
}

bool PinceController::getDroppedGamePieces()
{
    return this->_hasDroppedGamePieces;
}