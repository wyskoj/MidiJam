//
// Created by Jacob on 3/17/2026.
//

#include "AnimationController.h"

#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

// Global linked list of all active animation controllers.
// Declared extern in files that need it; defined here as it is owned by this subsystem.
extern AnimationControllerNode* g_pAnimationControllerList;

// FUNCTION: MIDIJAM 0x448430
IAnimationController::IAnimationController() {
    InitBaseClass();

    if (QueryPerformanceFrequency(&lpFrequency)) {
        isHighResolutionTiming = true;
        QueryPerformanceCounter(&lpPerformanceCount);
        tickToMs = 1.0 / static_cast<double>(lpFrequency.QuadPart) * 1000.0;
    }
    else {
        isHighResolutionTiming = false;
        lastTick = timeGetTime();
    }

    num_frames = 0;
    elapsed = 0.0;
}

// FUNCTION: MIDIJAM 0x448650
IAnimationController::~IAnimationController() {
    Destroy();
}

// FUNCTION: MIDIJAM 0x447FD0
void IAnimationController::InitBaseClass() {
    // Register this controller in the global linked list.
    AnimationControllerNode* pNewNode = new AnimationControllerNode();
    pNewNode->pController = this;
    pNewNode->pNext = g_pAnimationControllerList;
    g_pAnimationControllerList = pNewNode;
}

// FUNCTION: MIDIJAM 0x4484C0
void IAnimationController::Update() {
    if (isHighResolutionTiming)
        QueryPerformanceCounter(&lpPerformanceCount);
    else
        lastTick = timeGetTime();

    elapsed = ComputeElapsed();
}

// FUNCTION: MIDIJAM 0x448580
// MATCH: EXACT
void IAnimationController::TickFrame() {
    if (!num_frames)
        elapsed = ComputeElapsed();
    ++num_frames;
}

// FUNCTION: MIDIJAM 0x448510
double IAnimationController::ComputeElapsed() {
    if (num_frames > 0)
        return elapsed;

    if (!isHighResolutionTiming)
        return static_cast<double>(timeGetTime() - lastTick);

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    counter.QuadPart -= lpPerformanceCount.QuadPart;
    return static_cast<double>(counter.QuadPart) * tickToMs;
}

// FUNCTION: MIDIJAM 0x4485C0
void IAnimationController::UntickFrame() {
    if (!--num_frames) {
        if (isHighResolutionTiming) {
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            lpPerformanceCount.QuadPart = counter.QuadPart
                - static_cast<LONGLONG>(elapsed / tickToMs);
        }
        else {
            lastTick = timeGetTime() - static_cast<DWORD>(elapsed);
        }
    }
}

// FUNCTION: MIDIJAM 0x448680
// MATCH: EXACT
void IAnimationController::Destroy() {
    UnregisterFromGlobalList();
}

// FUNCTION: MIDIJAM 0x448020
void IAnimationController::UnregisterFromGlobalList() const {
    AnimationControllerNode* pCurrent = g_pAnimationControllerList;
    AnimationControllerNode* pPrev = nullptr;

    while (pCurrent) {
        if (pCurrent->pController == this) {
            if (pPrev)
                pPrev->pNext = pCurrent->pNext;
            else
                g_pAnimationControllerList = pCurrent->pNext;

            free(pCurrent);
            return;
        }
        pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }
}

// FUNCTION: MIDIJAM 0x448100
void IAnimationController::DestroyAndUnregister(bool freeMemory) {
    UnregisterFromGlobalList();
    if (freeMemory)
        free(this);
}

// FUNCTION: MIDIJAM 0x4483C0
// MATCH: EXACT
IAnimationController* CreateAnimationController() {
    return new IAnimationController();
}
