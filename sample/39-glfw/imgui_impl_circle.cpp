// dear imgui: Platform Binding for Circle
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)
// (Info: Circle is a cross-platform general purpose library for handling windows, inputs, graphics context creation, etc.)
// (Requires: SDL 2.0. Prefer SDL 2.0.4+ for full feature support.)

// Implemented features:
//  [X] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [X] Platform: Clipboard support.
//  [X] Platform: Keyboard arrays indexed using SDL_SCANCODE_* codes, e.g. ImGui::IsKeyPressed(SDL_SCANCODE_SPACE).
//  [X] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.
// Missing features:
//  [ ] Platform: Circle handling of IME under Windows appears to be broken and it explicitly disable the regular Windows IME. You can restore Windows IME by compiling SDL with SDL_DISABLE_WINDOWS_IME.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2019-07-21: Inputs: Added mapping for ImGuiKey_KeyPadEnter.
//  2019-04-23: Inputs: Added support for SDL_GameController (if ImGuiConfigFlags_NavEnableGamepad is set by user application).
//  2019-03-12: Misc: Preserve DisplayFramebufferScale when main window is minimized.
//  2018-12-21: Inputs: Workaround for Android/iOS which don't seem to handle focus related calls.
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-11-14: Changed the signature of ImGui_ImplCircle_ProcessEvent() to take a 'const SDL_Event*'.
//  2018-08-01: Inputs: Workaround for Emscripten which doesn't seem to handle focus related calls.
//  2018-06-29: Inputs: Added support for the ImGuiMouseCursor_Hand cursor.
//  2018-06-08: Misc: Extracted imgui_impl_sdl.cpp/.h away from the old combined Circle+OpenGL/Vulkan examples.
//  2018-06-08: Misc: ImGui_ImplCircle_InitForOpenGL() now takes a SDL_GLContext parameter.
//  2018-05-09: Misc: Fixed clipboard paste memory leak (we didn't call SDL_FreeMemory on the data returned by SDL_GetClipboardText).
//  2018-03-20: Misc: Setup io.BackendFlags ImGuiBackendFlags_HasMouseCursors flag + honor ImGuiConfigFlags_NoMouseCursorChange flag.
//  2018-02-16: Inputs: Added support for mouse cursors, honoring ImGui::GetMouseCursor() value.
//  2018-02-06: Misc: Removed call to ImGui::Shutdown() which is not available from 1.60 WIP, user needs to call CreateContext/DestroyContext themselves.
//  2018-02-06: Inputs: Added mapping for ImGuiKey_Space.
//  2018-02-05: Misc: Using SDL_GetPerformanceCounter() instead of SDL_GetTicks() to be able to handle very high framerate (1000+ FPS).
//  2018-02-05: Inputs: Keyboard mapping is using scancodes everywhere instead of a confusing mixture of keycodes and scancodes.
//  2018-01-20: Inputs: Added Horizontal Mouse Wheel support.
//  2018-01-19: Inputs: When available (SDL 2.0.4+) using SDL_CaptureMouse() to retrieve coordinates outside of client area when dragging. Otherwise (SDL 2.0.3 and before) testing for SDL_WINDOW_INPUT_FOCUS instead of SDL_WINDOW_MOUSE_FOCUS.
//  2018-01-18: Inputs: Added mapping for ImGuiKey_Insert.
//  2017-08-25: Inputs: MousePos set to -FLT_MAX,-FLT_MAX when mouse is unavailable/missing (instead of -1,-1).
//  2016-10-15: Misc: Added a void* user_data parameter to Clipboard function handlers.

#include "kernel.h"
#include "imgui.h"
#include "imgui_impl_circle.h"
#include <linux/spinlock.h>
// CIRCLE
#include <SDL.h>
// Data
//static SDL_Window*  g_Window = NULL;
static Uint64       g_Time = 0;
static bool g_MouseButton[3];
static int mx, my;
//static SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = {};
//static char*        g_ClipboardTextData = NULL;
extern unsigned GetTicks();
#if 0	
static const char* ImGui_ImplCircle_GetClipboardText(void*)
{
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = SDL_GetClipboardText();
    return g_ClipboardTextData;
}
#endif	

#if 0	
static void ImGui_ImplCircle_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}
#endif	

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
static DEFINE_SPINLOCK (mouse_lock);
void ImGui_ImplCircle_ProcessEvent(TMouseEvent Event, unsigned nButtons, unsigned nPosX, unsigned nPosY)
{
	spin_lock (&mouse_lock);
    switch (Event)
    {
	case MouseEventMouseMove:
	{
		mx = nPosX;
		my = nPosY;
		//io.MousePos = ImVec2((float)nPosX, (float)nPosY);
	}
	case MouseEventMouseDown:
	{
		g_MouseButton[0] = nButtons & MOUSE_BUTTON_LEFT ? true : false;
		g_MouseButton[1] = nButtons & MOUSE_BUTTON_RIGHT ? true : false;
		g_MouseButton[2] = nButtons & MOUSE_BUTTON_MIDDLE ? true : false;
		break;
	}
	case MouseEventMouseUp:
	{
		g_MouseButton[0] = nButtons & MOUSE_BUTTON_LEFT ? false : true;
		g_MouseButton[1] = nButtons & MOUSE_BUTTON_RIGHT ? false : true;
		g_MouseButton[2] = nButtons & MOUSE_BUTTON_MIDDLE ? false : true;
		break;
	}
	default:
	{
	}
#if 0	
	case MouseEventMouseUp:
	{
		if (nButtons & MOUSE_BUTTON_LEFT) g_MousePressed[0] = false;
		if (nButtons & MOUSE_BUTTON_RIGHT) g_MousePressed[1] = false;
		if (nButtons & MOUSE_BUTTON_MIDDLE) g_MousePressed[2] = false;
		return;
	}
#endif	
    // case SDL_MOUSEWHEEL:
        // {
            // if (event->wheel.x > 0) io.MouseWheelH += 1;
            // if (event->wheel.x < 0) io.MouseWheelH -= 1;
            // if (event->wheel.y > 0) io.MouseWheel += 1;
            // if (event->wheel.y < 0) io.MouseWheel -= 1;
            // return true;
        // }
    // case SDL_MOUSEBUTTONDOWN:
        // {
            // if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
            // if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
            // if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
            // return true;
        // }
    // case SDL_TEXTINPUT:
        // {
            // io.AddInputCharactersUTF8(event->text.text);
            // return true;
        // }
    // case SDL_KEYDOWN:
    // case SDL_KEYUP:
        // {
            // int key = event->key.keysym.scancode;
            // IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
            // io.KeysDown[key] = (event->type == SDL_KEYDOWN);
            // io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            // io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            // io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
            // io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
            // return true;
        // }
    }
	spin_unlock (&mouse_lock);
    return;
}

static void ImGui_ImplCircle_UpdateMouse()
{
	ImGuiIO& io = ImGui::GetIO();
	spin_lock (&mouse_lock);
	io.MousePos = ImVec2((float)mx, (float)my);
	io.MouseDown[0] = g_MouseButton[0];
	io.MouseDown[1] = g_MouseButton[1];
	io.MouseDown[2] = g_MouseButton[2];
	spin_unlock (&mouse_lock);
}

static bool ImGui_ImplCircle_Init(SDL_Window* window)
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_circle";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    // io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    // io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    // io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    // io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    // io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    // io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    // io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    // io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    // io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    // io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    // io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    // io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    // io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    // io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    // io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    // io.KeyMap[ImGuiKey_KeyPadEnter] = SDL_SCANCODE_RETURN2;
    // io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    // io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    // io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    // io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    // io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    // io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    // io.SetClipboardTextFn = ImGui_ImplCircle_SetClipboardText;
    // io.GetClipboardTextFn = ImGui_ImplCircle_GetClipboardText;
    // io.ClipboardUserData = NULL;

#if 0
    g_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    g_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
#endif
#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)window;
#endif

    return true;
}
bool ImGui_ImplCircle_InitForGLES()
{
	return ImGui_ImplCircle_Init(0);
}

bool ImGui_ImplCircle_InitForOpenGL(SDL_Window* window, void* sdl_gl_context)
{
    (void)sdl_gl_context; // Viewport branch will need this.
    return ImGui_ImplCircle_Init(window);
}

bool ImGui_ImplCircle_InitForVulkan(SDL_Window* window)
{
#if !SDL_HAS_VULKAN
    IM_ASSERT(0 && "Unsupported");
#endif
    return ImGui_ImplCircle_Init(window);
}

bool ImGui_ImplCircle_InitForD3D(SDL_Window* window)
{
#if !defined(_WIN32)
    IM_ASSERT(0 && "Unsupported");
#endif
    return ImGui_ImplCircle_Init(window);
}

void ImGui_ImplCircle_Shutdown()
{
#if 0	
    g_Window = NULL;

    // Destroy last known clipboard data
    if (g_ClipboardTextData)
        SDL_free(g_ClipboardTextData);
    g_ClipboardTextData = NULL;

    // Destroy SDL mouse cursors
    for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
        SDL_FreeCursor(g_MouseCursors[cursor_n]);
    memset(g_MouseCursors, 0, sizeof(g_MouseCursors));
#endif
}

#if 0
static void ImGui_ImplCircle_UpdateGamepads()
{
    ImGuiIO& io = ImGui::GetIO();
    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

#if 0
    // Get gamepad
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller)
    {
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        return;
    }

    // Update gamepad inputs
    #define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0) ? 1.0f : 0.0f; }
    #define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
    MAP_BUTTON(ImGuiNavInput_Cancel,        SDL_CONTROLLER_BUTTON_B);               // Circle / B
    MAP_BUTTON(ImGuiNavInput_Menu,          SDL_CONTROLLER_BUTTON_X);               // Square / X
    MAP_BUTTON(ImGuiNavInput_Input,         SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
    MAP_BUTTON(ImGuiNavInput_DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
    MAP_BUTTON(ImGuiNavInput_DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
    MAP_BUTTON(ImGuiNavInput_DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
    MAP_BUTTON(ImGuiNavInput_DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
    MAP_BUTTON(ImGuiNavInput_FocusPrev,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_FocusNext,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_BUTTON(ImGuiNavInput_TweakSlow,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
    MAP_BUTTON(ImGuiNavInput_TweakFast,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
    MAP_ANALOG(ImGuiNavInput_LStickLeft,    SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiNavInput_LStickRight,   SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiNavInput_LStickUp,      SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
    MAP_ANALOG(ImGuiNavInput_LStickDown,    SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    #undef MAP_BUTTON
    #undef MAP_ANALOG
#endif	
}
#endif
void ImGui_ImplCircle_NewFrame(int w, int h)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(1.f,1.f);
    static Uint64 frequency = 1000000; //SDL_GetPerformanceFrequency();
    Uint64 current_time = GetTicks(); //SDL_GetPerformanceCounter();
    io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
    g_Time = current_time;
	ImGui_ImplCircle_UpdateMouse();
#if 0	
    // Update game controllers (if enabled and available)
    ImGui_ImplCircle_UpdateGamepads();
#endif	
}
