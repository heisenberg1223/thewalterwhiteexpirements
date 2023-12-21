#include "App.hpp"

int main(int argc, char **argv) {
  int *window_width = new int(1600);
  int *window_height = new int(900);

  App app {window_width, window_height, "the walter white expirements"};

  return app.OnExecute();
}
