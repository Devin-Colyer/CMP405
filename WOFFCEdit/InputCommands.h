#pragma once

struct InputCommands
{
	// flight
	bool forward;
	bool back;
	bool right;
	bool left;
	bool flyUp;
	bool flyDown;
	bool slowFlight;

	// camera - keyboard
	bool yawRight;
	bool yawLeft;
	bool pitchUp;
	bool pitchDown;

	// mouse
	int mouseX, mouseY;
	int inWindowMouseX, inWindowMouseY;
	int originalMouseX, originalMouseY;
	bool lmbDown, rmbDown, useMouseForCam;
};
