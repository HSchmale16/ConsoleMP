/**\file audioDecoder.h
 * \author Henry J Schmale
 * \date   April 4, 2015
 */

#ifndef AUDIODECODER_H_INC
#define AUDIODECODER_H_INC

#include <stdio.h>

char     sdFname[L_tmpnam]; // Name of the file containing the decoded data

/**\brief Initializes the audio decoder system. And starts up ffmpeg
 */
int initDecoder();

int decodeFile(char *fname);

#endif // AUDIODECODER_H_INC
