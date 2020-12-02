#include <visualizer/unfair_pong_app.h>

using unfairpong::visualizer::UnfairPongApp;

void prepareSettings(UnfairPongApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(UnfairPongApp, ci::app::RendererGl, prepareSettings);
