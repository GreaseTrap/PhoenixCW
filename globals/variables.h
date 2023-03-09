#pragma once
#include <iostream>

namespace vars
{
	inline int32_t index = 0;
	inline uintptr_t game_base = 0x0;

	// features
	inline bool godmode = false;
	inline bool crits = false;
	inline bool inf_money = false;
	inline bool inf_ammo = false;
	inline bool auto_fire = false;
	inline bool rapid_fire = false;
	inline bool instakill = false;
	inline bool weapon_cycle = false;
	inline bool round_skip = false;
	
	inline float_t run_speed = 1.0f;
	inline float_t jump_height = 1.0f;

	inline int32_t cycleint = 1;
	inline int32_t cycle_kills = 5;
	inline int64_t weapon_ids[] = { 0, 333, 346, 315, 347, 350, 388, 395, 364, 353, 422, 409, 448, 438, 366, 450, 309, 328,
		340, 439, 349, 424, 445, 293, 323, 335, 389, 441, 344, 463, 373, 312, 341, 308, 406, 421, 465, 285, 404, 320, 311, 466, 449, 372,
		334, 431, 317, 459, 365, 458, 433, 430, 361, 301, 414, 390 };
}