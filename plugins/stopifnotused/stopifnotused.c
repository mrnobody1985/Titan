#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"
#include "../../titan/list.h"


char pluginname[] = "Stop if not used";
char plugindesc[] = "Auschalten der Box wenn in einem bestimmten Zeitraum keine Aktion erfolgte";
char pluginpic[] = "%pluginpath%/stopifnotused/stopifnotused.png";

int pluginaktiv = 0;

struct stimerthread* StopIfNotUsed = NULL;


void StopIfNotUsed_thread()
{
	int tostop = 0;
	while (StopIfNotUsed->aktion != STOP)
	{
		if(getconfig("stopifnotused_timetostop", NULL) == NULL)
			tostop = 120*60;
		else
			tostop = atoi(getconfig("stopifnotused_timetostop", NULL)) * 60;
		
		if((time(NULL) - status.lastrcaction) >= tostop && status.recording <= 0)
		{
			if (textbox(_("Message"), _("You have executed a defined period of no function.\n\nStop the box??? "), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 650, 200,15, 0) != 2)
			{
				oshutdown(1, 1);
				break;
			}
		} 
	sleep(10);	
	}
	StopIfNotUsed = NULL;
}

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/stopifnotused/skin.xml", 0, 0);
	readscreen(tmpstr, 117, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "Stop if not used Plugin loadet !!!");
	if(ostrcmp(getconfig("stopifnotused_autostart", NULL), "ja")  == 0)
		StopIfNotUsed = addtimer(&StopIfNotUsed_thread, START, 10000, 1, NULL, NULL, NULL);
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	char* tmpstr = NULL;
	
	pluginaktiv = 0;
	debug(10, "Stop if not used removed !!!");
	free(tmpstr); tmpstr = NULL;
	delmarkedscreen(117);
}

	//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	int rcret = 0;
	int running = 0;
		
	struct skin* notused_main = getscreen("notused_main");
	struct skin* listbox = getscreennode(notused_main, "listbox");
	struct skin* autostart = getscreennode(notused_main, "autostart");
	struct skin* timetostop = getscreennode(notused_main, "timetostop");
	struct skin* startstop = getscreennode(notused_main, "b3");
	struct skin* tmp = NULL;
	
	
	addchoicebox(autostart, "ja", _("yes"));
	addchoicebox(autostart, "nein", _("no"));
	if(getconfig("stopifnotused_autostart", NULL) == NULL)
		setchoiceboxselection(autostart, "nein");
	else
		setchoiceboxselection(autostart, getconfig("stopifnotused_autostart", NULL));
	
	addchoicebox(timetostop, "30", _("30"));
	addchoicebox(timetostop, "60", _("60"));
	addchoicebox(timetostop, "90", _("90"));
	addchoicebox(timetostop, "120", _("120"));
	addchoicebox(timetostop, "180", _("180"));
	addchoicebox(timetostop, "240", _("240"));
	if(getconfig("stopifnotused_timetostop", NULL) == NULL)
		setchoiceboxselection(timetostop, "120");
	else
		setchoiceboxselection(timetostop, getconfig("stopifnotused_timetostop", NULL));
	
	if(StopIfNotUsed == NULL)
		running = 0;
	else
		running = 1;
		
	if(running == 0)
		changetext(startstop, "Start");
	else
		changetext(startstop, "Stop");
	
	drawscreen(notused_main, 0);
	addscreenrc(notused_main, listbox);
	
	tmp = listbox->select;
	
	while(1)
	{
		addscreenrc(notused_main, tmp);
		rcret = waitrc(notused_main, 0, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("stopifnotused_autostart", autostart->ret);
			addconfig("stopifnotused_timetostop", timetostop->ret);
			break;
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(running == 0)
				StopIfNotUsed = addtimer(&StopIfNotUsed_thread, START, 10000, 1, NULL, NULL, NULL);
			else
				StopIfNotUsed->aktion = STOP;
			break;
		}
	}
		
	
	delownerrc(notused_main);
	clearscreen(notused_main);
	
}
