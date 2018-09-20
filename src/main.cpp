#include <iostream>
#include <cstdlib>
#include <time.h>

#include "World.h"

World w;        // construct a default world object

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

int main() {
    signal(SIGINT, signalHandler);
    srand((int) time(NULL));
    w.build();
    w.camera_ptr->opencl_render_scene(w);

    return(0);
}
