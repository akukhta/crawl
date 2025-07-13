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

#include "easy-confirm-type.h"

static std::unordered_map<std::string, command_type> commands = {
    {"Attack Left", CMD_ATTACK_LEFT}, {"Attack Right", CMD_ATTACK_RIGHT}, {"Attack Up", CMD_ATTACK_UP},
    {"Attack Down", CMD_ATTACK_DOWN}, {"Rest", CMD_REST}, {"Display Inventory", CMD_DISPLAY_INVENTORY},
    {"Display Known Items", CMD_DISPLAY_KNOWN_OBJECTS}, {"Display Map", CMD_DISPLAY_MAP},
    {"Display Skills", CMD_DISPLAY_SKILLS}, {"Display Mutations", CMD_DISPLAY_MUTATIONS},
    {"Display Religion", CMD_DISPLAY_RELIGION}, {"Display Runes", CMD_DISPLAY_RUNES},
    {"Save Game and quit", CMD_SAVE_GAME}, {"Quit", CMD_QUIT}, {"Autoexplore", CMD_EXPLORE},
    {"Autofight", CMD_AUTOFIGHT}, {"Autofight without movement", CMD_AUTOFIGHT_NOMOVE},
    {"Memorise Spell", CMD_MEMORISE_SPELL}, {"Go upstairs", CMD_GO_UPSTAIRS}, {"Go downstairs", CMD_GO_DOWNSTAIRS},
    {"Zoom in", CMD_ZOOM_IN}, {"Zoom out", CMD_ZOOM_OUT}, {"Edit player's tile", CMD_EDIT_PLAYER_TILE},
    {"Game Menu", CMD_GAME_MENU}, {"Cast spell", CMD_CAST_SPELL}, {"List armour", CMD_LIST_ARMOUR},
    {"List gold", CMD_LIST_GOLD}, {"List jewellery", CMD_LIST_JEWELLERY}, {"Toggle auto pickup", CMD_TOGGLE_AUTOPICKUP},
    {"Toggle Sound", CMD_TOGGLE_SOUND},
};

CommandPaletteEntry::CommandPaletteEntry(const string &txt, command_type cmd)
    : MenuEntry(txt), cmd(cmd) {}

void CommandPalette::add_command(CommandPaletteEntry *entry)
{
    entry->on_click = [this](MenuEntry &e)
    {
        on_command_click(dynamic_cast<CommandPaletteEntry &>(e));
    };

    all_entries.push_back(entry);
    add_entry(entry);
}

bool CommandPalette::exit_on_click() const
{
    return true;
}

void CommandPalette::select_item_index(int idx, int qty)
{
    on_command_click(dynamic_cast<CommandPaletteEntry &>(*items[idx]));
    Menu::select_item_index(idx, qty);
}

void CommandPalette::update_title()
{
    auto fs = calc_title();

    if (title)
    {
        fs.textcolour(title->colour);
        fs += title->get_text() + " ";
    }

    if (title2)
    {
        fs.textcolour(title2->colour);
        fs += title2->get_text();
    }

    m_ui.title->set_text(fs);
}

bool CommandPalette::process_key(int keyin)
{
    char c = static_cast<char>(keyin);

    if (c == '\b')
    {
        remove_char();
        return true;
    }

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        add_char(c);
        return true;
    }

    return Menu::process_key(keyin);
}

void CommandPalette::update_items(std::string const &pattern)
{
    items.clear();
    matching_entries.clear();

    std::for_each(all_entries.begin(), all_entries.end(),
        [this, &pattern](MenuEntry* entry)
        {
            if (entry->text.find(pattern) != std::string::npos)
            {
                matching_entries.push_back(entry);
                add_entry(entry);
            }
        });

    entries_stack.push(matching_entries);

    update_menu(true);
}

void CommandPalette::undo_update_items()
{
    if (!entries_stack.empty())
    {
        items.clear();
        entries_stack.pop();

        auto const& source = entries_stack.empty()
            ? all_entries : matching_entries = entries_stack.top();

        std::for_each(source.begin(), source.end(),
            [this](MenuEntry *entry){add_entry(entry);});

        update_menu(true);
    }
}

void CommandPalette::on_command_click(CommandPaletteEntry &entry)
{
    selectedCommand = &entry;
    complete = true;
}

void CommandPalette::remove_char()
{
    if (title2 && !title2->text.empty())
    {
        title2->text.resize(title2->text.length() - 1);
        update_title();
        undo_update_items();
    }
}

void CommandPalette::add_char(char c)
{
    if (title2)
    {
        title2->text += c;
        update_title();
        update_items(title2->text);
    }
}

command_type display_command_palette()
{
    CommandPalette menu;

    MenuEntry *me = new MenuEntry("Command Palette:", MEL_TITLE);
    me->colour = LIGHTBLUE;

    MenuEntry *inputBox = new MenuEntry("", MEL_TITLE);
    inputBox->colour = GREEN;

    menu.set_title(inputBox, false);
    menu.set_title(me, true);
    //menu.set_min_col_width(2);

    for (auto const it: commands)
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
