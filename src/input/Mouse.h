//
// Created by Victus on 24/3/2025.
//

#ifndef MOUSE_H
#define MOUSE_H
#include "raylib.h"


class Mouse {
public:
    static void EnableCursorHiding();
    static Vector2 GetDelta();
};



#endif //MOUSE_H
