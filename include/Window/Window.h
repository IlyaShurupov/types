#pragma once

#include "Geometry/Rect.h"
#include "BitMap/Color.h"
#include "Strings/Strings.h"

class Window {

	Rect<float> wrld_rec;
	Rect<float> bounds;
	struct NVGcontext* nvg = nullptr;
	struct GLFWwindow* window = nullptr;

public:

	Window();


	void BeginFrame();
	void EndFrame();

	void SetBounds(const Rect<float>& wrld_rec);
	void SetCanvasRect(const Rect<float>& rect);

	void RRect(Rect<float> rect, const Color& col, float radius = 0);
	void DrawBounds(const Rect<float>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<float>& head, const vec2<float>& tail, const Color& col, short thickness);
	void Text(char* str, float x, float y, float font_scale, const Color& col);
	void Text(const char* str, float x, float y, float font_scale, const Color& col);
	void Clear(const Color& col);

	void GetCrsr(vec2<float>& crs);

	~Window();
};
