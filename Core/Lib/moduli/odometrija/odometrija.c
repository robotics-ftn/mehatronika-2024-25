/*
 * odometrija.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Andrej Čilag
 */

#include "odometrija.h"
#include "tim.h"
#include <math.h>

static float R; // Precnik pasivnog tocka
static float b; // Rastojanje izmedju dva pasivna tocka

static int16_t vd_inc = 0;
static int16_t vl_inc = 0;
static float vd = 0.0;
static float vl = 0.0;

static float V = 0.0; // Linearna brzina robota
static float w = 0.0; // omega - ugaona brzina robota
static float fi = 0.0; // Orijentacija robota
static float x = 0.0; // Pozicija robota
static float y = 0.0; // Pozicija robota

static float inc2mm = 0.0;

void
odometrija_init ()
{
  // Inicijalizacija svih promenljivih
  R = 200.0;
  b = 75.0;

  float Oot = R * M_PI;
  float N = 1000.0 / Oot;
  float n = N * 2048 * 4; // 2048 - rezolucija enkodera, 4 - kvadraturno dekodiranje
  inc2mm = 1000.0 / n;
}

void
odometrija ()
{
  vd_inc = tim_brzina_1 (); // Brzina desnog pasivnog tocka
  //vl_inc = tim_brzina_2 ();
  vl_inc = vd_inc;

  vd = vd_inc * inc2mm;
  vl = vl_inc * inc2mm;

  V = (vd + vl) / 2;
  w = (vd - vl) / b;

  // Integral je predstavljen sumom
  fi += w;
  x += V * cos (fi);
  y += V * sin (fi);
}
