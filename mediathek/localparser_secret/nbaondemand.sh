#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
FROM=$3
PAGE=$4
FILENAME=`echo $PAGE | tr '/' '.'`

ARCH=`cat /etc/.arch`
URL=http://livetv.sx
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=NBA-On-Demand

wgetbin="wget -q -T2"
TMP=/tmp/parser
#TMP=/var/usr/local/share/titan/plugins/tithek/parser/tmp

#rm -rf $TMP > /dev/null 2>&1
mkdir $TMP > /dev/null 2>&1

if [ "$SRC" = "/mnt/parser" ];then
	TYPE="$SRC - Shell script"
elif [ `echo $SRC | grep ^"/var/swap" | wc -l` -gt 0 ];then
	TYPE="Swap - Shell script"
elif [ `echo $SRC | grep ^"/mnt/swapextensions" | wc -l` -gt 0 ];then
	TYPE="Mnt - Shell script"
else
	TYPE="Flash - Shell script"
fi

init()
{
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Suche#$SRC $SRC suche#http://atemio.dyndns.tv/mediathek/menu/search.jpg#giga.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	echo "Latest#$SRC $SRC listvideos latest en/videotourney/3#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Games#$SRC $SRC submenu games en/videotourney/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
#	echo "Highlights#$SRC $SRC submenu highlights en/videotourney/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Teams#$SRC $SRC submenu teams en/leagueresults/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
  	echo "$TMP/$PARSER.$INPUT.list"
}

listvideos()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed 's!height=27!\nfound=!g' | grep '^found=' | grep -v LiveTV >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL=`echo $ROUND | sed 's!<a href="/en/eventinfo/!\nurl="/en/eventinfo/!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed 's!width="30%"> <b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`
			SCORE=`echo $ROUND | sed 's!<font color="#949494"><b>!\nextra=<!g' | grep ^extra= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`			

			if [ `echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | grep "Full match record" | cut -d'"' -f2 | head -n1 | wc -l` -eq 0 ];then
				FULLVIDEO=""
			else
				FULLVIDEO="Full"
			fi

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			if [ "$FROM" = "latest" ];then
				EXTRA=`echo $ROUND | sed 's!data-pop="ddr_!\ndate=_!g' | grep ^date= | cut -d'_' -f2`
			else
				EXTRA=`echo $ROUND | sed 's!<span class="date">!\ndate=<!g' | grep ^date= | cut -d'<' -f2`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$SCORE" ];then
				TITLE="$TITLE ($SCORE)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$FULLVIDEO" ];then
				TITLE="$TITLE ($FULLVIDEO)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ "$URL" != "http://xlivetv.sx" ] && [ `cat $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list | grep "#$URL#" | wc -l` -eq 0 ];then
				piccount=`expr $piccount + 1`

				URL="$SRC $SRC playvideos $FROM $URL"

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

playvideos()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<td width="33%">!\nfound=!g' | sed 's!<br> </td>!\n<br> </td>!g' | grep '^found=' >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		while read -u 3 ROUND; do

			URL=http://xlivetv.sx`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed 's!<a alt="!\ntitle="!g' | grep ^title= | cut -d'"' -f2 | sed 's!&ndash;!-!g'`

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ "$URL" != "http://xlivetv.sx" ] && [ `cat $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list | grep "#$URL#" | wc -l` -eq 0 ];then
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#98"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

submenu()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
		piccount=0

		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1
#		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/en/team/!\n<a href="/en/team/!g' | grep '^<a href="/en/team/' | tr ' ' '~' >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/en/team/!\n<a href="/en/team/!g' | grep '^<a href="/en/team/' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL="$SRC $SRC listvideos $FROM `echo $ROUND | cut -d'"' -f2 | sed 's!/calendar/!/video/!g'`"
			TITLE=`echo $ROUND | sed 's!<b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
	
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				piccount=$[$piccount+1]
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	listvideos) $INPUT;;
	playvideos) $INPUT;;
	submenu) $INPUT;;
esac