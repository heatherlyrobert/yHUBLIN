#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = yHUBLIN
# additional standard and outside libraries
LIB_STD    = 
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = 
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = yHUBLIN_show
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#


#*============================----beg-of-source---============================*#

##*---(standard variables)-------------*#
#COMP    = gcc -c -std=c99 -g -pg -Wall -Wextra
#INCS    =
#LINK    = gcc
#LIBS    = 
#COPY    = cp -f
#CLEAN   = rm -f
#ECHO    = echo
#
#all                : yHUBLIN.so.1.0 yHUBLIN.a
#
#yHUBLIN.so.1.0     : yHUBLIN.o
#	${LINK}  -shared -Wl,-soname,libyHUBLIN.so.1  ${LIBS}  -o libyHUBLIN.so.1.0  yHUBLIN.o
#
#yHUBLIN.a          : yHUBLIN.o
#	ar       rcs  libyHUBLIN.a  yHUBLIN.o
#
#yHUBLIN.o          : yHUBLIN.h yHUBLIN.c
#	${COMP}  -fPIC ${INCS} yHUBLIN.c
#
##*---(utilities)----------------------*#
#clean              :
#	${CLEAN} *.o
#	${CLEAN} *~
#	${CLEAN} temp*
#
#install            :
#	_lib -s yHUBLIN
#	_lib -a yHUBLIN
#	ldconfig
#
#remove             :

#*============================----end-of-source---============================*#
