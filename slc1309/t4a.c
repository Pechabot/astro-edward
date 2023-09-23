// t4a.c - routines for calendar symbolic information

/*********************************************************************************
Licence for SLC - Tibetan Calendar software for Sherab Ling

Copyright (c) 2009-2013 Edward Henning

Permission is hereby granted, free of charge, to any person  obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so, subject
to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include "tc.h"
#include "tc.ext"
#include "bcd.h"
#include "bcd.ext"

extern int yan_kwong_flg;
extern int zin_phung_flg;
extern int klu_bzlog_flg;
extern int klu_thebs_flg;
extern int nyi_nag_flg;

// T4A.C: Overflow from t4.c

// Routine to check for special days, festivals, anniversaries, etc.
// Many are currently commented out. Uncomment if needed.
int chk_spec ( int m, int t )
  {
    switch ( m )
      {
//      case 1:
//        if ( t == 14L )
//          {
//            sprintf ( outbuf, "%s",
//              "Anniversary of Milarepa." );
//            return (1);
//          }
//        else if ( t == 15L )
//          {
//            sprintf ( outbuf, "%s",
//              "Anniversary of Marpa." );
//            return (1);
//          }
//      break;
        case 2:
          if ( t == 15 )
            {
              sprintf ( outbuf, "%s", "Revelation of the Kalacakra Tantra." );
              return (1);
            }
        break;
        case 3:
          // 7 for Phugpa, 8 for Tsurphu
          if ( t == 8 )
            {
              sprintf ( outbuf, "%s", "Birth of the Buddha." );
              return (1);
            }
          else if ( t == 15 )
            {
              sprintf ( outbuf, "%s",
                "Enlightenment and Parinirvana of the Buddha." );
              return (1);
            }
        break;
//      case 4:
//      break;
        case 5:
          if ( t == 4 )
            {
              sprintf ( outbuf, "%s", "Turning of the Wheel of the Dharma." );
              return (1);
            }
          else if ( t == 15 )
            {
              sprintf ( outbuf, "%s",
                        "The Buddha's entry into the womb of his mother." );
              return (1);
            }
        break;
//      case 7:
//      break;
        case 8:
          if ( t == 22 )
            {
              sprintf ( outbuf, "%s",
                "Descent of the Buddha from the realm of the gods." );
              return (1);
            }
        break;

        case 12:
          if ( t == 1 )
            {
              sprintf ( outbuf, "%s",
                "From 1st to 15th, Demonstration of Miracles." );
              return (1);
            }//        if ( t == 18L )
        break;
        default:
          ;
        break;
      }
    return (0);
  } // END - chk_spec ()

int chk_solar_term ( int *gza, int *nyib, char *t_str ) // NOT CURRENTLY USED
//   chk_solar_term ( gzadag, nyibar, solar_term_str );
  {
    int sundb1, sundb2; // Mean sun at daybreak, begin and end
    int sunmid;
    int x; //, y;
    int s1, r1, s2, r2, s2t, s2t1, s2t2, s2t3;
    int s2long, s2long1, s2long2, s2long3;
    int f; // fraction of day at lunar day. out of 21600
// To nearest breath/dbugs, Sun moves each day 0;4,26,2 -  1598/21600

    // Time of day in pala of end of lunar day:
    f = ( gzadag[1] * 60 + gzadag[2] ) * 6 + gzadag[3]; 
    // Longitude of mean Sun in breaths at end of lunar day:    
    sundb1 = (( nyib[0] * 60 + nyib[1] ) * 60 + nyib[2] ) * 6 + nyib[3];
    sunmid = sundb1;
    sundb1 = sundb1 - ( f * 1598 ) / 21600; // Solar longitude at beginning of solar day
    sundb2 = sundb1 + 1598; // Solar longitude at end of solar day

// Now, find where we are in solar terms. First is actually 3 sgang:

// 3 sgang - 0;31,30  4 dbugs - 1;39,0 -  11340 + 24300 = 35640
// 4 sgang - 2;46,30  5 dbugs - 3;54,0 - 59940 + 24300 = 84240
// 5 sgang - 5;1,30  6 dbugs - 6;9,0 - etc.
// 6 sgang - 7;16,30 7 dbugs - 8;24,0 -
// 7 sgang - 9;31,30  8 dbugs - 10;39,0 -
// 8 sgang - 11;46,30  9 dbugs - 12;54,0
// 9 sgang - 14;1,30  10 dbugs - 15;9,0
// 10 sgang - 16;16,30  11 dbugs - 17;24,0
// 11 sgang - 18;31,30  12 dbugs - 19;39.0
// 12 sgang - 20;46,30  1 dbugs - 21;54,0
// 1 sgang - 23;1,30   2 dbugs - 24;9,0
// 2 sgang - 25;16,30  3 dbugs - 26;24,0

    if ( sundb1 < 11340 ) // First in list, "sgang 3"
      sundb1 = sundb1 + 583200; // Full circle in breaths
    x = sundb1 - 11340; // to here
    s1 = x / 24300;
    r1 = x - ( s1 * 24300 );

    if ( sundb2 < 11340 )
      sundb2 = sundb2 + 583200; // Full circle in breaths
    x = sundb2 - 11340;
    s2 = x / 24300;
    r2 = x - ( s2 * 24300 );

//    printf ( "Term index = %d, remainder = %d\n", s1, r1 );
//    printf ( "Second term index = %d, remainder = %d\n", s2, r2 );

// Convert to solar terms, "1 dbugs" = 1, etc.

    s1 = s1 + 6;
    if ( s1 > 24 )
      s1 = s1 - 24;
    s2 = s2 + 6;
    if ( s2 > 24 )
      s2 = s2 - 24;

    if ( s1 == s2 )
      return (0); // No solar term

// Now, find the time of the term.
// between each solar term, 24300 "dbugs"
// Solar daily motion = 1598 "dbugs"

    f = (( 24300 - r1 ) * 1598 ) / (24300 + r2 - r1); // This is a number of breaths
    s2t2 = f / 60;
    s2t3 = f % 60;   
    
//    printf ( "Fraction = %d\n", f );     
    
    s2long = ( s2 - 6 ) * 24300 + 11340;
    if ( s2long < 0 )
      s2long = s2long + 583200;

    if ( s2long < 0 )
      s2long = s2long + 583200;
    else if ( s2 == 1 )
      s2long = s2long + 583200;

    if ( sundb1 > s2long ) // It must have been incremented, so do the same:
      s2long = s2long + 583200;

    if ( s2long < 0 )
      s2long = s2long + 583200;
    s2long = ( s2long + 3 ) / 6;
    s2long2 = s2long / 60;
    s2long3 = s2long % 60;
    s2long1 = s2long2 / 60;
    s2long2 = s2long2 % 60;

    x = ( 1 + s2 ) / 2;

    if ( s2 % 2 ) // If index odd, then "dbugs"
      {
        sprintf ( solar_term_str, "%d,%d,%d %d dbugs %d;%d,%d",
                  gzadag[0], s2t2, s2t3, x, s2long1, s2long2, s2long3 );
      }
    else // "sgang"
      {
        sprintf ( solar_term_str, "%d,%d,%d %d sgang %d;%d,%d",
                  gzadag[0], s2t2, s2t3, x, s2long1, s2long2, s2long3 );
      }
    return (1); // solar term found
  } // chk_solar_term

void mak_lmchange_string ( int *f_ml, int *s_ml, int dow )
  {
    int i;
    int fst_ml[6], scd_ml[6];
    int lm1, lm2, lmx;
    int chga, chgb, nxtlm;

    if ( f_ml[0] == s_ml[0] )
      {
        lmchg[0] = '\0';
        lmchgU[0] = '\0';
        lmchgE[0] = '\0';        
        return;
      }

    for ( i = 0; i < 6; ++i )
      {
        fst_ml[i] = f_ml[i];
        scd_ml[i] = s_ml[i];
      }

    nxtlm = scd_ml[0];
    if ( scd_ml[0] == 0 )
      scd_ml[0] = 27;

    scd_ml[0] = scd_ml[0] - fst_ml[0];
    fst_ml[0] = 0;

// reduce to "dbugs":

    lm1 = ( fst_ml[1] * 60 + fst_ml[2] ) * 6 + fst_ml[3];
    lm2 = ( ( scd_ml[0] * 60 + scd_ml[1] ) * 60 + scd_ml[2] ) * 6 + scd_ml[3];
    lmx = s_ml[0];

// Time to lunar mansion change:

    chgb = ( 60 * ( 60 * 60 * 6 - lm1 ) ) / ( lm2 - lm1 );
    chga = chgb / 5;
    chgb = chgb - chga * 5 + 1;

    get_phrochen ( &phrodx2, dow, lmx, 0 );

    // GET NEW BYUNGPHROD
  
    byung_phro ( tmpstr, dow, lmx, 0 );
        
    if ( chga == 1 || chga == 4 || chga == 8 || chga == 10 || chga == 11 )
      sprintf ( lmchg, "%s gi %d nas %s. %s-%s, %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );
    else if ( chga == 0 )               
      sprintf ( lmchg, "%s kyi %d nas %s. %s-%s, %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );
    else if ( chga == 5 || chga == 2 )               
      sprintf ( lmchg, "%s gyi %d nas %s. %s-%s, %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );               
    else 
      sprintf ( lmchg, "%s'i %d nas %s. %s-%s, %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );                              

    if ( chga == 1 || chga == 4 || chga == 8 || chga == 10 || chga == 11 )               
      sprintf ( lmchgU, "%s gi X %d X nas %s X %s %s X %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );
    else if ( chga == 0 )                              
      sprintf ( lmchgU, "%s kyi X %d X nas %s X %s %s X %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );
    else if ( chga == 5 || chga == 2 )                              
      sprintf ( lmchgU, "%s gyi X %d X nas %s X %s %s X %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );
    else               
      sprintf ( lmchgU, "%s'i X %d X nas %s X %s %s X %s", cycanimT[chga], chgb, lunmanT[nxtlm], // nxtlm,
               fourelemT[ dowkelem[ dow ]], fourelemT[ lunmanelem[ lmx ]], phrodchen[phrodx2] );                                             

//  sprintf ( lmchgE, "From %d of %s, %s. %s-%s, %s", chgb, cycanim[chga], lunmanS[nxtlm], // nxtlm,
//             fourelemE[ dowkelem[ dow ]], fourelemE[ lunmanelem[ lmx ]], phrodchen[phrodx2] );               
    sprintf ( lmchgE, "From %d of %s, %s, %s-%s, %s", chgb, cycanim[chga], lunmanS[nxtlm], // nxtlm,
               fourelemE[ dowkelem[ dow ]], fourelemE[ lunmanelem[ lmx ]], tmpstr /*, phrodchen[phrodx2]*/ );
  } // END - mak_lmchange_string

int calc_byedpa ( int * mlong, int * ndag )
  {
    int byedpa, x;
    clear_a_b ();
    sub_gen ( lista, mlong, ndag, 27, sun_f ); // At sunrise for Moon only
    byedpa = lista[0] * 60 + lista[1];
    byedpa = byedpa / 27;
    if ( byedpa == 0 )
      x = 7;
    else if ( byedpa == 57 )
      x = 8;
    else if ( byedpa == 58 )
      x = 9;
    else if ( byedpa == 59 )
      x = 10;
    else
      x = ( byedpa - 1 ) % 7;
    return ( x );
  } // END - calc_byedpa

void byung_phro ( char *b_str, int doweek, int lm, int monlong1 )
  { // Could do with another source for these
    int x, y;
    *b_str = '\0';
    
// int lunmanelem[27] = { 3, 2, 2, 0, 3, 1, 3, 2, 1, 2, 2, 3, 3, 3, 3, 2, 0, 0,
//                       1, 1, 0, 0, 1, 0, 2, 1, 1 };
//char *fourelemE[4] = { "Earth", "Water", "Fire", "Wind" };
//char *fourelemT[4] = { "sa", "chu", "me", "rlung" };
//int dowkelem[7] = { 0, 2, 1, 2, 1, 3, 0 };
    x = lunmanelem[lm];
    y = dowkelem[doweek];
    switch ( x ) 
      {
        case 0: // earth
          if ( y == 0 ) strcat ( b_str, byungphrodE[0] ); // earth
          if ( y == 1 ) strcat ( b_str, byungphrodE[2] ); // water
          if ( y == 2 ) strcat ( b_str, byungphrodE[8] ); // fire
          if ( y == 3 ) strcat ( b_str, byungphrodE[6] ); // wind
          break;
        case 1: // water
          if ( y == 0 ) strcat ( b_str, byungphrodE[2] ); // earth
          if ( y == 1 ) strcat ( b_str, byungphrodE[1] ); // water
          if ( y == 2 ) strcat ( b_str, byungphrodE[9] ); // fire
          if ( y == 3 ) strcat ( b_str, byungphrodE[7] ); // wind
          break;
        case 2: // fire
          if ( y == 0 ) strcat ( b_str, byungphrodE[8] ); // earth
          if ( y == 1 ) strcat ( b_str, byungphrodE[9] ); // water
          if ( y == 2 ) strcat ( b_str, byungphrodE[3] ); // fire
          if ( y == 3 ) strcat ( b_str, byungphrodE[5] ); // wind
          break;
        case 3: // wind
          if ( y == 0 ) strcat ( b_str, byungphrodE[6] ); // earth
          if ( y == 1 ) strcat ( b_str, byungphrodE[7] ); // water
          if ( y == 2 ) strcat ( b_str, byungphrodE[5] ); // fire
          if ( y == 3 ) strcat ( b_str, byungphrodE[4] ); // wind
          break;
      }    
  } // END - byung_phro

// VKP, 1,148
void get_phrochen ( int *px, int doweek, int lunmanx, int monlong1 )
  { // NEEDS CHECKING FOR (1)-26,27 of 2010 ???
    int x;
    x = lunmanx;
    if ( x > 21 )
      ++x;
    else if ( x == 21 )
      {
        if ( monlong1 >= 30 )
          x = 22;
      }
    else if ( x > 21 )
      ++x;
    *px = 4 * ( doweek - 1 );
    if ( doweek == 0 )
    *px = 24;
    *px = x - *px;
    if ( *px < 0 )
      *px = *px + 28;
  } // END - get_phrochen
  
void zatse_phro ( int *zapro, int *tsepro, int gza, int tshe, int lm , int monlong1 ) // This is for the five gzaphrod[]
  { // Not finished - still needs work
    *zapro = 0; *tsepro = 0;
    switch ( gza )
      {
        case 0:
          if ( lm == 3 ) *zapro = 1;
          else if ( lm == 12 ) *zapro = 2;
          else if ( lm == 19 ) *zapro = 3;
          if ( tshe == 6 || tshe == 7 || tshe == 21 || tshe == 22 )
            *tsepro = 1;
          break;
        case 1:
          if ( lm == 12 ) *zapro = 1;
          else if ( lm == 16 ) *zapro = 2;
          else if ( lm == 9 ) *zapro = 3;
          if ( tshe == 11 || tshe == 12 || tshe == 26 || tshe == 27 )
            *tsepro = 1;
          break;
        case 2:
          if ( lm == 21 && monlong1 < 30 ) *zapro = 1;  // first half only!
          else if ( lm == 2 ) *zapro = 2;
          else if ( lm == 15 ) *zapro = 3;
          if ( tshe == 10 || tshe == 11 || tshe == 25 || tshe == 26 )
            *tsepro = 1;
          break;
        case 3:
          if ( lm == 0 ) *zapro = 1;
          else if ( lm == 23 ) *zapro = 2;
          else if ( lm == 5 ) *zapro = 3;
          if ( tshe == 9 || tshe == 10 || tshe == 24 || tshe == 25 )
            *tsepro = 1;
          break;
        case 4:
          if ( lm == 16 ) *zapro = 1;
          else if ( lm == 0 ) *zapro = 2;
          else if ( lm == 18 ) *zapro = 3;
          if ( tshe == 2 || tshe == 3 || tshe == 17 || tshe == 18 )
            *tsepro = 1;
          break;
        case 5:
          if ( lm == 7 ) *zapro = 1;
          else if ( lm == 4 ) *zapro = 2;
          else if ( lm == 22 ) *zapro = 3;
          if ( tshe == 5 || tshe == 6 || tshe == 20 || tshe == 21 )
            *tsepro = 1;
          break;
        case 6:
          if ( lm == 26 ) *zapro = 1;
          else if ( lm == 3 ) *zapro = 4;
          if ( tshe == 1 || tshe == 2 || tshe == 16 || tshe == 17 )
            *tsepro = 1;
          break;
      }
  } // END - zatse_phro

void chk_tsephro ( char *str, int phro1, int phro2, int gza1, int gza2 )
  { // sreg_str, tsephro1, tsephro2, gzadag[1], gzadag[2]
    int ts_chga, ts_chgb;
    *str = '\0';
    if ( phro1 == 0 && phro2 == 0 )
      return;

// First get time of lunar day change:

   ts_chgb = gza1;
   if ( gza2 > 30 )
     ++gza1;
   if ( gza1 > 60 )
     gza1 = 60;
   ts_chga = ts_chgb / 5;
   ts_chgb = ts_chgb - ts_chga * 5 + 1;

    if ( phro1 == 1 && phro2 == 0 )
      {
        if ( ts_chga == 1 || ts_chga == 4 || ts_chga == 8 || ts_chga == 10 || ts_chga == 11 )
          sprintf ( sreg_str, "sreg tshes %s gi %d bar", cycanimT[ts_chga], ts_chgb );
        else if ( ts_chga == 0 )               
          sprintf ( sreg_str, "sreg tshes %s kyi %d bar", cycanimT[ts_chga], ts_chgb );
        else if ( ts_chga == 5 || ts_chga == 2 )               
          sprintf ( sreg_str, "sreg tshes %s gyi %d bar", cycanimT[ts_chga], ts_chgb );
        else 
          sprintf ( sreg_str, "sreg tshes %s'i %d bar", cycanimT[ts_chga], ts_chgb );
      }
    else if ( phro1 == 0 && phro2 == 1 )
      {
        if ( ts_chga == 1 || ts_chga == 4 || ts_chga == 8 || ts_chga == 10 || ts_chga == 11 )
          sprintf ( sreg_str, "%s gi %d nas sreg tshes", cycanimT[ts_chga], ts_chgb );        
        else if ( ts_chga == 0 )               
          sprintf ( sreg_str, "%s kyi %d nas sreg tshes", cycanimT[ts_chga], ts_chgb );            
        else if ( ts_chga == 5 || ts_chga == 2 )               
          sprintf ( sreg_str, "%s gyi %d nas sreg tshes", cycanimT[ts_chga], ts_chgb );                
        else 
          sprintf ( sreg_str, "%s'i %d nas sreg tshes", cycanimT[ts_chga], ts_chgb );                    
      }
    else if ( phro1 == 1 && phro2 == 1 )
      {
        sprintf ( sreg_str, "sreg tshes" );
      }
  } // END - chk_tsephro

void gen_phrod ( int lm, int dow, int frac )
  {
    gen_phrod_str[0] = 0;
    if ( dow == 0 )
      {
        switch ( lm )
          {
            case 3: strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 12: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 19: strcpy ( gen_phrod_str, "sreg sbyor zung sbyor" ); break;
            case 22: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 14: strcpy ( gen_phrod_str, "bdud rgyal bkris nyi" ); break;
            case 1: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 26: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 10: strcpy ( gen_phrod_str, "mi 'phrod nyi" ); break;
            case 23: strcpy ( gen_phrod_str, "bdud nyi 'phel nyi" ); break;
          }
      }
    else if ( dow == 1 )
      {
        switch ( lm )
          {
            case 12: strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 16: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 9: strcpy ( gen_phrod_str, "sreg sbyor zung sbyor" ); break;
            case 2: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 18: strcpy ( gen_phrod_str, "bdud rgyal" ); break;
            case 15: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 23: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 17: case 22: strcpy ( gen_phrod_str, "mi 'phrod nyi" ); break;
            case 25: strcpy ( gen_phrod_str, "bkris nyi" ); break;
            case 3: strcpy ( gen_phrod_str, "'phel nyi" ); break;
          }
      }
    else if ( dow == 2 )
      {
        switch ( lm )
          {

            case 20: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 15: strcpy ( gen_phrod_str, "sreg sbyor mi 'phrod nyi" ); break;
            case 11: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 21: if ( frac >= 30 ) strcpy ( gen_phrod_str, "'grub sbyor bdud rgyal" );
                     else strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 17: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 25: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 10: strcpy ( gen_phrod_str, "zung sbyor" ); break;
            case 8: strcpy ( gen_phrod_str, "bkris nyi" ); break;
            case 14: strcpy ( gen_phrod_str, "'phel nyi" ); break;
          }
      }
    else if ( dow == 3 )
      {
        switch ( lm )
          {
            case 0: strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 23: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 5: strcpy ( gen_phrod_str, "sreg sbyor zung sbyor" ); break;
            case 8: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 25: strcpy ( gen_phrod_str, "bdud rgyal" ); break;
            case 26: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 24: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 16: strcpy ( gen_phrod_str, "mi 'phrod nyi" ); break;
            case 10: strcpy ( gen_phrod_str, "bkris nyi" ); break;
            case 7: strcpy ( gen_phrod_str, "'phel nyi" ); break;
          }
      }
    else if ( dow == 4 )
      {
        switch ( lm )
          {
            case 16: strcpy ( gen_phrod_str, "'grub sbyor 'phel nyi" ); break;
            case 0: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 18: strcpy ( gen_phrod_str, "sreg sbyor zung sbyor" ); break;
            case 19: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 2: strcpy ( gen_phrod_str, "bdud rgyal" ); break;
            case 12: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 1: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 17: strcpy ( gen_phrod_str, "mi 'phrod nyi bkris nyi" ); break;
          }
      }
    else if ( dow == 5 )
      {
        switch ( lm )
          {
            case 7: strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 4: strcpy ( gen_phrod_str, "'chi sbyor bdud nyi" ); break;
            case 22: strcpy ( gen_phrod_str, "sreg sbyor" ); break;
            case 6: strcpy ( gen_phrod_str, "bdud rgyal" ); break;
            case 3: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 16: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 21: if (frac<30) strcpy ( gen_phrod_str, "mi 'phrod nyi" ); break;
            case 10: strcpy ( gen_phrod_str, "zung sbyor" ); break;
            case 11: strcpy ( gen_phrod_str, "bkris nyi" ); break;
            case 25: strcpy ( gen_phrod_str, "'phel nyi" ); break;
          }
      }
    else if ( dow == 6 )
      {
        switch ( lm )
          {
            case 26: strcpy ( gen_phrod_str, "'grub sbyor" ); break;
            case 8: strcpy ( gen_phrod_str, "'chi sbyor" ); break;
            case 3: strcpy ( gen_phrod_str, "sreg sbyor mi 'phrod nyi" ); break;
            case 7: strcpy ( gen_phrod_str, "bdud nyi" ); break;
            case 10: strcpy ( gen_phrod_str, "bdud rgyal" ); break;
            case 6: strcpy ( gen_phrod_str, "'jig nyi" ); break;
            case 9: strcpy ( gen_phrod_str, "mi mthun nyi" ); break;
            case 4: strcpy ( gen_phrod_str, "zung sbyor 'phel nyi" ); break;
            case 2: strcpy ( gen_phrod_str, "bkris nyi" ); break;
          }
      }
  } // END - gen_phrod ()

int get_tenbrel ( int m, int t )
  {
    int sx, td;
    sx = m - 11;
    if ( sx < 0 )
      sx = sx + 12;
    if ( t <= 15 )
      {
        if ( t <= 12 )
          td = sx + ( t - 1 );
        else
          td = sx + ( t - 11 );
      }
    else
      {
        if ( t <= 27 )
          td = sx - ( t - 16 );
        else
          td = sx - ( t - 2 );
      }
    while ( td < 0 )
      td = td + 12;
    while ( td > 11 )
      td = td - 12;
    return ( td );
  } // END - get_tenbrel

void get_bishti_str ( int tt )
  {
    bishti_str[0] = 0;
    bishti_str2[0] = 0;
    if ( tt == 4 )
      {
      strcpy ( bishti_str, "bishti E." );
      strcpy ( bishti_str2, "biªti X shar" );
      }
    else if ( tt == 8 )
      {
      strcpy ( bishti_str, "bishti S." );
      strcpy ( bishti_str2, "biªti X lho" );
      }
    else if ( tt == 11 )
      {
      strcpy ( bishti_str, "bishti W." );
      strcpy ( bishti_str2, "biªti X nub" );
      }
    else if ( tt == 15 )
      {
      strcpy ( bishti_str, "bishti N." );
      strcpy ( bishti_str2, "biªti X byang" );
      }
    else if ( tt == 18 )
      {
      strcpy ( bishti_str, "bishti SE." );
      strcpy ( bishti_str2, "biªti X lho shar" );
      }
    else if ( tt == 22 )
      {
      strcpy ( bishti_str, "bishti SW." );
      strcpy ( bishti_str2, "biªti X lho nub" );
      }
    else if ( tt == 25 )
      {
      strcpy ( bishti_str, "bishti NW." );
      strcpy ( bishti_str2, "biªti X nub byang" );
      }
    else if ( tt == 29 )
      {
      strcpy ( bishti_str, "bishti NE." );
      strcpy ( bishti_str2, "biªti X byang shar" );
      }
  } // END - get_bishti_str ()

void chk_namgo ( int tt ) // n
  {
    namgo_str[0] = 0;
    if ( tt == 1 || tt == 11 || tt == 21 )
      strcpy ( namgo_str, "mgron" );
    else if ( tt == 2 || tt == 12 || tt == 22 )
      strcpy ( namgo_str, "tshong" );
    else if ( tt == 3 || tt == 13 || tt == 23 )
      strcpy ( namgo_str, "bu" );
    else if ( tt == 4 || tt == 14 || tt == 24 )
      strcpy ( namgo_str, "dmag" );
    else if ( tt == 5 || tt == 15 || tt == 25 )
      strcpy ( namgo_str, "gnyen" );
    else if ( tt == 6 || tt == 16 || tt == 26 )
      strcpy ( namgo_str, "mkhar" );
    else if ( tt == 7 || tt == 17 || tt == 27 )
      strcpy ( namgo_str, "bag" );
    else if ( tt == 8 || tt == 18 || tt == 28 )
      strcpy ( namgo_str, "dur" );
    else if ( tt == 9 || tt == 19 || tt == 29 )
      strcpy ( namgo_str, "shid" );
    else if ( tt == 10 || tt == 20 || tt == 30 )
      strcpy ( namgo_str, "spyi" );
  } // END - chk_namgo ()
