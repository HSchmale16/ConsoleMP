/** \file main.cpp
 */

#include "audio.h"

int main(int argc, char *argv[]){
    initSoundSystem();
    return playFile(argv[1]);
}
