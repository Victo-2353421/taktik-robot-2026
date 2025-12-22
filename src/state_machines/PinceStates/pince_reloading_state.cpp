#include "pince_reloading_state.h"

void PinceReloadingState::onEnter()
{
    Serial.println("> Moving pince towards piquet...");
    this->_controller->setReachedReloading(false);

    this->_controller->closePince();

    previousMillis = millis();
    previousDropDelayMillis = millis();
}

void PinceReloadingState::onExecute()
{
    this->updateLimits();
    this->checkHasReachedReloading();
    this->checkHasDroppedGamePieces();

    this->handleVerticalAlignment();
    this->handleHorizontalAlignment();
    this->handleAutomaticFlipAngle();
}

void PinceReloadingState::handleVerticalAlignment()
{
    if (this->_hasReachedVerticalReloadingLimit)
    {
        this->_controller->translateVertical(0);
        return;
    }

    if (this->_hasReachedUpperLimit)
    {
        this->_controller->translateVertical(-1);
    }
    else
    {
        this->_controller->translateVertical(1);
    }
}

void PinceReloadingState::handleHorizontalAlignment()
{
    if (this->_hasReachedHorizontalReloadingLimit)
    {
        this->_controller->translateHorizontal(0);
        return;
    }

    if (this->_hasReachedLeftLimit)
    {
        this->_controller->translateHorizontal(-1);
    }
    else
    {
        this->_controller->translateHorizontal(1);
    }
}

void PinceReloadingState::handleAutomaticFlipAngle()
{
    if (!this->_hasReachedVerticalReloadingLimit || !this->_hasReachedHorizontalReloadingLimit)
    {
        return;
    }

    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= PINCE_FLIP_SPEED)
    {
        _controller->setCurrentFlipAngle(_controller->getCurrentFlipAngle() + 1);
        previousMillis = currentMillis;
    }
}

void PinceReloadingState::checkHasReachedReloading()
{
    // Validate flip angle
    if (this->_controller->getCurrentFlipAngle() < PINCE_FLIP_MAX_ANGLE)
    {
        return;
    }

    // Validate limit switches
    if (!this->_hasReachedVerticalReloadingLimit || !this->_hasReachedHorizontalReloadingLimit)
    {
        return;
    }

    Serial.println("> Positioned pince towards piquet.");
    this->_controller->setReachedReloading(true);
}

void PinceReloadingState::checkHasDroppedGamePieces()
{
    // Validate that the pince has reached the reloading position
    if (!this->_controller->getReachedReloading())
    {
        return;
    }

    // Drop the game pieces
    this->_controller->openPince();

    // Wait for the drop delay to be met
    uint32_t currentMillis = millis();
    if (currentMillis - this->previousDropDelayMillis >= PINCE_FLIP_SPEED)
    {
        this->_controller->setDroppedGamePieces(true);
        Serial.println("> Dropped game pieces.");
        this->previousDropDelayMillis = currentMillis;
    }
}

void PinceReloadingState::updateLimits()
{
    this->_hasReachedUpperLimit = CrcLib::GetDigitalInput(PINCE_UPPER_LIMIT_SWITCH_PIN) == HIGH && CrcLib::GetDigitalInput(PINCE_VERTICAL_RELOAD_LIMIT_SWITCH_PIN) == LOW && CrcLib::GetDigitalInput(PINCE_LOWER_LIMIT_SWITCH_PIN) == LOW;
    this->_hasReachedLeftLimit = CrcLib::GetDigitalInput(PINCE_LEFT_LIMIT_SWITCH_PIN) == HIGH && CrcLib::GetDigitalInput(PINCE_HORIZONTAL_RELOAD_LIMIT_SWITCH_PIN) == LOW && CrcLib::GetDigitalInput(PINCE_RIGHT_LIMIT_SWITCH_PIN) == LOW;
    this->_hasReachedVerticalReloadingLimit = CrcLib::GetDigitalInput(PINCE_VERTICAL_RELOAD_LIMIT_SWITCH_PIN) == HIGH && CrcLib::GetDigitalInput(PINCE_UPPER_LIMIT_SWITCH_PIN) == LOW && CrcLib::GetDigitalInput(PINCE_LOWER_LIMIT_SWITCH_PIN) == LOW;
    this->_hasReachedHorizontalReloadingLimit = CrcLib::GetDigitalInput(PINCE_HORIZONTAL_RELOAD_LIMIT_SWITCH_PIN) == HIGH && CrcLib::GetDigitalInput(PINCE_LEFT_LIMIT_SWITCH_PIN) == LOW && CrcLib::GetDigitalInput(PINCE_RIGHT_LIMIT_SWITCH_PIN) == LOW;
}

void PinceReloadingState::onExit()
{
    this->_controller->setReachedReloading(false);
}