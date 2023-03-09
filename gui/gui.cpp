#include "gui.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include "../globals/variables.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU)
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = (WNDPROC)WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowTitleAlign = { 0.50f, 0.50f };

	style->Colors[ImGuiCol_Border] = { 1.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_Button] = { 0.1f, 0.1f, 0.1f, 1.0f };
	style->Colors[ImGuiCol_ButtonHovered] = { 0.5f, 0.5f, 0.5f, 1.0f };
	style->Colors[ImGuiCol_ButtonActive] = { 0.5f, 0.5f, 0.5f, 1.0f };
	style->Colors[ImGuiCol_CheckMark] = { 1.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_SliderGrab] = { 1.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_SliderGrabActive] = { 1.0f, 0.0f, 0.0f, 1.0f };
	style->Colors[ImGuiCol_FrameBg] = { 0.1f, 0.1f, 0.1f, 1.0f };
	style->Colors[ImGuiCol_FrameBgHovered] = { 0.5f, 0.5f, 0.5f, 1.0f };
	style->Colors[ImGuiCol_FrameBgActive] = { 0.1f, 0.1f, 0.1f, 1.0f };
	style->Colors[ImGuiCol_TitleBgActive] = style->Colors[ImGuiCol_WindowBg];

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 16.0f);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void gui::Render() noexcept
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