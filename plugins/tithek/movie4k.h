#ifndef MOVIE4K_H
#define MOVIE4K_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* movie4k(char* link)
{
	debug(99, "link: %s", link);
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *path = NULL, *url = NULL, *streamurl = NULL, *id = NULL, *hname = NULL;

	if(link == NULL) return NULL;

	int count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(link, ";", &count);
	if(ret1 != NULL && count >= 3)
	{
		path = ostrcat(ret1[0].part, NULL, 0, 0);
		debug(99, "path: %s", path);

		id = ostrcat(ret1[1].part, NULL, 0, 0);
		debug(99, "id: %s", id);

		hname = ostrcat(ret1[2].part, NULL, 0, 0);
		debug(99, "hname: %s", hname);

		tmpstr = gethttp("www.movie4k.to", path, 80, NULL, NULL, 5000, NULL, 0);

		url = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr, 0);

		if(ostrstr(url, "http://") == NULL)
		{
			free(url), url = NULL;
			url = string_resub("<iframe src=", "\" width", tmpstr, 0);
		}
		
		if(ostrstr(url, "http://www.facebook.com") != NULL)
		{
			free(url), url = NULL;
			url = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr);
		}
		
		url = string_replace_all("/embed/", "/file/", url, 1);
		url = string_replace_all("\"", "", url, 1);
					
		tmpstr1 = ostrcat(url, NULL, 0, 0);

		int count2 = 0;
		struct splitstr* ret2 = NULL;
		ret2 = strsplit(tmpstr1, "/", &count2);

		if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare") == 0)
			streamurl = putlocker("Sockshare.com", ret2[3].part);
		else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker") == 0)
			streamurl = putlocker("Sockshare.com", ret2[3].part);
		else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Filenuke") == 0)
			streamurl = filenuke("FileNuke.com", ret2[2].part);
		else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamclou") == 0)
			streamurl = streamcloud("StreamCloud.eu", ret2[2].part);
		else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamcloud") == 0)
			streamurl = streamcloud("StreamCloud.eu", ret2[2].part);
		else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "VidStream") == 0)
			streamurl = vidstream("VidStream.in", ret2[2].part);
		else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Flashx") == 0)
			streamurl = flashx("FlashX.tv", ret2[3].part);
		else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Xvidstage") == 0)
			streamurl = xvidstage("XvidStage.com", ret2[2].part);
		else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Nowvideo") == 0)
		{
			tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
			tmpstr2 = string_replace("embed.php?v=", "", tmpstr2, 1);
			streamurl = nowvideo("NowVideo.eu", tmpstr2);
		}
		free(ret2), ret2 = NULL;
	}
	free(ret1), ret1 = NULL;

	free(url), url = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(path), path = NULL;
	free(hname), hname = NULL;
	free(id), id = NULL;

	return streamurl;
}

int movie4k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;
	int incount = 0;
	char* tmpstr = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;
	char* episode = NULL;
	char* line = NULL;
	char* menu = NULL;
	char* str = NULL;
	char* pic = NULL;
	char* type = NULL;


	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(flag == 0)
	{
		if(searchstr == NULL)
			search = textinputhist("Search", " ", "searchhist");
		else
			search = textinputhist("Search", searchstr, "searchhist");
	}
	else
		search = ostrcat(link, NULL, 0, 0);

	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');

		char* send = NULL;
		send = ostrcat(send, "GET /searchAutoCompleteNew.php?search=the HTTP/1.1\r\n", 1, 0);
		send = ostrcat(send, "Accept-Encoding: identity\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
		send = ostrcat(send, "Host: www.movie4k.to\r\nAccept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		tmpstr = gethttpreal("movie4k.to", "/", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		free(send); send = NULL;

		char* key = string_resub("securekey=", "&search", tmpstr, 0);
		debug(99, "key: %s", key);

		send = ostrcat(send, "GET /movies.php?list=search&securekey=", 1, 0);
		send = ostrcat(send, key, 1, 0);
		send = ostrcat(send, "&search=", 1, 0);
		send = ostrcat(send, search, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\n", 1, 0);
		send = ostrcat(send, "Accept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Host: www.movie4k.to\r\n", 1, 0);
		send = ostrcat(send, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n", 1, 0);
		send = ostrcat(send, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nCookie: xxx2=ok;\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttpreal("movie4k.to", "/", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
//		folgen = string_replace_all("\n", "", folgen, 1);
		stringreplacechar(folgen, '\n', ' ');
//		folgen = string_replace_all("\t", "", folgen, 1);
		stringreplacechar(folgen, '\t', ' ');
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR id=\"coverPreview", "</TD> </TR>\n<TR id=\"cover1Preview", folgen, 1);

		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					incount += 1;
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);
					name = string_resub(".html\">", "</a>", ret1[j].part, 0);

					char* id = string_resub("online-film-", ".html", link, 0);
					if(id == NULL)
						id = string_resub("watch-movie-", ".html", link, 0);
					if(id == NULL)
						id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", link);


					from = ostrcat("#coverPreview", id, 0, 0);
					pic = string_resub(from, from, tmpstr, 0);	
					pic = string_resub("<img src='", "' alt=", pic, 1);	
					
					if(ostrstr(link, "online-serie-") != NULL)
						type = ostrcat("40", NULL, 0, 0);
					else
						type = ostrcat("34", NULL, 0, 0);


					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);
					line = ostrcat(line, name, 1, 0);

					line = ostrcat(line, lang, 1, 0);	
					line = ostrcat(line, "#http://www.movie4k.to/", 1, 0);
					line = ostrcat(line, link, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, pic, 1, 0);
					line = ostrcat(line, "#movie4k_search_", 1, 0);
					line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
					line = ostrcat(line, ".jpg#Movie4k - Search#", 1, 0);
					line = ostrcat(line, type, 1, 0);
					line = ostrcat(line, "\n", 1, 0);
					free(str), str = NULL;
											
					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
					free(episode), episode = NULL;
					free(from), from = NULL;
					free(pic), pic = NULL;
					free(type), type = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		

		if(line != NULL)
		{
			menu = ostrcat("/tmp/tithek/movie4k.search.list", NULL, 0, 0);
			writesys(menu, line, 0);
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
			free(menu); menu = NULL;
		}

		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;
	return ret;
}

int movie4k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *line = NULL, *url = NULL, *url2 = NULL, *url3 = NULL, *url4 = NULL, *pathnew = NULL, *hname = NULL, *id = NULL, *logfile = NULL, *tmphname = NULL, *hnamein = NULL, *nolinks = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		if(ostrstr(tmpstr, "links\[") == NULL)
		{
			hnamein = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", tmpstr, 0);
			nolinks = ostrcat(tmpstr, NULL, 0, 0);
		}

		tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);

		int count = 0;
		int incount = 0;
		int i;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);		

		if(ret1 != NULL && count > 0)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if((!ostrncmp("links", ret1[i].part, 5) && nolinks == NULL) || nolinks != NULL)
				{
					if(nolinks == NULL)
					{
						ret1[i].part = string_replace_all("</a>&nbsp;</td>", "</a></td>", ret1[i].part, 0);

						pathnew = string_resub("<a href=\\\"", "\\", ret1[i].part, 0);
						tmphname = string_resub("&nbsp;", "</a", ret1[i].part, 0);

						id = oregex(".*-online-film-(.*[0-9]{1,10}).html.*", pathnew);							
						if(id == NULL)
							id = string_resub("watch-movie-", ".html", pathnew, 0);
						if(id == NULL)
							id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", pathnew);
						if(id == NULL)
							id = oregex(".*movie.php?id=(.*[0-9]{1,10})&part=.*", tmpstr1);

						debug(99, "(%d) pathnew: %s hname: %s id: %s",i ,pathnew, tmphname, id);

						if(ostrstr(pathnew, "tvshows-") != NULL)
						{
							pathnew = string_replace("tvshows-", "", pathnew, 1);
							pathnew = string_replace(id, "", pathnew, 1);
							pathnew = string_replace("-", "", pathnew, 1);
							pathnew = string_replace(".html", "", pathnew, 1);
							pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
							pathnew = ostrcat(pathnew, id, 1, 0);
							pathnew = ostrcat(pathnew, ".html", 1, 0);
						}	

						debug(99, "(%d) convert pathnew: %s", i, pathnew);

						logfile = ostrcat("/tmp/movie4k4_pathnew1", id, 0, 0);
						logfile = ostrcat(logfile, "_", 1, 0);
						logfile = ostrcat(logfile, tmphname, 1, 0);

						if(getconfigint("debuglevel", NULL) == 99)
							writesys(logfile, pathnew, 0);
						free(logfile), logfile= NULL;
						url = ostrcat(pathnew, NULL, 0, 0);
					}
					else
					{
						id = oregex(".*-online-film-(.*[0-9]{1,10}).html.*", path);							
						if(id == NULL)
							id = string_resub("watch-movie-", ".html", path, 0);
						if(id == NULL)
							id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", path);
						if(id == NULL)
							id = oregex(".*movie.php?id=(.*[0-9]{1,10})&part=.*", tmpstr1);

						tmphname = ostrcat(hnamein, NULL, 0, 0);
					}	

					int countj = 0;
					if(ostrstr(tmpstr1, "&part=1") != NULL)
						countj = 1;
					if(ostrstr(tmpstr1, "&part=2") != NULL)
						countj = 2;
					if(ostrstr(tmpstr1, "&part=3") != NULL)
						countj = 3;
					if(ostrstr(tmpstr1, "&part=4") != NULL)
						countj = 4;

					if(id != NULL)
					{
						if(countj >= 2)
						{	
							free(url), url = NULL;
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=1", 1, 0);
							url = ostrcat(pathnew, NULL, 0, 0);
						}

						if(countj >= 2)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=2", 1, 0);
							url2 = ostrcat(pathnew, NULL, 0, 0);
						}

						if(countj >= 3)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=3", 1, 0);
							url3 = ostrcat(pathnew, NULL, 0, 0);

						}						

						if(countj >= 4)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=4", 1, 0);
							url4 = ostrcat(pathnew, NULL, 0, 0);
						}
					}

					int type = 41;

					if(ostrcmp(tmphname, "Sockshare") == 0)
						hname = ostrcat("Sockshare.com", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Putlocker") == 0)
						hname = ostrcat("Putlocker.com", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Filenuke") == 0)
						hname = ostrcat("FileNuke.com", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Streamclou") == 0)
						hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Streamcloud") == 0)
						hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
					else if(ostrcmp(tmphname, "VidStream") == 0)
						hname = ostrcat("VidStream.in", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Flashx") == 0)
						hname = ostrcat("FlashX.tv", NULL, 0, 0);
					else if(ostrcmp(tmphname, "PrimeShare") == 0)
						hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Xvidstage") == 0)
						hname = ostrcat("XvidStage.com", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Nowvideo") == 0)
						hname = ostrcat("NowVideo.eu", NULL, 0, 0);
					else if(ostrcmp(tmphname, "Nowvideo") == 0)
						hname = ostrcat("NowVideo.eu", NULL, 0, 0);
					else
					{
						hname = ostrcat(tmphname, " (coming soon)", 0, 0);
						type = 66;
					}
						
					if(ostrcmp(url, url2) != 0)
					{
						debug(99, "hname: %s (Part1) url: %s id: %s", hname, url, id);
					}
					else
					{
						debug(99, "hname: %s url: %s id: %s", hname, url, id);
					}
					
					incount += 1;
					line = ostrcat(line, hname, 1, 0);
					if(url2 != NULL && ostrcmp(url, url2) != 0)
						line = ostrcat(line, " (Part1)", 1, 0);					
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, url, 1, 0);
					line = ostrcat(line, ";", 1, 0);
					line = ostrcat(line, id, 1, 0);
					line = ostrcat(line, ";", 1, 0);				
					line = ostrcat(line, tmphname, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
					line = ostrcat(line, hname, 1, 0);
					line = ostrcat(line, ".jpg#movie4k_", 1, 0);
					line = ostrcat(line, hname, 1, 0);
					line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, oitoa(type), 1, 0);
					line = ostrcat(line, "\n", 1, 0);
					free(hname), hname = NULL;

					if(url2 != NULL && ostrcmp(url, url2) != 0)
					{						
						int type = 41;
	
						if(ostrcmp(tmphname, "Sockshare") == 0)
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Putlocker") == 0)
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Filenuke") == 0)
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamclou") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamcloud") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "VidStream") == 0)
							hname = ostrcat("VidStream.in", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Flashx") == 0)
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Xvidstage") == 0)
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else
						{
							hname = ostrcat(tmphname, " (coming soon)", 0, 0);
							type = 66;
						}
					
						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part2) url: %s id: %s", hname, url2, id);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part2)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url2, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, id, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, tmphname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#kinox_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(hname), hname = NULL;
					}
					if(url3 != NULL && ostrcmp(url, url3) != 0)
					{
						int type = 41;
	
						if(ostrcmp(tmphname, "Sockshare") == 0)
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Putlocker") == 0)
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Filenuke") == 0)
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamclou") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamcloud") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "VidStream") == 0)
							hname = ostrcat("VidStream.in", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Flashx") == 0)
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Xvidstage") == 0)
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else
						{
							hname = ostrcat(tmphname, " (coming soon)", 0, 0);
							type = 66;
						}
																
						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part3) url: %s id: %s", hname, url3, id);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part3)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url3, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, id, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, tmphname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#movie4k_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(hname), hname = NULL;
					}
					if(url4 != NULL && ostrcmp(url, url4) != 0)
					{
						int type = 41;
	
						if(ostrcmp(tmphname, "Sockshare") == 0)
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Putlocker") == 0)
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Filenuke") == 0)
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamclou") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamcloud") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "VidStream") == 0)
							hname = ostrcat("VidStream.in", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Flashx") == 0)
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Xvidstage") == 0)
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else
						{
							hname = ostrcat(tmphname, " (coming soon)", 0, 0);
							type = 66;
						}												

						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part3) url: %s id: %s", hname, url4, id);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part4)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url4, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, id, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, tmphname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#movie4k_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(hname), hname = NULL;
					}
					free(tmphname), tmphname = NULL;

					if(nolinks != NULL)
					{
						printf("break\n");
						break;
					}

				}

				free(hname), hname = NULL;										
				free(url), url = NULL;
				free(url2), url2 = NULL;
				free(url3), url3 = NULL;
				free(url4), url4 = NULL;
				free(pathnew), pathnew = NULL;
				free(logfile), logfile = NULL;					
			}
		}
		free(ret1), ret1 = NULL;
	}
	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int movie4k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *id = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL, *episode = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
/*
	if(!ostrncmp("tvshows-", path, 8))
	{
		debug(99, "path: %s",path);
		tpath = ostrcat(path, NULL, 0, 0);
		id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", path);
		tpath = string_replace("tvshows-season-", "", tpath, 1);
		tpath = string_replace(".html", "", tpath, 1);
		tpath = ostrcat(tpath, "-online-serie-", 1, 0);
		tpath = ostrcat(tpath, id, 1, 0);
		tpath = ostrcat(tpath, ".html", 1, 0);
		debug(99, "convertpath: %s",tpath);
	}
	else
*/
		tpath = ostrcat(path, NULL, 0, 0);

	tmpstr = gethttp(ip, tpath, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		if(ostrstr(tmpstr, "episodeform") != NULL)
		{
			int i;
			for(i = 1; i < 30; i++)
			{
				from = ostrcat(from, "<FORM name=\"episodeform", 1, 0);
				from = ostrcat(from, oitoa(i), 1, 0);
				from = ostrcat(from, "\">", 1, 0);
				folgen = string_resub(from, "</FORM>", tmpstr, 0);
				folgen = string_resub("<OPTION></OPTION>", "</SELECT>", folgen, 1);
				folgen = string_replace_all("><", ">\n<", folgen, 1);
				folgentmp = ostrcat(folgen, NULL, 0, 0);

				if(folgen != NULL)
				{
					int count = 0;
					int incount = 0;
					int j;
					struct splitstr* ret1 = NULL;
					ret1 = strsplit(folgen, "\n", &count);
						
					if(ret1 != NULL && count > 0)
					{
						int max = count + 1;
						for(j = 1; j < max; j++)
						{
							link = string_resub("<OPTION value=\"", "\"", ret1[j-1].part, 0);
							id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", link);
							episode = oregex(".*>Episode (.*[0-9]{1,10})</OPTION>.*", ret1[j-1].part);

 							debug(99, "(S%d/E%s)(%d) link: %s id: %s", i, episode, j, link, id);
							link = string_replace("tvshows-", "", link, 1);
							link = string_replace(id, "", link, 1);
							link = string_replace("-", "", link, 1);
							link = string_replace(".html", "", link, 1);
							link = ostrcat(link, "-online-serie-", 1, 0);
							link = ostrcat(link, id, 1, 0);
							link = ostrcat(link, ".html", 1, 0);
							debug(99, "(S%d/E%d) convertlink: %s id: %s", i, j, link, id);

							incount += 1;
							line = ostrcat(line, _("Season"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, _("Episode"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
//							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, episode, 1, 0);					
							line = ostrcat(line, "#http://www.movie4k.to/", 1, 0);
							line = ostrcat(line, link, 1, 0);
							line = ostrcat(line, "#", 1, 0);
							line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
//							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, episode, 1, 0);
							line = ostrcat(line, ".jpg", 1, 0);																
							line = ostrcat(line, "#movie4k_search_", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
//							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, episode, 1, 0);
							line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
							line = ostrcat(line, title, 1, 0);
							line = ostrcat(line, "#34\n", 1, 0);
							free(id), id = NULL;
							free(episode), episode = NULL;
						}
					}
					free(ret1), ret1 = NULL;
				}				
				free(from), from = NULL;
				free(folgen), folgen = NULL;
				free(folgentmp), folgentmp = NULL;
			}
		}
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		if(series == 0)
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.list", NULL, 0, 0);
		else
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.ser.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie4k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1;
	char* ip = NULL, *pos = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
		folgen = string_replace_all("\n", "", folgen, 1);
		folgen = string_replace_all("\t", "", folgen, 1);
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR>", "</TD> </TR>\n<TR>", folgen, 1);

		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);

					name = oregex(".*><a href=.*\">(.*)</a></TD>.*", ret1[j].part);

					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);

					season = oregex(".*Season:(.*[0-9]{1,3}).*", name);
					season = strstrip(season);
					debug(99, "(S%s) input: %s%s",season, name, lang);

					line = ostrcat(line, _("Season"), 1, 0);
					line = ostrcat(line, " ", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, " ", 1, 0);
										
					line = ostrcat(line, lang, 1, 0);	
					line = ostrcat(line, "#http://www.movie4k.to/", 1, 0);
					line = ostrcat(line, link, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, ".jpg", 1, 0);
					line = ostrcat(line, "#s", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, ".jpg", 1, 0);
					line = ostrcat(line, "#Movie4k - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#39\n", 1, 0);
					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		tmpstr = ostrcat("/tmp/tithek/movie4k.series.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie4k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1;
	char* ip = NULL, *pos = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;
	char* episode = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
		folgen = string_replace_all("\n", "", folgen, 1);
		folgen = string_replace_all("\t", "", folgen, 1);
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR>", "</TD> </TR>\n<TR>", folgen, 1);

		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);
					name = oregex(".*><a href=.*\">(.*)</a></TD>.*", ret1[j].part);

					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);

					season = oregex(".*Season:(.*),.*", name);
					season = strstrip(season);
					episode = oregex(".*Episode:(.*[0-9]{1,3}).*", name);
					episode = strstrip(episode);
					debug(99, "(S%s/E%s) input: %s%s",season, episode, name, lang);

					line = ostrcat(line, _("Season"), 1, 0);
					line = ostrcat(line, " ", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, " ", 1, 0);
					line = ostrcat(line, _("Episode"), 1, 0);
					line = ostrcat(line, " ", 1, 0);				
					line = ostrcat(line, episode, 1, 0);
					line = ostrcat(line, " ", 1, 0);
					line = ostrcat(line, lang, 1, 0);	
					line = ostrcat(line, "#http://www.movie4k.to/", 1, 0);
					line = ostrcat(line, link, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, "e", 1, 0);
					line = ostrcat(line, episode, 1, 0);
					line = ostrcat(line, ".jpg", 1, 0);
					line = ostrcat(line, "#s", 1, 0);
					line = ostrcat(line, season, 1, 0);
					line = ostrcat(line, "e", 1, 0);
					line = ostrcat(line, episode, 1, 0);
					line = ostrcat(line, ".jpg", 1, 0);
					line = ostrcat(line, "#Movie4k - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#34\n", 1, 0);
					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
					free(episode), episode = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		tmpstr = ostrcat("/tmp/tithek/movie4k.series.listed.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}


#endif