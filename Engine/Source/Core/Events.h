#pragma once
#include "ChilliDefinitions.h"
#include <queue>
#include <functional>
#include "SubSystem.h"
#include <Windows.h>
#include <bitset>

#pragma warning(disable:4251)

namespace Engine::Core {

	struct EventData {
		uint64_t wparam = 0;
		int64_t lparam = 0;
		uint32_t msg = 0;
		HWND handle;
	};

	enum class EventType : uint32_t {
		None = 0, WindowResized = 5, FocusGained = 7, FocusLost = 8, Close = 16, KeyDown = 256, KeyUp = 257, Character = 258, SysKeyDown = 260, SysKeyUp = 261, MouseMove = 512,
		LeftMousePressed = 513, LeftMouseReleased = 514, RightMousePressed = 516, RightMouseReleased = 517, MouseWheel = 522, MouseLeave = 675
	};

	enum class Key : int {
		Backspace = 8, Tab = 9, Clear = 12, Return = 13, Shift = 16, Control = 17, Alt = 18, Pause = 19, CapsLock = 20, Escape = 27, Space = 32, PageUp = 33, PageDown = 34, End = 35,
		Home = 36, LeftArrow = 37, UpArrow = 38, RightArrow = 39, DownArrow = 40, Select = 41, Print = 42, Execute = 43, PrintScreen = 44, Insert = 45, Delete = 46, Help = 47,
		Zero = 48, One = 49, Two = 50, Three = 51, Four = 52, Five = 53, Six = 54, Seven = 55, Eight = 56, Nine = 57, A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71,
		H = 72, I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88, Y = 89, Z = 90, LeftWindows = 91,
		RightWindows = 92, Applications = 93, Sleep = 95, Numpad0 = 96, Numpad1 = 97, Numpad2 = 98, Numpad3 = 99, Numpad4 = 100, Numpad5 = 101, Numpad6 = 102, Numpad7 = 103,
		Numpad8 = 104, Numpad9 = 105, Multiply = 106, Add = 107, Separator = 108, Subtract = 109, Decimal = 110, Divide = 111, F1 = 112, F2 = 113, F3 = 114, F4 = 115, F5 = 116,
		F6 = 117, F7 = 118, F8 = 119, F9 = 120, F10 = 121, F11 = 122, F12 = 123, F13 = 124, F14 = 125, F15 = 126, F16 = 127, F17 = 128, F18 = 129, F19 = 130, F20 = 131, F21 = 132,
		F22 = 133, F23 = 134, F24 = 135, NumLock = 144, ScrollLock = 145, LeftShift = 160, RightShift = 161, LeftControl = 162, RightControl = 163, LeftMenu = 164, RightMenu = 165,
		BrowserBack = 166, BrowserForward = 167, BrowserRefresh = 168, BrowserStop = 169, BrowserSearch = 170, BrowserFavourites = 171, BrowserHome = 172, VolumeMute = 173,
		VolumeDown = 175, NextTrack = 176, PrevTrack = 177, Stop = 175, Play = 179, LaunchMail = 180, LaunchMedia = 181, LaunchApp1 = 182, LaunchApp2 = 183, Colon = 186, Plus = 187,
		Comma = 188, Minus = 189, Period = 190, ForwardSlash = 191, Tilde = 192, OpeningBracket = 219, Pipe = 220, ClosingBracket = 221, Quote = 222, Misc = 223, Misc2 = 225,
		AngleBracket = 226, Misc3 = 227, Misc4 = 228, Process = 229, Misc5 = 230, Packet = 231, Attn = 246, Crsel = 247, Exsel = 248, Ereof = 249, Play2 = 250, Zoom = 251, Clear2 = 254
	};
	
	class CHILLI_API Events : public SubSystem { 
	public:
		Events() = default;
		~Events();
		Events(const Events&) = delete;
		Events& operator=(const Events&) = delete;
		Events(Events&&)noexcept = default;
		static SystemType GetSystemType();
		void ProcessFrame()override;
		void Push(const EventData*const e);
		void Subscribe(const EventType type, std::function<void()> callback);
		const std::pair<const int64_t&, const int64_t&> GetMousePosition()const;
		const int64_t GetScreenWidth()const;
		const int64_t GetScreenHeight()const;
		bool GetMouseLeftPressed()const;
		bool GetMouseRightPressed()const;
		short GetMouseWheelDelta()const;
		bool GetKeyPressed(Key key)const;
		bool GetKeyReleased(Key key)const;
		bool GetInWindow()const;
	private:
		void ClearBuffer();
		void TrimBuffer();
		void ResetKeyboard();
		void ReadEvent(const EventData* const data_in);
		int64_t m_screenWidth = 0;
		int64_t m_screenHeight = 0;
		int64_t m_mouseX = 0;
		int64_t m_mouseY = 0;
		bool m_leftMousePressed = 0;
		bool m_rightMousePressed = 0;
		bool m_inWindow = true;
		unsigned char m_keycode = 0;
		short m_wheelDelta = 0;
		std::queue<const EventData*> m_eventBuffer;
		std::vector<std::pair<const EventType, std::function<void()>>> m_subscribers;
		std::bitset<256> m_keys;
	};
}