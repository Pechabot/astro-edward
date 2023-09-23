// t4b.c - routines for calendar symbolic information - sa bdag

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
 
extern char gen_phrod_str[];
extern char sreg_str[];
extern char lmchg[];
extern char byungphro_str[];
extern char solar_term_str[];

// Check for main Earth-lords, "sa bdag"  
int chk_sadag ( int m, int t )
  {
    int i;
    sadag_str[0] = 0;
    if ( m == 1 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "khra drag shar du 'tshog nor rdzas mi dbyung dbul nyi" );
              break;
            case 4:       
              strcpy ( sadag_str, "ba dan" );
              break;
            case 5:       
              strcpy ( sadag_str, "sa bdag grub klu bzlog" );
              break;              
            case 7:       
              strcpy ( sadag_str, "nyi shar klu rgyu gnyan thebs nyi nag ngan pa dgu 'dzoms ste mtshur srod" );
              break;                            
            case 8:       
              strcpy ( sadag_str, "sa bdag yul gnyan rgyu 3 phung po zor thogs skya rengs dus rgyal po rgyu bya dus hal khyi zas 'tshol" );
              break;              
            case 10:       
              strcpy ( sadag_str, "ngom shing shar nas nub tu rgyu klu bzlog" );
              break;                                          
            case 11:       
              strcpy ( sadag_str, "ka khyung rA hu rgyu dbul nyi king kang" );
              break;              
            case 13:       
              strcpy ( sadag_str, "yan kwong" );
              break;                            
            case 14:       
              strcpy ( sadag_str, "sde brgyad yos dus nas nub nas shar du thebs klu thebs" );
              break;              
            case 15:       
              strcpy ( sadag_str, "klu bzlog king kang sde brgyad thebs klu gza' bzlog" );
              break;                                                        
            case 16:
              strcpy ( sadag_str, "dbul nyi gza' bdun zas 'tshol ba dan" );
              break;              
            case 17:       
              strcpy ( sadag_str, "sde brgyad nyin phyed dang sprel glang dus bzlog sa bdag bzlog nyi shar klu rgyu king kang" );
              break;                            
            case 18:       
              strcpy ( sadag_str, "skya rengs dus rgyal po rgyu phung po zor thogs gza' bdun zas 'tshol" );
              break;              
            case 19:       
              strcpy ( sadag_str, "sde brgyad nyin phyed dang sprel glang dus bzlog sa bdag bzlog gnyan bzlog" );
              break;                                          
            case 22:       
              strcpy ( sadag_str, "srod la ma mo rgyu" );
              break;              
            case 27:       
              strcpy ( sadag_str, "king kang ka khyung nyi shar klu rgyu" );
              break;                            
            case 28:       
              strcpy ( sadag_str, "phag dus pi ling gi thebs rA hu rgyu skya rengs dus rgyal po rgyu king kang nyi nag chung nam phyed ba dan" );
              break;              
            case 30:       
              strcpy ( sadag_str, "sa bdag grub" );
              break;                                                        
            default:  
              return (0); 
              break;           
          }
      }
    else if ( m == 2 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "sde brgyad bzlog sa bdag bzlog ba dan" );
              break;              
            case 2:       
              strcpy ( sadag_str, "rA hu king kang dbul nyi" );
              break;                            
            case 3:       
              strcpy ( sadag_str, "king kang" );
              break;              
            case 4:       
              strcpy ( sadag_str, "gshin pho rog shar" ); // Really? Both?
              break;                            
            case 5:       
              strcpy ( sadag_str, "gshin pho rog shar" );
              break;              
            case 6:       
              strcpy ( sadag_str, "nyi shar dus btsan rgyu sa bdag yal gnyan rgyu" );
              break;                                                        
            case 7:       
              strcpy ( sadag_str, "gnam khyi sbyor" );
              break;              
            case 8:       
              strcpy ( sadag_str, "sa bdag bzlog sa bdag grub sde brgyad bzlog klu bzlog phung po zor thogs" );
              break;                            
            case 9:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu" );
              break;              
            case 10:       
              strcpy ( sadag_str, "klu thebs" );
              break;                                          
            case 11:       
              strcpy ( sadag_str, "sde brgyad bzlog yan kwong" );
              break;              
            case 12:       
              strcpy ( sadag_str, "nor mi 'byung klu gza' bzlog" );
              break;                            
            case 13:       
              strcpy ( sadag_str, "khra drug shar du 'tshem gnyan thebs ba dan" );
              break;                            
            case 14:       
              strcpy ( sadag_str, "nyi nag" );
              break;                                                        
            case 15:       
              strcpy ( sadag_str, "gnyan bzlog" );
              break;              
            case 16:       
              strcpy ( sadag_str, "sde brgyad phag gi dus shar nas nub tu thebs nyi shar btsan rgyu rta dus hal khyi zas 'tshol" );
              break;                            
            case 18:       
              strcpy ( sadag_str, "nam phyed ro dgu rgyu bas che las rab gnas 'dzem 3 klu bzlog sprel dus pi ling gi thebs nyi nag chung nyi thal" );
              break;              
            case 19:       // CHECK: ????
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu nam phyed nam phyed ro dgu rgyu" );
              break;                                          
            case 20:       
              strcpy ( sadag_str, "ngom shing shar nas nub tu rgyu klu bzlog" );
              break;              
            case 21:       
              strcpy ( sadag_str, "sde brgyad bzlog dbul nyi king kang rA hu klu bzlog" );
              break;                            
            case 22:       
              strcpy ( sadag_str, "king kang rA hu klu bzlog" );
              break;              
            case 24:       
              strcpy ( sadag_str, "king kang ka khyung" );
              break;                                                                      
            case 25:       
              strcpy ( sadag_str, "ka khyung ba dan" );
              break;                                                                                    
            case 26:       
              strcpy ( sadag_str, "nyi shar btsan rgyu" );
              break;                                                                      
            case 28:       
              strcpy ( sadag_str, "sde brgyad bzlog klu bzlog gza' bdun zas 'tshol" );
              break;                                                                      
            case 29:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu" );
              break;                                                                      
            default:  
              return (0); 
              break;           
          }
      }  
    else if ( m == 3 )
      {
        switch ( t )
          {
            case 3:       
              strcpy ( sadag_str, "dbul nyi" );
              break;              
            case 4:       
              strcpy ( sadag_str, "gnam khyi sbyor gnyan rgyu" );
              break;                            
            case 6:       
              strcpy ( sadag_str, "sa bdag yal" );
              break;              
            case 8:       
              strcpy ( sadag_str, "sbrul dus pi ling gi thebs king kang dbul nyi nyi nag chung nyi dros nor mi dbyung" );
              break;                                          
            case 9:       
              strcpy ( sadag_str, "phung po zor thogs sde brgyad bzlog yan kwong" );
              break;              
            case 10:       
              strcpy ( sadag_str, "ba dan" );
              break;                            
            case 13:       
              strcpy ( sadag_str, "nyi myur gshin rje rgyu gza' bdun zas 'tshol" );
              break;              
            case 18:       
              strcpy ( sadag_str, "ro dgu rgyu bas nyi dros don gnyer lam zhugs 'dzem gnyan bzlog king kang gza' rgod zas tshol rA hu sa bdag grub" );
              break;                                                                  
            case 20:       
              strcpy ( sadag_str, "khra drug shar du 'tshog" );
              break;              
            case 21:       
              strcpy ( sadag_str, "gnyen thebs nyi nag nam phyed" );
              break;                            
            case 22:       
              strcpy ( sadag_str, "sde brgyad phag dus lho nas byang du thebs ba dan" );
              break;              
            case 24:       
              strcpy ( sadag_str, "stag dus hal khyi zas 'tshol" );
              break;              
            case 25:       
              strcpy ( sadag_str, "klu thebs" );
              break;                                                        
            case 27:       
              strcpy ( sadag_str, "dbul nyi" );
              break;              
            case 28:       // Unfinished ????
              strcpy ( sadag_str, "3 king kang ka khyung nyi dros ro dgu rgyu don gnyer lam zhugs 'dzem pi ling nyi shar shar nas" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "srod la bdud rgyu" );
              break;              
            case 30:       
              strcpy ( sadag_str, "sde brgyad bzlog sa bdag bzlog" );
              break;                                          
            default:  
              return (0); 
              break;           
          }
      }                
    else if ( m == 4 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "khra drug lhor 'tshog sa bdag grub" );
              break;              
            case 2:       
              strcpy ( sadag_str, "bya byi'i dus sde brgyad bzlog sa bdag bzlog" );
              break;                            
            case 3:       
              strcpy ( sadag_str, "gnyan thebs phung po zor thogs" );
              break;              
            case 4:       
              strcpy ( sadag_str, "dbul nyi" );
              break;                                          
            case 5:       
              strcpy ( sadag_str, "nyi dros ro dgu rgyu bas mdos zor 'phen mi rung 3 sa bdag bzlog bya bya'i dus sde brgyad bzlog pi ling nyin phyed lho nas" );
              break;              
            case 6:       
              strcpy ( sadag_str, "sa bdag yal" );
              break;                            
            case 7:       
              strcpy ( sadag_str, "nyi shar klu rgyu gnyan rgyu gnam khyi sbyor yan kwong ba dan" );
              break;              
            case 8:       
              strcpy ( sadag_str, "mche ba can byang du gnas skya rengs dus rgyal po rgyu dbul nyi klu thebs dbul lag sde brgyad yos dus thebs nyi nag nyi dros nor mi dbyung" );
              break;                                                                  
            case 9:       
              strcpy ( sadag_str, "glang dus hal khyis zas 'tshol" );
              break;              
            case 15:       
              strcpy ( sadag_str, "klu thebs" );
              break;                            
            case 16:       
              strcpy ( sadag_str, "king kang gza' rgod zas 'tshol gnyan bzlog rA hu" );
              break;              
            case 17:       
              strcpy ( sadag_str, "nyi shar klu rgyu" );
              break;              
            case 18:       
              strcpy ( sadag_str, "skya rengs shar dus rgyal po rgyu" );
              break;                                                        
            case 19:       
              strcpy ( sadag_str, "ba dan" );
              break;              
            case 20:       
              strcpy ( sadag_str, "klu bzlog" );
              break;                            
            case 22:       
              strcpy ( sadag_str, "srod la ma mo rgyu gza' rgod zas 'tshol rA hu" );
              break;              
            case 23:       
              strcpy ( sadag_str, "gnyan thebs" );
              break;                                          
            case 25:       // klu bzlog twice
              strcpy ( sadag_str, "dra chen rgyu klu bzlog klu gza' bzlog phag dus pi ling gi thebs yod nyi nag ngam phyed" );
              break;              
            case 26:       
              strcpy ( sadag_str, "king kang ka khyung" );
              break;                            
            case 27:       
              strcpy ( sadag_str, "nyi shar klu rgyu gza' bdun zas 'tshol" );
              break;              
            case 28:       
              strcpy ( sadag_str, "skya rengs dus rgyal po rgyu" );
              break;                                                        
            case 29:       
              strcpy ( sadag_str, "gshin pho rog byang" );
              break;              
            default:  
              return (0); 
              break;           
          }
      }    
    else if ( m == 5 )
      {
        switch ( t )
          {
            case 3:       
              strcpy ( sadag_str, "sa bdag bzlog" );
              break;              
            case 4:       
              strcpy ( sadag_str, "ba dan" );
              break;                            
            case 5:       
              strcpy ( sadag_str, "dbul nyi yan kwong" );
              break;              
            case 6:       
              strcpy ( sadag_str, "sde brgyad sbrul dus bzlog nyi shar btsan rgyu" );
              break;                                          
            case 7:       
              strcpy ( sadag_str, "gnam khyi sbyor" );
              break;              
            case 8:       
              strcpy ( sadag_str, "klu bzlog" );
              break;                            
            case 9:       
              strcpy ( sadag_str, "nyi phyed gnod sbyin rgyu" );
              break;              
            case 10:       
              strcpy ( sadag_str, "gnyan rgyu" );
              break;                                                                  
            case 11:       
              strcpy ( sadag_str, "ka khyung dbul nyi king kang" );
              break;              
            case 13:       
              strcpy ( sadag_str, "khra drug lhor 'tshog gza' rgod zas 'tshol rA hu phung po zor thogs" );
              break;                            
            case 15:       
              strcpy ( sadag_str, "3 king kang klu bzlog srod ro dgu rgyu bas bya ba spyi dang rim gro spang sde brgyad lho nub nas byang shar du 'brug dus thebs klu gza' bzlog" );
              break;              
            case 16:       
              strcpy ( sadag_str, "nor mi dbyung sde brgyad sbrul dus bzlog nyi shar btsan rgyu nyi nag nyin phyed rgyu gza' bdun zas 'tshol ba dan" );
              break;              
            case 17:       
              strcpy ( sadag_str, "king kang" );
              break;                                                        
            case 18:       
              strcpy ( sadag_str, "sde brgyad sbrul dus bzlog phag dus hal khyis zas 'tshol" );
              break;              
            case 19:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu sa bdag grub" );
              break;                            
            case 20:       
              strcpy ( sadag_str, "klu thebs" );
              break;              
            case 22:       
              strcpy ( sadag_str, "klu thebs gza' bdun zas 'tshol" );
              break;                                          
            case 23:       
              strcpy ( sadag_str, "phung po zor thogs" );
              break;              
            case 26:       
              strcpy ( sadag_str, "sa bdag yal nyi shar btsan rgyu" );
              break;                            
            case 27:       
              strcpy ( sadag_str, "ka khyung king kang" );
              break;              
            case 28:       
              strcpy ( sadag_str, "king kang ba dan" );
              break;                                                        
            case 29:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu" );
              break;              
            case 30:       
              strcpy ( sadag_str, "ngom shing lho nas byang du rgyu" );
              break;                            
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 6 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "ba dan" );
              break;              
            case 2:       
              strcpy ( sadag_str, "king kang rA hu" );
              break;                            
            case 3:       
              strcpy ( sadag_str, "king kang sa bdag grub yan kwong" );
              break;              
            case 5:       
              strcpy ( sadag_str, "klu thebs sbrul dus pi ling gi thebs nyi nag chung dgongs ka" );
              break;                                          
            case 6:       
              strcpy ( sadag_str, "dbul nyi" );
              break;              
            case 9:       
              strcpy ( sadag_str, "sde brgyad thebs phung po zor thogs" );
              break;                            
            case 11:       
              strcpy ( sadag_str, "sde brgyad rta dus bzlog sa bdag bzlog klu gza' bzlog klu bzlog" );
              break;              
            case 12:       
              strcpy ( sadag_str, "ngom shing shar nas nub tu rgyu" );
              break;                                                                  
            case 13:       
              strcpy ( sadag_str, "klu bzlog nyi myur gshin rje rgyu gza' bdu zas 'tshol ba dan" );
              break;              
            case 15:       
              strcpy ( sadag_str, "dra chen rgyu sa bdag yal klu bzlog" );
              break;                            
            case 18:       
              strcpy ( sadag_str, "gza' rgod zas 'tshol" );
              break;              
            case 19:       
              strcpy ( sadag_str, "gshin pho rog byang du gnas" );
              break;              
            case 20:       
              strcpy ( sadag_str, "khra drug lhor 'tshog klu thebs" );
              break;                                                        
            case 22:       
              strcpy ( sadag_str, "dbul nyi king kang" );
              break;              
            case 24:       
              strcpy ( sadag_str, "ka khyung rgyu king kang nyi nag nam phyed" );
              break;                            
            case 25:       
              strcpy ( sadag_str, "3 klu thebs myur smad kha la ro dgu rgyu bas mkhar las thog 'bubs 'dzem pi ling nyin phyed lho ba dan" );
              break;              
            case 26:       
              strcpy ( sadag_str, "gnam khyi sbyor" );
              break;                                          
            case 27:       
              strcpy ( sadag_str, "gnyan rgyu bya dus hal khyi zas 'tshol" );
              break;              
            case 28:       
              strcpy ( sadag_str, "nor mi dbyung" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "srod la bdud rgyu" );
              break;              
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 7 )
      {
        switch ( t )
          {
            case 1:       // nyi tha' ?????
              strcpy ( sadag_str, "tho rengs ro dgu rgyu bas phyugs kyi g.yang 'gug rim gro ngan gshin pho rog nub khra drug nub tu 'tshog pi ling nyi tha' nub nas yan kwong" );
              break;              
            case 5:       
              strcpy ( sadag_str, "klu bzlog" );
              break;                            
            case 6:       // TWICE ???
              strcpy ( sadag_str, "klu bzlog" );
              break;              
            case 7:       
              strcpy ( sadag_str, "phung po zor thogs dbul nyi nyi shar klu rgyu" );
              break;                                          
            case 8:       
              strcpy ( sadag_str, "skya rengs dus rgyal po rgyu king kang" );
              break;              
            case 9:       
              strcpy ( sadag_str, "gnyan rgyu gza' rgod zas 'tshol klu thebs gnyan thebs nyi nag rtse shar dus" );
              break;                            
            case 10:       
              strcpy ( sadag_str, "klu bzlog ngom shing shar nas nub tu rgyu sbrul dus hal khyi zas 'tshol ba dan" );
              break;              
            case 15:       
              strcpy ( sadag_str, "sgo 'brug dus sde brgyad byang shar phyogs su kha bltas thebs klu gza' bzlog" );
              break;                                                                  
            case 16:       
              strcpy ( sadag_str, "klu gza' bzlog" );
              break;              
            case 17:       
              strcpy ( sadag_str, "nyi shar klu rgyu" );
              break;                            
            case 18:       
              strcpy ( sadag_str, "king kang gnyan bzlog skya rengs dus rgyal po rgyu gza' bdun zas 'tshol" );
              break;              
            case 19:       
              strcpy ( sadag_str, "dbul nyi klu thebs gnam khyi sbyor sa bdag grub" );
              break;              
            case 20:       
              strcpy ( sadag_str, "nyi dros hal khyi zas 'tshol" );
              break;                                                        
            case 21:       
              strcpy ( sadag_str, "khyi dus pi ling gi thebs nyi nag chung nam phyed" );
              break;              
            case 22:       
              strcpy ( sadag_str, "gza' rgod zas 'tshol srod la ma mo rgyu gza' bdun zas 'tshol ba dan" );
              break;                            
            case 23:       
              strcpy ( sadag_str, "sde brgyad 'brug dus nub byang thebs" );
              break;              
            case 25:       
              strcpy ( sadag_str, "sde brgyad glang dus bzlog sa bdag bzlog" );
              break;                                          
            case 27:       
              strcpy ( sadag_str, "nyi shar klu rgyu" );
              break;              
            case 28:       
              strcpy ( sadag_str, "rA hu skya rengs glang dus bzlog sa bdag bzlog" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "sa bdag yal yan kwong" );
              break;              
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 8 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "dra chen rgyu" );
              break;              
            case 3:       
              strcpy ( sadag_str, "sde brgyad glang dus bzlog sa bdag bzlog klu bzlog" );
              break;                            
            case 5:       
              strcpy ( sadag_str, "klu thebs" );
              break;              
            case 6:       
              strcpy ( sadag_str, "nyi shar btsan rgyu klu gza' bzlog klu bzlog mche ba can byang du gnas" );
              break;                                          
            case 7:       
              strcpy ( sadag_str, "ba dan" );
              break;              
            case 8:       
              strcpy ( sadag_str, "dbul nyi" );
              break;                            
            case 9:       
              strcpy ( sadag_str, "sa bdag grub klu bzlog nyin phyed gnod sbyin rgyu" );
              break;              
            case 10:       
              strcpy ( sadag_str, "dbul nyi sde brgyad yos byi'i dus nub nas shar du thebs nor mi dbyung" );
              break;                                                                  
            case 11:       
              strcpy ( sadag_str, "lug dus pi ling gi thebs nyi dros ro dgu rgyu bas shid ston mgron po spangs nyi nag chung nyin phyed 3" );
              break;              
            case 12:       
              strcpy ( sadag_str, "sa bdag yal" );
              break;                            
            case 13:       
              strcpy ( sadag_str, "sde brgyad glang dus bzlog khra drug nub tu 'tshog" );
              break;              
            case 15:       
              strcpy ( sadag_str, "klu thebs" );
              break;              
            case 16:       
              strcpy ( sadag_str, "nyi shar btsan rgyu king kang rA hu klu bzlog mche ba can byang du gnas gza' bdun zas 'tshol" );
              break;                                                        
            case 17:       
              strcpy ( sadag_str, "sde brgyad yos bya'i dus nub nas shar du thebs phung po zor thogs" );
              break;              
            case 18: // Many repetitions: "sde brgyad yos bya'i dus nub nas shar du thebs"
              strcpy ( sadag_str, "sde brgyad yos bya'i dus nub nas shar du thebs nyi nag" );
              break;                            
            case 19:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu sde brgyad yos bya'i dus nub nas shar du thebs gnam khyi sbyor ba dan" );
              break;              
            case 20:       
              strcpy ( sadag_str, "sde brgyad yos bya'i dus nub nas shar du thebs ngom shing shar nas nub tu rgyu 'brug dus hal khyi zas 'tshol" );
              break;                                          
            case 21:       
              strcpy ( sadag_str, "dra chen rgyu sde brgyad yos bya'i dus nub nas shar du thebs" );
              break;              
            case 22:       
              strcpy ( sadag_str, "sde brgyad yos bya'i dus nub nas shar du thebs gza' bdun zas 'tshol" );
              break;                            
            case 23:       
              strcpy ( sadag_str, "sde brgyad glang dus bzlog gnyan thebs" );
              break;              
            case 24:       // Now, byi'i ???
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs" );
              break;                                                        
            case 25:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs" );
              break;              
            case 26:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs nyi shar btsan rgyu king kang ka khyung" );
              break;                            
            case 27:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs yan kwong" );
              break;              
            case 28:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs" );
              break;                                          
            case 29:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs nyin phyed gnod sbyin rgyu" );
              break;              
            case 30:       
              strcpy ( sadag_str, "sde brgyad yos byi'i dus nub nas shar du thebs gnyan rgyu gnyan bzlog" );
              break;                            
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 9 ) // to here
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "gshin pho rog nub gnyan bzlog klu thebs rA hu sde brgyad yos dus bzlog 'brug dus pi ling thebs nyi nag chung srod" );
              break;              
            case 2:       
              strcpy ( sadag_str, "sde brgyad yos dus bzlog sa bdag bzlog" );
              break;                            
            case 4:       
              strcpy ( sadag_str, "sa bdag yal ba dan" );
              break;              
            case 7:       
              strcpy ( sadag_str, "sa bdag grub gnyan thebs gnam khyi sbyor" );
              break;                                          
            case 8:       
              strcpy ( sadag_str, "sde brgyad rta bya'i dus nub byang du thebs phung po zor thogs" );
              break;              
            case 9:       
              strcpy ( sadag_str, "klu gza' bzlog dbul nyi klu bzlog" );
              break;                            
            case 10:       
              strcpy ( sadag_str, "klu bzlog" );
              break;              
            case 11:       
              strcpy ( sadag_str, "ka khyung rgyu klu thebs king kang" );
              break;                                                                  
            case 13:       
              strcpy ( sadag_str, "nyi myur gshin rje rgyu" );
              break;              
            case 15:       
              strcpy ( sadag_str, "king kang klu gza' bzlog" );
              break;                            
            case 16:       
              strcpy ( sadag_str, "gza' bdun zas 'tshol ba dan" );
              break;              
            case 17:       
              strcpy ( sadag_str, "king kang" );
              break;              
            case 19:       
              strcpy ( sadag_str, "klu bzlog klu gza' bzlog" );
              break;                                                        
            case 20:       
              strcpy ( sadag_str, "tho rengs hal khyi zas 'tshol" );
              break;              
            case 21:       
              strcpy ( sadag_str, "klu thebs dbul nyi 3 dra chen rgyu srod dus ro dgu rgyu smrar lab pham pi ling nyi thul nub nas" );
              break;                            
            case 22:       
              strcpy ( sadag_str, "dbul nyi klu thebs gza' bdun zas 'tshol" );
              break;              
            case 23:       
              strcpy ( sadag_str, "klu thebs" );
              break;                                          
            case 25:       
              strcpy ( sadag_str, "khra drug nub tu 'tshog yan kwong" );
              break;              
            case 27:       
              strcpy ( sadag_str, "nor mi dbyung ka khyung rgyu gnyan rgyu nyi nag tho rengs zla shar king kang" );
              break;                            
            case 28:       
              strcpy ( sadag_str, "king kang sde brgyad rta bya'i dus nub byang du thebs phung po zor thogs ba dan" );
              break;              
            case 29:       
              strcpy ( sadag_str, "klu gza' bzlog srod la bdud rgyu dbul nyi" );
              break;                                                        
            case 30:       
              strcpy ( sadag_str, "sde brgyad rta bya'i dus nub byang du thebs stag dus hal khyi zas 'tshol" );
              break;              
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 10 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "dbul nyi gnyan thebs khra drug byang du 'tshog phung po zor thogs ba dan" );
              break;              
            case 2:       
              strcpy ( sadag_str, "king kang rA hu" );
              break;                            
            case 3:       
              strcpy ( sadag_str, "king kang" );
              break;              
            case 7:       
              strcpy ( sadag_str, "gnyan bzlog nyi shar klu rgyu" );
              break;                                          
            case 8:       
              strcpy ( sadag_str, "sde brgyad byi du bzlog klu thebs skya rengs dus rgyal po rgyu" );
              break;              
            case 9:       
              strcpy ( sadag_str, "sde brgyad byi du bzlog klu bzlog dra chen rgyu sa bdag yal 3 ro dgu nam phyed yol rgyu bas drag las 'phru log spang pi ling byang nas" );
              break;                            
            case 10:       
              strcpy ( sadag_str, "klu bzlog nyi nag nyin phyed dbul nyi" );
              break;              
            case 11:       
              strcpy ( sadag_str, "bya dus hal khyi zas 'tshol" );
              break;                                                                  
            case 13:       // Duplication again. Correct ???
              strcpy ( sadag_str, "sde brgyad sprel glang dus byang phyogs thebs gshin pho rog lhor gza' bdun zas 'tshol ba dan" );
              break;              
            case 14:       
              strcpy ( sadag_str, "sde brgyad sprel glang dus byang du thebs" );
              break;                            
            case 15:       
              strcpy ( sadag_str, "sde brgyad sprel glang dus byang du thebs klu thebs" );
              break;              
            case 16:       
              strcpy ( sadag_str, "sde brgyad sprel glang dus byang du thebs gza' rgod zas 'tshol gnam khyi sbyor" );
              break;              
            case 17:       
              strcpy ( sadag_str, "nyi shar klu rgyu" );
              break;                                                        
            case 18:       
              strcpy ( sadag_str, "sde brgyad byi dus bzlog klu thebs skya rengs dus rgyal po rgyu sa bdag bzlog" );
              break;              
            case 19:       
              strcpy ( sadag_str, "klu bzlog klu gza' bzlog dbul nyi gnyan rgyu" );
              break;                            
            case 20:       
              strcpy ( sadag_str, "nor mi dbyung" );
              break;              
            case 22:       
              strcpy ( sadag_str, "gza' rgod zas 'tshol srod la ma mo rgyu king kang" );
              break;                                          
            case 23:       
              strcpy ( sadag_str, "yan kwong" );
              break;              
            case 24:       
              strcpy ( sadag_str, "ka khyung rgyu king kang" );
              break;                            
            case 25:       
              strcpy ( sadag_str, "ba dan" );
              break;              
            case 26:       
              strcpy ( sadag_str, "sde brgyad byi dus bzlog klu bzlog" );
              break;                                                        
            case 27:       
              strcpy ( sadag_str, "sa bdag grub nyi shar klu rgyu" );
              break;              
            case 28:       
              strcpy ( sadag_str, "skya rengs dus rgyal po rgyu" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "bya dus pi ling gi thebs nyi nag chung nyi shar" );
              break;              
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 11 )
      {
        switch ( t )
          {
            case 1:       
              strcpy ( sadag_str, "dbul nyi" );
              break;              
            case 2:       
              strcpy ( sadag_str, "sa bdag yal" );
              break;                            
            case 6:       
              strcpy ( sadag_str, "klu gza' bzlog klu bzlog nyi shar btsan rgyu" );
              break;              
            case 7:       
              strcpy ( sadag_str, "klu thebs" );
              break;                                          
            case 8:       
              strcpy ( sadag_str, "gza' bdun zas 'tshol king kang" );
              break;              
            case 9:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu" );
              break;                            
            case 10:       
              strcpy ( sadag_str, "dbul nyi gnyan rgyu ba dan" );
              break;              
            case 12:       
              strcpy ( sadag_str, "nor mi dbyung" );
              break;                                                                  
            case 13:       
              strcpy ( sadag_str, "khra drug byang du 'tshog" );
              break;              
            case 15:       
              strcpy ( sadag_str, "klu thebs" );
              break;                            
            case 16:       
              strcpy ( sadag_str, "klu bzlog nyi shar btsan rgyu phung po zor thogs dbul nyi" );
              break;              
            case 17:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs klu thebs" );
              break;              
            case 18:       
              strcpy ( sadag_str, "king kang rA hu gza' bdun zas 'tshol" );
              break;                                                        
            case 19:       
              strcpy ( sadag_str, "nyin phyed gnod sbyin rgyu rta dus pi ling gi thebs gnam khyi sbyor ro dgu nam phyed rgyu bzo rigs spangs pi ling nam phyed byang nas" );
              break;              
            case 20:       
              strcpy ( sadag_str, "sde brgyad bya dus bzlog nor mi btang  nyi nag chung zla shar klu bzlog" );
              break;                            
            case 21:       
              strcpy ( sadag_str, "klu thebs yan kwong" );
              break;              
            case 22:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs sprel dus hal khyi zas 'tshol ba dan" );
              break;                                          
            case 23:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs"  );
              break;              
            case 24:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs" );
              break;                            
            case 25:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs gnyan thebs" );
              break;              
            case 26:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs klu bzlog nyi shar btsan rgyu" );
              break;                                                        
            case 27:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs klu gza' bzlog klu bzlog sa bdag bzlog" );
              break;              
            case 28:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs kin kang ka khyung rgyu" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs nyin phyed gnod sbyin rgyu" );
              break;              
            case 30:       
              strcpy ( sadag_str, "sde brgyad byi rta'i dus shar lhor thebs ngom shing lho nas byang du rgyu sa bdag grub gnyan bzlog" );
              break;                                          
            default:  
              return (0); 
              break;           
          }          
      }                      
    else if ( m == 12 )
      {
        switch ( t )
          {
            case 3:       
              strcpy ( sadag_str, "gnyan rgyu yos dus hal khyi zas 'tshol" );
              break;              
            case 4:       
              strcpy ( sadag_str, "mche ba can byang du gnas" );
              break;                            
            case 6:       
              strcpy ( sadag_str, "sa bdag yal" );
              break;              
            case 7:       
              strcpy ( sadag_str, "ba dan" );
              break;                                          
            case 9:       
              strcpy ( sadag_str, "dbul nyi phung po zor thogs yos dus pi ling gi thebs nyi nag chung nyin phyed" );
              break;              
            case 12:       
              strcpy ( sadag_str, "ngom shing shar nas nub tu rgyu" );
              break;                            
            case 13:       
              strcpy ( sadag_str, "nyi myur gshin rje rgyu" );
              break;              
            case 14:       
              strcpy ( sadag_str, "sa bdag bzlog" );
              break;                                                                  
            case 16:       
              strcpy ( sadag_str, "king kang gza' bdun zas 'tshol" );
              break;              
            case 19:       
              strcpy ( sadag_str, "sa bdag grub yan kwong ba dan" );
              break;                            
            case 22:       
              strcpy ( sadag_str, "gza' bdun zas 'tshol" );
              break;              
            case 24:       
              strcpy ( sadag_str, "gnam khyi sbyor" );
              break;              
            case 25:       
              strcpy ( sadag_str, "khra drug byang du 'tshog" );
              break;                                                        
            case 26:       
              strcpy ( sadag_str, "king kang ka khyung" );
              break;              
            case 27:       
              strcpy ( sadag_str, "sde brgyad khyi dus bzlog" );
              break;                            
            case 29:       
              strcpy ( sadag_str, "dbul nyi srod la bdud rgyu tho rengs ro dgu rgyu bas lha sgrub sngags zlas spang pi ling nam phyed byang nas 3" );
              break;              
            case 30:       
              strcpy ( sadag_str, "nor mi dbyung nyi nag nam phyed dus" );
              break;                                          
            default:  
              return (0); 
              break;           
          }          
      }                      
    else
      return (0);
    i = strlen ( sadag_str);
    if ( i > sadag_str_len ) // Max 35 in testing.
      sadag_str_len = i;
    return (1);      
  } // END - chk_sadag ()
  
// Check for main Earth-lords, "sa bdag"  
int chk_sadag2 ( int m, int t )
  {
    int i = 0;
    int n;
    
// First, "yan kwong": // Data from Kongleg.
    yan_kwong_flg = 0;

    if ( m == 1 && t == 13 ) yan_kwong_flg = 1;
    else if ( m == 2 && t == 11 ) yan_kwong_flg = 1;
    else if ( m == 3 && t == 9 ) yan_kwong_flg = 1;
    else if ( m == 4 && t == 7 ) yan_kwong_flg = 1;
    else if ( m == 5 && t == 5 ) yan_kwong_flg = 1;
    else if ( m == 6 && t == 3 ) yan_kwong_flg = 1;
    else if ( m == 7 && t == 1 ) yan_kwong_flg = 1;
    else if ( m == 7 && t == 29 ) yan_kwong_flg = 1;
    else if ( m == 8 && t == 27 ) yan_kwong_flg = 1;
    else if ( m == 9 && t == 25 ) yan_kwong_flg = 1;
    else if ( m == 10 && t == 23 ) yan_kwong_flg = 1;
    else if ( m == 11 && t == 21 ) yan_kwong_flg = 1;
    else if ( m == 12 && t == 19 ) yan_kwong_flg = 1;

// Now, "zin phung":

    zin_phung_flg = 0;

// These data are taken from Mongolian data in VKP2.DOC

    if ( m == 1 || m == 2 || m == 3 ) // dpyid gsum, etc.
      {
        if ( t == 1 || t == 7 || t == 13 || t == 19 || t == 25 )
          zin_phung_flg = 1;
      }
    else if ( m == 4 || m == 5 || m == 6 )
      {
        if ( t == 6 || t == 12 || t == 18 || t == 24 || t == 30 )
          zin_phung_flg = 1;
      }
    else if ( m == 7 || m == 8 || m == 9 )
      {
        if ( t == 3 || t == 9 || t == 15 || t == 21 || t == 27 )
          zin_phung_flg = 1;
      }
    else if ( m == 10 || m == 11 || m == 12 )
      {
        if ( t == 4 || t == 10 || t == 16 || t == 22 || t == 28 )
          zin_phung_flg = 1;
      } 

//  zin_phung_flg = 0; // Take it out for now. Need to check data.

// Now, "klu bzlog":

    klu_bzlog_flg = 0;

    if ( m == 1L )
      {
        if ( t == 5L || t == 10L || t == 15L )
          klu_bzlog_flg = 1;
      }
    else if ( m == 2L )
      {
        if ( t == 8L || t == 18L || t == 20L || t == 22L || t == 28L )
          klu_bzlog_flg = 1;
      }
    else if ( m == 4L )
      {
        if ( t == 20L || t == 25L )
          klu_bzlog_flg = 1;
      }
    else if ( m == 5L )
      {
        if ( t == 8L || t == 15L )
          klu_bzlog_flg = 1;
      } 
    else if ( m == 6L )
      {
        if ( t == 11L || t == 13L || t == 15L || t == 23L )
          klu_bzlog_flg = 1;
      } 
    else if ( m == 7L )
      {
        if ( t == 5L || t == 6L )
          klu_bzlog_flg = 1;
      }       
    else if ( m == 8L )
      {
        if ( t == 3L || t == 6L || t == 9L || t == 13L || t == 16L )
          klu_bzlog_flg = 1;
      }       
    else if ( m == 9L )
      {
        if ( t == 9L || t == 10L || t == 19L )
          klu_bzlog_flg = 1;
      }       
    else if ( m == 10L )
      {
        if ( t == 9L || t == 10L || t == 19L || t == 26L )
          klu_bzlog_flg = 1;
      }       
    else if ( m == 11L )
      {
        if ( t == 2L || t == 6L || t == 16L || t == 20L || t == 26L )
          klu_bzlog_flg = 1;
      }       

// Now, "klu thebs":

    klu_thebs_flg = 0;

    if ( m == 1L )
      {
        if ( t == 14L )
          klu_thebs_flg = 1;
      }
    else if ( m == 2L )
      {
        if ( t == 10L )
          klu_thebs_flg = 1;
      }
    else if ( m == 3L )
      {
        if ( t == 25L )
          klu_thebs_flg = 1;
      }      
    else if ( m == 4L )
      {
        if ( t == 8L || t == 15L )
          klu_thebs_flg = 1;
      }
    else if ( m == 5L )
      {
        if ( t == 20L || t == 22L )
          klu_thebs_flg = 1;
      } 
    else if ( m == 6L )
      {
        if ( t == 5L || t == 20L || t == 25L )
          klu_thebs_flg = 1;
      } 
    else if ( m == 7L )
      {
        if ( t == 9L || t == 19L )
          klu_thebs_flg = 1;
      }       
    else if ( m == 8L )
      {
        if ( t == 5L || t == 15L )
          klu_thebs_flg = 1;
      }       
    else if ( m == 9L )
      {
        if ( t == 1L || t == 11L || t == 21L || t == 22L || t == 23L )
          klu_thebs_flg = 1;
      }       
    else if ( m == 10L )
      {
        if ( t == 8L || t == 15L || t == 18L )
          klu_thebs_flg = 1;
      }       
    else if ( m == 11L )
      {
        if ( t == 7L || t == 15L || t == 21L )
          klu_thebs_flg = 1;
      }       

// Now, "nyi nag": // Data from Vaidurya dkar po.

    nyi_nag_flg = 0; 

    if ( m == 1L && t == 7L ) nyi_nag_flg = 1;
    else if ( m == 2L && t == 14L ) nyi_nag_flg = 1;
    else if ( m == 3L && t == 21L ) nyi_nag_flg = 1;
    else if ( m == 4L && t == 8L ) nyi_nag_flg = 1;
    else if ( m == 5L && t == 16L ) nyi_nag_flg = 1;
    else if ( m == 6L && t == 24L ) nyi_nag_flg = 1;
    else if ( m == 7L && t == 9L ) nyi_nag_flg = 1;
    else if ( m == 8L && t == 18L ) nyi_nag_flg = 1;
    else if ( m == 9L && t == 27L ) nyi_nag_flg = 1;
    else if ( m == 10L && t == 10L ) nyi_nag_flg = 1;
    else if ( m == 11L && t == 20L ) nyi_nag_flg = 1;
    else if ( m == 12L && t == 30L ) nyi_nag_flg = 1;

// Now, build string      

    n = yan_kwong_flg + klu_bzlog_flg + klu_thebs_flg + zin_phung_flg + nyi_nag_flg;
    if ( n == 0 )
      return (0);
    sadag_str2[0] = '\0';      
    if ( yan_kwong_flg )
      {
        strcat ( sadag_str2, "yan kwong" );
        i++;   
      }
    if ( klu_bzlog_flg )
      {
        if ( i )
          strcat ( sadag_str2, ", " );
        strcat ( sadag_str2, "klu bzlog" );
        i++;   
      }      
    if ( klu_thebs_flg )
      {
        if ( i )
          strcat ( sadag_str2, ", " );
        strcat ( sadag_str2, "klu thebs" );
        i++;   
      }            
    if ( zin_phung_flg )
      {
        if ( i )
          strcat ( sadag_str2, ", " );
        strcat ( sadag_str2, "zin phung" );
        i++;   
      }      
    if ( nyi_nag_flg )
      {
        if ( i )
          strcat ( sadag_str2, ", " );
        if ( m == 1L )  
          strcat ( sadag_str2, "nyi nag ngan pa dgu 'dzom" );        
        else
          strcat ( sadag_str2, "nyi nag" );
        i++;   
      }            
    return (1);      
  } // END - chk_sadag2 ()
