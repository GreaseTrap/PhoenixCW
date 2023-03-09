#pragma once
namespace bases
{
    inline uintptr_t pc_ptr, pp_ptr, zm_global_base, zm_bot_base, zm_bot_list_base;
    inline uintptr_t player_base = 0x112931C8;
    inline uintptr_t jump_height = 0x11391F90;
    inline uintptr_t skip_round = 0x333D8;
}

namespace pc
{
    inline uintptr_t pc_arraysize = 0xB970;
    inline uintptr_t pc_current_weapon_id = 0x28;
    inline uintptr_t pc_set_weapon_id = 0xB0;
    inline uintptr_t pc_set_weapon_id2 = 0xF0;
    inline uintptr_t pc_godmode = 0xE67;
    inline uintptr_t pc_rapid_fire_1 = 0xE6C;
    inline uintptr_t pc_rapid_fire_2 = 0xE80;
    inline uintptr_t pc_ammo = 0x13D4;
    inline uintptr_t pc_points = 0x5D24;
    inline uintptr_t pc_run_speed = 0x5C70;
    inline uintptr_t pc_num_shots = 0xFE4;
    inline uintptr_t pc_kill_count = 0x5D48;
    inline uintptr_t pc_crit = 0x10DA;
    inline uintptr_t pc_auto_fire = 0xE70;
}

namespace zm
{
    inline uintptr_t zm_botlist = 0x8;
    inline uintptr_t zm_bot_arraysize = 0x5F8;
    inline uintptr_t zm_bot_health = 0x398;
    inline uintptr_t zm_bot_max_health = 0x39C;
    inline uintptr_t zm_bot_coords = 0x2D4;
}