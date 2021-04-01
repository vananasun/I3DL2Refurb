#pragma once
#include <map>
#include "vananagui/VananaGUI.h"
class Van {
public:
    /**
     * Pairs the current thread to a VananaGUI instance.
     *
     * @param VananaGUI* ref Pointer
     */
    static void set(VananaGUI* ref);

    /**
     * Retrieves reference to VananaGUI paired to current thread.
     *
     * @returns VananaGUI& ref
     */
    static VananaGUI& me();
};
