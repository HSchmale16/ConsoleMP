/** \file main.cpp
 */

extern "C"{
    #include "audioDecoder.h"
    #include "playback.h"
}
#include "sql.h"

int main(int argc, char *argv[]){
    initDecoder();
}
