#include "main_controller.h"

void MainController::initialize(BaseController *controller)
{
    this->_piquetController = new PiquetController();
    this->_piquetController->initialize(this);

    this->_pinceController = new PinceController();
    this->_pinceController->initialize(this);

    this->_propulseurController = new PropulseurController();
    this->_propulseurController->initialize(this);
}

void MainController::update()
{
    this->handleMove();

    this->_piquetController->update();
    this->_pinceController->update();
    this->_propulseurController->update();

    /**
     * Control the reloading sequence
     */
    if (!this->_startedReloadingSequence)
    {
        //this->checkForStartReloadingSequence();
    }
}

void MainController::handleMove()
{
    int32_t forwardChannel = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    int32_t yawChannel = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    int32_t strafeChannel = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);

    int32_t frontLeft = constrain(forwardChannel - yawChannel - strafeChannel, -128, MAXIMUM_MOVEMENT_SPEED);
    int32_t backLeft = constrain(forwardChannel - yawChannel + strafeChannel, -128, MAXIMUM_MOVEMENT_SPEED);
    int32_t frontRight = constrain(forwardChannel + yawChannel + strafeChannel, -128, MAXIMUM_MOVEMENT_SPEED);
    int32_t backRight = constrain(forwardChannel + yawChannel - strafeChannel, -128, MAXIMUM_MOVEMENT_SPEED);

    /*Serial.print("> Front left: ");
    Serial.print(frontLeft);
    Serial.print(", Back left: ");
    Serial.print(backLeft);
    Serial.print(", Front right: ");
    Serial.print(frontRight);
    Serial.print(", Back right: ");
    Serial.println(backRight);*/

    CrcLib::SetPwmOutput(FRONT_LEFT_DRIVE_PIN, frontLeft);
    CrcLib::SetPwmOutput(BACK_LEFT_DRIVE_PIN, backLeft);
    CrcLib::SetPwmOutput(FRONT_RIGHT_DRIVE_PIN, frontRight);
    CrcLib::SetPwmOutput(BACK_RIGHT_DRIVE_PIN, backRight);
}

void MainController::checkForStartReloadingSequence()
{
    int32_t buttonState = CrcLib::ReadDigitalChannel(RELOAD_KEY);
    if (buttonState == HIGH && !_startedReloadingSequence)
    {
        this->_startedReloadingSequence = true;
        this->startReloadingSequence();
    }
}

void MainController::startReloadingSequence()
{
    this->_piquetController->startReloadingSequence();
    this->_pinceController->startReloadingSequence();
}

void MainController::onPiquetReachedIdle()
{
    this->_startedReloadingSequence = false;
}

void MainController::onPiquetReachedReloading()
{
    this->checkForReturnToIdle();
}

void MainController::onPinceReachedReloading()
{
    this->checkForReturnToIdle();
}

void MainController::onPinceDroppedGamePieces()
{
    this->checkForReturnToIdle();
}

void MainController::checkForReturnToIdle()
{
    if (!this->_piquetController->getReachedReloading())
    {
        return;
    }

    if (!this->_pinceController->getReachedReloading())
    {
        return;
    }

    if (!this->_pinceController->getDroppedGamePieces())
    {
        return;
    }

    this->_piquetController->idle();
    this->_pinceController->changeState(new PinceManualState(this->_pinceController));
}