#include "gui.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include "../globals/variables.h"

void Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"PHOENIX [FREE] BOCW",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("NOTICE:").x) / 2.f);
	ImGui::TextColored({ 0.5f, 0.5f, 0.0f, 1.0f }, "NOTICE:");
	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("THIS IS A FREE VERSION OF OUR OLD PAID TOOL WITH REMOVED FEATURES").x) / 2.f);
	ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "THIS IS A FREE VERSION OF OUR OLD PAID TOOL WITH REMOVED FEATURES");
	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("THIS TOOL WAS ORIGINALLY DEVELOPED BY PHOENIX").x) / 2.f);
	ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "THIS TOOL WAS ORIGINALLY DEVELOPED BY PHOENIX");

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("HAPPY CHEATING ~ PHOENIX DEV TEAM <3").x) / 2.f);
	ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "HAPPY CHEATING ~ PHOENIX DEV TEAM <3");
	ImGui::NewLine();
	ImGui::BeginChild(1, { ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().y }, true);

	if (ImGui::Button("ZOMBIES")) vars::index = 0;
	if (ImGui::Button("PLAYERS")) vars::index = 1;
	if (ImGui::Button("MISC")) vars::index = 2;

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild(2, ImGui::GetContentRegionAvail(), true);

	switch (vars::index)
	{
	case 0:
	{
		ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "ZOMBIES");
		ImGui::Checkbox("ONE SHOT ZOMBIES", &vars::instakill);
		ImGui::Checkbox("ROUND SKIP", &vars::round_skip);
		break;
	}
	case 1:
	{
		ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "PLAYER FEATURES");
		ImGui::Checkbox("GODMODE", &vars::godmode);
		ImGui::Checkbox("FORCE HEADSHOTS", &vars::crits);
		ImGui::Checkbox("INFINITE POINTS", &vars::inf_money);
		ImGui::Checkbox("INFINITE AMMO", &vars::inf_ammo);
		ImGui::Checkbox("AUTO FIRE", &vars::auto_fire);
		ImGui::Checkbox("RAPID FIRE", &vars::rapid_fire);
		ImGui::Checkbox("WEAPON CYCLE", &vars::weapon_cycle);
		ImGui::NewLine();
		ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "LOBBY SHIT");
		ImGui::SliderFloat("PLAYER SPEED", &vars::run_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("JUMP HEIGHT", &vars::jump_height, 1.0f, 10.0f);
		break;
	}
	case 2:
	{
		for (int32_t k = 0; k < 5; k++)
		{
			ImGui::NewLine();
		}
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("MADE BY SIV (Tm9vdGll UnKnoWnCheaTs)").x) / 2.f);
		ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "MADE BY SIV (Tm9vdGll UnKnoWnCheaTs)");
		break;
	}
	}

	ImGui::EndChild();
	ImGui::End();
}
