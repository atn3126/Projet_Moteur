#include "InputManager.h"
#include <windows.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

int InputManager::GetKeyPressed()
{
    for (int i = 0; i < 256; i++) {
        if (GetAsyncKeyState(i) & 0x8000) {
            return i;
        }
    }
    return -1; // Aucune touche enfoncée
}