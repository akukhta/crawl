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

CommandPaletteEntry::CommandPaletteEntry(const string &txt, command_type cmd)
    : MenuEntry(txt), cmd(cmd)
{
}

void CommandPalette::add_command(CommandPaletteEntry *entry)
{
    entry->on_click = [this](MenuEntry &e)
    {
        on_command_click(dynamic_cast<CommandPaletteEntry&>(e));
    };

    add_entry(entry);
}

bool CommandPalette::exit_on_click() const
{
    return true;
}

void CommandPalette::select_item_index(int idx, int qty)
{
    on_command_click(dynamic_cast<CommandPaletteEntry&>(*items[idx]));
    Menu::select_item_index(idx, qty);
}

void CommandPalette::on_command_click(CommandPaletteEntry& entry)
{
    selectedCommand = &entry;
    complete = true;
}

command_type display_command_palette()
{
    CommandPalette menu;

    MenuEntry* me =
            new MenuEntry("Command Palette",
                MEL_TITLE);
    me->colour = LIGHTBLUE;

    menu.set_title(me, true, true);

    for (auto const it : commands)
    {
        menu.add_command(new CommandPaletteEntry{it.first, it.second});
    }

    //menu.cycle_mode();
     menu.show(true);
    
    if (!crawl_state.doing_prev_cmd_again)
    {
        redraw_screen();
        update_screen();
    }

    if (menu.selectedCommand)
    {
        mpr("Selected command: " + command_to_name(menu.selectedCommand->cmd));
        return menu.selectedCommand->cmd;
    }

    return CMD_NO_CMD;
}
