
#include "benchmarker.h"

benchmarker* bmk;

void imGuiWindowInitialize() {
	bmk = new benchmarker();
}

void imGuiWindowFinalize() {
	delete bmk;
}

void imGuiWindowDraw() {
	bmk->draw();
}