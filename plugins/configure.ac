AC_INIT(tuxbox-apps-titan-plugins,0.0.1,,tuxbox-titan-plugins)
AM_INIT_AUTOMAKE(tuxbox-titan-plugins,0.0.1)

TUXBOX_APPS

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL

AC_ARG_ENABLE([multicom322], 
        [AS_HELP_STRING(--enable-multicom322,    enable multicom322 stuff)], 
        [case "${enableval}" in 
                yes) enable_multicom322=true ;; 
                no) enable_multicom322=false ;; 
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-multicom322]) ;; 
        esac]) 
AM_CONDITIONAL([ENABLE_MULTICOM322], [test x$enable_multicom322 = xtrue])
AC_SUBST(MULTICOM322)

AC_ARG_ENABLE([multicom324], 
        [AS_HELP_STRING(--enable-multicom324,    enable multicom324 stuff)], 
        [case "${enableval}" in 
                yes) enable_multicom324=true ;; 
                no) enable_multicom324=false ;; 
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-multicom324]) ;; 
        esac]) 
AM_CONDITIONAL([ENABLE_MULTICOM324], [test x$enable_multicom324 = xtrue])
AC_SUBST(MULTICOM324)



AC_OUTPUT([
Makefile
autotimer/Makefile
browser/Makefile
callmonitor1/Makefile
catcatch/Makefile
dlna/Makefile
dvdplayer/Makefile
facebook/Makefile
filemanager/Makefile
gmediarender/Makefile
hello/Makefile
imdb/Makefile
imdbapi/Makefile
instar/Makefile
keylock/Makefile
kravencfg/Makefile
lcdpearl1/Makefile
lcdsamsung/Makefile
mboxinfo/Makefile
mc/Makefile
networkbrowser/Makefile
optimize/Makefile
panel/Makefile
permtime/Makefile
readerconfig/Makefile
rgui/Makefile
scriptexec/Makefile
stock/Makefile
stopifnotused/Makefile
streaminfo/Makefile
tinews/Makefile
tithek/Makefile
tiwakeup/Makefile
tmc/Makefile
tmdb/Makefile
TopfieldVFD/Makefile
tsSchnitt/Makefile
sudoku/Makefile
usbreset/Makefile
weather/Makefile
wins3/Makefile
wm2014/Makefile
xupnpd/Makefile
zapback/Makefile
])