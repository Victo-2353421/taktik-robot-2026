#ifndef PINCE_RELOADING_STATE_H
#define PINCE_RELOADING_STATE_H

#include "../../controllers/PinceController/pince_controller.h"
#include "../../state_machines/state.h"
#include "variables.h"

class PinceController;

class PinceReloadingState : public State
{
public:
    PinceReloadingState(PinceController *controller) : _controller(controller) {};

    void onEnter() override;
    void onExecute() override;
    void onExit() override;

    /**
     * Moves the pince up or down on wether it's above or below the reloading limit switch
     */
    void handleVerticalAlignment();
    
    /**
     * Moves the pince left or right on wether it's left or right of the reloading limit switch
     */
    void handleHorizontalAlignment();
    
    /**
     * Moves the pince flip angle if the positions are reached
     */
    void handleAutomaticFlipAngle();

    /**
     * Updates the limit switch states
     */
    void updateLimits();

    /**
     * Checks if the pince has reached the reloading position
     */
    void checkHasReachedReloading();
    
    /**
     * Checks if the pince has dropped the game pieces
     */
    void checkHasDroppedGamePieces();

private:
    PinceController *_controller;
    uint32_t previousMillis = 0;
    uint32_t previousDropDelayMillis = 0;

    bool _hasReachedVerticalReloadingLimit = false;
    bool _hasReachedHorizontalReloadingLimit = false;
    bool _hasReachedUpperLimit = false;
    bool _hasReachedLeftLimit = false;
};

#endif
