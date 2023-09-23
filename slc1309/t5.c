// t5.c - all routines and data for printing to disk a full calendar

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
#include "wordan.h"
#include "wordan.def"

extern int yan_kwong_flg;
extern int zin_phung_flg;
extern int klu_bzlog_flg;
extern int klu_thebs_flg;
extern int nyi_nag_flg;

extern int nxt_monlong[6];

char transtr1[200];
char transtr2[200];

void prn_cal2 ( int cy )
  {
    int i, more, n, newmth, lhag, chad, nextchad, nextlhag;
    int intercal, delaymth;
    int yr_gender;    // 1 = female, odd; 0 = male, even
    int mt_gender;    // 1 = female, odd; 0 = male, even
    int mthnum; // Adjusted, without negative.
    int cur_year; 
    int ltt, zla0, zla1;
    int lastjd = 0;
    int curjd, nextjd; // Last and next JD calculations.
    int  lunmanx, sbyorba, byedpax;
    int  prv_lunmanx, fst_sbyorba, fst_byedpax;
    int fst_lunmanx; 
    int bir_spizag;    
    int Cmonth;
    int tenbrelx;
    int sd_clunmanx;
    int ld_parx;
    int ld_smex, sd_smex;
    int yr_animx, mt_animx, ld_animx, sd_animx;
    int yr_elemx, mt_elemx, sd_elemx;
    int e, g, x5, x6;
    int new_year;
    
    datchek = 0;
//    getch ();
    d_bug = 0;

    cur_year = cy;
    new_year = 1;
    ty = cur_year;
    tm = 1;
    zeromthfg = 0;

    sprintf ( filenam, "sl2_%d.txt", ty );
    fptgt = fopen ( filenam, "w" );
    if ( fptgt == NULL )
      {
        printf ( "Error opening target file!\n" );
        getch ();
      }
    sprintf ( filenam, "sl3_%d.txt", ty );
    fptgt2 = fopen ( filenam, "w" );
    if ( fptgt2 == NULL )
      {
        printf ( "Error opening target file!\n" );
        getch ();
      }
    outbuf[0] = 0xef;  
    outbuf[1] = 0xbb;   
    outbuf[2] = 0xbf;            
    outbuf[3] = 0;                
    fprintf ( fptgt2, "%s", outbuf );         

// tt not yet set
// First, check to see if we are not at adjusted 12th month:

    zla_dag (ty, tm); // But only if moving one month at a time!

    zla0 = zladag[0];
    zla1 = zladag[1];

    adj_zla ();
    if ( adj_mth == 12 )
      {
        if ( !zeromthfg ) // Should always work
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
            cur_mth = zladag[0];  // Needed for general day
            gen_mth = cur_mth + 14608; // General month number - from which epoch??     
            gza_dru (cur_mth);
            
            nyi_dru (cur_mth);    
                    
            rilchaf ( cur_mth ); // MOVED, from gza_dag, NEW, 11/9/94
// DO WE HAVE NEW YEAR?
            if ( ( adj_mth == -1 || adj_mth == 1 ) && new_year )
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
                                  cycelem1T[ yr_elemx ],
                                  cycgendT[ yr_gender ],
                                  cycanimT[ yr_animx ] );
                fprintf ( fptgt, "%s", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "%s", outbuf );                

                sprintf ( transtr1, "lo gsar X %d X %s X %s X %s", ty,
                                  cycelem1T[ yr_elemx ],
                                  cycgendT[ yr_gender ],
                                  cycanimT[ yr_animx ] );
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); 
                fprintf ( fptgt2, "%s\n", outbuf );                
                new_year = 0;
              }
          } // END - if ( newmth )
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

        if ( intercal ) 
          {
            sprintf ( outbuf,
            "Tibetan Lunar Month: %d (Intercalary) - %s-%s-%s",
            -adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );
            
            sprintf ( transtr1, "hor zla X %d snga ma X %s X %s X %s",
            -adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );                        
          }
        else if ( delaymth )
          {
            sprintf ( outbuf, "Tibetan Lunar Month: %d (Delayed) - %s-%s-%s",
            adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );
            
            sprintf ( transtr1, "hor zla X %d phyi ma X %s X %s X %s",
            adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );                        
          }
        else
          {
            sprintf ( outbuf, "Tibetan Lunar Month: %d - %s-%s-%s",
            adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );
            
            sprintf ( transtr1, "hor zla X %d X %s X %s X %s",
            adj_mth, cycelem1T[ mt_elemx ],
            cycgendT[ mt_gender ], cycanimT[ mt_animx ] );            
          }
        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "\n%s\n", outbuf );
        if ( d_bug )
          fprintf ( fptgt2, "\n%s\n", outbuf );        
        str2uni ( transtr1 ); 
        sprintf ( outbuf, "%s", wordstr8 );
        fprintf ( fptgt2, "%s\n", outbuf );                                    

        sprintf ( outbuf, "Month: %d;%d", zladag[0], zladag[1] );
        fprintf ( fptgt, "\n%s\n", outbuf );
        if ( d_bug )        
          fprintf ( fptgt2, "\n%s\n", outbuf );        
        sprintf ( outbuf, "zla ba X %d X %d", zladag[0], zladag[1] );        
        str2uni ( outbuf ); 
        sprintf ( outbuf, "%s", wordstr8 );        
        fprintf ( fptgt2, "%s\n", outbuf ); 
                       
        sprintf ( outbuf, "Anomaly: %d;%d", rilcha[0], rilcha[1] );
        fprintf ( fptgt, "%s\n", outbuf );
        if ( d_bug )        
          fprintf ( fptgt2, "%s\n", outbuf );        
        sprintf ( outbuf, "ril cha X %d X %d", rilcha[0], rilcha[1] );        
        str2uni ( outbuf ); 
        sprintf ( outbuf, "%s", wordstr8 );        
        fprintf ( fptgt2, "%s\n", outbuf );
                
        tse_dru (0);
        nyi_lon (0);        
        add_gen (tsebar, gzadru, tsedru, 7, gza_f);

        tt = 0;        
        add_gen (nyibar, nyidru, nyilon, 27, sun_f );        
        nyi_dag (nyibar); // MUST BE DONE FIRST        
        gza_dag (tsebar);
        spi_zagf ();

        x6 = gzadru[4] * 67; 
        x5 = x6 / 707;
        x6 = x6 - x5 * 707;
        sprintf ( outbuf, "Mean Weekday: %d;%d,%d,%d,%d,%d",
                          gzadru[0], gzadru[1], gzadru[2], gzadru[3], x5, x6 );        
        fprintf ( fptgt, "%s\n", outbuf );  
        if ( d_bug )        
          fprintf ( fptgt2, "%s\n", outbuf );          
        sprintf ( outbuf, "gza' dhru X %d X %d X %d X %d X %d", 
                  gzadru[0], gzadru[1], gzadru[2], gzadru[3], x5, x6 );        
        str2uni ( outbuf ); 
        sprintf ( outbuf, "%s", wordstr8 );        
        fprintf ( fptgt2, "%s\n", outbuf );        
             
        x6 = nyidru[4] * 13;
        x5 = x6 / 67;
        x6 = x6 - x5 * 67;     
        
        x5 = nyidru[4] / 6811; // these from SLC
        x6 = nyidru[4] % 6811;        
           
        sprintf ( outbuf, "Mean Sun: %d;%d,%d,%d,%d",
                          nyidru[0], nyidru[1], nyidru[2], nyidru[3], x5, x6 );
//                        nyidru[0], nyidru[1], nyidru[2], nyidru[3], x5, x6 );                          
        fprintf ( fptgt, "%s\n\n", outbuf );
        if ( d_bug )        
          fprintf ( fptgt2, "%s\n\n", outbuf );        
        sprintf ( outbuf, "nyi dhru X %d X %d X %d X %d X %d", 
                  nyidru[0], nyidru[1], nyidru[2], nyidru[3], x5, x6 );        
        str2uni ( outbuf ); 
        sprintf ( outbuf, "%s", wordstr8 );        
        fprintf ( fptgt2, "%s\n\n", outbuf );        
       
        tt = 1;
        ltt = 1;
        
// START OF DAY LOOP ********************** 
        do
          {  
            nextlhag = 0;
            lhag = 0;
            chad = 0;
            nextchad = 0;

// Work out previous and next days to determine CHAD or LHAG

            tt = ltt - 1;
            tse_dru (tt);
            nyi_lon (tt);            
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );            
            nyi_dag (nyibar); // Must be done before gza_dag            
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
                prv_nyidag[i] = nyidag[i];
                prv_gzadag[i] = gzadag[i];
                prv_monlong[i] = monlong[i];
              }

            tt = ltt + 1;
            tse_dru (tt);
            nyi_lon (tt);            
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );            
            nyi_dag (nyibar); // Must be done before gza_dag            
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
                nxt_nyidag[i] = nyidag[i];
                nxt_gzadag[i] = gzadag[i];
                nxt_monlong[i] = monlong[i];
              }            
            
// Recalculate for current tithi: 

            tt = ltt;
            tse_dru (tt);
            nyi_lon (tt);            
            add_gen (tsebar, gzadru, tsedru, 7, gza_f );
            add_gen (nyibar, nyidru, nyilon, 27, sun_f );            
            nyi_dag (nyibar); // Must be done before gza_dag            
            gza_dag (tsebar);
            for ( i = 0; i < 5; ++i ) 
              monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );            
            spi_zagf ();
            curjd = juldat; // Added from SLC 
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
                sprintf ( outbuf, "Previous nyidag: %d;%d,%d,%d,%d",
                  prv_nyidag[0], prv_nyidag[1], prv_nyidag[2], prv_nyidag[3], prv_nyidag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Previous gzadag: %d;%d,%d,%d,%d",
                  prv_gzadag[0], prv_gzadag[1], prv_gzadag[2], prv_gzadag[3], prv_gzadag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Previous monlong: %d;%d,%d,%d,%d",
                  prv_monlong[0], prv_monlong[1], prv_monlong[2], prv_monlong[3], prv_monlong[4] );
                fprintf ( fptgt, "%s\n", outbuf );                               
                
                sprintf ( outbuf, "Current nyidag: %d;%d,%d,%d,%d",
                  nyidag[0], nyidag[1], nyidag[2], nyidag[3], nyidag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Current gzadag: %d;%d,%d,%d,%d",
                  gzadag[0], gzadag[1], gzadag[2], gzadag[3], gzadag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Current monlong: %d;%d,%d,%d,%d",
                  monlong[0], monlong[1], monlong[2], monlong[3], monlong[4] );
                fprintf ( fptgt, "%s\n", outbuf );                                               
                          
                sprintf ( outbuf, "Next nyidag: %d;%d,%d,%d,%d",
                  nxt_nyidag[0], nxt_nyidag[1], nxt_nyidag[2], nxt_nyidag[3], nxt_nyidag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Next gzadag: %d;%d,%d,%d,%d",
                  nxt_gzadag[0], nxt_gzadag[1], nxt_gzadag[2], nxt_gzadag[3], nxt_gzadag[4] );
                fprintf ( fptgt, "%s\n", outbuf );               
                sprintf ( outbuf, "Next monlong: %d;%d,%d,%d,%d",
                  nxt_monlong[0], nxt_monlong[1], nxt_monlong[2], nxt_monlong[3], nxt_monlong[4] );
                fprintf ( fptgt, "%s\n", outbuf );                                                         
                sprintf ( outbuf, "bir_spizag: %d", bir_spizag );
                fprintf ( fptgt1, "%s\n", outbuf );                                                                         
                datchek = 0;
               }

// Calculate lunar mansion at daybreak:
// add solar longitude to tithi * factor. Then subtract hours, etc., from gzadag

            clear_a_b ();
            for ( i = 0; i < 5; ++i )
            monlong[i] = 0;
            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen ( monlong, monlong, tt, 27, sun_f );
            add_gen ( monlong, monlong, nyidag, 27, sun_f );
            for ( i = 0; i < 6; ++i )
              tt_monlong[i] = monlong[i];
   
            listb[0] = 0;
            for ( i = 1; i < 5; ++i )
            listb[i] = gzadag[i];
            listb[4] = ( sun_f * listb[4] ) / gza_f;
// This gives longitude at daybreak:
            sub_gen ( monlong, monlong, listb, 27, sun_f );
            lunmanx = monlong[0];
            
// Solar day data: - picks up again here similar to SLC. 

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

// rten 'brel:                  
            tenbrelx = get_tenbrel ( mthnum, tt ); 
            get_bishti_str ( tt ); // These three not in t4.c
            chk_namgo ( tt );

            if ( chad )   // This is for omitted lunar day 
              {
// LINE 1 - Omitted            
                sprintf ( outbuf, "%d. Omitted. %s",
                          tt, dga2T[tt-1] );
// Should include "gza'" for end of lunar day, plus solar longitude.
                fprintf ( fptgt, "%s\n\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "%s\n\n", outbuf );                

                sprintf ( transtr1, "%d X chad X %s",
                          tt, dga2T[tt-1] );
                sprintf ( outbuf, "%s", wordstr8 ); 
                fprintf ( fptgt2, "%s\n", outbuf );                              

// LINE 2 - Omitted

                sprintf ( outbuf,
                    "No: %d;%d,%d %d;%d,%d",
                     gzadag[0], gzadag[1], gzadag[2],
                     nyidag[0], nyidag[1], nyidag[2] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );            
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                            

                if ( tt == 15 || tt == 30 )
                  {
                    do_plans ( bir_spizag - 1 );         
                    sprintf ( outbuf, "Planets (%d):", spizag - 1 );
                    fprintf ( fptgt, "\n%s\n", outbuf );
                    fprintf ( fptgt2, "\n%s\n", outbuf );  
                    prn_plans2 ();                
                    do_rahu ( cur_mth, tt );                     
                    prn_rahu2 (tt);                     
                  }            
            
              }
            else  // If duplicated: 
              {
// NYIDAG is not changed after here, so we can now calculate zodiacal sign:
// Zodiac signs, degrees, minutes -  ZS_s, ZS_d, ZS_m;
                lm2signs ( &ZS_s, &ZS_d, &ZS_m, nyidag ); // Was nyibar
                if ( lhag )  // If this is the first of two weekdays.
                  {
                    // Put here any special calculations for extra day 
                    // At this point, the gzadag, nyidag & monlong are correct for
                    // the second of the two days with same tithi. We need for the
                    // first. Previous tithi ended just before start of day we need.
                    clear_a_b ();
                    listb[0] = 1;
                    for ( i = 1; i < 5; ++i )
                      lista[i] = prv_gzadag[i]; // Previous lunar day
                    sub_gen ( listb, listb, lista, 7, gza_f );
                    l2bcd ( bcdx0, sun_f );
                    mulbcdl ( bcdx0, bcdx0, listb[4] );
                    divbcdl ( bcdx1, bcdx0, gza_f );
                    listb[4] = bcd2l ( bcdx1 ); 
                    add_gen ( fst_monlong, prv_monlong, listb, 27, sun_f );

// This is now the moon longitude at daybreak of first duplicated day

                    fst_lunmanx = fst_monlong[0]; 
                    listb[0] = 0;
                    for ( i = 1; i < 5; ++i )
                      listb[i] = gzadag[i];
                    l2bcd ( bcdx0, sun_f );
                    mulbcdl ( bcdx0, bcdx0, listb[4] );
                    divbcdl ( bcdx1, bcdx0, gza_f );
                    listb[4] = bcd2l ( bcdx1 ); 
                    sub_gen ( cur_monlong, tt_monlong, listb, 27, sun_f );
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
                      {  
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
                prv_lunmanx = prv_monlong[0];

// Now calculate yoga, sbyor ba: -- First of duplicated day 

        add_gen ( fst_sbyorlong, fst_monlong, nyidag, 27, sun_f ); 
        fst_sbyorba = fst_sbyorlong[0];

// Now calculate karana, sbyed pa:  -- not used at present

// This different in SLC and TCGBK

        fst_byedpax = calc_byedpa ( fst_monlong, nyidag );
        juldat = juldat - 1;
        jul2date ( juldat ); // This is normally done by spi_zagf

// Solar day data: - calculating for first of duplicated day 

        sd_animx = ( juldat - 2 ) % 12;
        sd_elemx = ( ( juldat - 3 ) / 2 ) % 5;
        sd_clunmanx = ( juldat - 17 ) % 28;
        sd_smex = ( juldat - 2 ) % 9 + 1; 

// For general and special "'phrod":
        gen_phrod ( fst_lunmanx, doweek, fst_monlong[1] );
// For 'phrod chen/sbyor:
        get_phrochen ( &phrodx, doweek, prv_lunmanx, prv_monlong[1] );
              
               // LINE 1. - First of duplicated day  
                  
                sprintf ( outbuf, "%d: %s. %s. %s",
                          tt, dga2T[tt-1], 
                          dayoweekT[ doweek ], 
                          lunmanT[ fst_lunmanx ] );
                fprintf ( fptgt, "%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "%s\n", outbuf );                

                sprintf ( transtr1, "%d X %s X %s X %s",
                          tt, dga2T[tt-1],
                          dayoweekT[ doweek ],
                          lunmanT[ fst_lunmanx ] );
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); // a b c d                          
                fprintf ( fptgt2, "%s\n", outbuf );  
                
               // LINE 2. - First of duplicated day - e
                                        
                sprintf ( outbuf, "e) %s-%s",
                          fourelemT[ dowkelem[ doweek ] ], // e
                          fourelemT[ lunmanelem[ fst_lunmanx ] ] ); // e
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug ) 
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                                        
                                          
                sprintf ( transtr1, "%s %s",
                          fourelemT[ dowkelem[ doweek ] ], // e
                          fourelemT[ lunmanelem[ fst_lunmanx ] ] ); // e
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "e) %s", wordstr8 ); // e  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
               // LINE 3. - First of duplicated day - f              

                sprintf ( outbuf, "f) %s", phrodchen[phrodx] ); // f
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );    
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                    
                
                sprintf ( transtr1, "%s", phrodchen[phrodx] ); // f
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "f) %s", wordstr8 ); // f  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                  

               // LINE 4. - First of duplicated day - g
                
                sprintf ( outbuf, "g) %s", lmchg );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                str2uni ( lmchgU ); 
                sprintf ( outbuf, "g) %s", wordstr8 ); // i                 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                             

               // LINE 5. - First of duplicated day - h
                
                sprintf ( outbuf, "h) %s", gen_phrod_str );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                
                str2uni ( gen_phrod_str ); 
                sprintf ( outbuf, "h) %s", wordstr8 ); // h
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                            
                
               // LINE 6. - First of duplicated day - i
               
                sprintf ( outbuf, "i) %s", yogaT[fst_sbyorba] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                str2uni ( yogaT[fst_sbyorba] ); 
                sprintf ( outbuf, "i) %s", wordstr8 ); // i 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                                

                // LINE 7. - First of duplicated day - j                              

                sprintf ( outbuf, "j) %s", tenbrelT[tenbrelx] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                str2uni ( tenbrelT[tenbrelx] ); 
                sprintf ( outbuf, "j) %s", wordstr8 ); // j 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf ); 

                // LINE 8. - First of duplicated day - k

                sprintf ( outbuf, "k) %s", bishti_str ); 
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                         
                
                str2uni ( bishti_str2 ); 
                sprintf ( outbuf, "k) %s", wordstr8 ); // k
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                                
                // LINE 9. First of duplicated day - l
            
                sprintf ( outbuf, "l) %s %s %d",
                          cycanimT[ ld_animx ], cycpar[ ld_parx ], ld_smex );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1, "%s X %s X %d",
                          cycanimT[ ld_animx ], cycpar[ ld_parx ], ld_smex );
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "l) %s", wordstr8 );
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                

                // LINE 10. First of duplicated day - m
            
                if ( chk_sadag ( Cmonth, tt ) )
                  {
                    sprintf ( outbuf, "m) %s", sadag_str );
                    fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                    if ( d_bug )
                      fprintf ( fptgt2, "\x20\x20%s\n", outbuf );   
                    str2uni ( sadag_str ); 
                    sprintf ( outbuf, "m) %s", wordstr8 ); // m
                    fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                  }       
                else
                  {
                    fprintf ( fptgt, "\x20\x20m)\n", outbuf );
                    fprintf ( fptgt2, "\x20\x20m)\n", outbuf );
                  }                                            
                
                // LINE 11. First of duplicated day - n, tshes kyi gnam sgo (N) namgo_str
                
                sprintf ( outbuf, "n) %s", namgo_str );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                
                str2uni ( namgo_str ); 
                sprintf ( outbuf, "n) %s", wordstr8 ); // n  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                  

                // LINE 12. First of duplicated day - o
                                
                sprintf ( outbuf, "o) %s-%s",
                            cycelem1T[ sd_elemx ],
                            cycanimT[ sd_animx ] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                sprintf ( transtr1, "%s %s", 
                          cycelem1T[ sd_elemx ],
                          cycanimT[ sd_animx ] );                                
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "o) %s", wordstr8 ); // o 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                
                
                // LINE 13. First of duplictaed day - p
                
                sprintf ( outbuf, "p) %s", ClunmanK[ sd_clunmanx ] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                  
               
                sprintf ( transtr1, "%s", ClunmanK[ sd_clunmanx ] );                                
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "p) %s", wordstr8 ); // o 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                 
                
                // LINE 14. First of duplicated day                
                
                sprintf ( outbuf,
                  "No: %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                       ( prv_gzadag[0] + 1 ) % 7, 60, 0,
                       fst_monlong[0], fst_monlong[1], fst_monlong[2],
                       nyidag[0], nyidag[1], nyidag[2],
                       fst_sbyorlong[0], fst_sbyorlong[1], fst_sbyorlong[2],
                       ZS_s, ZS_d, ZS_m );                       
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1,
                  "%d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d",
                       ( prv_gzadag[0] + 1 ) % 7, 60, 0,
                       fst_monlong[0], fst_monlong[1], fst_monlong[2],
                       nyidag[0], nyidag[1], nyidag[2],
                       fst_sbyorlong[0], fst_sbyorlong[1], fst_sbyorlong[2],
                       ZS_s, ZS_d, ZS_m );                       
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "No: %s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf ); 

                // LINE 15. First of duplicated day
                  
                sprintf ( outbuf, "%02d.%02d.%4d", wd, wm, wy );                          
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1, "%02d X %02d X %4d", wd, wm, wy );                          
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n\n", outbuf );                 

// Now do second of duplicated lunar day 

            juldat = juldat + 1;  // This is readjustment!
            jul2date ( juldat );

// Solar day data:   

            sd_animx = ( juldat - 2 ) % 12;
            sd_elemx = ( ( juldat - 3 ) / 2 ) % 5;
            sd_clunmanx = ( juldat - 17 ) % 28;
            sd_smex = ( juldat - 2 ) % 9 + 1;  
            
// sbyor ba: 

            add_gen ( sbyorlong, cur_monlong, nyidag, 27, sun_f ); 
            sbyorba = sbyorlong[0];

// Now calculate karana, sbyed pa:
            byedpax = calc_byedpa ( cur_monlong, nyidag );
       
            gen_phrod ( lunmanx, doweek, cur_monlong[1] );       
            lm2signs ( &ZS_s, &ZS_d, &ZS_m, nyidag ); // Needs to be done again       
            mak_lmchange_string ( cur_monlong, scd_monlong, doweek );

            // LINE 1. second of duplicated  

            sprintf ( outbuf, "%d: %s. %s",
                      tt, 
                      dayoweekT[ doweek ],
                      lunmanT[ lunmanx ] );
            fprintf ( fptgt, "%s\n", outbuf );
            if ( d_bug )
              fprintf ( fptgt2, "%s\n", outbuf );                

            sprintf ( transtr1, "%d X %s X %s",
                      tt, 
                      dayoweekT[ doweek ],
                      lunmanT[ lunmanx ] );
            str2uni ( transtr1 ); 
            sprintf ( outbuf, "%s", wordstr8 ); // a b c d                          
            fprintf ( fptgt2, "%s\n", outbuf );  
                
            // LINE 2. second of duplicated - e
                
            sprintf ( outbuf, "e) %s-%s",
                      fourelemT[ dowkelem[ doweek ] ], // e
                      fourelemT[ lunmanelem[ lunmanx ] ] ); // e
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );
            if ( d_bug ) 
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                                        
                                      
            sprintf ( transtr1, "%s %s",
                      fourelemT[ dowkelem[ doweek ] ], // e
                      fourelemT[ lunmanelem[ lunmanx ] ] ); // e
            str2uni ( transtr1 ); 
            sprintf ( outbuf, "e) %s", wordstr8 ); // e  
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

            // LINE 3. second of duplicated - f
                
            sprintf ( outbuf, "f) %s", phrodchen[phrodx] ); // f
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );    
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                    
              
            sprintf ( transtr1, "%s", phrodchen[phrodx] ); // f
            str2uni ( transtr1 ); 
            sprintf ( outbuf, "f) %s", wordstr8 ); // f  
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                  
                
            // LINE 4. second of duplicated - g
                
            sprintf ( outbuf, "g) %s", lmchg ); 
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
            str2uni ( lmchgU ); 
            sprintf ( outbuf, "g) %s", wordstr8 ); // i                 
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                             

            // LINE 5. second of duplicated - h

            sprintf ( outbuf, "h) %s", gen_phrod_str );
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                
            str2uni ( gen_phrod_str ); 
            sprintf ( outbuf, "h) %s", wordstr8 ); // h
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                            

            // LINE 6. second of duplicated - i

            sprintf ( outbuf, "i) %s", yogaT[sbyorba] ); 
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

            str2uni ( yogaT[sbyorba] ); 
            sprintf ( outbuf, "i) %s", wordstr8 ); // i 
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                
            // LINE 7. second of duplicated -- o  

            sprintf ( outbuf, "o) %s-%s",
                        cycelem1T[ sd_elemx ],
                        cycanimT[ sd_animx ] );
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                
              
            sprintf ( transtr1, "%s %s", 
                       cycelem1T[ sd_elemx ],
                       cycanimT[ sd_animx ] );                                
            str2uni ( transtr1 ); 
            sprintf ( outbuf, "o) %s", wordstr8 ); // o 
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                

            // LINE 7. second of duplicated -- p 

            sprintf ( outbuf, "p) %s", ClunmanK[ sd_clunmanx ] );
            fprintf ( fptgt, "\x20\x20%s\n", outbuf );
            if ( d_bug )
              fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                  
               
            sprintf ( transtr1, "%s", ClunmanK[ sd_clunmanx ] );                                
            str2uni ( transtr1 ); 
            sprintf ( outbuf, "p) %s", wordstr8 ); // o 
            fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                 

            // LINE 8. Second of duplicated 
                
                sprintf ( outbuf,
                  "No: %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m ); 
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1,
                  "%d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m ); 
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "No: %s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf ); 

            // LINE 8. Second of duplicated 

                sprintf ( outbuf, "%02d.%02d.%4d", wd, wm, wy );                          
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1, "%02d X %02d X %4d", wd, wm, wy );                          
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n\n", outbuf );                 
                
                if ( tt == 15 || tt == 30 )
                  {
                    do_plans ( bir_spizag ); //-- second of duplicated
                    sprintf ( outbuf, "Planets (%d):", spizag );
                    fprintf ( fptgt, "\n%s\n", outbuf );
                    fprintf ( fptgt2, "\n%s\n", outbuf );                    
                    prn_plans2 ();                    
                    do_rahu ( cur_mth, tt );                     
                    prn_rahu2 (tt);                     
                  }
                } // END OF if ( lhag )
              else  // No lhag - normal day 
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

// CALCULATE SBYORBA: -- normal day 

        add_gen ( sbyorlong, cur_monlong, nyidag, 27, sun_f ); 
        sbyorba = sbyorlong[0];     
      
// NOW CALCULATE BYED PA (not currently used):

        byedpax = calc_byedpa ( cur_monlong, nyidag );
                    
        gen_phrod ( lunmanx, doweek, cur_monlong[1] );           
    
// For 'phrod chen/sbyor:
        get_phrochen ( &phrodx, doweek, lunmanx, cur_monlong[1] );
   
                // LINE 1. NORMAL DAY 
                sprintf ( outbuf, "%d: %s. %s. %s. \n  e) %s-%s \n  f) %s",
                          tt, dga2T[tt-1],
                          dayoweekT[ doweek ],
                          lunmanT[ lunmanx ],
                          fourelemT[ dowkelem[ doweek ] ], // e
                          fourelemT[ lunmanelem[ lunmanx ] ], // e
                          phrodchen[phrodx] ); // f
                fprintf ( fptgt, "%s\n", outbuf ); 

                sprintf ( outbuf, "%d: %s. %s. %s",
                          tt, dga2T[tt-1],
                          dayoweekT[ doweek ],
                          lunmanT[ lunmanx ] );
                if ( d_bug )
                  fprintf ( fptgt2, "%s\n", outbuf );
                
                sprintf ( transtr1, "%d X %s X %s X %s",
                          tt, dga2T[tt-1],
                          dayoweekT[ doweek ],
                          lunmanT[ lunmanx ] );
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); // a b c d                          
                fprintf ( fptgt2, "%s\n", outbuf );    
                
                // LINE 2. NORMAL DAY - e                        
                                          
                sprintf ( transtr1, "%s %s",
                          fourelemT[ dowkelem[ doweek ] ], // e
                          fourelemT[ lunmanelem[ lunmanx ] ] ); // e
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "e) %s", wordstr8 ); // e  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                // LINE 3. NORMAL DAY - f              
                
                sprintf ( transtr1, "%s", phrodchen[phrodx] ); // f
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "f) %s", wordstr8 ); // f  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                // LINE 4. NORMAL DAY - g      
                
                sprintf ( outbuf, "g) %s", lmchg );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                str2uni ( lmchgU ); 
                sprintf ( outbuf, "g) %s", wordstr8 ); // i                 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                
                // LINE 5. NORMAL DAY - h                     
                
                sprintf ( outbuf, "h) %s", gen_phrod_str );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                
                str2uni ( gen_phrod_str ); 
                sprintf ( outbuf, "h) %s", wordstr8 ); // h
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                            

                // LINE 6. NORMAL DAY - i              

                sprintf ( outbuf, "i) %s", yogaT[sbyorba] ); 
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                str2uni ( yogaT[sbyorba] ); 
                sprintf ( outbuf, "i) %s", wordstr8 ); // i 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                
                
                // LINE 7. NORMAL DAY - j                              

                sprintf ( outbuf, "j) %s", tenbrelT[tenbrelx] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                str2uni ( tenbrelT[tenbrelx] ); 
                sprintf ( outbuf, "j) %s", wordstr8 ); // j 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf ); 
                
                // LINE 8. NORMAL DAY - k

                sprintf ( outbuf, "k) %s", bishti_str );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                         
                
                str2uni ( bishti_str2 ); 
                sprintf ( outbuf, "k) %s", wordstr8 ); // k
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                 
                // LINE 9. NORMAL DAY - l  
                
                sprintf ( outbuf, "l) %s %s %d",
                          cycanimT[ ld_animx ], cycpar[ ld_parx ], ld_smex );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1, "%s X %s X %d",
                          cycanimT[ ld_animx ], cycpar[ ld_parx ], ld_smex ); 
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "l) %s", wordstr8 );
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                // LINE 10. NORMAL DAY - m
                
                if ( chk_sadag ( Cmonth, tt ) )
                  {
                    sprintf ( outbuf, "m) %s", sadag_str );
                    fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                    if ( d_bug )
                      fprintf ( fptgt2, "\x20\x20%s\n", outbuf );   
                    str2uni ( sadag_str ); 
                    sprintf ( outbuf, "m) %s", wordstr8 ); // m

                    fprintf ( fptgt2, "\x20\x20%s\n", outbuf );
                  }       
                else
                  {
                    fprintf ( fptgt, "\x20\x20m)\n", outbuf );
                    fprintf ( fptgt2, "\x20\x20m)\n", outbuf );
                  }
                 
                // LINE 11. NORMAL DAY - tshes kyi gnam sgo (N)                          

                sprintf ( outbuf, "n) %s", namgo_str );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                
                str2uni ( namgo_str ); 
                sprintf ( outbuf, "n) %s", wordstr8 ); // n  
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
               
                // LINE 12. NORMAL DAY - o
                
                sprintf ( outbuf, "o) %s-%s",
                            cycelem1T[ sd_elemx ],
                            cycanimT[ sd_animx ] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                

                sprintf ( transtr1, "%s %s", 
                          cycelem1T[ sd_elemx ],
                          cycanimT[ sd_animx ] );                                
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "o) %s", wordstr8 ); // o 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                  
                // LINE 13. NORMAL DAY - p                  
                  
                sprintf ( outbuf, "p) %s", ClunmanK[ sd_clunmanx ] );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                                                  
               
                sprintf ( transtr1, "%s", ClunmanK[ sd_clunmanx ] );                                
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "p) %s", wordstr8 ); // o 
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                 
               
                // LINE 14. NORMAL DAY - No              
               
                sprintf ( outbuf,
                  "No: %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m );
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );
                if ( d_bug )
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf );                
                
                sprintf ( transtr1,
                  "%d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d X X %d X %d X %d",
                          gzadag[0], gzadag[1], gzadag[2],
                          cur_monlong[0], cur_monlong[1], cur_monlong[2],
                          nyidag[0], nyidag[1], nyidag[2],
                          sbyorlong[0], sbyorlong[1], sbyorlong[2],
                          ZS_s, ZS_d, ZS_m );                

                str2uni ( transtr1 ); 
                sprintf ( outbuf, "No: %s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n", outbuf ); 

                // LINE 15. NORMAL DAY                               
                
                sprintf ( outbuf, "%02d.%02d.%4d", wd, wm, wy );                          
                fprintf ( fptgt, "\x20\x20%s\n", outbuf );                
                if ( d_bug )
                  fprintf ( fptgt2, "\x20\x20%s\n\n", outbuf );                
                
                sprintf ( transtr1, "%02d X %02d X %4d", wd, wm, wy );                          
                str2uni ( transtr1 ); 
                sprintf ( outbuf, "%s", wordstr8 ); // m
                fprintf ( fptgt2, "\x20\x20%s\n\n", outbuf ); 

                if ( tt == 15 || tt == 30 )
                  {
                    do_plans ( bir_spizag );                
                    sprintf ( outbuf, "Planets (%d):", spizag );
                    fprintf ( fptgt, "\n%s\n", outbuf );
                    fprintf ( fptgt2, "\n%s\n", outbuf );                    
                    prn_plans2 ();                
                    do_rahu ( cur_mth, tt );                     
                    prn_rahu2 (tt);                     
                  }                  
              } // End of normal day, no "lhag"
          } // End of normal or duplicated; ie. not omitted. 

            ++ltt;  
            newmth = 0;
            lastjd = juldat;
          } while ( ltt <= 30 ); // End of day loop 

        newmth = 1;
        if ( !zeromthfg )
          ++tm;
        if ( tm > 12 )
          {
            ++ty;
            tm = 1;
          }
        if ( adj_mth == 12 )
          {
            // First check for following intercalary:
            // Check removed - no longer needed.
              more = 0; // We have finished.
          }
      } while ( more );       // End of month loop

    fclose ( fptgt );
    fclose ( fptgt2 );    
  } // END - prn_cal2 ()
