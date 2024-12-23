/*
 * pid.c
 *
 *  Created on: Dec 2, 2024
 *      Author: Andrej Čilag
 */

#include "pid.h"
#include <math.h>
#include "stm32f4xx.h"
#include "../odometrija/odometrija.h"

// Konstante, promenljive i njihova inicijalizacija
// ...
static float e = 0.0;
static float e_pre = 0.0;

//static float ref = 0.0; // zadavacemo
static float merena = 0.0; // merena vrednost sa senzora

static float u_p = 0.0;
static float u_i = 0.0;
static float u = 0.0;

static float kp = 0.0;
static float ki = 0.0;

// TODO: Dodati argument za odabir motora
void
pid_brzina (float ref)
{
  // PI regulator
  merena = odometrija_brzina_d ();
  e = ref - merena;

  u_p = kp * (e - e_pre);
  u_i = ki * e;
  u = u + u_p + u_i;

  e_pre = e;

  // saturacija
  if (u > 4200.0)
    {
      u = 4200.0;
    }
  else if (u < -4200.0)
    {
      u = -4200.0;
    }

  // TODO: Napraviti funkciju koja radi promenu smera i zadavanje pwm-a motoru
  if (u > 0.0)
    {
      // Jedna strana okretanja motora
      GPIOA->ODR |= (0b1 << 8); // DIR
    }
  else
    {
      // Druga strana okretanja motora
      GPIOA->ODR &= ~(0b1 << 8); // DIR
    }
  TIM3->CCR2 = fabs (u); // PWM

}
//
//void
//pid ()
//{
//  // Zadavanje reference (pozicija i orijentacija) (p0, t0)
//  // Izracunamo poziciju u koordinatnom sistemu robota (p1)
//  // t - orijetacija ka zeljenoj poziciji
//  // d - potreban predjeni put
//  // tf - finalna orijentacija
//
//  // TODO: srediti izraze!
//  p1x = cos (fi) * p0x + sin (fi) * p0y - (cos (fi) * x + sin (fi) * y);
//  p1y = cos (fi) * p0y - sin (fi) * p0x - (cos (fi) * y - sin (fi) * x);
//
//  t = atan2 (p1y / p1x); // -pi / pi
//  d = sqrt (p1x * p1x + p1y * p1y);
//  tf = t0 - fi;
//
//  // Prva faza
//  if (stanje == 0)
//    {
//      uo_p = kp_o * (t - t_pre);
//      uo_i = ki_o * t;
//      uo = uo + uo_p + uo_i;
//
//      t_pre = t;
//
//      // upravljanje prosledjujemo na brzinaski regulator
//      // jednom motoru zadajemo + upravljanje, drugom - upravljanje
//      // desnom motoru ide +, levom - (pozitivan matematički smer)
//      ref_brezine_d = uo;
//      ref_brezine_l = -uo;
//
//      if (fabs (t) < epsilon_o)
//	{
//	  stanje = 1;
//	}
//    }
//  // Druga faza
//  else if (stanje == 1)
//    {
//      uo_p = kp_o * (t - t_pre);
//      uo_i = ki_o * t;
//      uo = uo + uo_p + uo_i;
//      t_pre = t;
//
//      ud_p = kp_d * (d - d_pre);
//      ud_i = ki_d * d;
//      ud = ud + ud_p + ud_i;
//      d_pre = d;
//
//      // upravljanje prosledjujemo na brzinaski regulator
//      ref_brezine_d = ud + uo;
//      ref_brezine_l = ud - uo;
//
//      if (fabs (d) < epsilon_p)
//	{
//	  stanje = 2;
//	}
//    }
//  // Treca faza
//  else if (stanje == 2)
//    {
//      uo_p = kp_o * (tf - tf_pre);
//      uo_i = ki_o * tf;
//      uo = uo + uo_p + uo_i;
//
//      tf_pre = tf;
//
//      // upravljanje prosledjujemo na brzinaski regulator
//      // jednom motoru zadajemo + upravljanje, drugom - upravljanje
//      // desnom motoru ide +, levom - (pozitivan matematički smer)
//      ref_brezine_d = uo;
//      ref_brezine_l = -uo;
//    }
//
//}
