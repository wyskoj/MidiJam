//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_ANIMATIONCONTROLLER_H
#define MIDIJAM_ANIMATIONCONTROLLER_H

#include <windows.h>

class IAnimationController;

/**
 * A node in the global linked list of all active animation controllers.
 * Registered in InitBaseClass, used to tick all controllers each frame.
 */
struct AnimationControllerNode
{
    IAnimationController*  pController;
    AnimationControllerNode* pNext;
};

/**
 * Timer-based animation controller supporting both high-resolution
 * (QueryPerformanceCounter) and low-resolution (timeGetTime) timing.
 *
 * Despite the I prefix, this is a concrete class, not an interface.
 * The prefix likely reflects the original developer's convention for
 * a base class intended to be subclassed.
 *
 * Registered in a global linked list on construction via InitBaseClass.
 * TickFrame/UntickFrame support pausing by counting active tickers —
 * elapsed time is frozen while num_frames > 0.
 *
 * Original struct layout (sizeof = 0x29):
 *   +0x00  IAnimationController_VTable*  vtable
 *   +0x04  LARGE_INTEGER                 lpFrequency
 *   +0x0C  double                        tickToMs
 *   +0x10  int                           lastTick
 *   +0x14  LARGE_INTEGER                 lpPerformanceCount
 *   +0x1C  bool                          isHighResolutionTiming
 *   +0x1D  int                           num_frames
 *   +0x21  double                        elapsed
 */
class IAnimationController
{
public:
    LARGE_INTEGER lpFrequency;
    double        tickToMs;
    int           lastTick;
    LARGE_INTEGER lpPerformanceCount;
    bool          isHighResolutionTiming;
    int           num_frames;
    double        elapsed;

    /**
     * Constructs the controller, registers it in the global list,
     * and initializes high-resolution or low-resolution timing.
     */
    IAnimationController();

    /**
     * Registers this controller in the global animation controller linked list
     * and sets the base vtable. Called at the start of the constructor.
     */
    void InitBaseClass();

    /**
     * Resets the timing reference to now and recomputes elapsed.
     */
    virtual void Update();

    /**
     * Returns elapsed time in milliseconds since the last Update() call.
     * If num_frames > 0 (paused), returns the frozen elapsed value.
     */
    virtual double ComputeElapsed();

    /**
     * Freezes elapsed time and increments the frame counter.
     * Elapsed is captured before incrementing so the first tick records
     * the correct time.
     */
    void TickFrame();

    /**
     * Decrements the frame counter. When it reaches zero, resyncs the
     * timing reference so elapsed continues correctly from the frozen value.
     */
    void UntickFrame();

    /**
     * Destroys this controller. Called by Ms3dBundle during cleanup.
     */
    virtual ~IAnimationController();

    /**
     * Marks the controller for destruction and removes it from the global list.
     */
    void Destroy();

    void UnregisterFromGlobalList();
    void DestroyAndUnregister(bool freeMemory);
};

/**
 * Factory function — allocates and constructs an IAnimationController.
 * Called from Ms3dBundle::Init.
 */
IAnimationController* CreateAnimationController();

#endif // MIDIJAM_ANIMATIONCONTROLLER_H