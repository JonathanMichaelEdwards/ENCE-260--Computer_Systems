// ****************************************************************************
// File:     maps.h                                                           *
// Authors:  Jonathan Edwards & Ben Empson                                    *
// Date:     17 Oct 2018                                                      *
// Descr:    Header file for maps.c                                            *
// ****************************************************************************


#ifndef MAPS_H
#define MAPS_H


#include <stdio.h>
#include "maps.h"


// Map Layout.
#define ROWS 14
#define COLS 8

// Goes through map positions.
#define MAP_SET *(map + mapPos.y * COLS + mapPos.x)

// Returning map that the players will play on.
uint8_t *getMap(void);


#endif // MAPS_H