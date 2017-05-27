/*----------------------------------------------------------------------------*/
/*-------                 START OF SOURCE :: hublin                    -------*/
/*----------------------------------------------------------------------------*/


/*===[[ START HDOC ]]=========================================================*/
/*---[[ HEADER ]]-------------------------------------------------*

 *   niche         : human input
 *   application   : keyboarding
 *   program       : hublin
 *   purpose       : invent word shortcuts based on frequency to speed input
 *   base_system   : gnu/linux
 *   lang_name     : c (primarily ansi-c, but with some C89 extensions)
 *   created       : 2009-11
 *   author        : the_heatherlys
 *   dependencies  : none
 *
 */
/*---[[ PURPOSE ]]------------------------------------------------*

 *   entering english text into a computer is extremely common and will become
 *   more so as computers take over more and more support tasks.  the act of
 *   inputing the text involves depressing keys on a keyboard as fast as
 *   possible while keeping errors to a minimum so that the hands can attempt
 *   to keep up with the brain.
 *   
 *   there are several ways of attempting to make this better...
 *      - minimize finger travel, direction, and strain (keyboard layout)
 *      - defaulting answers to questions or giving short lists
 *      - and, using short nonsense words to stand for larger words
 *
 *   hublin is our personal take on yublin which attempts to do the third
 *   option by assigning longer words to unused one and two letter combinations
 *   which would otherwise not be used.  the idea is to save as many keystrokes
 *   as possible while keeping learning down.
 *
 *   as we are starting to use hand devices more and more, it is best not to
 *   get to clever with laptop keyboards and do something that can help with
 *   iphone or blacberry input as that is a supreme pain.  on these devices
 *   the letter keys, backspace, space, shift, and return are all that is easily
 *   available; so no mode keys, shift is only on one side so not horribly
 *   useful
 *
 *   the basic principles will be...
 *      - stick to all lower case shortcuts (upper for acronyms, slang, ...)
 *      - no dragging fingers required (for special device stuff)
 *      - single and double letter words stay just as themselves (clean)
 *      - assign the first 24 longer words to open letters (except a, i)
 *      - attempt to use shortcuts that start with the same letter as word
 *      - assign words in order of frequency to letters in them
 *
 *
 *
 *   while the online yublin engine can easily do this, i would not have learned
 *   nearly as much using it directly, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the unlying idea is to use this build to learn new programming points
 *   and put additional pressure on me to improve my standards, maintenance,
 *   and coding environment
 *
 */
/*===[[ END HDOC ]]===========================================================*/


#include <stdio.h>
#include <string.h>

#define  VERBOSE   if (g_verbose == 'y')

struct t_words
{
   int         rank;
   char        word[100];
   int         sc;
   char        how;
} v_words[2000];
int    v_nwords;
int    v_sorted[2000];

struct t_shortcuts
{
   char       sc[2];
   int        word;
} v_short[1000];
int    v_nshort;


int    g_short;              /* 1-2 letter word that just is                  */
int    g_perfect;            /* first and second letter match                 */
int    g_letters;            /* any combination of letters in order           */
int    g_first;              /* match first letter, random other letter       */
int    g_any;                /* random first letter, any other letter in word */
int    g_seq;                /* randomly assigned in sequence                 */
int    g_skipped;            /* skipped as it would not save enough letters   */

char   g_verbose = 'y';


/*===========================--------------------=============================*/
/*====---                         preparation                                 */
/*===========================--------------------=============================*/

int load_words ()
{
   VERBOSE printf("   1. load words from stdin .................. ");
   int    _i   = 0;
   int   x_len = 0;
   char  x_buf[100];
   while (!feof(stdin)) {
      fgets(x_buf, 100, stdin);
      sscanf(x_buf, "%d\t%s\n", &v_words[_i].rank, v_words[_i].word);
      x_len = strlen(v_words[_i].word);
      v_words[_i].sc   = -1;
      v_words[_i].how  = '-';
      //printf(" %4d :: %4d :: %-15s :: %d\n", _i, v_words[_i].rank, v_words[_i].word, v_words[_i].sc);
      ++_i;
      if (_i > 800) break;
   }
   VERBOSE printf("%d words\n", _i);
   v_nwords = _i;
   return 0;
}

int generate_shortcut_placeholders (void)
{
   VERBOSE printf("   2. generate shortcut placeholders ......... ");
   v_nshort    = 0;
   char    _m, _n;
   for ( _n = 0; _n < 26; _n++) {
      //printf("   %4d %c\n", v_nshort, _n + 97);
      v_short[v_nshort].sc[0] = _n + 97;
      v_short[v_nshort].sc[1] = ' ';
      v_short[v_nshort].word  = -1;
      v_nshort++;
   }
   for ( _n = 0; _n < 26; _n++) {
      for ( _m = 0; _m < 26; _m++) {
         //printf("   %4d %c%c\n", v_nshort, _n + 97, _m + 97);
         v_short[v_nshort].sc[0] = _n + 97;
         v_short[v_nshort].sc[1] = _m + 97;
         v_short[v_nshort].word  = -1;
         v_nshort++;
      }
   }
   VERBOSE printf("%d shortcuts\n", v_nshort);
   return 0;
}




/*===========================--------------------=============================*/
/*====---                          short-cuts                                 */
/*===========================--------------------=============================*/

int find_small_word (char a_1st, char a_2nd)
{
   int _i = 0;               // word iterator
   if (a_2nd == ' ') a_2nd = '\0';
   for (_i = 0; _i <= v_nwords; ++_i) {
      if (strlen(v_words[_i].word) > 2) continue;
      if (v_words[_i].word[0] != a_1st) continue;
      if (v_words[_i].word[1] != a_2nd) continue;
      return _i;
   }
   return -1;
}

int assign_shortcut (char a_1st, char a_2nd, int a_index, char a_how)
{
   int i = 0;               // word iterator
   for (i = 0; i < v_nshort; ++i) {
      if (v_short[i].word >= 0)       continue;   /* already assigned        */
      if (v_short[i].sc[0] != a_1st)  continue;
      if (v_short[i].sc[1] != a_2nd)  continue;
      /*> if (a_how == '@') printf("   found %c%c\n", v_short[_i].sc[0], v_short[_i].sc[1]);   <*/
      v_short[i].word       = a_index;
      v_words[a_index].sc   = i;
      v_words[a_index].how  = a_how;
      return 0;
   }
   return 1;
}



/*===========================--------------------=============================*/
/*====---                           drivers                                   */
/*===========================--------------------=============================*/



/*---(assign all one and two letter words in list)--------------*/
int assign_short_words()
{
   VERBOSE printf("   3. assign short words (1-2 letters) ....... ");
   g_short = 0;
   int i = 0;               // word iterator
   char x_1st = ' ';
   char x_2nd = ' ';
   for (i = 0; i <= v_nwords; ++i) {
      if (strlen(v_words[i].word) > 2) continue;
      if (strlen(v_words[i].word) < 1) continue;      /* accidental nulls    */
      x_1st = v_words[i].word[0];
      x_2nd = v_words[i].word[1];
      if (x_2nd == '\0') x_2nd = ' ';
      assign_shortcut(x_1st, x_2nd, i, '@');
      ++g_short;
      /*> printf("short word = <<%s>>\n", v_words[_i].word);                          <*/
   }
   VERBOSE printf("%d shorts (@)\n", g_short);
   return 0;
}

int assign_word(int a_index)
{
   //---(word variables)-------------------------#
   char _word[15];
   strncpy(_word, v_words[a_index].word, 15);
   int _len = strlen(_word);
   //---(letter iterators)-----------------------#
   //printf("   TESTING = %s\n", _word);
   int _i = 0;   // 1st char
   int _j = 0;   // 2nd char
   int _rc = 0;  // return code
   /*---(try one letter options)-----------------*/
   if (_len <= 2) return 1;
   for (_i = 0; _i <= _len - 1; ++_i) {
      _rc = assign_shortcut(_word[_i], ' ', a_index, '+');
      if (_rc == 0) {
         //printf("      found as %c\n", _word[_i]);
         return 0;
      }
   }
   for (_i = 'a'; _i <= 'z'; ++_i) {
      _rc = assign_shortcut(_i, ' ', a_index, '!');
      if (_rc == 0) {
         //printf("      found as %c\n", _i);
         return 0;
      }
   }
   /*---(switch to two letter options)-----------*/
   if (_len <= 3) return 1;
   for (_i = 0; _i <= _len - 1; ++_i) {
      for (_j = _i + 1; _j <= _len; ++_j) {
         if (_i == 0 && _j == 1) {
            _rc = assign_shortcut(_word[_i], _word[_j], a_index, '*');
            if (_rc == 0) {
               ++g_perfect;
               return 0;
            }
         } else {
            _rc = assign_shortcut(_word[_i], _word[_j], a_index, '+');
            if (_rc == 0) return 0;
         }
      }
   }
   //---(complete)-------------------------------#
   return 1;
}

int assign_by_letters()
{
   g_letters = 0;
   int _rc = 0;
   int _i = 0;               // word iterator
   g_perfect = 0;
   for (_i = 0; _i < v_nwords; ++_i) {
      if (v_words[_i].sc < 0) {
         _rc = assign_word(_i);
         if (_rc == 0) ++g_letters;
      }
   }
   VERBOSE printf("   4. perfect matches ........................ ");
   VERBOSE printf("%d assigned (*)\n", g_perfect);
   VERBOSE printf("   5. assign by letters ...................... ");
   VERBOSE printf("%d assigned (+)\n", g_letters - g_perfect);
   return 0;
}

int force_with_first_letter (void)
{
   VERBOSE printf("   6. forced with one-letter ................. ");
   //---(word variables)-------------------------#
   char x_word[15];
   g_first = 0;
   int  i = 0;               // word iterator
   int  j = 0;               // alpha iterator
   int  x_rc = 0;              // return code
   for (i = 0; i <= v_nwords; ++i) {
      if (v_words[i].sc >= 0)           continue;
      strncpy(x_word, v_words[i].word, 15);
      int x_len = strlen(x_word);
      if (x_len <= 3)                   continue;
      for (j = 'a'; j <= 'z'; ++j) {
         x_rc = assign_shortcut(x_word[0], j, i, '1');
         if (x_rc != 0) continue;
         ++g_first;
         break;
      }
   }
   VERBOSE printf("%d assigned (1)\n", g_first);
   return 0;
}

int force_with_any_letter (void)
{
   VERBOSE printf("   7. forced with any letter ................. ");
   //---(word variables)-------------------------#
   char x_word[15];
   g_any = 0;
   int  i = 0;               // word iterator
   int  j = 0;               // alpha iterator
   int  k = 0;               // letter iterator
   int  x_rc = 0;              // return code
   for (i = 0; i <= v_nwords; ++i) {
      if (v_words[i].sc >= 0)           continue;
      strncpy(x_word, v_words[i].word, 15);
      int x_len = strlen(x_word);
      if (x_len <= 3)                   continue;
      for (j = 'a'; j <= 'z'; ++j) {
         for (k = 1; k <= x_len; ++k) {
            x_rc = assign_shortcut(j, x_word[k], i, '2');
            if (x_rc != 0) continue;
            ++g_any;
            break;
         }
         if (x_rc == 0) break;
      }
   }
   VERBOSE printf("%d assigned (2)\n", g_any);
   return 0;
}

int force_remaining (void)
{
   VERBOSE printf("   8. assign by pure sequence ................ ");
   g_seq = 0;
   int _i = 0;               // word iterator
   int _j = 0;               // word iterator
   int _rc = 0;               // word iterator
   //for (_i = 0; _i < v_nwords; ++_i) {
   for (_i = 0; _i < v_nwords; ++_i) {
      if (v_words[_i].sc < 0) {
         if (strlen(v_words[_i].word) > 3) {
            for (_j = 0; _j < v_nshort; ++_j) {
               _rc = assign_shortcut(v_short[_j].sc[0], v_short[_j].sc[1], _i, '#');
               if (_rc == 0) {
                  ++g_seq;
                  break;
               }
            }
         } else {
            /*> printf("     SKIPPING %s\n", v_words[_i].word);                       <*/
            ++g_skipped;
         }
      }
   }
   VERBOSE printf("%d assigned (#)\n", g_seq);
   VERBOSE printf("   9. and, skipped (too short) ............... ");
   VERBOSE printf("%d skipped\n", g_skipped);
   return 0;
}





/*===========================--------------------=============================*/
/*====---                          printing                                   */
/*===========================--------------------=============================*/

int print_word(int a_word)
{
   int sc = v_words[a_word].sc;
   printf("%3d = %-15s :: ", a_word + 1, v_words[a_word].word);
   if (sc >= 0)  printf("%c%c :: %c\n", v_short[sc].sc[0], v_short[sc].sc[1], v_words[a_word].how);
   else          printf("-- ::\n");
   return 0;
}

int print_shortcut(int a_shortcut)
{
   if (a_shortcut >= 0) {
      printf("   %c%c :: ",
            v_short[a_shortcut].sc[0], v_short[a_shortcut].sc[1]);
   } else {
      printf("   -- :: ");
   }
   printf("%-15s (%4d)",
         v_words[v_short[a_shortcut].word].word, v_short[a_shortcut].word);
   if (v_short[a_shortcut].word >= 0)
      printf(" %c\n", v_words[v_short[a_shortcut].word].how);
   else
      printf("\n");
   return 0;
}

int print_shortcut_petal(int a_shortcut)
{
   printf("   { \"%c%c\", ",
         v_short[a_shortcut].sc[0], v_short[a_shortcut].sc[1]);
   printf("\"%s\" },\n",
         v_words[v_short[a_shortcut].word].word);
   return 0;
}

int print_shortcut_traditional(int a_shortcut)
{
   printf("%c%c - ",
         v_short[a_shortcut].sc[0], v_short[a_shortcut].sc[1]);
   printf("%s\n",
         v_words[v_short[a_shortcut].word].word);
   return 0;
}

int print_all_words (void)
{
   printf("\nprint all words in freq order ..................................\n");
   int _i = 0;               // word iterator
   char x_1st = ' ';
   char x_2nd = ' ';
   for (_i = 0; _i < v_nwords; ++_i) {
      x_1st = ' ';
      x_2nd = ' ';
      if (v_words[_i].sc >= 0) {
         x_1st = v_short[v_words[_i].sc].sc[0];
         x_2nd = v_short[v_words[_i].sc].sc[1];
         if (strlen(v_words[_i].word) == 1) x_2nd = ' ';
      }
      printf("%-15s : %c%c : %c : %4d\n",
            v_words[_i].word, x_1st, x_2nd,
            v_words[_i].how, v_words[_i].sc + 1);
   }
   printf("................................................................\n");
   return 0;
}

int iterate_shortcuts()
{
   int _i = 0;               // word iterator
   int x_count = 0;
   printf("\nprint top 100 in freq order ....................................\n");
   x_count = 0;
   for (_i = 0; _i < 100; ++_i) {
      print_word(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint top 100 in shortcut order ................................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0)   continue;
      if (v_short[_i].word > 100) continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned by letters (no forces).............\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how == '#') continue;
      if (v_words[v_short[_i].word].how == '1') continue;
      if (v_words[v_short[_i].word].how == '2') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned to exact matches ..................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how != '@') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned by perfect match ..................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how != '*') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned by two letters (non-perfect) ......\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how != '+') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned by first letters ..................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how != '1') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all shortcuts assigned by any letters ....................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      if (v_short[_i].word < 0) continue;
      if (v_words[v_short[_i].word].how != '2') continue;
      print_shortcut(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   printf("\nprint all stortcuts in alpha order .............................\n");
   x_count = 0;
   for (_i = 0; _i < v_nshort; ++_i) {
      print_shortcut_traditional(_i);
      ++x_count;
   }
   printf("   TOTAL %d\n", x_count);
   return 0;
}



int print_quicksheet(char a_type)
{
   int col;
   int row;
   int x_sc;
   int x_word;
   int x_maxrow = 63;
   int x_maxcol = 12;
   int x_total = g_short + g_letters + g_first + g_any + g_seq;
   printf("hublin (v06, 2009-11) :: a system of logical, alphabetic keyboard shortcuts for 650 of the 800 most frequently used american english words.");
   if (a_type == 's')      printf("       BY SHORTCUT ORDER          ");
   else if (a_type == 'w') printf("       BY WORD ALPHA ORDER        ");
   else                    printf("       BY WORD RANK ORDER         ");
   printf("based on yublin by Jon Aquino\n");
   for (row = -1; row <= x_maxrow; ++row) {
      for (col = 0; col < x_maxcol; ++col) {
         if (row == -1) {
            printf("-- ------------  ");
            continue;
         }
         if (row == x_maxrow) {
            printf("-- ------------  ");
            continue;
         }
         if (a_type == 's') {
            if (col == 0) { 
               if (row < 26) {
                  printf("%c  %-11.11s   ", v_short[row].sc[0], v_words[v_short[row].word].word);
                  continue;
               }
               switch (row) {
               case  30: printf("assign...        ");             break;
               case  31: printf("%3d a-z SC       ", 26);         break;
               case  32: printf("%3d aa-zz SC     ", 26 * 26);    break;
               case  33: printf("%3d SLOTS        ", 26 + (26 * 26));         break;
               case  34: printf("%3d direct       ", g_short);    break;
               case  35: printf("%3d perfect      ", g_perfect);  break;
               case  36: printf("%3d letters      ", g_letters - g_perfect);  break;
               case  37: printf("%3d first        ", g_first);    break;
               case  38: printf("%3d non-first    ", g_any);      break;
               case  39: printf("%3d by seq       ", g_seq);      break;
               case  40: printf("%3d TOTAL        ", x_total);    break;
               case  41: printf("%3d too short    ", g_skipped);  break;
               case  42: printf("%3d GRAND        ", x_total + g_skipped);    break;
               case  50: printf("kluges...        ");             break;
               case  51: printf("can/one swap     ");             break;
               default : printf("                 ");             break;
               }
            } else {
               x_sc = ((col - 1) * x_maxrow + row) + 26;
               if (x_sc < v_nshort)  printf("%c%c %-11.11s   ", v_short[x_sc].sc[0], v_short[x_sc].sc[1], v_words[v_short[x_sc].word].word);
               else                  printf("                 ");
            }
         } else if (a_type == 'w') {
            x_sc = (col * x_maxrow + row);
            x_word = v_sorted[x_sc];
            if (x_sc < v_nshort)  printf("%c%c %-11.11s   ", v_short[x_word].sc[0], v_short[x_word].sc[1], v_words[v_short[x_word].word].word);
            else                  printf("                 ");
         } else {
            x_sc   = (col * x_maxrow + row);
            x_word = x_sc;
            if (x_sc < v_nwords) {
               if (v_words[x_word].sc >= 0) printf("%c%c %-11.11s   ", v_short[v_words[x_word].sc].sc[0], v_short[v_words[x_word].sc].sc[1], v_words[x_word].word);
               else                         printf(  "-- %-11.11s   ", v_words[x_word].word);
            }
            else                  printf("                 ");
         }
      }
      printf("\n");
   }
   printf("principle :: 1) use frequency ranked list, 2) assign 1-2 letter words as is, 3) match by letters in words, 4) force using one letter, 5) randomly assign, 6) skip if less than two letters saved        \n");
   return 0;
}


int print_petal()
{
   int i = 0;               /* first letter iterator                          */
   int j = 0;               /* second letter iterator                         */
   printf("struct {\n");
   printf("   char   abbr[5];\n");
   printf("   char   word[15];\n");
   printf("} hublin[1000] = {\n");
   for (i = 0; i < 26; ++i) {
      for (j = 0; j < 27; ++j) {
         if (j == 0) {
            print_shortcut_petal(i);
            continue;
         }
         print_shortcut_petal((i + 1) * 26 + (j - 1));
      }
   }
   printf("};\n");
   printf("\n");
   printf("struct {\n");
   printf("   char   abbr[5];\n");
   printf("   char   word[15];\n");
   printf("} hublin_alpha[1000] = {\n");
   for (i = 0; i < v_nshort; ++i) {
      j = v_sorted[i];
      print_shortcut_petal(j);
   }
   printf("};\n");
   return 1;
}

int sort_words()
{
   int i = 0;                /* word iterator       */
   int j = 0;                /* word iterator       */
   int tmp = 0;              /* temp value          */
   char word1[15] = "";
   char word2[15] = "";
   for (i = 0; i < v_nwords; ++i) {
      v_sorted[i] = i;
   }
   /*---(bubble sort)-------------------*/
   for (i = v_nshort - 2; i > 0; --i) {
      /*> printf("COUNT DOWN %03d\n", i);                                             <*/
      for (j = v_nshort - 2; j > 0; --j) {
         strncpy(word1, v_words[v_short[v_sorted[j]].word].word,     15);
         strncpy(word2, v_words[v_short[v_sorted[j + 1]].word].word, 15);
         /*> printf("checking %03d <<%-15.15s>> is less than %03d <<%-15.15s>>  ", j, word1, j + 1, word2);   <*/
         if (strcmp(word1, word2) > 0) {
            tmp             = v_sorted[j];
            v_sorted[j]     = v_sorted[j + 1];
            v_sorted[j + 1] = tmp;
            strncpy(word1, v_words[v_short[v_sorted[j]].word].word,     15);
            strncpy(word2, v_words[v_short[v_sorted[j + 1]].word].word, 15);
            /*> printf("SWAPPED, now %03d <<%-15.15s>> is less than %03d <<%-15.15s>>\n", j, word1, j + 1, word2);   <*/
         }
      }
   }
   /*---(complete)----------------------*/
   return 0;
}



int main (int argc, char *argv[])
{
   if (argc > 1) g_verbose = 'n';
   VERBOSE printf("------------------------------------------------------begin---\n");
   VERBOSE printf("hublin -- keyboard short-cut generator...\n");
   load_words();
   generate_shortcut_placeholders();
   assign_short_words();
   g_skipped = 0;
   assign_by_letters();
   force_with_first_letter();
   force_with_any_letter();
   force_remaining();
   sort_words();
   /*---(hateful forcing)---------------*/
   v_short[ 2].word = v_short[15].word;
   v_short[15].word = 23;
   v_words[23].sc = v_words[35].sc;
   v_words[35].sc = 2;
   /*---(end hateful forcing)-----------*/
   if (argc > 1) {
      if (strcmp(argv[1], "-s") == 0) print_quicksheet('s');
      if (strcmp(argv[1], "-w") == 0) print_quicksheet('w');
      if (strcmp(argv[1], "-r") == 0) print_quicksheet('r');
      if (strcmp(argv[1], "-p") == 0) print_petal();
      if (strcmp(argv[1], "-a") == 0) {
         print_quicksheet('s');
         print_quicksheet('w');
         print_quicksheet('r');
      }
   } else {
      iterate_shortcuts();
      print_all_words();
   }
   /*> char x_word[15]  = "";                                                         <* 
    *> char x_abbrev[5] = "";                                                         <* 
    *> strncpy(x_word, "type",          15);                                          <* 
    *> hublin_reverse(x_word, x_abbrev);                                              <* 
    *> printf("%s = %s\n", x_word, x_abbrev);                                         <* 
    *> strncpy(x_word, "love",          15);                                          <* 
    *> hublin_reverse(x_word, x_abbrev);                                              <* 
    *> printf("%s = %s\n", x_word, x_abbrev);                                         <* 
    *> strncpy(x_word, "process",       15);                                          <* 
    *> hublin_reverse(x_word, x_abbrev);                                              <* 
    *> printf("%s = %s\n", x_word, x_abbrev);                                         <* 
    *> strncpy(x_word, "anarchy",       15);                                          <* 
    *> hublin_reverse(x_word, x_abbrev);                                              <* 
    *> printf("%s = %s\n", x_word, x_abbrev);                                         <*/
   VERBOSE printf("--------------------------------------------------------end---\n");
   return 0;
}



//=============================================================================#
//===[[ ASSIGN WORDS ]]========================================================#
//=============================================================================#









