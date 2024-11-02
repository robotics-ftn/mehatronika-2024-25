/*
 * odometrija.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Andrej Čilag
 */

#include "odometrija.h"
#include "../../periferije/encoder/encoder.h"

#include <math.h>

static float b;
static float r;

static int16_t vd_inc;
static int16_t vl_inc;

static float vd;
static float vl;

static float inc2mm;

static float w = 0;
static float v = 0;

static float fi = 0;
static float x = 0;
static float y = 0;

void
odometrija_init ()
{
  b = 290.0; // mm - razdaljina izmedju dva pasivna tocka
  r = 75.0; // mm - precnik pasivnih tockova

  float o_ot = r * M_PI;
  float N = 1000.0 / o_ot;
  float n = N * 2048 * 4;
  inc2mm = 1000.0 / n;
}

void
odometrija ()
{
  vd_inc = encoder_brzina1 ();
//  vl_inc = encoder_brzina2 ();
  vl_inc = -vd_inc;

  vd = vd_inc * inc2mm;
  vl = vl_inc * inc2mm;

  w = (vd - vl) / b;
  v = (vd + vl) / 2;

  fi += w;
  x += v * cos (fi);
  y += v * sin (fi);

}
