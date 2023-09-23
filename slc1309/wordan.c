/*********************************************************************************
Licence for Tibw - Tibetan printing software

Copyright (c) 1985-2013 Edward Henning

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

/**** Main Parsing routines ****/
/** Queue handling, file reading **/

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <dos.h>
#include "wordan.h"
#include "wordan.ext"
#include "tc.h"
#include "tc.ext"
#include "tibform.ids"

int  chradv;  /** For specifying jump over a character **/

struct skt_map
  {
    int   chr;
    int   id;
  };

// Could lose some of these now:
struct skt_map s_mp[26] = { 129,  ID_SKT_A,          /** Long A **/
                            131,  ID_SKT_I,          /** Long I **/
                            133,  ID_SKT_U,          /** Long U **/
                            135,  ID_SKT_R,          /** RI     **/
                            137,  ID_SKT_LR,         /** Long RI **/
                            139,  ID_SKT_LI,         /** LI     **/
                            141,  ID_SKT_LLI,        /** Long LI **/
                            143,  ID_NOR_NG,         /** N dot over **/
                            145,  ID_NOR_TR,         /** T dot under **/
                            147,  ID_NOR_DR,         /** D dot under **/
                            149,  ID_NOR_NR,         /** N dot under **/
                            151,  ID_SKT_M,          /** M dot under **/
                            153,  ID_NOR_SH,         /** S slash **/
                            157,  ID_SKT_SZH,        /** S dot under **/
                            160,  ID_SKT_H,          /** H dot under **/
                            163,  ID_SKT_A,          /** NEW == Long A **/
                            164,  ID_NOR_NY,         /** N tilda **/
                            166,  ID_SKT_LLI,        /** NEW == Long LI **/
                            168,  ID_NOR_NG,         /** NEW == N dot over **/
                            170,  ID_SKT_SZH,        /** NEW == S dot under **/
                            'v',  ID_SKT_V,          /** V **/
                            'f',  ID_SKT_FO,
                            'q',  ID_SKT_Q,
                              0,  0 };

struct wrd_struct nwd_qu[12];
int     nqu_ptr;      /*** Pointer into word queue ***/
struct wrd_struct twd_qu[12];

char *numStr[10];


/** ROUTINE TO GET SANSKRIT ID OF SINGLE CHARACTER **/
/** RETURNS 0 IF NOT KNOWN **/
/** This is used for specific recognition only **/
int getsktid ( char c ) /** SKT **/
  {
    int i, j;
    i = 0;
    j = 0;
    while ( s_mp[i].chr != 0 )
      {
         if ( s_mp[i].chr == c )
           j = s_mp[i].id;
         ++i;
      }
    return ( j );
  }

/*** ROUTINE TO ROUGH CHECK TEST WORD FOR SANSKRIT ***/
/*** RETURNS ZERO IF NOT FOUND ***/
int chk4skt ( void ) /** SKT **/
  {
    int i, j;
    i = 0;
    if ( strncmp ( wrd_tst, "hra", 3 ) == 0 )
      return ( 1 );
    if ( strncmp ( wrd_tst, "hwa", 3 ) == 0 )
      return ( 1 );
    while ( wrd_tst[i] != '\0' )
      {
        if ( j = getsktid ( wrd_tst[i] ) )
          return ( j );
        else
          ++i;
      }
    switch ( wrd_tst[0] )
      {
        case 'g':
          if ( wrd_tst[1] == 'h' )
            return ( 1 );
          else if ( wrd_tst[1] == 'g' && wrd_tst[3] == '\0' )
            return ( 1 );
          else if ( wrd_tst[1] == 'h' && wrd_tst[2] == 'r' )
            return ( 1 ); break;
        case 'c':
          if ( wrd_tst[1] == 'y' )
            return ( 1 ); break;
        case 'j':
          if ( wrd_tst[1] == 'h' )
            return ( 1 ); break;
        case 't':
          if ( wrd_tst[1] == 'y' )
            return ( 1 );
          else if ( wrd_tst[1] == 's' && wrd_tst[2] == 'y' )  // cya, tsy
            return ( 1 );
          else if ( wrd_tst[1] == 'v' )
            return ( 1 );
          else if ( wrd_tst[1] == 'w' )
            return ( 1 );
          else if ( wrd_tst[1] == 'r' && wrd_tst[2] == 'w' )
            return ( 1 ); break;
        case 'd':
          if ( wrd_tst[1] == 'h' )
            return ( 1 );
          else if ( wrd_tst[1] == 'd' && wrd_tst[3] == '\0' )
            return ( 1 ); break;
        case 'n':
          if ( wrd_tst[1] == 46 ) /* n. */
            return ( 1 ); break;
        case 'p':
          if ( wrd_tst[1] == 'h' && wrd_tst[2] == 'y' // phywa - NEW 11/5/2001
                                 && wrd_tst[3] == 'w' ) // Needs to be treated as
                                                        // Sanskrit
            return ( 1 ); break;
        case 'b':
          if ( wrd_tst[1] == 'h' )
            return ( 1 );
          else if ( wrd_tst[1] == 'b' && wrd_tst[3] == '\0' )
            return ( 1 ); break;
        case 'm':
          if ( wrd_tst[1] == 'm' )
            return ( 1 ); break;
        case 'w':
          if ( wrd_tst[1] == 'w' )
            return ( 1 ); break;
        case 'y':
          if ( wrd_tst[1] == 'y' )
            return ( 1 ); break;
        case 'r':
          if ( wrd_tst[1] == 'r' )
            return ( 1 );
          else if ( wrd_tst[1] == 'd' && wrd_tst[2] == 'h' )
            return ( 1 ); break;
        case 's':
          if ( wrd_tst[1] == 's' )
            return ( 1 );
          else if ( wrd_tst[1] == 'y' && wrd_tst[2] == 'e' )
            return ( 1 ); break;
        case 'h':
          if ( wrd_tst[1] == 'y' )
            return ( 1 ); break;
        default: ; break;
      }
    i = 0;
    while ( wrd_tst[i+1] != '\0' )
/**** THIS NEEDS LOOKING AT ****/
      {
        if ( wrd_tst[i] == 'a' && wrd_tst[i+1] == 'i' )
          return ( 1 );
        else if ( wrd_tst[i] == 'a' && wrd_tst[i+1] == 'u' )
          return ( 1 );
        else
          ++i;
      }
    return ( 0 );
  } // END - chk4skt ()

/** Routine to check an ID for being a vowel **/
/** Returns 1 if it is, 0 if not **/
int issktvwl ( int id ) /** SKT **/
  {
    if ( id == ID_NOR_I || id == ID_NOR_U || id == ID_NOR_E || id == ID_NOR_O
         || id == ID_NOR_SA || id == ID_SKT_AU || id == ID_SKT_AI || id ==
         ID_SKT_A || id == ID_SKT_I || id == ID_SKT_U || id == ID_SKT_R
         || id == ID_SKT_LR || id == ID_SKT_LI || id == ID_SKT_LLI )
      return ( 1 );
    else
      return ( 0 );
  }

/** Routine to determine general character ID **/
/** Translates internal ids **/
int detsktid ( char *s ) /** SKT **/
  {
    int j;
    chradv = 1;
    switch ( *s )
      {
//      case 39: j = ID_NOR_A; break; // CHANGED 14-5-94
        case 39: j = ID_SKT_AVA; break; // Avagraha, or, stop before 'o
        case 46: j = ID_STOP; break;
/*** First, the diacritics ***/
        case 163: j = ID_SKT_A; break;           /** Long A **/
        case 131: j = ID_SKT_I; break;           /** Long I **/
        case 133: j = ID_SKT_U; break;           /** Long U **/
        case 135:                                /** RI   **/
          if ( *(s+1) == 'i' )
            {
              j = ID_SKT_R;
              chradv = 2;
            }
          else if ( *(s+1) == 131 )   // LONG I
            {
              j = ID_SKT_LR;
              chradv = 2;
            }
          else
            j = ID_SKT_R;
          break;
        case 137:                     // Long RI
          if ( *(s+1) == 'i' || *(s+1) == 131 )  // LONG I
            chradv = 2;
          j = ID_SKT_LR;
          break;
        case 139:                    // LI
          if ( *(s+1) == 'i' )
            {
              j = ID_SKT_LI;
              chradv = 2;
            }
          else if ( *(s+1) == 131 )  // LONG I
            {
              j = ID_SKT_LLI;
              chradv = 2;
            }
          else
            j = ID_SKT_LI;
          break;
        case 166:
          if ( *(s+1) == 'i' || *(s+1) == 131 )  // LONG I
            chradv = 2;
          j = ID_SKT_LLI;
          break;
        case 168: j = ID_NOR_NG; break;          // N dot over
        case 145:
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_TRH;                    // TH dot under
              chradv = 2;
            }
          else
            j = ID_NOR_TR;                       // T dot under
          break;
        case 147:
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_DRH;                    // DH dot under
              chradv = 2;
            }
          else
            j = ID_NOR_DR;                       // D dot under
          break;
        case 149: j = ID_NOR_NR; break;          // N dot under
        case 151: j = ID_SKT_M; break;           // M dot under
        case 153: j = ID_NOR_SH; break;          // S slash
        case 170: j = ID_SKT_SZH; break;         // S dot under
        case 160:
          if ( *(s+1) == 'p' ) // Hp
            {
              j = ID_SKT_FO;
              chradv = 2;
            }
          else if ( *(s+1) == 'k' ) // Hk
            {
              j = ID_SKT_Q;
              chradv = 2;
            }
          else
            j = ID_SKT_H;
          break;           /** H dot under **/
// ADD HERE OTHER ASPIRATES.
        case 241: // NEW - standard "ñ"
        case 164: j = ID_NOR_NY; break;          /** N tilda **/
/*** Now, the 'normal' characters ***/
        case 'k':
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_KH;
              chradv = 2;
            }
          else if ( *(s+1) == 157 ) /** KZHA **/
            {
              j = ID_SKT_KZH;
              chradv = 2;
            }
          else
            j = ID_NOR_K;
          break;
        case 'g':
          if ( *(s+1) == 'h' )
            {
              j = ID_SKT_GH;
              chradv = 2;
            }
          else
            j = ID_NOR_G;
          break;
        case 'c':
          if ( *(s+1) == 'h' )                  /** CH **/
            {
              j = ID_NOR_TSH;
              chradv = 2;
            }
          else                                  /** C **/
            j = ID_NOR_TS;
          break;
        case 'j':
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_DZH;
              chradv = 2;
            }
          else
            j = ID_NOR_DZ;
          break;
        case 'd':
          if ( *(s+1) == 'z' && *(s+2) == 'h' )
            {
              j = ID_NOR_DZH;
              chradv = 3;
            }
          else if ( *(s+1) == 'z' )
            {
              j = ID_NOR_DZ;
              chradv = 2;
            }
          else if ( *(s+1) == 'h' )
            {
              j = ID_SKT_DH;
              chradv = 2;
            }
          else
            j = ID_NOR_D;
          break;
        case 't':
          if ( *(s+1) == 's' && *(s+2) == 'h' ) /** CH **/
            {
              j = ID_NOR_TSH;
              chradv = 3;
            }
          else if ( *(s+1) == 's' )             /** C **/
            {
              j = ID_NOR_TS;
              chradv = 2;
            }
          else if ( *(s+1) == 'h' )             /** TH **/
            {
              j = ID_SKT_TH;
              chradv = 2;
            }
          else
            j = ID_NOR_T;
          break;
        case 'n': j = ID_NOR_N; break;
        case 'p':
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_PH;
              chradv = 2;
            }
          else
            j = ID_NOR_P;
          break;
        case 'b':
          if ( *(s+1) == 'h' )
            {
              j = ID_SKT_BH;
              chradv = 2;
            }
          else
            j = ID_NOR_B;
          break;
        case 's':
          if ( *(s+1) == 'h' )
            {
              j = ID_NOR_SH;  /** S slash - this is in addition to the
                                  diacritic **/
              chradv = 2;
            }
          else
            j = ID_NOR_S;
          break;
        case 'm': j = ID_NOR_M; break;
        case 'y': j = ID_NOR_Y; break;
        case 'r': j = ID_NOR_R; break;
        case 'l': j = ID_NOR_L; break;
        case 'v': j = ID_SKT_V; break;
        case 'w': j = ID_SKT_V; break;  /** Is this a good idea? **/
        case 'f': j = ID_SKT_FO; break;  // Hp
        case 'q': j = ID_SKT_Q; break;   // Hk
        case 'h':
          if ( *(s+1) == 'r' )
            {
              j = ID_SKT_HR;
              chradv = 2;
            }
          else
            j = ID_NOR_H;
          break;
/*** Now, the other vowels ***/
        case 'a':
          if ( *(s+1) == 'i' )
            {
              j = ID_SKT_AI;
              chradv = 2;
            }
          else if ( *(s+1) == 'u' )
            {
              j = ID_SKT_AU;
              chradv = 2;
            }
          else
            j = ID_NOR_SA;
          break;
        case 'i': j = ID_NOR_I; break;
        case 'u': j = ID_NOR_U; break;
        case 'e': j = ID_NOR_E; break;
        case 'o': j = ID_NOR_O; break;
        default: j = 0; break;
      }
    return ( j );
  } // END - detsktid ()

/** MAIN ROUTINE TO ANALYSE SANSKRIT WORD **/
void an_skt ( void ) /** SKT **/
  {
    int i, c, more, vwltst;
    int src_ptr, tgt_ptr;  // Pointers into test word and target array
    int hld_ptr;  // Pointer to start of current syllable in target
    lng = 2;  // Indicates Sanskrit
    for ( i = 0; i < 16; ++i ) // Clear the Sanskrit buffer
      sktstr[i] = 0;

    src_ptr = 0;
    tgt_ptr = 0;
    while ( wrd_tst[src_ptr] != '\0' ) // ONE SYLLABLE AT A TIME
      {
        hld_ptr = tgt_ptr;
        ++tgt_ptr;
        more = 1;
        do     // ONE CHARACTER AT A TIME
          {
            c = detsktid ( wrd_tst + src_ptr );
            src_ptr += chradv;   /** How far to skip in test word **/
            ++sktstr[hld_ptr];   /** Indicates another character added **/
            sktstr[tgt_ptr++] = c;
            vwltst = issktvwl ( c );
            if ( c == ID_SKT_AVA )
              {
                if ( *( wrd_tst + src_ptr ) == 'h' )
                  vwltst = ID_NOR_SA;
              }
/*** TEST HERE FOR VISARGA AFTER A CONSONANT ***/
            if ( c == ID_SKT_H && sktstr[hld_ptr] == 1 ) // IT'S THE FIRST
              more = 0;
            else if ( wrd_tst[src_ptr] == 160 /* SKT_H */ && !vwltst )
              {  /* ONLY KNOWN OCCURENCE IS IN PHATH */
                ++sktstr[hld_ptr]; /** Indicates another character added **/
                sktstr[tgt_ptr++] = ID_NOR_SA;
                more = 0;  // END THIS SYLLABLE
              }
            else if ( wrd_tst[src_ptr] == '\0' || vwltst )
              {
              // WE HAVE TO TEST FOR THE POSSIBILITY THAT THE NEXT
              // CHARACTER IS AN ANUSVARA
                c = detsktid ( wrd_tst + src_ptr );
                if ( vwltst && c == ID_SKT_M ) /* M dot */
                  {
                    ++sktstr[hld_ptr]; /* Indicates character added */
                    sktstr[tgt_ptr++] = c;
                    ++src_ptr;
                  }
                more = 0;  // END THIS SYLLABLE
              }
          } while ( more );
      }
    i = 0;
  } // END - an_skt ()

/**** ROUTINE TO CHECK BACKWARDS FOR DOUBLE TRANSCRIBED LETTER ****/
int bck_dc ( char *str )
  {
    int  vm1;
    vm1 = 0;
    vm1stp = 0;
    if ( *str == 'h' && *(str - 1) == 'k' )
      {
        vm1 = ID_NOR_KH;
        vm1stp = 2;
      }
    else if ( *str == 'g' && *(str - 1) == 'n' )
      {
        vm1 = ID_NOR_NG;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 'c' )
      {
        vm1 = ID_NOR_CH;
        vm1stp = 2;
      }
    else if ( *str == 'y' && *(str - 1) == 'n' )
      {
        vm1 = ID_NOR_NY;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 't' )
      {
        vm1 = ID_NOR_TH;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 'p' )
      {
        vm1 = ID_NOR_PH;
        vm1stp = 2;
      }
    else if ( *str == 's' && *(str - 1) == 't' )
      {
        vm1 = ID_NOR_TS;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 's' && *(str - 2 ) == 't' )
      {
        vm1 = ID_NOR_TSH;
        vm1stp = 3;
      }
    else if ( *str == 'z' && *(str - 1) == 'd' )
      {
        vm1 = ID_NOR_DZ;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 'z' )
      {
        vm1 = ID_NOR_ZH;
        vm1stp = 2;
      }
    else if ( *str == 'h' && *(str - 1) == 's' )
      {
        vm1 = ID_NOR_SH;
        vm1stp = 2;
      }
    return ( vm1 );
  }

/**** ROUTINE TO DETERMINE A SINGLE TRANSCRIBED CONSONANT ****/
int bck_sc ( char chr )
  {
    int    j, hit;
    j = -1;
    hit = 0;
    switch ( chr )
      {
        case 'k': j = ID_NOR_K; break;
        case 'g': j = ID_NOR_G; break;
        case 'c': j = ID_NOR_C; break;
        case 'j': j = ID_NOR_J; break;
        case 't': j = ID_NOR_T; break;
        case 'd': j = ID_NOR_D; break;
        case 'n': j = ID_NOR_N; break;
        case 'p': j = ID_NOR_P; break;
        case 'b': j = ID_NOR_B; break;
        case 'm': j = ID_NOR_M; break;
        case 'w': j = ID_NOR_W; break;
        case 'z': j = ID_NOR_Z; break;
        case  39: j = ID_NOR_A; break;
        case 'y': j = ID_NOR_Y; break;
        case 'r': j = ID_NOR_R; break;
        case 'l': j = ID_NOR_L; break;
        case 's': j = ID_NOR_S; break;
        case 'h': j = ID_NOR_H; break;
        case 'a': j = ID_NOR_SA; break;
        default: j = 0; break;
      }
    return ( j );
  }

/**** ROUTINE TO ANALYSE A VOWEL ****/
int chk_vw ( char chrct )
  {
    if ( chrct == 'a' )
      return ( ID_NOR_A );
    else if ( chrct == 'i' )
      return ( ID_NOR_I );
    else if ( chrct == 'u' )
      return ( ID_NOR_U );
    else if ( chrct == 'e' )
      return ( ID_NOR_E );
    else if ( chrct == 'o' )
      return ( ID_NOR_O );
    else if ( chrct == '\0' )
      return ( ID_NOR_A );
    return ( ID_NOR_A );
  }

/**** ROUTINE TO ANALYSE FLAG WORD IN TEST BUFFER ****/
int spec_an ( void )
  {
    if ( wrd_tst[1] == 'N' || wrd_tst[1] == 'n' )
      {
        flgs = 1;     /** New line **/
        return ( 1 );
      }
    else if ( wrd_tst[1] == 'S' || wrd_tst[1] == 's' )
      {
        flgs = 2;  /** Toggle between font sizes - NEEDED ?? **/
        if ( sml_fnt == 0 )
          sml_fnt = 1;
        else
          sml_fnt = 0;
        return ( 0 );
      }
    else if ( wrd_tst[1] == 'P' || wrd_tst[1] == 'p' )
      {
        flgs = 16;    // New page
        return ( 1 );
      }
    else if ( wrd_tst[1] == 'T' && wrd_tst[2] == 'S' )
      {
        return ( 2 );   /** Side bar title text **/
      }
    else if ( wrd_tst[1] == 'T' && wrd_tst[2] == 'P' )
      {
        flgs = 4;  /** Title page **/
        return ( 1 );
      }
    return ( 0 );  // NEW - TO FIX COMPILER WARNING
  }

/**** MAIN ROUTINE TO ANALYSE WORD IN TEST BUFFER ****/
int word_an ( void )
  {
    int   i, vwlpstn;
    int   vm1, errhit;
    lng = 0; flgs = 0;
    prf = 0; sup = 0; rot = 0; sub = 0; sub2 = 0;
    vw1 = 0; vw2 = 0; vw3 = 0; suf1 = 0; suf2 = 0; shd = 0;
    errhit = 0;
    if ( wrd_tst[0] == '$' )
      {
        if ( wrd_tst[1] == 'X' || wrd_tst[1] == 'x' ) // NEW - 15/11/2003
          {
            rot = ID_SWIDSH; //  148 sbrul shad/"twiddly" shad
            goto an_end;
          }
        return ( spec_an () );
      }
    fixskt ();
    if ( chk4skt () ) /** Do rough check for Sanskrit, to save time **/
      {
        an_skt ();
        return ( 1 );
      }
// First, deal with any special difficult cases:
    if ( strcmp ( wrd_tst, "'ang'o" ) == 0 )
      {
        vw1 = ID_NOR_A;
        suf1 = ID_NOR_NG;
        vw2 = ID_NOR_O;
        lng = 1;          // Indicates Tibetan word
        return ( 1 );
      }
/**** FIND FIRST VOWEL IN SYLLABLE ****/
    i = -1;
    vwlpstn = -1;
    do
      {
        ++i;
        if ( wrd_tst[i] == 'a' || wrd_tst[i] == 'i' || wrd_tst[i] == 'u'
          || wrd_tst[i] == 'e' || wrd_tst[i] == 'o' )
        vwlpstn = i; /** Mark vowel position in string **/
      } while ( wrd_tst[i + 1] != '\0' && vwlpstn == -1 );

   if ( vwlpstn == -1 )
      {
        printf ("\nPARSING WORD: %s\n", wrd_tst );
        printf ( "ERROR - VOWEL NOT FOUND!\n" );
        getch ();
      }
    else
      vw1 = chk_vw ( wrd_tst[i] );  /** Determine ID for vowel **/
    if ( vwlpstn == 0 )
      {
        rot = ID_NOR_SA;
        goto get_suf;    /**** Jump to determine any suffix ****/
      }
    /**** DETERMINE PREVIOUS CONSONANT ****/
    /**** CHECK FOR DOUBLE TRANSCRIBED ****/
    i = vwlpstn - 1;
    vm1 = bck_dc ( wrd_tst + i ); /** Check for double **/
/**** NONE OF THESE CAN BE SUBJOINED, SO:- ****/
    if ( vm1stp ) /** Set to 2 or 3 depending on double, eg. kh, tsh **/
      {
        rot = vm1;
      }
/**** CHECK FOR POSSIBLE SUBJOINS ****/
    else  /** i points to character immediately before vowel **/
      {
        if ( ( wrd_tst[i] == 'y' || wrd_tst[i] == 'r' || wrd_tst[i] == 'l'
          || wrd_tst[i] == 'w' || wrd_tst[i] == 'h' ) && i > 0 )
          {
            if ( wrd_tst[i] == 'y' )
              {
                if ( wrd_tst[i-1] != 46 )
                  sub = ID_NOR_Y;
                else if ( wrd_tst[i-1] == 't' )
                  sub = 0;
              }
            else if ( wrd_tst[i] == 'r' )
              sub = ID_NOR_R;
            else if ( wrd_tst[i] == 'l' )
              sub = ID_NOR_L;
            else if ( wrd_tst[i] == 'h' )
              sub = ID_NOR_H;
            else if ( wrd_tst[i] == 'w' )
              sub = ID_NOR_W;
            rot = 0;
          }
        if ( !sub )
/**** DETERMINE ROOT ****/
          rot = bck_sc ( wrd_tst[i] ); /** Determines a single consonant **/
        vm1stp = 1;
        if ( sub == ID_NOR_W && wrd_tst[i-1] == 'r' )
          {
            if ( i == 1 )
              {
                rot = ID_NOR_R;
                vm1stp = 1;
              }
            else
              {
                sub = ID_NOR_R;
                sub2 = ID_NOR_W;
                vm1stp = 2;
              }
          }
        if ( sub == ID_NOR_H && wrd_tst[i-1] == 'l' )
          {    /*** This might be a bit flaky ***/
            rot = ID_NOR_H;
            sup = ID_NOR_L;
            sub = 0;
            vm1stp = 1;
          }
      }
/**** We arrive here either with the root or subjoin determined, and, only ONE of these ****/
/**** POINT TO START OF DETERMINED CHARACTER ****/
    i = vwlpstn - vm1stp;
    if ( i == 0 )
      goto get_suf;      /**** Jump to determine any suffix ****/
/**** IF SUBJOIN, DETERMINE ROOT ****/
    if ( sub )
      {
        --i;
        rot = bck_dc ( wrd_tst + i );
        if ( rot == 0 )
          rot = bck_sc ( wrd_tst[i] );
        else
          i = i + 1 - vm1stp;

        if ( i == 0 )    /**** No prefix or superjoin ****/
          goto get_suf;
      }
/**** IF ROOT, DETERMINE SUPERJOIN ****/
    --i;
    if ( wrd_tst[i] == 'r' )
      sup = ID_NOR_R;
    else if ( wrd_tst[i] == 's' )
      sup = ID_NOR_S;
    else if ( wrd_tst[i] == 'l' )
      sup = ID_NOR_L;
    if ( sup && i == 0 )
      goto get_suf;
    if ( sup )
      --i;
/****  DETERMINE PREFIX ****/
    if ( wrd_tst[i] == 'g' )
      prf = ID_NOR_G;
    else if ( wrd_tst[i] == 'd' )
      prf = ID_NOR_D;
    else if ( wrd_tst[i] == 'b' )
      prf = ID_NOR_B;
    else if ( wrd_tst[i] == 'm' )
      prf = ID_NOR_M;
    else if ( wrd_tst[i] == 39 )
      prf = ID_NOR_A;
    else if ( wrd_tst[i-1] == 'g' && wrd_tst[i] == 46 ) /** 'g.' **/
      {
        prf = ID_NOR_G;
        rot = ID_NOR_Y;
        sub = 0;
      }
    if ( !prf )
      {
        errhit = 1;
      }
/**** DETERMINE SUFFIX, IF ANY ****/
    get_suf: i = vwlpstn + 1;
    if ( wrd_tst[i] == '\0' )
      goto an_end;
    if ( wrd_tst[i] == 39 )
    /** We have a second vowel **/
      {
        ++i;
        vw2 = chk_vw ( wrd_tst[i] );
        ++i;
        if ( wrd_tst[i] == '\0' )
          goto an_end;
        if ( wrd_tst[i] == 39 )
        /** We have a third vowel **/
          {
            ++i;
            vw3 = chk_vw ( wrd_tst[i] );
            goto an_end;
          }
      }
    if ( wrd_tst[i] == 'n' && wrd_tst[i + 1] == 'g' )
      {
        suf1 = ID_NOR_NG;
        i += 2;
      }
    else
      {
        suf1 = bck_sc ( wrd_tst[i] );
        ++i;
      }
    if ( wrd_tst[i] == '\0' )
      goto an_end;
    if ( wrd_tst[i] == 's' )
      suf2 = ID_NOR_S;
    else
      {
        errhit = 1;
      }
    ++i;
    if ( wrd_tst[i] != '\0' )
      {
        errhit = 1;
      }
    an_end:
    ++i;
    if ( errhit )
      an_skt ();
    else
      lng = 1;  /** Indicates Tibetan word **/
    return ( 1 );
  }   /** END word_an **/

void str2uni ( char *src )
  {
    int i, j, k, x, more, shad;
    int needtseg = 0;

    wordstr8[0] = 0; // Target string for output
    if ( *src == '\0' )    
      return;
    
    i = 0;
    j = 0;
    k = 0;
    more = 1;
    do
      {
// Get one word at a time:
        i = 0; shad = 0;
        while ( *( src + j ) == 32 )
          ++j;
        while ( *( src + j ) != 32 && *( src + j ) != '/' && *( src + j ) != 0 )
          {
            wrd_tst[i++] = *( src + j++ );
          }
        wrd_tst[i] = 0;
        if ( wrd_tst[0] >= '0' && wrd_tst[0] <= '9' )
          {
            num2uni (); // What about tseg ???
            needtseg = 0;
          }
        else if ( strcmp ( wrd_tst, "X" ) == 0 )
          {
            uni2utf8 ( 32 );
            add2wrdstr ();
          }
        else
          {
            x = word_an ();
            conv2uni (); // Adds word to wordstr8
            needtseg = 1;
            while ( *( src + j ) == 32 )
              ++j;
            if ( *( src + j ) >= '0' && *( src + j ) <= '9' ||
                 *( src + j ) == 'X' )
              needtseg = 0;
          }
            while ( *( src + j ) == 32 )
              ++j;
            // Now check for shad:
            if ( *( src + j ) == '/' )
              {
                j++;
                shad = 1;
                while ( *( src + j ) == 32 )
                  ++j;
                if ( *( src + j ) == '/' )
                  {
                    j++;
                    shad = 2;
                  }
                if ( shad == 1 )
                  {
                    uni2utf8 ( 0x0f0d );
                    add2wrdstr ( );
                    uni2utf8 ( 32 );
                    add2wrdstr ();
                  }
                if ( shad == 2 )
                  {
//                    uni2utf8 ( 0x0f0e );
//                    add2wrdstr ( );
                    uni2utf8 ( 0x0f0d );
                    add2wrdstr ( );
                    uni2utf8 ( 32 );
                    add2wrdstr ();
                    uni2utf8 ( 0x0f0d );
                    add2wrdstr ( );
                  }
              } // END - if ( *( src + j ) == '/' )
            if ( needtseg )
              {
                addtseg ( );
                needtseg = 0;
              }
        if ( *( src + j ) == 0 ) // Really need this twice???
          more = 0;
      } while ( more );
  } // END - str2uni ()

void conv2uni ( void ) // This should do the whole word
  {
    int x, d_bug;
    d_bug = 0;
    if ( d_bug )
      {
//        printf ("\nWord   = %s\n", wrd_tst );
/**        printf ("Prefix = %s\n", chrs[prf] );
        printf ("Super  = %s\n", chrs[sup] );
        printf ("Root   = %s\n", chrs[rot] );
        printf ("Sub    = %s\n", chrs[sub] );
        printf ("Sub2   = %s\n", chrs[sub2] );
        printf ("Vowel1 = %s\n", chrs[vw1] );
        printf ("Vowel2 = %s\n", chrs[vw2] );
        printf ("Vowel3 = %s\n", chrs[vw3] );
        printf ("Suf1   = %s\n", chrs[suf1] );
        printf ("Suf2   = %s\n", chrs[suf2] ); **/
        if ( lng == 1 )
          printf ("Language = Tibetan\n" );
        else if ( lng == 2 )
          printf ("Language = Sanskrit\n" );
        else
          printf ("Language UNKNOWN\n" ); 
      }
    if ( lng == 2 )
      {
        skt2uni ();
        return;
      }
    if ( prf )
      {
        x = convnor ( prf );
        uni2utf8 ( x );
//        printf ( "Prefix = %d\n", prf );
//        printf ( "Prefix uni = %d\n", x );
//        printf ( "Char str 0: %d\n", utf8char[0] );
//        printf ( "Char str 1: %d\n", utf8char[1] );
//        printf ( "Char str 2: %d\n", utf8char[2] );
//        printf ( "Char str: %s\n", utf8char );
//        strcpy ( utf8str, utf8char );
        add2wrdstr ();
      }
    if ( sup )
      {
        x = convnor ( sup );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convsub ( rot );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    else
      {
        x = convnor ( rot );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    if ( sub )
      {
        x = convsub ( sub );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    if ( vw1 )
      {
        x = convvow ( vw1 );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    if ( suf1 )
      {
        x = convnor ( suf1 );
        uni2utf8 ( x );
        add2wrdstr ();
        if ( suf2 )
          {
            x = convnor ( suf2 );
            uni2utf8 ( x );
            add2wrdstr ();
          }
      }
    else if ( vw2 )
      {
        x = convnor ( ID_NOR_A );
        uni2utf8 ( x );
        add2wrdstr ();
        if ( vw2 )
          {
            x = convvow ( vw2 );
            uni2utf8 ( x );
            add2wrdstr ();
          }
        if ( vw3 )
          {
            x = convnor ( ID_NOR_A );
            uni2utf8 ( x );
            add2wrdstr ();
            if ( vw3 )
              {
                x = convvow ( vw3 );
                uni2utf8 ( x );
                add2wrdstr ();
              }
          }
      }
  } // END - conv2uni ()

void addtseg ( void )
  { // 0f0b
    uni2utf8 ( 0x0f0b );
    add2wrdstr ( );
  }

void uni2utf8 ( int x )
  {
    unsigned char a, b, c, d, e;
    a = b = c = d = e = 0;
    if ( x <= 0x7f )
      {
        a = (unsigned int) x;
        utf8char[0] = a;
        utf8char[1] = 0;
        return;
      }
    else if ( x > 0x7f && x <= 0x07ff )
      { // 110xxxxx 10xxxxxx
      ;
      }
    else if ( x > 0x7ff && x <= 0xffff )
      { // 1110xxxx 10xxxxxx 10xxxxxx
        // 11100000 = 224
        // 10000000 = 128
        // 00001111 = 15
        // 00111111 = 63
        c = 128 + ( 63 & x );
        x = x >> 6;
        b = 128 + ( 63 & x );
        x = x >> 6;
        a = 224 + ( 15 & x );
        utf8char[0] = a;
        utf8char[1] = b;
        utf8char[2] = c;
        utf8char[3] = 0;
      }
  }

int convnor ( int x )
  {
    int j = 0;
    switch ( x )
      {
        case ID_NOR_K:  j = 0x0f40; break;
        case ID_NOR_KH: j = 0x0f41; break;
        case ID_NOR_G:   j = 0x0f42; break;
        case ID_NOR_GH:   j = 0x0f43; break;
        case ID_NOR_NG:   j = 0x0f44; break;
        case ID_NOR_C:   j = 0x0f45; break;
        case ID_NOR_CH:   j = 0x0f46; break;
        case ID_NOR_J:   j = 0x0f47; break;
        case ID_NOR_NY:  j = 0x0f49; break;
        case ID_NOR_TR:  j = 0x0f4a; break;
        case ID_NOR_TRH: j = 0x0f4b; break;
        case ID_NOR_DR:  j = 0x0f4c; break;
        case ID_NOR_DRH: j = 0x0f4d; break;
        case ID_NOR_NR:  j = 0x0f4e; break;
        case ID_NOR_T:   j = 0x0f4f; break;
        case ID_NOR_TH:  j = 0x0f50; break;
        case ID_NOR_D:   j = 0x0f51; break;
        case ID_NOR_DH:  j = 0x0f52; break;
        case ID_NOR_N:   j = 0x0f53; break;
        case ID_NOR_P:   j = 0x0f54; break;
        case ID_NOR_PH:  j = 0x0f55; break;
        case ID_NOR_B:  j = 0x0f56; break;
        case ID_NOR_BH:  j = 0x0f57; break;
        case ID_NOR_M:   j = 0x0f58; break;
        case ID_NOR_TS:  j = 0x0f59; break;
        case ID_NOR_TSH: j = 0x0f5a; break;
        case ID_NOR_DZ:  j = 0x0f5b; break;
        case ID_NOR_DZH: j = 0x0f5c; break;
        case ID_NOR_W:   j = 0x0f5d; break;
        case ID_NOR_ZH:  j = 0x0f5e; break;
        case ID_NOR_Z:   j = 0x0f5f; break;
        case ID_NOR_A:   j = 0x0f60; break;
        case ID_NOR_Y:   j = 0x0f61; break;
        case ID_NOR_R:   j = 0x0f62; break;
        case ID_NOR_L:   j = 0x0f63; break;
        case ID_NOR_SH:  j = 0x0f64; break;
        case ID_SKT_SZH: j = 0x0f65; break;
        case ID_NOR_S:  j = 0x0f66; break;
        case ID_NOR_H:  j = 0x0f67; break;
        case ID_NOR_SA: j = 0x0f68; break;
        case ID_SKT_KZH: j = 0x0f69; break;
        default: j = 0; break;
      }
    return ( j );
  } // END = convnor ()

int convsub ( int x )
  {
    int j = 0;
    switch ( x )
      {
        case ID_NOR_K:  j = 0x0f90; break;
        case ID_NOR_KH: j = 0x0f91; break;
        case ID_NOR_G:   j = 0x0f92; break;
        case ID_NOR_GH:   j = 0x0f93; break;
        case ID_NOR_NG:   j = 0x0f94; break;
        case ID_NOR_C:   j = 0x0f95; break;
        case ID_NOR_CH:   j = 0x0f96; break;
        case ID_NOR_J:   j = 0x0f97; break;
        case ID_NOR_NY:  j = 0x0f99; break;
        case ID_NOR_TR:  j = 0x0f9a; break;
        case ID_NOR_TRH: j = 0x0f9b; break;
        case ID_NOR_DR:  j = 0x0f9c; break;
        case ID_NOR_DRH: j = 0x0f9d; break;
        case ID_NOR_NR:  j = 0x0f9e; break;
        case ID_NOR_T:   j = 0x0f9f; break;
        case ID_NOR_TH:  j = 0x0fa0; break;
        case ID_NOR_D:   j = 0x0fa1; break;
        case ID_NOR_DH:  j = 0x0fa2; break;
        case ID_NOR_N:   j = 0x0fa3; break;
        case ID_NOR_P:   j = 0x0fa4; break;
        case ID_NOR_PH:  j = 0x0fa5; break;
        case ID_NOR_B:  j = 0x0fa6; break;
        case ID_NOR_BH:  j = 0x0fa7; break;
        case ID_NOR_M:   j = 0x0fa8; break;
        case ID_NOR_TS:  j = 0x0fa9; break;
        case ID_NOR_TSH: j = 0x0faa; break;
        case ID_NOR_DZ:  j = 0x0fab; break;
        case ID_NOR_DZH: j = 0x0fac; break;
        case ID_NOR_W:   j = 0x0fad; break;
        case ID_NOR_ZH:  j = 0x0fae; break;
        case ID_NOR_Z:   j = 0x0faf; break;
        case ID_NOR_A:   j = 0x0fb0; break;
        case ID_NOR_Y:   j = 0x0fb1; break;
        case ID_NOR_R:   j = 0x0fb2; break;
        case ID_NOR_L:   j = 0x0fb3; break;
        case ID_NOR_SH:  j = 0x0fb4; break;
        case ID_SKT_SZH: j = 0x0fb5; break;
        case ID_NOR_S:  j = 0x0fb6; break;
        case ID_NOR_H:  j = 0x0fb7; break;
        case ID_NOR_SA: j = 0x0fb8; break;
        case ID_SKT_KZH: j = 0x0fb9; break;
        default: j = 0; break;
      }
    return ( j );
  } // END - convsub ()

int convvow ( int x )
  {
    int j = 0;
    switch ( x )
      {
        case ID_VOW_I:  j = 0x0f72; break;
        case ID_VOW_IR: j = 0x0f73; break;
        case ID_VOW_U:   j = 0x0f74; break;
        case ID_VOW_E:   j = 0x0f7a; break;
        case ID_VOW_AI:   j = 0x0f7b; break;
        case ID_VOW_O:   j = 0x0f7c; break;
        case ID_VOW_AU:   j = 0x0f7d; break;
        default: j = 0; break;
      }
    return ( j );
  } // END - convvow ()

void add2wrdstr ( void )
  {
    int i = 0;
    int j = 0;
    // First find place to add:
    while ( wordstr8[i] != 0 )
      ++i;
    while ( utf8char[j] != 0 )
      wordstr8[i++] = utf8char[j++];
    wordstr8[i] = 0;
  } // END - add2wrdstr ()

void fixskt ( void )
  {
    char c;
    int i;
    i = 0;
    while ( wrd_tst[i] != 0 )
      {
        c = wrd_tst[i];
        switch ( c )
          {
            case 'J': c = 164; break; // n-tilda, Harvard-Kyoto
            case 129: c = 163; break; // Long a, prel1
            case 'A': c = 163; break; // Long a, Harvard-Kyoto
            case 'I': c = 131; break; // Long i, Harvard-Kyoto
            case 'U': c = 133; break; // Long u, Harvard-Kyoto
            case 'R': c = 135; break; // vowel ri, Harvard-Kyoto
            case 'L': c = 139; break; // vowel li, Harvard-Kyoto
            case 141: c = 166; break; // long vowel li, prel1
            case 143: c = 168; break; // n, dot over, prel1
            case 'G': c = 168; break; // n, dot over, Harvard-Kyoto
            case 'T': c = 145; break; // t dot, Harvard-Kyoto
            case 'D': c = 147; break; // d dot, Harvard-Kyoto
            case 'N': c = 149; break; // n, dot under, Harvard-Kyoto
            case 'M': c = 151; break; // m, dot under, Harvard-Kyoto
            case 'Z': c = 153; break; // s, acute, Harvard-Kyoto
            case 157: c = 170; break; // s, dot under, prel1
            case 'S': c = 170; break; // s, dot under, Harvard-Kyoto
            case 'H': c = 160; break; // visarga, Harvard-Kyoto
            default: ; break;
          }
        wrd_tst[i++] = c;
      }
  } // END - fixskt ()

void skt2uni ( void )
  {
    int x;
    if ( strcmp ( wrd_tst, "o—" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convvow ( ID_VOW_O );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f7e ); // anusvara
        add2wrdstr ();
        return;
      }
    else if ( strcmp ( wrd_tst, "£ " ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convsub ( ID_NOR_A );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f7f ); // visarga
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "h…—" ) == 0 )
      {
        x = convnor ( ID_NOR_H );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convsub ( ID_NOR_A );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convvow ( ID_VOW_U );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f7e ); // anusvara
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "dhru" ) == 0 )
      {
        x = convnor ( ID_NOR_DH );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convsub ( ID_NOR_R );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convvow ( ID_VOW_U );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "‡i" ) == 0 )
      {
        x = convnor ( ID_NOR_R );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f80 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "‹i" ) == 0 )
      {
        x = convnor ( ID_NOR_L );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f80 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "l£" ) == 0 )
      {
        x = convnor ( ID_NOR_L );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "w£" ) == 0 )
      {
        x = convnor ( ID_NOR_W );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "r£" ) == 0 )
      {
        x = convnor ( ID_NOR_R );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "y£" ) == 0 )
      {
        x = convnor ( ID_NOR_Y );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "h£" ) == 0 )
      {
        x = convnor ( ID_NOR_H );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "£l" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
        x = convnor ( ID_NOR_L );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "£r" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
        x = convnor ( ID_NOR_R );
        uni2utf8 ( x );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "£" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f71 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "au" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f7d );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "ai" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f7b );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "‡ƒ" ) == 0 )
      {
        x = convnor ( ID_NOR_R );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f81 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "ƒ" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f73 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "‹ƒ" ) == 0 )
      {
        x = convnor ( ID_NOR_L );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f81 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "…" ) == 0 )
      {
        x = convnor ( ID_NOR_SA );
        uni2utf8 ( x );
        add2wrdstr ();
        uni2utf8 ( 0x0f75 );
        add2wrdstr ();
      }
    else if ( strcmp ( wrd_tst, "biªti" ) == 0 )
      {
        x = convnor ( ID_NOR_B );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convvow ( ID_VOW_I );
        uni2utf8 ( x );
        add2wrdstr ();
        x = convnor ( ID_SKT_SZH );
        uni2utf8 ( x );
        add2wrdstr ();        
        x = convsub ( ID_NOR_T );
        uni2utf8 ( x );
        add2wrdstr (); 
        x = convvow ( ID_VOW_I );
        uni2utf8 ( x );
        add2wrdstr ();                       
      }
  }

void num2uni ( void )
  {
    char c;
    int i, x;
    i = 0;
    while ( wrd_tst[i] != 0 )
      {
        c = wrd_tst[i++];
        switch ( c )
          {
            case '0': x = 0x0f20; break;
            case '1': x = 0x0f21; break;
            case '2': x = 0x0f22; break;
            case '3': x = 0x0f23; break;
            case '4': x = 0x0f24; break;
            case '5': x = 0x0f25; break;
            case '6': x = 0x0f26; break;
            case '7': x = 0x0f27; break;
            case '8': x = 0x0f28; break;
            case '9': x = 0x0f29; break;
            default: ; break;
          }
        uni2utf8 ( x );
        add2wrdstr ();
      }
  } // END - num2uni ()
