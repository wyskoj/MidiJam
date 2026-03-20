//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_MACROS_H
#define MIDIJAM_MACROS_H

#include "model/Ms3dBundle.h"

// ---------------------------------------------------------------------------
// Model loading
// Allocates, constructs, and loads a Ms3dBundle from the HWF archive.
// On any failure, displays an error box and returns 0 from the enclosing
// function (valid only inside functions that return int/BOOL).
// ---------------------------------------------------------------------------
#define LOAD_MODEL(var, filename) \
var = new Ms3dBundle(); \
if (!var->LoadFromHWF(filename, g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems)) { \
MessageBoxA(0, "Couldn't load the model data\\model.ms3d", "Error", 0x10u); \
return 0; \
}

// ---------------------------------------------------------------------------
// Texture operations
// ---------------------------------------------------------------------------
#define REPLACE_TEX(model, oldTex, newTex) \
    model->ReplaceTexture((oldTex), (newTex))

#define APPLY_TEX(model) \
    model->ApplyTextures(g_hwfStream, g_pHwfAppendix, g_nHwfAppendixItems);

// ---------------------------------------------------------------------------
// Recoil accumulation — adds amount to recoil and clamps to maxVal.
// ---------------------------------------------------------------------------
#define ADD_RECOIL(recoil, amount, maxVal) \
    do { (recoil) += (amount); if ((recoil) > (maxVal)) (recoil) = (maxVal); } while(0)

// ---------------------------------------------------------------------------
// Key debounce — if the frame counter is active, increment and reset at limit.
// ---------------------------------------------------------------------------
#define DEBOUNCE_KEY(frameVar, limit) \
    do { if ((frameVar) > 0 && ++(frameVar) > (limit)) (frameVar) = 0; } while(0)

#endif // MIDIJAM_MACROS_H
