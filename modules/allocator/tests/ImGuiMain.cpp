
#include "benchmarker.h"

class ImgUIbenchmarker : public imGuiDefaultApp {
  benchmarker bnm;

 public:
  ImgUIbenchmarker() {}
  bool mainloop_tick() override {
    bnm.draw();
    return true;
  }
};

int main() {
  ImgUIbenchmarker app;
  app.mainloop();
}