VERSION=$1
TYPE=$2
BUILDARCH=`uname -a | awk {'print $13'}`

#rm -rf ${HOME}/netsurf-$VERSION

if [ -z $1 ] || [ -z $2 ];then
	echo "----------------------------------------------------------------------"
	echo "use $HOME/build.sh <2.8|2.9|3.0|3.1|3.2|git|svn> <mipsel|sh4|$BUILDARCH>"
	echo "----------------------------------------------------------------------"
	exit
fi

if [ "$VERSION" = "2.8" ] && [ ! -e "$HOME/src/netsurf-$1.tar.gz" ];then
	wget http://download.netsurf-browser.org/netsurf/releases/source-full/netsurf-2.8-full-src.tar.gz -O $HOME/src/netsurf-$1.tar.gz
fi

if [ "$VERSION" = "2.8" ] && [ ! -e "$HOME/netsurf-$1" ];then
	tar -zxvf $HOME/src/netsurf-$1.tar.gz -C $HOME
fi

###

if [ "$VERSION" = "2.9" ] && [ ! -e "$HOME/src/netsurf-$1.tar.gz" ];then
	wget http://download.netsurf-browser.org/netsurf/releases/source-full/netsurf-2.9-full-src.tar.gz -O $HOME/src/netsurf-$1.tar.gz
fi

if [ "$VERSION" = "2.9" ] && [ ! -e "$HOME/netsurf-$1" ];then
	tar -zxvf $HOME/src/netsurf-$1.tar.gz -C $HOME
fi

###

if [ "$VERSION" = "3.0" ] && [ ! -e "$HOME/src/netsurf-$1.tar.gz" ];then
	wget http://download.netsurf-browser.org/netsurf/releases/source-full/netsurf-3.0-full-src.tar.gz -O $HOME/src/netsurf-$1.tar.gz
fi

if [ "$VERSION" = "3.0" ] && [ ! -e "$HOME/netsurf-$1" ];then
	tar -zxvf $HOME/src/netsurf-$1.tar.gz -C $HOME
	mv $HOME/netsurf-full-3.0 $HOME/netsurf-$1
fi

###

if [ "$VERSION" = "3.1" ] && [ ! -e "$HOME/src/netsurf-$1.tar.gz" ];then
	wget http://download.netsurf-browser.org/netsurf/releases/source-full/netsurf-all-3.1.tar.gz -O $HOME/src/netsurf-$1.tar.gz
fi

if [ "$VERSION" = "3.1" ] && [ ! -e "$HOME/netsurf-$1" ];then
	tar -zxvf $HOME/src/netsurf-$1.tar.gz -C $HOME
	mv $HOME/netsurf-all-3.1 $HOME/netsurf-$1
fi

###

if [ "$VERSION" = "3.2" ] && [ ! -e "$HOME/src/netsurf-$1.tar.gz" ];then
	wget http://download.netsurf-browser.org/netsurf/releases/source-full/netsurf-all-3.2.tar.gz -O $HOME/src/netsurf-$1.tar.gz
fi

if [ "$VERSION" = "3.2" ] && [ ! -e "$HOME/netsurf-$1" ];then
	tar -zxvf $HOME/src/netsurf-$1.tar.gz -C $HOME
	mv $HOME/netsurf-all-3.2 $HOME/netsurf-$1
fi

###

if [ "$VERSION" = "git" ] && [ ! -e "$HOME"/titandev/netsurf-git ];then
	REPOLIST="buildsystem libwapcaplet nsgenbind libnsfb libparserutils libcss libdom libhubbub libnsgif libnsbmp libsvgtiny librosprite libutf8proc librufl libnsutils libnsfb netsurf"
	
	mkdir ${HOME}/netsurf-$VERSION
	cd ${HOME}/netsurf-$VERSION
		
	for REPO in $REPOLIST; do
		echo git clone git://git.netsurf-browser.org/$REPO.git
		git clone git://git.netsurf-browser.org/$REPO.git
	done
fi

###
echo 11

if [ "$VERSION" = "svn" ] && [ ! -e "$HOME"/titandev/netsurf-svn ];then
echo 22
	mkdir ${HOME}/netsurf-$VERSION
	cd ${HOME}/netsurf-$VERSION
	git clone git://git.netsurf-browser.org/buildsystem.git
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/hubbub-0.1.1 libhubbub
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libcss-0.1.1 libcss
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libnsbmp-0.0.3 libnsbmp
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libnsfb-0.0.1 libnsfb
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libnsgif-0.0.3 libnsgif
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libparserutils-0.1.1 libparserutils
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libpencil-0.0.1 libpencil
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/librosprite-0.0.1 librosprite
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/librufl-0.0.1 librufl
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libsvgtiny-0.0.1 libsvgtiny
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/libwapcaplet-0.1.0 libwapcaplet
	svn checkout http://sbnc.dyndns.tv/svn/titan/netsurf/netsurf-2.8 netsurf
	svn export http://sbnc.dyndns.tv/svn/titan/netsurf/Makefile Makefile

fi

############

if [ "$VERSION" != "2.8" ] && [ "$VERSION" != "2.9" ];then
	rm -rf "$HOME"/netsurf-$VERSION/build-Linux-framebuffer
	# Make the workspace directory
	mkdir -p ${HOME}/netsurf-$VERSION/inst
	# Change to it
	cd ${HOME}/netsurf-$VERSION
	# Prepare environment script
cat > env.sh <<'EOF'
export PKG_CONFIG_PATH=${HOME}/netsurf-$VERSION/inst/lib/pkgconfig::
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/netsurf-$VERSION/inst/lib
export PREFIX=${HOME}/netsurf-$VERSION/inst
EOF
	# Change to workspace
	cd ${HOME}/netsurf-$VERSION
	# Prepare shell environment
	source env.sh

	sed "s!NETSURF_USE_JS := AUTO!NETSURF_USE_JS := NO!g" -i $HOME/netsurf-$VERSION/netsurf/Makefile.defaults
	sed "s!NETSURF_USE_MOZJS := AUTO!NETSURF_USE_MOZJS := NO!g" -i $HOME/netsurf-$VERSION/netsurf/Makefile.defaults

elif [ "$VERSION" = "3.0" ] || [ "$VERSION" = "3.1" ] || [ "$VERSION" = "3.2" ]; then
	sed "s!NETSURF_USE_MNG := YES!NETSURF_USE_MNG := NO!g" -i $HOME/netsurf-$VERSION/netsurf-$VERSION/Makefile.defaults
	sed "s!NETSURF_FB_FRONTEND := sdl!NETSURF_FB_FRONTEND := linux!g" -i $HOME/netsurf-$VERSION/netsurf-$VERSION/Makefile.defaults
	cd "$HOME"/netsurf-$1
fi

############

rm -rf "$HOME"/netsurf-$VERSION/netsurf/build-*
rm -rf "$HOME"/netsurf-$VERSION/netsurf-$VERSION/build-*

if [ $TYPE = "mipsel" ];then
	STM=mips360
	DISTRO=atemio
	BOX=inihdp
	ARCH=`uname -a | awk {'print $13'}`-linux
	export CC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc
	export AR="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-ar
elif [ $TYPE = "sh4" ];then
	STM=stm24
	export CC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-gcc
	export AR="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-ar
else
	export CC=gcc
	export AR=ar
fi

echo "####################################################################"
echo CC $CC
ls $CC
echo AR $AR
ls $AR
echo PREFIX $PREFIX
ls $PREFIX
echo --------------------------------------------------------------------
echo PKG_CONFIG_PATH $PKG_CONFIG_PATH
ls $PKG_CONFIG_PATH
echo --------------------------------------------------------------------
echo PKG_CONFIG $PKG_CONFIG
ls $PKG_CONFIG
echo --------------------------------------------------------------------
echo CPPFLAGS $CPPFLAGS
ls $CPPFLAGS
echo "####################################################################"

#cp "$HOME"/netsurf-git/libnsfb/src/surface/linux.c.$TYPE "$HOME"/netsurf-git/libnsfb/src/surface/linux.c

#if [ "$VERSION" = "3.0" ] || [ "$VERSION" = "3.1" ] || [ "$VERSION" = "3.2" ] || [ "$VERSION" = "git" ] || [ "$VERSION" = "svn" ]; then
#if [ "$VERSION" = "all" ]; then
if [ "$VERSION" = "3.0" ] || [ "$VERSION" = "3.1" ] || [ "$VERSION" = "3.2" ] || [ "$VERSION" = "git" ]; then
	make -C buildsystem clean
	# Build the core libraries in turn (order matters due to dependencies)
	make -C libwapcaplet clean
	make -C libparserutils clean
	make -C libcss clean
	make -C libhubbub clean
	make -C libdom clean
	# Build the core image decoders
	make -C libnsbmp clean
	make -C libnsgif clean
	# Build the optional decoders
	make -C librosprite clean
	make -C libsvgtiny clean
	if [ "$VERSION" = "git" ]; then
		make -C librufl clean
		make -C libutf8proc clean
		make -C libnsutils clean
	fi
	make -C nsgenbind clean
	make -C libnsfb clean
	make TARGET=framebuffer clean

	make -C buildsystem install
	# Build the core libraries in turn (order matters due to dependencies)
	make -C libwapcaplet install
	make -C libparserutils install
	ls -al /home/titandev/netsurf-3.2/libcss
	make -C libcss install
	make -C libhubbub install
	make -C libdom install
	# Build the core image decoders
	make -C libnsbmp install
	make -C libnsgif install
	# Build the optional decoders
	make -C librosprite install
	make -C libsvgtiny install
	if [ "$VERSION" = "git" ]; then
		make -C librufl install
		make -C libutf8proc install
		make -C libnsutils install
	fi
	make -C nsgenbind install
	make -C libnsfb install
	
	if [ "$TYPE" = "$BUILDARCH" ] && [ "$VERSION" = "git" ];then
		cp ${HOME}/netsurf-git/inst/bin/nsgenbind /usr/bin/
	fi

	cd netsurf
	make TARGET=framebuffer
	cd ..
elif [ "$VERSION" = "2.8" ] || [ "$VERSION" = "2.9" ] || [ "$VERSION" = "svn" ]; then
#elif [ "$1" = "makefile" ]; then
	make TARGET=framebuffer clean
	make TARGET=framebuffer
	
	LIBNAME=`ls -1 | grep libnsfb`
	
	echo LIBNAME $LIBNAME 
	#exit
	make -C $LIBNAME TARGET=framebuffer clean
	make -C $LIBNAME TARGET=framebuffer

#if [ "$VERSION" = "2.8" ] || [ "$VERSION" = "2.9" ]; then	
	cp -a $LIBNAME/include/* /home/titandev/netsurf-$1/prefix-framebuffer/include/
	cat $HOME/netsurf-$1/$LIBNAME/libnsfb.pc.in | sed "s!prefix=PREFIX!prefix=$HOME/netsurf-$1/prefix-framebuffer!" > $HOME/netsurf-$1/prefix-framebuffer/lib/pkgconfig/libnsfb.pc
	cp $HOME/netsurf-$1/$LIBNAME/build-Linux-framebuffer-release-lib-static/libnsfb.a /home/titandev/netsurf-$1/prefix-framebuffer/lib

	ln -s "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/"$BOX"/usr/lib/pkgconfig/libpng16.pc $HOME/netsurf-$1/prefix-framebuffer/lib/pkgconfig/libpng.pc
	ln -s $HOME/netsurf-$1/prefix-framebuffer/lib/pkgconfig/libpng.pc $HOME/netsurf-$1/prefix-framebuffer/lib/pkgconfig/libpng16.pc
	ln -s "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/"$BOX"/usr/include/libpng15 $HOME/netsurf-$1/prefix-framebuffer/include/libpng15
	ln -s "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/"$BOX"/usr/lib/libpng.so $HOME/netsurf-$1/prefix-framebuffer/lib/libpng.so
	ln -s "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/"$BOX"/usr/lib/libpng.a $HOME/netsurf-$1/prefix-framebuffer/lib/libpng.a
	ln -s "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"/tmp/sysroots/"$BOX"/usr/lib/libpng.a $HOME/netsurf-$1/prefix-framebuffer/lib/libpng16.a

#fi	
	sleep 2	
	make TARGET=framebuffer
fi

if [ -e $HOME/netsurf-$VERSION/netsurf-"$VERSION"/nsfb ];then
	mv $HOME/netsurf-$VERSION/netsurf-"$VERSION"/nsfb $HOME/netsurf-"$VERSION"/netsurf-"$VERSION"/nsfb-$TYPE
elif [ -e $HOME/netsurf-"$VERSION"/netsurf/nsfb ];then
	mv $HOME/netsurf-"$VERSION"/netsurf/nsfb $HOME/netsurf-"$VERSION"/netsurf/nsfb-$TYPE
fi

echo -----------------------------------------------------------------------
if [ -e "$HOME/netsurf-$VERSION/netsurf-$VERSION/nsfb-$TYPE" ];then
	echo Build: $HOME/netsurf-$VERSION/netsurf-$VERSION/nsfb-$TYPE
elif [ -e "$HOME/netsurf-$VERSION/netsurf/nsfb-$TYPE" ];then
	echo Build: $HOME/netsurf-$VERSION/netsurf/nsfb-$TYPE
fi

echo -----------------------------------------------------------------------
echo List:
ls -1 $HOME/netsurf-$VERSION/netsurf*/nsfb*
echo -----------------------------------------------------------------------

echo "----------------------------------------------------------------------"
echo "use $HOME/build.sh <2.8|2.9|3.0|3.1|3.2|git|svn> <mipsel|sh4|$BUILDARCH>"
echo "----------------------------------------------------------------------"

