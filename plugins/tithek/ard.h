#ifndef ARD_H
#define ARD_H

char* ard(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL, *nummer = NULL;
	char* tmpstr2 = NULL, *title = NULL, *pic = NULL, *part1 = NULL, *part2 = NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	titheklog(debuglevel, "/tmp/tithek/ard1_tmpstr", NULL, tmpstr);
//	writesys("/var/usr/local/share/titan/plugins/tithek/ard1_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<div class=\"fsk\">") != NULL)
		{
			char* tmpstr2 = string_resub("<div class=\"fsk\">", "</div>", tmpstr, 0);
			tmpstr2 = string_replace(".", ".\n", tmpstr2, 1),
			textbox(_("Message"), tmpstr2, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
			free(tmpstr2), tmpstr2 = NULL;
		}

		tmpstr2 = string_resub("mediaCollection.setPreviewImage", "playerConfiguration = new PlayerConfiguration();", tmpstr, 0);
		titheklog(debuglevel, "/tmp/tithek/ard2_tmpstr2", NULL, tmpstr2);

//		writesys("/var/usr/local/share/titan/plugins/tithek/ard1_tmpstr2", tmpstr2, 0);

		int count = 0, i = 0;	
 		struct splitstr* ret1 = NULL;
 		struct menulist* mlist = NULL, *mbox = NULL;
		ret1 = strsplit(string_decode(tmpstr2,0), "\n", &count);
		for(i = 0; i < count; i++)
		{
			if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
			{
				debug(99, "Round (%d) %s",i, strstrip(ret1[i].part));
				streamurl = string_resub("<url>", "</url>", ret1[i].part,0);

				if(ostrstr(ret1[i].part, "mp4") != NULL)
					pic = ostrcat("mp4.png", NULL, 0, 0);
				else if(ostrstr(ret1[i].part, "3gp") != NULL)
					pic = ostrcat("3gp.png", NULL, 0, 0);
				else if(ostrstr(ret1[i].part, "flv") != NULL)
					pic = ostrcat("flv.png", NULL, 0, 0);

				if(ostrstr(ret1[i].part, "http://") != NULL)
				{
					streamurl = oregex(".*mediaCollection.addMediaStream.*(http://.*.mp4).*", ret1[i].part);
					nummer = string_resub("mediaCollection.addMediaStream\(", ", \"", ret1[i].part, 0);

					title = ostrcat("Http Stream (", NULL, 0, 0);
					title = ostrcat(title, nummer, 1, 0);
					title = ostrcat(title, ")", 1, 0);

					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}
				else if(ostrstr(ret1[i].part, "rtmp://") != NULL)
				{
					part1 = oregex(".*mediaCollection.addMediaStream.*(rtmp://.*)\", \"mp4:.*", ret1[i].part);
					part2 = oregex(".*mediaCollection.addMediaStream.*(mp4:.*)\", \".*", ret1[i].part);
//					part2 = oregex(".*mediaCollection.addMediaStream.*(mp4:.*)[?].*", ret1[i].part);
					part2 = stringreplacecharonce(part2, '?', '\0');

					streamurl = ostrcat(part1, part2, 0, 0);

					nummer = string_resub("mediaCollection.addMediaStream\(", ", \"", ret1[i].part, 0);

					title = ostrcat("RTMP Stream (", NULL, 0, 0);
					title = ostrcat(title, nummer, 1, 0);
					title = ostrcat(title, ")", 1, 0);

					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}

				free(part1), part1 = NULL;
				free(part2), part2 = NULL;		
				free(title), title = NULL;
				free(pic), pic = NULL;
				free(nummer), nummer = NULL;
				free(streamurl), streamurl = NULL;
			}
		}
		free(ret1), ret1 = NULL;
		free(tmpstr2), tmpstr2 = NULL;

		if(mlist != NULL)
		{
			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 1, 0);
			if(mbox != NULL)
			{
				free(streamurl), streamurl = NULL;

				debug(99, "mbox->name %s", mbox->name);
				debug(99, "mbox->text %s", mbox->text);
				streamurl = ostrcat(mbox->text, NULL, 0, 0);

			}
		}
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

	titheklog(debuglevel, "/tmp/tithek/ard3_streamurl", NULL, streamurl);

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif