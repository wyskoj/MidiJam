//
// Created by Jacob on 11/19/2025.
//

#ifndef MIDIJAM_INPUT_H
#define MIDIJAM_INPUT_H

extern __int16 g_isSpacePressed;
extern __int16 g_isAKeyPressed;
extern __int16 g_shiftPressed;
extern __int16 g_framesKeyRightPressed;
extern __int16 g_framesKeyLeftPressed;
extern __int16 g_framesKeyUpPressed;
extern __int16 g_framesKeyDownPressed;


void HandleKeyPresses();

#endif //MIDIJAM_INPUT_H