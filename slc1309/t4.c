// T4.C - all routines and data for printing to disk a full calendar

/*********************************************************************************
License for SLC - Tibetan Calendar software for Sherab Ling

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
#include "cmonth.h"

int m_m, t_t;

int get_chimonth ( int gm, int mnum )
  {
    int M;
    if ( gm < 13530 || gm > 16015 )
      M = mnum;
    else        
      M = cm[ gm - 13530 ];
    return ( M );
  } // END - get_chimonth

void prn_bir2_cal (void) 
  {
    int i, more, n, newmth, lhag, chad, nextchad, nextlhag;
    int zaphro1, tsephro1; // for daily information
    int zaphro2, tsephro2; // for daily information
    int intercal, delaymth;
    int yr_gender;    // 1 = female, odd; 0 = male, even
    int mt_gender;    // 1 = female, odd; 0 = male, even
    int mthnum; // Adjusted, without negative.
    int cur_year, last_year, x, y;
    int ltt, zla0, zla1;
    int lastjd = 0;
    int curjd, nextjd; // Last and next JD calculations.
    int lunmanx, sbyorba, /*byedpa,*/ byedpax;
    int prv_lunmanx, prv_sbyorba, prv_byedpax;
    int fst_lunmanx, fst_sbyorba, fst_byedpax;
    int bir_spizag;
    int Cmonth;
    int sd_clunmanx;
    int ld_parx; //, sd_parx;
    int ld_smex, sd_smex;
    int yr_animx, mt_animx, ld_animx, sd_animx;
    int yr_elemx, mt_elemx, /*ld_elemx,*/ sd_elemx;
    int nxt_lm;
    int phrochen1, phrochen2;
    int new_year;

    datchek = 0;

    cls ();
    printf ( "%s\n", e_str[epch] );
    printf ("Enter the starting year: ");
    scanf ("%d", &ty);
    printf ("\n");

    cur_year = ty;

    printf ("Enter the finish year: ");
    scanf ("%d", &last_year );
    printf ("\n");

    birdonextyear:

    new_year = 1;
    ty = cur_year;
    tm = 1;
    zeromthfg = 0;

    sprintf ( filenam, "sla_%d.txt", ty );
    fptgt1 = fopen ( filenam, "w" );
    if ( fptgt1 == NULL )
      {
        printf ( "Error opening target file!\n" );
        getch ();
      }

// tt not yet set
// First, check to see if we are not at adjusted 12th month:

    zla_dag (ty, tm); // But only if moving one month at a time!

    zla0 = zladag[0];
    zla1 = zladag[1];

    adj_zla ();
    if ( adj_mth == 12 )
      {
        if ( !zeromthfg )       // Check this out again...
          ++tm;
      }
    else if ( adj_mth == -1 )
      {
        zladag[0] = zla0;
        zladag[1] = zla1;
        zeromthfg = 0;
      }
    n = 0;
    more = 1;
    newmth = 1;
    do   // START OF MONTH LOOP 
      {
        intercal = 0;
        if ( adj_mth == -1 )
          intercal = 1;
        delaymth = 0;
        if ( newmth )
          {
            if ( !zeromthfg )   // We need to use the same data, twice.
              zla_dag (ty, tm); // But only if moving one month at a time!
            adj_zla ();
            cur_mth = zladag[0];  // NEEDED FOR GENERAL DAY
            gen_mth = cur_mth + 14608; // General month number - from which epoch???
            gza_dru (cur_mth);
            nyi_dru (cur_mth);
            rilchaf ( cur_mth ); 
// DO WE HAVE NEW YEAR?
            if ( ( adj_mth == -1 || adj_mth == 1 ) && new_year ) // TCGBK
              {
                if ( ty < 7 )
                  {
                    yr_animx = ( ty + 1200 - 7 ) % 12;
                    yr_elemx = ( ( ty + 1200 ) / 2 - 3 ) % 5;
                    yr_gender = ( ty + 1200 ) % 2; // 1 = female, odd; 0
                  }
                else
                  {
                    yr_animx = ( ty - 7 ) % 12;
                    yr_elemx = ( ty / 2 - 3 ) % 5;
                    yr_gender = ty % 2; // 1 = female, odd; 0 = male
                  }
                sprintf ( outbuf, "\nNew Year: %d, %s-%s-%s\n", ty,
                                  cycelem1[ yr_elemx ],
                                  cycgendE[ yr_gender ],
                                  cycanim[ yr_animx ] );
                fprintf ( fptgt1, "%s", outbuf );
                new_year = 0;
              }
          }

        if ( adj_mth < 0 ) // Intercalary 
          {
            intercal = 1;
            delaymth = 0;
          }
        else if ( adj_mth > 0 && ( zladag[1] == zlasho1+2 || zladag[1] == zlasho2+2 ) )
          {
            intercal = 0;
            delaymth = 1;
          }

        if ( adj_mth < 0 )
          mthnum = -adj_mth;
        else
          mthnum = adj_mth;

        Cmonth = get_chimonth ( gen_mth, mthnum );

        mt_animx = mthnum % 12;
        mt_elemx = yr_elemx + 1 + ( Cmonth + 1 ) / 2;

        if ( yr_gender == 0 ) // Male year, even CE number
          {
            if ( Cmonth == 11 || Cmonth == 12 )
              mt_elemx = yr_elemx + ( Cmonth + 1 ) / 2;
          }

        mt_elemx = mt_elemx % 5;
        mt_gender = ( Cmonth + 1 ) % 2; // There is no month zero

        i = mthnum - 2;
        if ( i < 0 )
          i = i + 12;        
        if ( intercal )  
          {
            sprintf ( outbuf,
            "Tibetan Lunar Month: %d (Intercalary) - %s-%s-%s",
            -adj_mth, cycelem1[ mt_elemx ],
            cycgendE[ mt_gender ], cycanim[ mt_animx ] );
            printf ( "%s\n", outbuf );
            fprintf ( fptgt1, "\n%s\n", outbuf );          
            sprintf ( outbuf, "%s/%s, %s.",
                  MnthS[i], MnthT[i], ZsignsE[i] );
            fprintf ( fptgt1, "%s\n", outbuf );                                
          }
        else if ( delaymth )
          {
            sprintf ( outbuf, "Tibetan Lunar Month: %d (Delayed) - %s-%s-%s",
            adj_mth, cycelem1[ mt_elemx ],
            cycgendE[ mt_gender ], cycanim[ mt_animx ] );
            printf ( "%s\n", outbuf );
            fprintf ( fptgt1, "\n%s\n", outbuf );                   
            sprintf ( outbuf, "%s/%s, %s.",
                  MnthS[i], MnthT[i], ZsignsE[i] );
            fprintf ( fptgt1, "%s\n", outbuf );                                 
          }
        else
          {
            sprintf ( outbuf, "Tibetan Lunar Month: %d - %s-%s-%s",
            adj_mth, cycelem1[ mt_elemx ],
            cycgendE[ mt_gender ], cycanim[ mt_animx ] );
            printf ( "%s\n", outbuf );
            fprintf ( fptgt1, "\n%s\n", outbuf );                    
            sprintf ( outbuf, "%s/%s, %s.",
                  MnthS[i], MnthT[i], ZsignsE[i] );
            fprintf ( fptgt1, "%s\n", outbuf );                    
          }
        sprintf ( outbuf, "Month: %d;%d", zladag[0], zladag[1] );
        fprintf ( fptgt1, "\n%s\n", outbuf );
        sprintf ( outbuf, "Anomaly: %d;%d", rilcha[0], rilcha[1] );
        fprintf ( fptgt1, "%s\n", outbuf );
        tse_dru (0);
        nyi_lon (0);
        add_gen (tsebar, gzadru, tsedru, 7, gza_f);
        if ( datchek )
           {
             sprintf ( outbuf, "First tsebar: %d;%d,%d,%d,%d",
              tsebar[0], tsebar[1], tsebar[2], tsebar[3], tsebar[4] );
             fprintf ( fptgt1, "%s\n", outbuf );                       
           }                       
        tt = 0;
        add_gen (nyibar, nyidru, nyilon, 27, sun_f );
        nyi_dag (nyibar); // MUST BE DONE FIRST
        gza_dag (tsebar);
        spi_zagf ();
        sprintf ( outbuf, "Mean Weekday: %d;%d,%d,%d,%d",
                          gzadru[0], gzadru[1], gzadru[2], gzadru[3], gzadru[4] );
        fprintf ( fptgt1, "%s\n", outbuf );
        x = nyidru[4] / 6811;
        y = nyidru[4] % 6811;
        sprintf ( outbuf, "Mean Sun: %d;%d,%d,%d,%d,%d",
                          nyidru[0], nyidru[1], nyidru[2], nyidru[3], x, y );
        fprintf ( fptgt1, "%s\n", outbuf );

        tt = 1;
        ltt = 1;
        
// START OF DAY LOOP - START OF DAY LOOP - START OF DAY LOOP ***************************
        
        do
          {   
            nextlhag = 0;
            lhag = 0;
            chad = 0;
            nextchad = 0;

// Work out previous and next lunar days to determine CHAD or LHAG and prepare other data
            
            tt = ltt - 1;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );
            nyi_dag (nyibar); // MUST BE DONE BEFORE gza_dag
            gza_dag (tsebar);
            spi_zagf ();
            lastjd = juldat;
            for ( i = 0; i < 5; ++i )
              monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );
            for ( i = 0; i < 6; ++i )
              {
                prv_nyibar[i] = nyibar[i];              
                prv_nyidag[i] = nyidag[i];
                prv_gzadag[i] = gzadag[i];
                prv_monlong[i] = monlong[i];
              }
// Next:
            tt = ltt + 1;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );
            nyi_dag (nyibar); // MUST BE DONE FIRST
            gza_dag (tsebar);
            spi_zagf ();
            nextjd = juldat;
            for ( i = 0; i < 5; ++i )
              monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );
            for ( i = 0; i < 6; ++i )
              {
                nxt_nyibar[i] = nyibar[i];              
                nxt_nyidag[i] = nyidag[i];
                nxt_gzadag[i] = gzadag[i];
                nxt_monlong[i] = monlong[i];
              }

// Now, calculate for current lunar day:

            tt = ltt;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );
            nyi_dag (nyibar); // MUST BE DONE FIRST
            gza_dag (tsebar);
            for ( i = 0; i < 5; ++i )
              monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );
            spi_zagf ();
            curjd = juldat;
            if ( curjd == lastjd + 2 )
              lhag = 1;
            if ( nextjd == curjd + 2 )
              nextlhag = 1;
            if ( curjd == lastjd )
              chad = 1;
            if ( curjd == nextjd )
              nextchad = 1;

            if (lhag)
              bir_spizag = spizag - 1;
            else
              bir_spizag = spizag;
            ++bir_spizag; // For their strange way of doing things.

            if ( datchek )
              {
               sprintf ( outbuf, "Current tsebar: %d;%d,%d,%d,%d",
                 tsebar[0], tsebar[1], tsebar[2], tsebar[3], tsebar[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
              
               sprintf ( outbuf, "Previous nyidag: %d;%d,%d,%d,%d",
                 prv_nyidag[0], prv_nyidag[1], prv_nyidag[2], prv_nyidag[3], prv_nyidag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Previous gzadag: %d;%d,%d,%d,%d",
                 prv_gzadag[0], prv_gzadag[1], prv_gzadag[2], prv_gzadag[3], prv_gzadag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Previous monlong: %d;%d,%d,%d,%d",
                 prv_monlong[0], prv_monlong[1], prv_monlong[2], prv_monlong[3], prv_monlong[4] );
               fprintf ( fptgt1, "%s\n", outbuf );                               
               
               sprintf ( outbuf, "Current nyidag: %d;%d,%d,%d,%d",
                 nyidag[0], nyidag[1], nyidag[2], nyidag[3], nyidag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Current gzadag: %d;%d,%d,%d,%d",
                 gzadag[0], gzadag[1], gzadag[2], gzadag[3], gzadag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Current monlong: %d;%d,%d,%d,%d",
                 monlong[0], monlong[1], monlong[2], monlong[3], monlong[4] );
               fprintf ( fptgt1, "%s\n", outbuf );                                               
                          
               sprintf ( outbuf, "Next nyidag: %d;%d,%d,%d,%d",
                 nxt_nyidag[0], nxt_nyidag[1], nxt_nyidag[2], nxt_nyidag[3], nxt_nyidag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Next gzadag: %d;%d,%d,%d,%d",
                 nxt_gzadag[0], nxt_gzadag[1], nxt_gzadag[2], nxt_gzadag[3], nxt_gzadag[4] );
               fprintf ( fptgt1, "%s\n", outbuf );               
               sprintf ( outbuf, "Next monlong: %d;%d,%d,%d,%d",
                 nxt_monlong[0], nxt_monlong[1], nxt_monlong[2], nxt_monlong[3], nxt_monlong[4] );
               fprintf ( fptgt1, "%s\n", outbuf );                                                         
               sprintf ( outbuf, "bir_spizag: %d", bir_spizag );
               fprintf ( fptgt1, "%s\n", outbuf );                                                         
               datchek = 0;
              }

//Calculate lunar mansion at daybreak:
// add solar longitude to tithi * factor. Then subtract hours, etc., from gzadag

            clear_a_b ();
            for ( i = 0; i < 5; ++i )
              monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );
            for ( i = 0; i < 6; ++i )
              tt_monlong[i] = monlong[i];
   
// Solar day data:

            sd_animx = ( juldat - 2 ) % 12;
            sd_elemx = ( ( juldat - 3 ) / 2 ) % 5;
            sd_clunmanx = ( juldat - 17 ) % 28;
            sd_smex = ( juldat - 2 ) % 9 + 1;  

// Lunar day data:

// If Chinese month is number 1, Trigram is Li, index = 1
            ld_parx = ( Cmonth - 1 ) * 30 + tt;
            ld_parx = ld_parx % 8;
// If Chinese month is number 1, "sme ba" is 1
            ld_smex = ( Cmonth - 1 ) * 30 + tt;
            ld_smex = ld_smex % 9;
            if ( ld_smex == 0 )
              ld_smex = 9;
// If Chinese month is number 1, Animal is Tiger, index = 11
            ld_animx = ( Cmonth - 1 ) * 30 + tt + 10;
            ld_animx = ld_animx % 12;

            if ( chad )  // This is for omitted lunar day 
              {
// LINE 1 - Omitted day.
                sprintf ( outbuf, "%d. Omitted: %s %s %d",
                        tt, cycanim[ ld_animx ], cycpar[ ld_parx ], ld_smex );
                fprintf ( fptgt1, "%s\n", outbuf );
              }
            else  // IF DUPLICATED: 
              {
// NYIDAG IS NOT CHANGED AFTER HERE, SO WE CAN CALCULATE ZODIACAL SIGN:
// Zodiac signs, minutes, seconds -  ZS_s, ZS_d, ZS_m;

                lm2signs ( &ZS_s, &ZS_d, &ZS_m, nyidag ); // Was nyibar
                if ( lhag )  // IF WE HAVE DUPLICATED LUNAR DATE - BOTH IN THIS BLOCK
                { 
                // Put here any special calculations for extra day -- *NEW*
                // At this point, the gzadag, nyidag & monlong are correct for
                // the second of the two days with same tithi. We need for the
                // first.
                // Previous tithi ended just before start of day we need.
               
// prv_monlong is the moon longitude at beginning of current lunar day.

                clear_a_b ();                
                listb[0] = 1;
                for ( i = 1; i < 5; ++i )
                  lista[i] = prv_gzadag[i];
                sub_gen ( listb, listb, lista, 7, gza_f );
                l2bcd ( bcdx0, sun_f );
                mulbcdl ( bcdx0, bcdx0, listb[4] );
                divbcdl ( bcdx1, bcdx0, gza_f );
                listb[4] = bcd2l ( bcdx1 ); 
                add_gen ( fst_monlong, prv_monlong, listb, 27, sun_f );

// This is now the moon longitude at daybreak of first of duplicated day

                fst_lunmanx = fst_monlong[0]; 
                listb[0] = 0;
                for ( i = 1; i < 5; ++i )
                  listb[i] = gzadag[i];
                l2bcd ( bcdx0, sun_f );
                mulbcdl ( bcdx0, bcdx0, listb[4] );
                divbcdl ( bcdx1, bcdx0, gza_f );
                listb[4] = bcd2l ( bcdx1 ); 
                sub_gen ( cur_monlong, monlong, listb, 27, sun_f );
// This is now the moon longitude at daybreak of second duplicated day
                lunmanx = cur_monlong[0]; 

// Now, for end of second duplicated day.
// First, if lunar day is not omitted 

                if ( nxt_gzadag[0] != gzadag[0] ) 
                  {
                    listb[0] = 0;
                    for ( i = 1; i < 5; ++i )
                      listb[i] = nxt_gzadag[i];
                    l2bcd ( bcdx0, sun_f );
                    mulbcdl ( bcdx0, bcdx0, listb[4] );
                    divbcdl ( bcdx1, bcdx0, gza_f );
                    listb[4] = bcd2l ( bcdx1 ); 
                    sub_gen ( scd_monlong, nxt_monlong, listb, 27, sun_f );
// This is now the moon longitude at end of second duplicated day
                  }
                else // if following lunar day IS omitted
                  {  // it can happen -- see, eg., phugpa 6th of 9th month 1457
                    clear_a_b ();                
                    listb[0] = 1;
                    for ( i = 1; i < 5; ++i )
                      lista[i] = nxt_gzadag[i];
                    sub_gen ( listb, listb, lista, 7, gza_f );
                    l2bcd ( bcdx0, sun_f );
                    mulbcdl ( bcdx0, bcdx0, listb[4] );
                    divbcdl ( bcdx1, bcdx0, gza_f );
                    listb[4] = bcd2l ( bcdx1 ); 
                    add_gen ( scd_monlong, nxt_monlong, listb, 27, sun_f );
// This is now the moon longitude at end of second duplicated day
                  }

            mak_lmchange_string ( fst_monlong, cur_monlong, doweek );
            prv_lunmanx = prv_monlong[0]; // This was earlier missing

// prv_monlong - Moon longitude at beginning of current lunar day. OK
// fst_monlong - Moon longitude at daybreak of first duplicated day
// cur_monlong - Moon longitude at daybreak of second duplicated day
// monlong - Moon longitude at end of current lunar day
// scd_monlong - Moon longitude at end of second duplicated day
// nxt_monlong - Moon longitude at end of next lunar day ??
           
//NOW CALCULATE SBYOR BA: 

// This, from TCGBK: 
                add_gen ( fst_sbyorlong, fst_monlong, nyidag, 27, sun_f ); 
                fst_sbyorba = fst_sbyorlong[0];
          
// NOW CALCULATE BYED PA:
                
                fst_byedpax = calc_byedpa ( fst_monlong, prv_nyidag );                
                byedpax = calc_byedpa ( cur_monlong, nyidag );                
                juldat = juldat - 1;
                jul2date ( juldat ); // This is normally done by spi_zagf

// Solar day data: - calculating for first of duplicated day 
// Done earlier, used for normal days, but needs redoing for first of duplicated
// and then again for second

                sd_animx = ( juldat - 2 ) % 12;
                sd_elemx = ( ( juldat - 3 ) / 2 ) % 5;
                sd_clunmanx = ( juldat - 17 ) % 28;
                sd_smex = ( juldat - 2 ) % 9 + 1; 

// For general and special "'phrod":
                gen_phrod ( fst_lunmanx, doweek, fst_monlong[1] ); // sets gen_phrod_str
// For 'phrod chen/sbyor:
                get_phrochen ( &phrodx, doweek, prv_lunmanx, prv_monlong[1] );
                byung_phro ( byungphro_str, doweek, prv_lunmanx, monlong[1] ); // ???        

// LINE 1. - First of duplicated day. 
                sprintf ( outbuf, "%d: %s %s. %s/%s(%d). %s-%s, %s; %s",
                          tt, 
                          dga2T[tt-1], 
                          dayoweek[doweek ], 
                          lunmanS[ fst_lunmanx ], lunmanT[ fst_lunmanx ], fst_lunmanx, // XXX
                          fourelemE[ dowkelem[ doweek ] ],
                          fourelemE[ lunmanelem[ fst_lunmanx ] ],
                          byungphro_str,
                          lmchgE );
                  fprintf ( fptgt1, "%s\n", outbuf );
            
// LINE 2. - First of duplicated day.
                sprintf ( outbuf, "%s/%s, %s/%s, %s, %s %d",
                          yogaS[ fst_sbyorba ], yogaT[ fst_sbyorba ],
                          byedS[ fst_byedpax ], byedT[ fst_byedpax ],
                          cycanim[ ld_animx ],
                          cycpar[ ld_parx ], ld_smex );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// LINE 3. - First of duplicated day.
                    sprintf ( outbuf, "Solar: %s-%s. %s %d" ,
                              cycelem1[ sd_elemx ],
                              cycanim[ sd_animx ],
                              Clunman[ sd_clunmanx ],
                              sd_smex );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// LINE 4. - First of duplicated day.

                sprintf ( outbuf,
                "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                       ( prv_gzadag[0] + 1 ) % 7, 60, 0,
                       fst_monlong[0], fst_monlong[1], fst_monlong[2],
                       nyidag[0], nyidag[1], nyidag[2],
                       fst_sbyorlong[0], fst_sbyorlong[1], fst_sbyorlong[2],
                       ZS_s, ZS_d, ZS_m );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );                    

// Have extra line here if there is a solar term this day: 

                if ( chk_solar_term ( prv_gzadag, prv_nyibar, solar_term_str ))
                  {
                    fprintf ( fptgt1, "\x20\x20%s\n", solar_term_str );
                  }

// LINE 6. - First of duplicated day.

                sprintf ( outbuf, "%d %s %d",
                          wd, wmonths[ (wm-1) ], wy);
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// NOW DO SECOND OF REPEATED LUNAR DAYS 

                juldat = juldat + 1;  // This is readjustment!
                jul2date ( juldat );

// Solar day data:  

                sd_animx = ( juldat - 2 ) % 12;
                sd_elemx = ( ( juldat - 3 ) / 2 ) % 5;
                sd_clunmanx = ( juldat - 17 ) % 28;
                sd_smex = ( juldat - 2 ) % 9 + 1;  // CHANGED 31/8/2004

// Calculate sbyor ba:

                add_gen ( sbyorlong, cur_monlong, nyidag, 27, sun_f ); 
                sbyorba = sbyorlong[0];

// Now calculate karana, sbyed pa:

                byedpax = calc_byedpa ( cur_monlong, nyidag );
                gen_phrod ( lunmanx, doweek, cur_monlong[1] );       
                lm2signs ( &ZS_s, &ZS_d, &ZS_m, nyidag ); // Needs to be done again       

                mak_lmchange_string ( cur_monlong, scd_monlong, doweek ); // details calculated above
                byung_phro ( byungphro_str, doweek, lunmanx, monlong[1] ); // ???

// LINE 1. - Second of duplicated day  

                sprintf ( outbuf, "%d: %s. %s/%s(%d). %s-%s, %s; %s",
                          tt, 
                          dayoweek[ doweek ],
                          lunmanS[ lunmanx ], lunmanT[ lunmanx ], lunmanx,
                          fourelemE[ dowkelem[ doweek ] ],
                          fourelemE[ lunmanelem[ lunmanx ] ],
                          byungphro_str,                          
                          lmchgE );
                          
                fprintf ( fptgt1, "%s\n", outbuf );

// LINE 2. - Second of duplicated day
                sprintf ( outbuf, "%s/%s, %s/%s, %s, %s %d",
                          yogaS[ sbyorba ], yogaT[ sbyorba ],
                          byedS[ byedpax ], byedT[ byedpax ],
                          cycanim[ ld_animx ], 
                          cycpar[ ld_parx ], ld_smex );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// LINE 3. - Second of duplicated day

                sprintf ( outbuf, "Solar: %s-%s. %s %d" ,
                          cycelem1[ sd_elemx ],
                          cycanim[ sd_animx ],
                          Clunman[ sd_clunmanx ],
                          sd_smex );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// LINE 4. - Second of duplicated day

                sprintf ( outbuf,
              "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m );                 
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );                 

// Have extra line here if there is a solar term this day: 

                if ( chk_solar_term ( gzadag, nyibar, solar_term_str ))
                  {
                    fprintf ( fptgt1, "\x20\x20%s\n", solar_term_str );
                  }

// LINE 6. - Second of duplicated day 

                sprintf ( outbuf, "%d %s %d",
                          wd, wmonths[(wm-1) ], wy);
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );
                }
              else  // NO LHAG - NORMAL DAY 
                {
//  CALCULATE LUNAR MANSION AT SUNRISE, AND END OF SOLAR DAY:  
                clear_a_b ();                
                listb[0] = 0;
                for ( i = 1; i < 5; ++i )
                  listb[i] = gzadag[i];
                l2bcd ( bcdx0, sun_f );
                mulbcdl ( bcdx0, bcdx0, listb[4] );
                divbcdl ( bcdx1, bcdx0, gza_f );
                listb[4] = bcd2l ( bcdx1 ); 
                sub_gen ( cur_monlong, tt_monlong, listb, 27, sun_f );
                lunmanx = cur_monlong[0];                 
// This is now the moon longitude at daybreak of current day             

// NOW, CALCULATE FOR END OF CURRENT, NORMAL, SOLAR DAY:

                clear_a_b ();                
                listb[0] = 1;
                for ( i = 1; i < 5; ++i )
                  lista[i] = gzadag[i]; // Previous lunar day
                sub_gen ( listb, listb, lista, 7, gza_f );
                l2bcd ( bcdx0, sun_f );
                mulbcdl ( bcdx0, bcdx0, listb[4] );
                divbcdl ( bcdx1, bcdx0, gza_f );
                listb[4] = bcd2l ( bcdx1 ); 
                add_gen ( scd_monlong, tt_monlong, listb, 27, sun_f );

                mak_lmchange_string ( cur_monlong, scd_monlong, doweek );
// the above returns null string if no change

// This not yet used. Need English translations

                get_phrochen ( &phrochen1, doweek, lunmanx, cur_monlong[1] );
                get_phrochen ( &phrochen2, doweek, nxt_lm, 0 /*scd_monlong[1]*/ );

                byung_phro ( byungphro_str, doweek, lunmanx, monlong[1] ); // ???

// CALCULATE SBYOR BA: 

                add_gen ( sbyorlong, cur_monlong, nyidag, 27, sun_f );
                sbyorba = sbyorlong[0];
          
// NOW CALCULATE BYED PA:

                byedpax = calc_byedpa ( cur_monlong, nyidag );

// LINE 1. Normal day 

                sprintf ( outbuf, "%d: %s %s. %s/%s(%d), %s-%s, %s; %s",
                          tt, 
                          dga2T[tt-1], 
                          dayoweek[ doweek ],
                          lunmanS[ lunmanx ], lunmanT[ lunmanx ], lunmanx, 
                          fourelemE[ dowkelem[ doweek ] ],
                          fourelemE[ lunmanelem[ lunmanx ] ],
                          byungphro_str,
                          lmchgE );
                fprintf ( fptgt1, "%s\n", outbuf );

// LINE 2. Normal day
                sprintf ( outbuf, "%s/%s, %s/%s, %s, %s %d",
                          yogaS[ sbyorba ], yogaT[ sbyorba ],
                          byedS[ byedpax ], byedT[ byedpax ],
                          cycanim[ ld_animx ],
                          cycpar[ ld_parx ], ld_smex ); 
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf ); 

// LINE 3. Normal day

                sprintf ( outbuf, "Solar: %s-%s. %s %d" ,
                              cycelem1[ sd_elemx ],
                              cycanim[ sd_animx ],
                              Clunman[ sd_clunmanx ],
                              sd_smex );
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// LINE 4. Normal day

                sprintf ( outbuf,
                "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],                          
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m );                
                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

// Have extra line here if there is a solar term this day: 

                if ( chk_solar_term ( gzadag, nyibar, solar_term_str ))
                  {
                    fprintf ( fptgt1, "\x20\x20%s\n", solar_term_str );
                  }
                else if ( nextchad )
                  if ( chk_solar_term ( nxt_gzadag, nxt_nyibar, solar_term_str ))
                    fprintf ( fptgt1, "\x20\x20%s\n", solar_term_str );
             
// LINE 5.
                sprintf ( outbuf, "%d %s %d",
                          wd, wmonths[ (wm-1) ], wy);

                fprintf ( fptgt1, "\x20\x20%s\n", outbuf );

                } // END - NO LHAG - NORMAL DAY

                if ( !delaymth && chk_spec ( mthnum, tt ) )
                  {
                    fprintf ( fptgt1, "  %s\n", outbuf );
                  }
              } // END - IF NORMAL OR DUPLICATED: 

            ++ltt;
            newmth = 0;
            lastjd = juldat;
          } while ( ltt <= 30 ); // END OF DAY LOOP

        newmth = 1;
        if ( !zeromthfg )
          ++tm;
        if ( tm > 12 )
          {
            ++ty;
            tm = 1;
          }
        if ( adj_mth == 12 )
          more = 0; // We have finished.
      } while ( more );        // END OF MONTH LOOP

    fclose ( fptgt1 );  
    if ( full_print )
      prn_cal2 ( cur_year );
    if ( cur_year < last_year )
      {
        ++cur_year;
        goto birdonextyear;
      }
  } // END - prn_bir2_cal ()
  
  
