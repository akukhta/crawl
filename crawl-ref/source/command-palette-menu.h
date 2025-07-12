/**
 * @file
 * @brief Functions for inventory related commands.
**/

#pragma once
#include "AppHdr.h"

#include "menu.h"

class CommandPalette : public Menu
{
public:
    CommandPalette():
        Menu(MF_SINGLESELECT | MF_ALLOW_FORMATTING
            | MF_ARROWS_SELECT | MF_WRAP | MF_INIT_HOVER)
    {
        set_tag("cmd_palette");
    }
};

void display_command_palette();
