#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "imdb.h"

char pluginname[] = "Imdb";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/panel/plugin.png";

int pluginaktiv = 0;
struct skin* pluginmenu = NULL;
int pluginflag = 0; //don't show the plugin in pluginmanager

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	struct skin* mainmenu = getscreen("mainmenu");
	struct skin* listbox = getscreennode(mainmenu, "listbox");
	struct skin* child = NULL;

	if(mainmenu == NULL || listbox == NULL)
		return;

	child = getscreennode(mainmenu, "imdb");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/imdb/skin.xml", 0, 0);
	readscreen(tmpstr, 201, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "Panel Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(201);
	pluginaktiv = 0;
	debug(10, "Imdb Plugin removed !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	imdb();
}
