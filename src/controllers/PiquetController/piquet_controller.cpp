#include "piquet_controller.h"

void PiquetController::initialize(BaseController *controller)
{
    this->_mainController = (MainController *)controller;

    this->_piquetServo.attach(PIQUET_ROTATOR_SERVO_PIN);
    this->_stopperServo.attach(PIQUET_STOPPER_SERVO_PIN);

    this->manual();

    Serial.println("> Contrôleur de piquet initialisé.");
}

void PiquetController::update()
{
    if (this->currentState != nullptr)
    {
        this->currentState->onExecute();
    }

    this->_piquetServo.write(this->_currentPiquetAngle);
    this->_stopperServo.write(this->_currentStopperAngle);
}

void PiquetController::startReloadingSequence()
{
    this->changeState(new PiquetReloadingState(this));
}

void PiquetController::manual()
{
    this->changeState(new PiquetManualState(this));
}

void PiquetController::idle()
{
    this->changeState(new PiquetIdleState(this));
}

void PiquetController::openStopper()
{
    Serial.println("> Piquet stopper opened.");
    this->_currentStopperAngle = 0;
}

void PiquetController::closeStopper()
{
    Serial.println("> Piquet stopper closed.");
    this->_currentStopperAngle = 90;
}

bool PiquetController::hasReachedIdle()
{
    return this->_hasReachedIdle;
}

void PiquetController::setReachedIdle(bool value)
{
    if (value == true)
    {
        this->_mainController->onPiquetReachedIdle();
    }

    this->_hasReachedIdle = value;
}

bool PiquetController::getReachedReloading()
{
    return this->_hasReachedReloading;
}

void PiquetController::setReachedReloading(bool value)
{
    if (value == true)
    {
        this->_mainController->onPiquetReachedReloading();
    }

    this->_hasReachedReloading = value;
}

int32_t PiquetController::getCurrentAngle()
{
    return this->_currentPiquetAngle;
}

void PiquetController::setCurrentAngle(int32_t angle)
{
    if (angle < PIQUET_IDLE_TARGET_ROTATION) {
        angle = PIQUET_IDLE_TARGET_ROTATION;
    } else if (angle > PIQUET_RELOADING_TARGET_ROTATION) {
        angle = PIQUET_RELOADING_TARGET_ROTATION;
    }

    this->_currentPiquetAngle = angle;
}