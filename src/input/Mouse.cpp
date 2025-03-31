//
// Created by Victus on 24/3/2025.
//

#include "Mouse.h"

#include "raylib.h"

void Mouse::EnableCursorHiding() {
    if (IsKeyPressed(KEY_LEFT_CONTROL)) {
        if (IsCursorHidden()) EnableCursor();
        else DisableCursor();
    }
}

Vector2 Mouse::GetDelta() {
    return GetMouseDelta();
}
