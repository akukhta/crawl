/**
 * @file
 * @brief Functions for inventory related commands.
**/

#include "AppHdr.h"

#include "command-palette-menu.h"
#include "message.h"
#include "output.h"
#include "macro.h"
#include <unordered_map>

static std::unordered_map<std::string, command_type> commands = {
    {"Attack Left", CMD_ATTACK_LEFT},
    {"Attack Right", CMD_ATTACK_RIGHT},
    {"Attack Up", CMD_ATTACK_UP},
    {"Attack Down", CMD_ATTACK_DOWN},
    {"Rest", CMD_REST},
    {"Display Inventory", CMD_DISPLAY_INVENTORY},
};

void display_command_palette()
{
    CommandPalette menu;

    MenuEntry* me =
            new MenuEntry("Command Palette",
                MEL_TITLE);

    menu.set_title(me);
    menu.add_entry(new MenuEntry("Commands", MEL_SUBTITLE));

    for (auto const it : commands)
    {
        menu.add_entry(new MenuEntry{it.first});
    }

    //menu.cycle_mode();
    menu.show(true);
    
    if (!crawl_state.doing_prev_cmd_again)
    {
        redraw_screen();
        update_screen();
    }
    mpr("Test command to name:");
    mpr(command_to_name(CMD_ATTACK_DOWN));
}
