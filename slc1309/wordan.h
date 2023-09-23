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

struct  wrd_struct
  {
    int     lng;    /** Language - 1 = Tib, 2 = Skt **/
    int     flgs;   /** Flags for special purposes **/
    int     prf;    /** Prefix **/
    int     sup;    /** Superjoined letter **/
    int     rot;    /** Root letter **/
    int     sub;    /** Subjoined letter **/
    int     sub2;   /** 2nd Subjoined letter **/
    int     vw1;    /** First vowel **/
    int     vw2;    /** Second vowel **/
    int     vw3;    /** Third vowel **/
    int     suf1;   /** First suffix **/
    int     suf2;   /** Second suffix **/
    int     shd;    /** Required 'shad' at end **/
    int     hm;     /** Total horizontal length in dots **/
    int     fnt;    /** Flag for small font **/
    int     topx;   /** Reserved **/
    char    wrd[12];
    char    sktwrd[20];
  };
  
int bck_dc ( char * );
int bck_sc ( char );
int chk_vw ( char );
int spec_an ( void );
int word_an ( void );
void str2uni ( char * );
void conv2uni ( void );
void uni2utf8 ( int );
int convnor ( int );
int convsub ( int );
int convvow ( int );
void add2wrdstr ( void );
void addtseg ( void );
int getsktid ( char );
int chk4skt ( void );
int issktvwl ( int );
int detsktid ( char * );
void an_skt ( void );
void fixskt ( void );
void skt2uni ( void );  
void num2uni ( void );
