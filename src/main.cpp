using namespace std;

#include <Magick++.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <csignal>

#include "World.h"

World w;        // construct a default world object

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

int main(int argc, char ** argv) {
    Magick::InitializeMagick(*argv);
    signal(SIGINT, signalHandler);
    srand((int) time(NULL));
    w.build();
    w.camera_ptr->render_scene(w);

    return(0);
}
