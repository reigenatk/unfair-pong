#include <visualizer/unfair_pong_app.h>

using idealgas::visualizer::IdealGasApp;

void prepareSettings(IdealGasApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(IdealGasApp, ci::app::RendererGl, prepareSettings);
