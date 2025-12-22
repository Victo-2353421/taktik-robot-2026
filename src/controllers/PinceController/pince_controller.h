#ifndef PINCE_CONTROLLER_H
#define PINCE_CONTROLLER_H

#include <Servo.h>
#include "controllers/MainController/main_controller.h"
#include "controllers/BaseController/base_controller.h"
#include "state_machines/PinceStates/pince_manual_state.h"
#include "state_machines/PinceStates/pince_reloading_state.h"

class MainController;

class PinceController : public BaseController
{
public:
    void initialize(BaseController *mainController = nullptr) override;
    void update() override;

    /**
     * Move the pince vertically
     * 1 = up, 0 = stop, -1 = down
     */
    void translateVertical(int32_t);
    
    /**
     * Move the pince horizontally
     * 1 = left, 0 = stop, -1 = right
     */
    void translateHorizontal(int32_t);
    
    /**
     * Move the pince horizontally
     * 1 = left, 0 = stop, -1 = right
     */
    void handleDoigts();

    bool areDoigtsOpen();
    
    /**
     * Is the upper limit switch reached
     */
    bool upperLimitReached();
    
    /**
     * Is the lower limit switch reached
     */
    bool lowerLimitReached();
    
    /**
     * Controls the movement of the poignet
     */
    void checkForPoignetMovement();
    
    /**
     * Start the reloading sequence for the piquet
     */
    void startReloadingSequence();

    /**
     * Returns if the pince has reached the reloading position
     */
    bool getReachedReloading();
    
    /**
     * Notify the controller that the pince has reached the reloading position
     */
    void setReachedReloading(bool);
    
    /**
     * Returns if the pince has dropped the game pieces
     */
    bool getDroppedGamePieces();
    
    /**
     * Notify the controller that the pince has dropped the game pieces
     */
    void setDroppedGamePieces(bool);

    /**
     * Returns the current angle of the pince's poignet
     */
    int32_t getCurrentPoignetAngle();

    /**
     * Sets the current angle of the pince's poignet
     */
    void setCurrentPoignetAngle(int32_t);

    /**
     * Returns the current angle of the pince's flip
     */
    int32_t getCurrentFlipAngle();

    /**
     * Sets the current angle of the pince's flip
     */
    void setCurrentFlipAngle(int32_t);

    /**
     * Ouvrir la pince
     */
    void openPince();

    /**
     * Fermer la pince
     */
    void closePince();

    /**
     * Déplacer le poignet à une position
     * @param angle Angle du poignet
     */
    void movePoignetPince(int32_t angle);

private:
    MainController *_mainController = nullptr;
    Servo _pinceDoigtServo1;
    Servo _pinceDoigtServo2;
    Servo _pincePoignetServo;
    Servo _pinceFlipServo;

    int32_t _currentPincePoignetAngle = 0;
    int32_t _currentPinceFlipAngle = PINCE_FLIP_MIN_ANGLE;
    int32_t _currentPinceDoigtAngle1 = 0;
    int32_t _currentPinceDoigtAngle2 = 0;
    bool _hasReachedReloading = false;
    bool _hasDroppedGamePieces = false;
    bool _areDoigtsOpen = false;
};

#endif