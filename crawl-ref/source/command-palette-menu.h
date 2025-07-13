/**
 * @file
 * @brief Functions for inventory related commands.
**/

#pragma once
#include "AppHdr.h"

#include "menu.h"
#include "command-type.h"

class CommandPaletteEntry : public MenuEntry
{
public:
    CommandPaletteEntry(const string &txt, command_type cmd = CMD_NO_CMD);
    command_type cmd;
};

class CommandPalette : public Menu
{
public:
    CommandPalette():
        Menu(MF_SINGLESELECT | MF_ALLOW_FORMATTING
            | MF_ARROWS_SELECT | MF_WRAP | MF_INIT_HOVER | MF_USE_TWO_COLUMNS)
    {
        set_tag("cmd_palette");
        menu_action = ACT_EXECUTE;
    }

    void add_command(CommandPaletteEntry* entry);

    bool exit_on_click() const override;
    void select_item_index(int idx, int qty = MENU_SELECT_INVERT) override;
    virtual void update_title();
    bool process_key(int keyin) override;
    CommandPaletteEntry* selectedCommand = nullptr;

private:
    void update_items(std::string const& pattern);
    void undo_update_items();

    void on_command_click(CommandPaletteEntry& entry);
    std::vector<MenuEntry*> all_entries;
    std::vector<size_t> matching_entries;

    std::stack<std::vector<size_t>> entries_stack;
};

command_type display_command_palette();
