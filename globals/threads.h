#pragma once
#include "../utils/memory.h"

namespace game
{
	void update_thread() noexcept;
	void instakill_thread(const Memory& mem) noexcept;
	void roundskip_thread(const Memory& mem) noexcept;
	void lobby_thread(const Memory& mem) noexcept;
	void weaponcycle_thread(const Memory& mem) noexcept;
}