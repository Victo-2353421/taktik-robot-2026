#ifndef PIQUET_CONTROLLER_H
#define PIQUET_CONTROLLER_H

#include <Servo.h>
#include "../MainController/main_controller.h"
#include "../BaseController/base_controller.h"
#include "../../state_machines/PiquetStates/piquet_manual_state.h"
#include "../../state_machines/PiquetStates/piquet_idle_state.h"
#include "../../state_machines/PiquetStates/piquet_reloading_state.h"

class MainController;

class PiquetController : public BaseController
{
public:
    void initialize(BaseController *mainController = nullptr) override;
    void update() override;

    /**
     * Start the reloading sequence for the piquet
     */
    void startReloadingSequence();

    /**
     * Sets the state for manual override
     */
    void manual();
    
    /**
     * Sets the state for idle
     */
    void idle();

    /**
     * Returns if the piquet has reached the idle position
     */
    bool hasReachedIdle();

    /**
     * Notify the controller that the piquet has reached the idle position
     */
    void setReachedIdle(bool);

    /**
     * Returns if the piquet has reached the reloading position
     */
    bool getReachedReloading();

    /**
     * Notify the controller that the piquet has reached the reloading position
     */
    void setReachedReloading(bool);

    /**
     * Returns the current angle of the piquet
     */
    int32_t getCurrentAngle();

    /**
     * Sets the current angle of the piquet
     */
    void setCurrentAngle(int32_t);

    /**
     * Open the piquet stopper
     */
    void openStopper();

    /**
     * Close the piquet stopper
     */
    void closeStopper();

private:
    MainController *_mainController = nullptr;
    int32_t _currentPiquetAngle = 0;
    int32_t _currentStopperAngle = 0;
    Servo _stopperServo;
    Servo _piquetServo;

    bool _hasReachedReloading = false;
    bool _hasReachedIdle = false;
};

#endif
