//
// Created by Jacob on 11/15/2025.
//
#include "AnimationController.h"

AnimationControllerNode* g_pAnimationControllerList = nullptr;

AnimationController::AnimationController() {
    InitBaseClass();
    if (QueryPerformanceFrequency(&this->lpFrequency)) {
        this->isHighResolutionTiming = 1;
        QueryPerformanceCounter(&this->lpPerformanceCount);
        this->tickToMs = 1.0 / this->lpFrequency.QuadPart * 1000.0;
    } else {
        this->isHighResolutionTiming = 0;
        this->lastTick = timeGetTime();
    }
    this->num_frames = 0;
    this->elapsed = 0.0;
}

void AnimationController::InitBaseClass() {
    AnimationControllerNode *pNewNode; // [esp+4h] [ebp-8h]

    // this->vtable = &IAnimationController_BaseVTable;
    // Register in global linked list
    pNewNode = static_cast<AnimationControllerNode *>(operator new(sizeof(AnimationControllerNode)));
    pNewNode->pController = this;
    pNewNode->pNext = g_pAnimationControllerList;
    g_pAnimationControllerList = pNewNode;
}

void AnimationController::TickFrame() {
    if (!this->num_frames)
        this->elapsed = ComputeElapsed();
    ++this->num_frames;
}

void AnimationController::UntickFrame() {
    LARGE_INTEGER PerformanceCount; // [esp+Ch] [ebp-8h] BYREF

    if (!--this->num_frames) {
        if (this->isHighResolutionTiming) {
            QueryPerformanceCounter(&PerformanceCount);
            this->lpPerformanceCount.QuadPart = PerformanceCount.QuadPart - (unsigned __int64) (
                                                    this->elapsed / this->tickToMs);
        } else {
            this->lastTick = (unsigned __int64) ((double) timeGetTime() - this->elapsed);
        }
    }
}

void AnimationController::Update() {
    if (this->isHighResolutionTiming)
        QueryPerformanceCounter(&this->lpPerformanceCount);
    else
        this->lastTick = timeGetTime();
    this->elapsed = ComputeElapsed();
}

double AnimationController::ComputeElapsed() const {
    LARGE_INTEGER PerformanceCount; // [esp+4h] [ebp-8h] BYREF

    if (this->num_frames > 0)
        return this->elapsed;
    if (!this->isHighResolutionTiming)
        return (timeGetTime() - this->lastTick);
    QueryPerformanceCounter(&PerformanceCount);
    PerformanceCount.QuadPart -= this->lpPerformanceCount.QuadPart;
    return PerformanceCount.QuadPart * this->tickToMs;
}

AnimationController *CreateAnimationController() {
    return new AnimationController();
}
