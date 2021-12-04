#pragma once 

#include "Types.h"

enum class InputState {
	NONE = 0,
	HOLD,
	PRESSED,
	RELEASED,
};

class Keyboard {

public:
	Keyboard();
	 
	InputState GetKeyState(int ascii_code, InputState current);
	void PumpEvents();
	void ClearEvents();
	
	~Keyboard();
};


#ifdef _WIN

#define KEY_LBUTTON        0x01
#define KEY_RBUTTON        0x02
#define KEY_ESC			       0x1B
#define KEY_CANCEL         0x03
#define KEY_MBUTTON        0x04    

#define KEY_PERIOD         0xBE
#define KEY_BRACKETS_l		 0xDB
#define KEY_BRACKETS_R		 0xDD
#define KEY_COMMA          0xBC

#define KEY_BACK           0x08
#define KEY_TAB            0x09

#define KEY_CLEAR          0x0C
#define KEY_RETURN         0x0D

#define KEY_SHIFT          0x10
#define KEY_CONTROL        0x11
#define KEY_MENU           0x12
#define KEY_PAUSE          0x13
#define KEY_CAPITAL        0x14

#define KEY0x1B
#define KEY_SPACE          0x20

#define KEY_NEXT           0x22
#define KEY_END            0x23
#define KEY_HOME           0x24

#define KEY_LEFT           0x25
#define KEY_UP             0x26
#define KEY_RIGHT          0x27
#define KEY_DOWN           0x28

#define KEY_DELETE         0x2E

#define KEY_MINUS					 0xBD
#define KEY_PLUS					 0xBB

#define KEY_DIVIDE				0x6F

#define KEY_OEM_1		0xBA	//  (:;)
#define KEY_OEM_2		0xBF	//  (? / )
#define KEY_OEM_3		0xC0	//  (~`)
#define KEY_OEM_4		0xDB	//  ({ [)
#define KEY_OEM_5		0xDC	//  (| \)
#define KEY_OEM_6		0xDD	//  (}] )
#define KEY_OEM_7		0xDE	//  (" ')
#define KEY_OEM_8		0xDF	//  (§ !)

/*
	KEY_0 - KEY_9 are the same as ASCII '0' - '9' 
	KEY_A - KEY_Z are the same as ASCII 'A' - 'Z' 
*/

#endif