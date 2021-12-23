#pragma once

#include "Geometry/Rect.h"
#include "BitMap/Color.h"
#include "Strings/Strings.h"

class Window {

	Rect<float> window_dim_prev;
	Rect<float> wrld_rec;
	Rect<float> bounds;
	struct NVGcontext* nvg = nullptr;
	struct GLFWwindow* window = nullptr;

public:

	Rect<float> window_dim;

	Window();


	void BeginFrame();
	void EndFrame();

	void SetBounds(const Rect<float>& wrld_rec);
	void SetCanvasRect(const Rect<float>& rect);
	void SetWindowDim(const Rect<float>& in);
	void ResetBunds();
	void RRect(Rect<float> rect, const Color& col, float radius = 0);
	void DrawBounds(const Rect<float>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<float>& head, const vec2<float>& tail, const Color& col, short thickness);
	void Text(char* str, float x, float y, float font_scale, const Color& col);
	void Text(const char* str, float x, float y, float font_scale, const Color& col);
	void Clear(const Color& col);

	vec2<float> GetCrsr();

	~Window();
};


bool UI_Active();