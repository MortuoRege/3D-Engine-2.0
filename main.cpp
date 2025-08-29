#include "./header_files/engine.h"

int main() {
    Engine engine;
    if (engine.init()) {
        engine.run();
    }

    engine.cleanup();
}
