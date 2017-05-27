
/*
 *   bs = past tense              = ed, d, or irregular form
 *   a  = office or function      = ate, ite
 *   t  = become like             = ize, ise, itize
 *   n  = continuous activity     = ing
 *   sp = plural                  = s, es
 *   i  = make, do                = fy, ify
 *   e  = person or agent         = en, ent, ant
 *   o  = act, process            = tion, sion
 *
 *   h  = having quality of       = ish
 *   m  = product, act, means     = ment, mant
 *   s  = without, missing        = less
 *   f  = full of                 = ful, fil
 *   r  = doer of, practitioner   = er, ar, eer, or, ist, ian, cian
 *   u  = characterized by        = ous, eous, ious, ose
 *   p  = having, abstract        = y
 *   l  = like, extent of         = ly, ily, ally
 *   d  = state of                = ty, ity
 *   g  = belongs to              = age
 *   w  =                         = ship
 *   c  = state or quality        = ence, ance, ancy, ency, cy, acy
 *
 *   q  = act, condition          = ism
 *   k  = having nature of        = ic, ac, ick, fic
 *   x  = state of                = ness
 *   y  =                         = ry, ary, ory, ery
 *   j  = has property of         = al, il, ile, ial
 *   b  = capable of being        = able, ible, ble
 *   z  =                         = oid
 *   v  = adjective               = ive, ative, itive
 *
 *
 *
 *
 */
/*> #define  DEBUG_P   if (0)                                                         <*/

#define     YHUBLIN_VER_NUM   "1.0a"
#define     YHUBLIN_VER_TXT   "moved to make_program makefile enhancement"

#define  MAXABBR        5 
#define  MAXFULL       20 
#define  MAXLETTER     40 
#define  MAXSINGLE     30
#define  MAXDOUBLE   1000
#define  MAXTRIPLE   4000

char        hublin_single         (char*, char*);
char        hublin_double         (char*, char*);
char        hublin_triple         (char*, char*);

char        hublin_mysingle       (char, char*, char*);
char        hublin_mydouble       (char, char*, char*);
char        hublin_mytriple       (char, char*, char*);

char        hublin_next           (char*, char*, char*);
char        hublin_reverse        (char*, char*);

