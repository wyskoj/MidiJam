//
// Created by Jacob on 11/15/2025.
//

#ifndef MIDIJAM_IANIMATIONCONTROLLER_H
#define MIDIJAM_IANIMATIONCONTROLLER_H
#include <windows.h>

class AnimationController {
public:
    LARGE_INTEGER lpFrequency;
    float tickToMs;
    int lastTick;
    LARGE_INTEGER lpPerformanceCount;
    bool isHighResolutionTiming;
    int num_frames;
    double elapsed;

    AnimationController();

    void InitBaseClass();

    void TickFrame();

    void UntickFrame();

    void Update();

    double ComputeElapsed() const;
};

struct AnimationControllerNode // sizeof=0x8
{
    AnimationController *pController;
    AnimationControllerNode *pNext;
};

extern AnimationControllerNode* g_pAnimationControllerList;

AnimationController *CreateAnimationController();


#endif //MIDIJAM_IANIMATIONCONTROLLER_H
