#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "tithek.h"

char pluginname[] = "TiTan Mediathek";
char plugindesc[] = "TiTan Mediathek";
char pluginpic[] = "%pluginpath%/tithek/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/tithek/skin.xml", 0, 0);
	readscreen(tmpstr, 195, 1);
	free(tmpstr); tmpstr = NULL;

	mkdir("/tmp/tithek", 777);
	pluginaktiv = 1;
	debug(10, "TiThek Info Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(195);
	system("rm -r /tmp/tithek");
	pluginaktiv = 0;
	debug(10, "TiThek Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	//with normal menu
	//screentithekmenu("http://atemio.dyndns.tv/mediathek/mainmenu.list");
	
	//with grid menu
	screentithekplay("http://atemio.dyndns.tv/mediathek/mainmenu.list", 1);
}
