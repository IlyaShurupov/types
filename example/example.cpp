
#include "Types.h"

int main() {

	Window window;
	Keyboard kb;

	Timer timer(1000 / 60.f);

	while (true) {

		timer.reset();

		kb.PumpEvents();
		if (InputState::PRESSED == kb.GetKeyState(KEY_ESC, InputState::NONE)) {
			break;
		}

		window.BeginFrame();

		window.Clear(Color(0.4, 0.4, 0.4, 0.99));

		window.Text("Hello Window Example.\n Press Esc to close.", 100, 100, 50, Color());

		window.EndFrame();

		timer.wait_out();
	}
}