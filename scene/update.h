//
// Created by Jacob on 3/18/2026.
//

#ifndef MIDIJAM_UPDATE_H
#define MIDIJAM_UPDATE_H

#include <windows.h>
#include <mmsystem.h>


BOOL UpdateMidiJam();
void __stdcall UpdateMidiJamMM(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
void HandleKeyPresses();

#endif // MIDIJAM_UPDATE_H