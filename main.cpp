/** \file main.cpp
 */

extern "C"{
    #include "audioDecoder.h"
}
#include "sql.h"

int main(int argc, char *argv[]){
    decodeFile(argv[1]);
}
