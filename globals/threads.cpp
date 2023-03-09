#include <iostream>
#include <thread>

#include "threads.h"
#include "variables.h"
#include "offsets.h"

#include "../utils/memory.h"

void game::update_thread() noexcept
{
	const auto mem = Memory("BlackOpsColdWar.exe");
	std::thread(game::instakill_thread, mem).detach();
	std::thread(game::roundskip_thread, mem).detach();
	std::thread(game::lobby_thread, mem).detach();
	std::thread(game::weaponcycle_thread, mem).detach();

	while (true)
	{
		vars::game_base = mem.GetModuleAddress("BlackOpsColdWar.exe");
		if (!vars::game_base)
		{
			exit(0);
		}
		bases::pc_ptr = mem.Read<uintptr_t>(vars::game_base + bases::player_base);
		bases::pp_ptr = mem.Read<uintptr_t>(vars::game_base + bases::player_base + 0x8);
		bases::zm_global_base = mem.Read<uintptr_t>(vars::game_base + bases::player_base + 0x60);
		bases::zm_bot_base = mem.Read<uintptr_t>(vars::game_base + bases::player_base + 0x68);
		bases::zm_bot_list_base = mem.Read<uintptr_t>(bases::zm_bot_base + zm::zm_botlist);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

// zombies
void game::instakill_thread(const Memory& mem) noexcept
{
	while (true)
	{
		if (vars::instakill)
		{
			for (int32_t i = 0; i < 90; i++)
			{
				const auto health = mem.Read<float_t>(bases::zm_bot_list_base + zm::zm_bot_health + (zm::zm_bot_arraysize * i));
				if (health > 0 && health < 50000)
				{
					mem.Write<int32_t>(bases::zm_bot_list_base + zm::zm_bot_health + (zm::zm_bot_arraysize * i), 1);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void game::roundskip_thread(const Memory& mem) noexcept
{
	while (true)
	{
		if (vars::round_skip)
		{
			mem.Write<BYTE>(vars::game_base + bases::skip_round + 0x02, 0x21);
		}
		else
		{
			mem.Write<BYTE>(vars::game_base + bases::skip_round + 0x02, 0x20);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// player
void game::lobby_thread(const Memory& mem) noexcept
{
	while (true)
	{
		if (vars::godmode)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<BYTE>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_godmode, 0xA0);
			}
		}
		else
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<BYTE>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_godmode, 0x20);
			}
		}

		if (vars::crits)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<BYTE>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_crit, 255);
			}
		}
		else
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<BYTE>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_crit, 0);
			}
		}

		if (vars::inf_ammo)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				for (int32_t k = 0; k < 7; k++)
				{
					mem.Write<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_ammo + (k * 0x4), 99);
				}
			}
		}

		if (vars::inf_money)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_points, 1337000);
			}
		}

		if (vars::auto_fire)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<bool>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_auto_fire, true);
			}
		}
		else
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<bool>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_auto_fire, false);
			}
		}

		if (vars::rapid_fire)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				mem.Write<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_rapid_fire_1, 0);
				mem.Write<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_rapid_fire_2, 0);
			}
		}

		for (int32_t i = 0; i < 4; i++)
		{
			mem.Write<float_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_run_speed, vars::run_speed);
		}
		mem.Write<float_t>(mem.Read<uintptr_t>(vars::game_base + bases::jump_height), vars::jump_height);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void game::weaponcycle_thread(const Memory& mem) noexcept
{
	while (true)
	{
		if (vars::weapon_cycle)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				const int32_t kills = mem.Read<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_kill_count);
				if (kills % vars::cycle_kills == 0)
				{
					mem.Write<int32_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_kill_count, kills + 1);
					mem.Write<int64_t>(bases::pc_ptr + (pc::pc_arraysize * i) + pc::pc_set_weapon_id, vars::weapon_ids[vars::cycleint]);
					vars::cycleint++;
				}
				if (vars::cycleint == 56)
				{
					vars::cycleint = 1;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}