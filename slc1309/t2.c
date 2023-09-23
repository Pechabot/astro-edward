/* T2.C */

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
#include <math.h>
#include <conio.h>
#include "tc.h"
#include "tc.ext"
#include "bcd.h"
#include "bcd.ext"

// Local to this file only:

int  spz_c, spz_b, spz_f, spz_j;

int gzada[6];
int nyida[6];
int nyidm[6], gzadm[6], tsedm[6], nyilm[6];

static int nyidmg[6] = { 2, 10, 58, 1, 17, 0 };
static int gzadmg[6] = { 1, 31, 50, 0, 480, 0 };
static int tsedmg[6] = { 0, 59, 3, 4, 16, 0 };
static int nyilmg[6] = { 0, 4, 21, 5, 43, 0 }; // Each lunar day

static int nyidmb[6] = { 2, 10, 58, 2, 10, 0 };
static int gzadmb[6] = { 1, 31, 50, 0, 0, 0 };
static int tsedmb[6] = { 0, 59, 3, 4, 0, 0 };
static int nyilmb[6] = { 0, 4, 21, 5, 9, 0 };

static int nyifac[6] = { 6, 45, 0, 0, 0, 0 };
static int nyihaf[6] = { 13, 30, 0, 0, 0, 0 };
static int nyibye[6] = { 4, 1, 1, 4, 6, 6};
static int nyidom[6] = { 6, 10, 11, 10, 6, 0};

static int gzabye[14] = { 5, 5, 4, 3, 2, 1, -1, -2, -3,
                                                  -4, -5, -5, -5, 5};
static int gzadom[14] = { 5, 10, 15, 19, 22, 24, 25, 24, 22,
                                                   19, 15, 10, 5, 0};

/**** Figures for Bir Tantra "grub rtsis", year 1987, new (1) ****/

static int gda1[6] = { 1, 42, 47, 3, 465, 0 }; //
static int nda1[6] = { 25, 41, 58, 2, 176930, 0 }; //

/**** Figures for Bir Tantra "grub rtsis", year -1000 (2) ****/

static int gdaD[6] = { 4,  6, 56, 5, 293, 0 }; //
static int ndaD[6] = { 22, 49, 17, 1, 2398189, 0 }; //

/*** ROUTINE TO SET FOR GRUB RTSIS ****/

void set_grub ( void )
  {
    int  i;
    for ( i = 0; i < 6; ++i )
      nyidm[i] = nyidmg[i];
    for ( i = 0; i < 6; ++i )
      gzadm[i] = gzadmg[i];
    for ( i = 0; i < 6; ++i )
      tsedm[i] = tsedmg[i];
    for ( i = 0; i < 6; ++i )
      nyilm[i] = nyilmg[i];
    sun_f = 67;
    gza_f = 707;
  } // END - set_grub ()

/*** ROUTINE TO SET FOR BYED RTSIS ****/

void set_byed ( void )
  {
    int  i;
    for ( i = 0; i < 6; ++i )
      nyidm[i] = nyidmb[i];
    for ( i = 0; i < 6; ++i )
      gzadm[i] = gzadmb[i];
    for ( i = 0; i < 6; ++i )
      tsedm[i] = tsedmb[i];
    for ( i = 0; i < 6; ++i )
      nyilm[i] = nyilmb[i];
    sun_f = 13;
    gza_f = 13;   
  } // END - set_byed ()

// Routine to set epoch parameters 

void set_epoch ( void )
  {
    char chr;
    int  i;
    printf ( "\n\nSelect from the following:\n\n" );
    for ( i = 0; i < 3; ++i )
      printf ( "%s\n", e_str[i] );
    do
      chr = getch ();
    while ( chr < '1' || chr > '2' );
    if ( chr >= '1' && chr <= '2' )
      epch = chr - '0' - 1;
    
    if ( chr == '1' )   // Sherab Ling Tantra, new - 1987
      {
        set_grub ();
        for ( i = 0; i < 6; ++i )
          gzada[i] = gda1[i];
        for ( i = 0; i < 6; ++i )
          nyida[i] = nda1[i];
        epch_yr = 1987;
        eyr_a = 38;   // Lower part of month calculation. // OK
        ril_a = 19; // OK
        ril_b = 111; // 48; // OK
        spz_b = 18; // 64 fraction - OK
        spz_c = 250; // 707 fraction - OK
        spz_f = 1;   // OK
        spz_j = 2446884;  //
        nyi_cnt = 86548;

        rahupart = 8; // 230 fraction for Rahu cycle //

        meradd = 6104; // Figures to be added for planets
        venadd = 1561; // nyin zhag dal ba. These are days of cycle. //
        maradd = 94; //
        jupadd = 4105; //
        satadd = 6867; //

// OVER-RIDE PREVIOUS SETTINGS:
        sun_f = 4815377;
        nyidm[3] = 2;   // These correct solar motion.
        nyidm[4] = 3846950;
// Motion each lunar day:
        for ( i = 0; i < 6; ++i )
          nyilm[i] = nyidm[i];
        div_g6 ( nyilm, 30, sun_f, 1 );

        zlasho1 = 16; // Maybe 17 & 18 are better??
        zlasho2 = 17;

        zlapure = 0;
        tsurlug = 0;
      }
    else if ( chr == '2' )  // Sherab Ling -1000
      {
        /*** D. Bir "Tantra method" ***/
        set_grub ();
        for ( i = 0; i < 6; ++i )
          gzada[i] = gdaD[i];
        for ( i = 0; i < 6; ++i )
          nyida[i] = ndaD[i];
        epch_yr = -1000;
        eyr_a = 45;   // Lower part of month calculation. // OK
        ril_a = 4; // OK
        ril_b = 82; // 19; // OK
        spz_b = 56;  // 64 fraction - OK
        spz_c = 416; // 707 fraction - OK
        spz_f = 5;   // 4 Changed from 5, 7/12/2009
        spz_j = 1355819;  // was 818
        nyi_cnt = 37566; // was 37565 - 115787 cycle

        rahupart = 91; // 230 fraction for Rahu cycle //

        meradd = -2; // -102L; // Figures to be added for planets
        venadd = 96; // 86L; // nyin zhag dal ba. These are days of cycle. //
        maradd = 672;// 671L; //
        jupadd = 372; // 371L; //
        satadd = 3168; // 3167L; //

// OVER-RIDE PREVIOUS SETTINGS:
        sun_f = 4815377;
        nyidm[3] = 2;   // These correct solar motion.
        nyidm[4] = 3846950;
// Motion each lunar day:
        for ( i = 0; i < 6; ++i )
          nyilm[i] = nyidm[i];
        div_g6 ( nyilm, 30, sun_f, 1 );

        zlasho1 = 16; // Maybe 18??
        zlasho2 = 17;
        zlapure = 0;
        tsurlug = 1;
      }      
  } // END - set_epoch ()

// ROUTINE TO CALCULATE POSITIONS OF RAHU

void do_rahu ( int m, int tt ) // KTC 96
  {
    int t, mth;

    mth = ( m + rahupart ) % 230;
    t = ( ( m + rahupart ) % 230 ) * 30 + 15;
    rahutsa[0] = 0;
    rahutsa[1] = 0;
    rahutsa[2] = 14;
    rahutsa[3] = 0;
    rahutsa[4] = 12;

    mul_gen ( rahutsa, rahutsa, t, 27, 23 );
    clear_a_b ();
    lista[0] = 27;
    sub_gen ( rahudong, lista, rahutsa, 27, 23 );
    listb[0] = 13;
    listb[1] = 30;
    add_gen ( rahujug, rahudong, listb, 27, 23 );

    rahutsa[0] = 0;
    rahutsa[1] = 0;
    rahutsa[2] = 14;
    rahutsa[3] = 0;
    rahutsa[4] = 12;
    mul_gen ( rahutsa, rahutsa, t + 15, 27, 23 ); // For new Moon.
    clear_a_b ();
    lista[0] = 27;
    sub_gen ( rahudong30, lista, rahutsa, 27, 23 );
    listb[0] = 13;
    listb[1] = 30;
    add_gen ( rahujug, rahudong, listb, 27, 23 );
    rahu_mth = mth;
  } // END - do_rahu ()

long long sunlong = 27*300*6*797*6811;
long long sun2next;
long long suncyc = 27*60*60*797*6811;

// Function to calculate true month, "zla ba rnam par dag pa"
void zla_dag ( int y, int m ) // KTC 16
  {
    int   yr, a, b; // c;
//    long long sol_long, sol_sign, nxt_sign; // Possible future use
    yr = y - epch_yr;
    
// Need to use this: m x 1;287179 + 0;5566410 (1,9343575)    
    
    a = 12 * yr + m - 3;
    if ( a >= 0 )
      {
        b = 2 * a + eyr_a;
        zladag[1] = b % 65;
        zladag[0] = a + b / 65;
      }
    else
      {
        zladag[1] = 0;
        zladag[0] = 0;
      }

    if ( a >= 0 )
      {
        b = a * 287179 + 5566410;
        zladagN[1] = b % 9343575;
        zladagN[0] = a + b / 9343575;
      }
    else
      {
        zladagN[1] = 0;
        zladagN[0] = 0;
      }      
    zladagN[1] = zladagN[1] * 65 / 9343575;
          
// This new calculation is unfinished. It is for more accurate intercalation.
      
// From here, new, for correction purposes.
// Mean monthly solar motion is: 2;10,58,2,564,5546 (797,6811)
// One solar cycle is: 27x60x60x6x797x6811 = 3165823634..
// One sign = 1520180784..
    nyi_dru (zladag[1]); // Result is in nyidru
    sunlong = ((((nyidru[0]*60+nyidru[1])*60+nyidru[2])*6+nyidru[3])*797+nyidru[4])*6811+nyidru[5];
  } // END - zla_dag ()

// Adjust moth number, if needed
void adj_zla ( void )
  {
    if ( zladag[1] == zlasho1 || zladag[1] == zlasho2 )
      adj_mth = -tm;
    else if ( zladag[1] > zlasho2 )
      adj_mth = tm - 1;
    else if ( zladag[1] == 0 || zladag[1] == 1 )
      {
        if ( !zeromthfg )
          {
            zeromthfg = 1;
            adj_mth = tm - 1;
            zladag[0] = zladag[0] - 1;
          }
        else
          {
            zeromthfg = 0;
            adj_mth = tm;
            zladag[0] = zladag[0] + 1;
          }
      }
    else  // Arrive here if 1 < zladag[1] < zlasho1
      adj_mth = tm;
    if ( adj_mth == 0 )
      adj_mth = 12;
  } // - adj_zla ()

// Function to calculate lunar anomaly, "ril cha"
void rilchaf ( int x ) // KTC 21
  {
    int   a, b;
    b = x + ril_b;
    a = 2 * x + ril_a + b / 126;
    rilcha[1] = b % 126;
    rilcha[0] = a % 28;
  } // END - void rilchaf ( long int x )

// Function to calculate general day, "spyi zhag"
// Calculates Julian for the integer Julian day count at noon UT
// on weekday of "gza' dag". Put into juldat.
void spi_zagf ( void ) // KTC 46
  {
    int b, c;
    spizag = cur_mth * 30 + tt;
    c = spizag + spz_c;
    spz_frac_c = c % 707; 
    b = spizag + spz_b;
    b = b + c / 707;
    spz_frac_b = b % 64;  
    spizag = spizag - b / 64;
    c = ( spizag + spz_f ) % 7;
    b = gzadag[0];
    if ( c != b )
      {
        if ( c > 4 && b < 2 )
          b += 7;
        else if ( b > 4 && c < 2 )
          c += 7;
        spizag = spizag + b - c;
      }
    if ( b - c > 2 )
      printf ( "\nERROR IN GENERAL DAY ROUTINE: %d\n", b - c );
    juldat = spizag + spz_j;
    jul2date ( juldat );
  } // - spi_zagf ();

int gregd2jul ( int D, int M, int Y )
{
  int JD;
  int calndar;  // Gregorian = 1.

// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

  if ( Y > 1582 )
    calndar = 1;
  else if ( Y < 1582 )
    calndar = 0;
  else
    {
      if ( M > 10 )
        calndar = 1;
      else if ( M < 10 )
        calndar = 0;
      else
        {
          if ( D >= 15 )
            calndar = 1;
          else
            calndar = 0;
        }
    }
  if ( calndar )  // Gregorian:
    {
// Seidelmann's formula (2). This routine is valid between March 1, 1600
// through Feb 28, 2100. Calculates Julian date starting at noon on the date in
// question.

//JD = D + ( 1461 * ( Y + ( ( M - 14 ) / 12L ) ) ) / 4L
//       + ( 367L * ( M - 2L - 12L * ( ( M - 14L ) / 12L ) ) ) / 12L
//       + ( 24002L - ( 12L * Y ) - M ) / 1200L + 1721073L + 1L;
// The 1 is added because these routines calculate for noon.

// CHANGED - the following is from ESAA, 1992, p. 604.

  JD = D + ( 1461 * ( Y + 4800 + ( M - 14 ) / 12 ) ) / 4
         + ( 367 * ( M - 2 - 12 * ( ( M - 14 ) / 12 ) ) ) / 12
         - ( 3 * ( ( Y + 4900 + ( M - 14 ) / 12 ) / 100 ) ) / 4 - 32075;
    }
  else  // Julian:
    {
      JD = 367 * Y - ( 7 * ( Y + 5001 + ( M - 9 ) / 7 )) / 4
           + ( 275 * M ) / 9 + D + 1729777;
    }
  return ( JD );
} // END - gregd2jul ()

void jul2date ( int jd )
  {
    int l, n, j, k, i;

// This algorithm is from photcopied notes, from James Neely.
// Also, checked with ESAA, 1992, p. 604
// Calculates date, at noon on which the Julian date starts.
// Julian 0 starts Greenwich mean noon on 1st Jan 4713 BC, Julian proleptic
// calendar.
// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

// First, get day of week:

  doweek = jd - 7 * (( jd + 1 ) / 7 ) + 2;
  if ( doweek == 7 )
    doweek = 0;
  if ( doweek > 7 )
    {
      printf ( "ERROR IN DAY OF WEEK ROUTINE:\n" );
      getch ();
    }

  if ( jd >= 2299161 )  // Gregorian calendar:
    { // This has been tested between March 1, 1600 and Jan 31, 2100
    l = jd + 68569;
    n = ( 4 * l ) / 146097;
    l = l - ( 146097 * n + 3 ) / 4;

//  wy = 4000 * ( l + 1L ) / 1461001;

    l2bcd ( bcda, 4000 );
    mulbcdl ( bcda, bcda, l + 1 );
    divbcdl ( bcda, bcda, 1461001 );
    wy = bcd2l ( bcda );

    l = l - ( 1461 * wy ) / 4 + 31;
    wm = ( 80 * l ) / 2447;
    wd = l - ( 2447 * wm ) / 80;
    l = wm / 11;
    wm = wm + 2 - 12 * l;
    wy = 100 * ( n - 49 ) + wy + l;
// j = month, k = day, i = year
    }
  else // Julian calendar
    {
      j = jd + 1402;
      k = ( j - 1 ) / 1461;
      l = j - 1461 * k;
      n = ( l - 1 ) / 365 - l / 1461;
      i = l - 365 * n + 30;
      j = ( 80 * i ) / 2447;
      wd = i - ( 2447 * j ) / 80;
      i = j / 11;
      wm = j + 2 - 12 * i;
      wy = 4 * k + n + i - 4716;
    }
  } // END - jul2date ()

// Function to calculate month mean Sun, "nyi ma'i dhru ba".
void nyi_dru ( int x )
  {
    mul_gen ( nyidru, nyidm, x, 27, sun_f );
    add_gen ( nyidru, nyidru, nyida, 27, sun_f );
  } // END - nyi_dru ()

// Function to calculate month mean weekday, "gza'i dhru ba".

void gza_dru ( int x )
  {
    mul_gen ( gzadru, gzadm, x, 7, gza_f );
    add_gen ( gzadru, gzadru, gzada, 7, gza_f );
  } // END - gza_dru ()

// Function to calculate lunar day solar longitude, "nyi ma'i longs spyod".
void nyi_lon ( int x )
  {
    mul_gen ( nyilon, nyilm, x, 27, sun_f );
  } // END - nyi_lon ()

// Function to calculate lunar day mean weekday, "tshes kyi dhru ba". 
void tse_dru ( int x )
  {
    mul_gen ( tsedru, tsedm, x, 7, gza_f );
  } // END - tse_dru ()

// Function to calculate true solar longitude, "nyi dag". 
void nyi_dag ( int *a1 )
  {
    int test, tquo, trem;

    clear_a_b ();
    listb[0] = 1;
    listb[1] = 43;
    listb[2] = 30;
    add_gen ( lista, a1, listb, 27, sun_f );
    sub_gen ( nyiwor, lista, nyifac, 27, sun_f );
    test = 60 * nyiwor[0] + nyiwor[1];
    if ( test < 810 )
      nyidor = 0;
    else
      {
        nyidor = 1;
        sub_gen ( nyiwor, nyiwor, nyihaf, 27, sun_f);
        test = 60 * nyiwor[0] + nyiwor[1];
      }
    trem = test % 135;
    tquo = test / 135;
    if ( tquo == 0 )
      tquo = 6;

    clear_a_b ();

    lista[2] = ( trem * 60 + nyiwor[2] ) * nyibye[(int)tquo - 1];
    lista[3] = nyiwor[3] * nyibye[(int)tquo - 1];
    lista[4] = nyiwor[4] * nyibye[(int)tquo - 1];

    div_g6 ( lista, 135, sun_f, 1 );

    clrlst (zerlst);
    add_gen ( lista, zerlst, lista, 27, sun_f );

    listb[1] = nyidom[(int)tquo - 1];
    if ( tquo == 3 || tquo == 4 || tquo == 5 ) // Then, subtract:
      sub_gen ( sol_cor, listb, lista, 27, sun_f );
    else
      add_gen ( sol_cor, listb, lista, 27, sun_f );

    if ( nyidor == 0 )
      sub_gen ( nyidag, a1, sol_cor, 27, sun_f );
    else
      add_gen ( nyidag, a1, sol_cor, 27, sun_f );
  } // END - nyi_dag ()

// Function to calculate true weekday, "gza' dag".
void gza_dag ( int *a1 )
  {
    int tot, trem, chasha, rilpo;
    int gzawor[5];

    clrlst (list1);
    clrlst (list2);
    clrlst (zerlst);
    rilpo = rilcha[0] + tt;
    trem = rilpo % 14;
    if ( trem == 0 )
      trem = 14;
    list2[1] = gzadom[(int)trem - 1];
    
//  gza_short_flg = 1; // This should be set in one place only
    if ( gza_short_flg ) // Flag for less accurate weekday adjustment
      {
        chasha = rilcha[1] * gzabye[trem - 1] * 254520; //60*6*707
        // Maximum value = 125 * 5 * 254520 = 159075000 - OK
        tot = chasha / 126;
      }
    else // This seems to go wrong. Needs checking
      {
        l2bcd ( bcda, ( 30 * rilcha[1] + tt ) * gzabye[(int)trem - 1] );
        mulbcdl ( bcda, bcda, 360 * gza_f );
        divbcdl ( bcda, bcda, 3780 );
        tot = bcd2l (bcda);
      }
    if ( tot < 0 )
      {
        list1[4] = -tot;
        add_gen ( list1, list1, zerlst, 7, gza_f );
        sub_gen ( list3, list2, list1, 7, gza_f );
      }
    else
      {
        list1[4] = tot;
        add_gen ( list1, list1, zerlst, 7, gza_f );
        add_gen ( list3, list2, list1, 7, gza_f );
      }
    if (( rilpo / 14 ) % 2 == 0 )
      add_gen ( gzawor, a1, list3, 7, gza_f);
    else
      sub_gen ( gzawor, a1, list3, 7, gza_f);

    l2bcd ( bcda, gzawor[4] );
    mulbcdl ( bcda, bcda, sun_f );
    divbcdl ( bcda, bcda, gza_f );
    gzawor[4] = bcd2l ( bcda );

//  The above replaces:
//  gzawor[4] = ( sun_f * gzawor[4] ) / gza_f;

    if ( nyidor == 0 )
      sub_gen ( gzadag, gzawor, sol_cor, 7, sun_f );
    else
      add_gen ( gzadag, gzawor, sol_cor, 7, sun_f );

// Convert back the lowest fractional part:

    l2bcd ( bcda, gzadag[4] );
    mulbcdl ( bcda, bcda, gza_f );
    divbcdl ( bcda, bcda, sun_f );
    gzadag[4] = bcd2l ( bcda );
  } // END - gza_dag ()
