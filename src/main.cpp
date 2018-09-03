#include "World.h"
#include <iostream>


World w;        // construct a default world object

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

int main() {
    signal(SIGINT, signalHandler);
    w.build();
    w.render_scene();

    return(0);
}
