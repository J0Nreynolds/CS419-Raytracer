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
    srand(time(NULL));
    w.build();
    w.render_scene();

    return(0);
}
