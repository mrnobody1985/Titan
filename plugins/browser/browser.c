#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

char pluginname[] = "Internet Browser";
char plugindesc[] = "Internet Browser";
char pluginpic[] = "%pluginpath%/browser/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "Browser Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "Browser Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	char* tmpstr = NULL;
	status.sec = 0; //deaktivate spinner
	setfbtransparent(255);
	
	tmpstr = ostrcat("nsfb -f linux ", getconfig("browserhome", NULL), 0, 0);
	system(tmpstr);
	free(tmpstr);
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
}
