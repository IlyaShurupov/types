#pragma once

#include "Macros.h"

#include "Geometry/Ray.h"
#include "BitMap/BitMap.h"
#include "Time/Timer.h"
#include "Strings/Strings.h"
//#include "IO/Parser.h"

#include "Containers/List.h"
#include "Containers/Array.h"
#include "Containers/Map.h"

#include "Memory/Mem.h"

#include "Keyboard/Keyboard.h"
#include "Window/Window.h"

#include "IO/File.h"
#include "IO/Log.h"

template <typename T1, typename T2>
struct pair {
	T1 t1; 
	T2 t2;
};

#undef NULL
#define NULL 0

#define TYPES_1