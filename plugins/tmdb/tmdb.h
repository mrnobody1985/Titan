#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC1 "/tmp/tmptmdb1.jpg"
#define TMPTMDBPIC2 "/tmp/tmptmdb2.jpg"
#define TMPTMDBPIC3 "/tmp/tmptmdb3.jpg"
#define TMPTMDBPIC4 "/tmp/tmptmdb4.jpg"
#define TMPTMDBPIC5 "/tmp/tmptmdb5.mvi"

struct tmdb* addtmdb(struct tmdb** first, int count, struct tmdb* last)
{
	//debug(1000, "in");
	struct tmdb *newnode = NULL, *prev = NULL, *node = *first;

	newnode = (struct tmdb*)calloc(1, sizeof(struct tmdb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		*first = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

int deltmdb(struct tmdb** first, struct tmdb* tnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	struct tmdb *node = *first, *prev = *first;

	while(node != NULL)
	{
		if(node == tnode)
		{
			ret = 0;
			if(node == *first)
			{
				*first = node->next;
				if(*first != NULL)
					(*first)->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->title); node->title = NULL;
			free(node->year); node->year = NULL;
			free(node->rated); node->rated = NULL;
			free(node->released); node->released = NULL;
			free(node->genre); node->genre = NULL;
			free(node->runtime); node->runtime = NULL;
			free(node->plot); node->plot = NULL;
			free(node->postermid); node->postermid = NULL;
			free(node->rating); node->rating = NULL;
			free(node->votes); node->votes = NULL;
			free(node->id); node->id = NULL;
			free(node->orgname); node->orgname = NULL;
			free(node->imdbid); node->imdbid = NULL;
			free(node->backdrop); node->backdrop = NULL;
			free(node->language); node->language = NULL;
			free(node->type); node->type = NULL;
			free(node->score); node->score = NULL;
			free(node->cover); node->cover = NULL;
			free(node->thumb); node->thumb = NULL;
			free(node->mvi); node->mvi = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return ret;
}

void freetmdb(struct tmdb** first, int flag)
{
	debug(1000, "in");
	struct tmdb *node = *first, *prev = *first;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltmdb(first, prev, flag);
	}

	unlink(TMPTMDBPIC1);
	unlink(TMPTMDBPIC2);
	unlink(TMPTMDBPIC3);
	unlink(TMPTMDBPIC4);
	unlink(TMPTMDBPIC5);

	debug(1000, "out");
}

char* savetmdbpic(char* imdbid, char* url, char* tmppic, char* pic, int flag1)
{
	char* ip = NULL, *pos = NULL, *path = NULL, *ret = NULL;
	char* savefile = NULL;

	debug(133, "url: %s", url);
	
	if(imdbid == NULL || url == NULL) return NULL;
	ip = string_replace("http://", "", url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	if(flag1 == 1)
	{
		savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
		savefile = ostrcat(savefile, imdbid, 1, 0);
		savefile = ostrcat(savefile, pic, 1, 0);
		if(!file_exist(savefile))
			gethttp(ip, path, 80, savefile, NULL, NULL, 0);
		ret = savefile;
	}
	else
	{
		gethttp(ip, path, 80, tmppic, NULL, NULL, 0);
		ret = ostrcat(tmppic, NULL, 0, 0);
	}

	free(ip); ip = NULL;
	return ret;
}

//flag: 0 = title search
//flag: 1 = imdbid search
//flag1: 0 = save pic in tmp
//flag1: 1 = save pic in mediadb path if pic not exist
//flag2: 2 = save no pic
struct tmdb* gettmdb(struct tmdb** first, char* title, int flag, int flag1)
{
	struct tmdb* tnode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *logdir = NULL, *logfile = NULL, *tmpsearch = NULL, *savefile = NULL, *timen = NULL, *log = NULL, *posterurl = NULL;

	int count = 0;
	int mvicount = 0;	
	tmpsearch = ostrcat("2.1/", NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, "Movie.search", 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, "Movie.imdbLookup", 1, 0);
		
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "de", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/xml/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	debug(133, "search: http://api.themoviedb.org/%s", tmpsearch);
	tmpstr = gethttp("api.themoviedb.org", tmpsearch, 80, NULL, NULL, NULL, 0);
	
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);
				
	free(tmpsearch); tmpsearch = NULL;

	logdir = ostrcat(getconfig("mediadbpath", NULL), "/.mediadbdebug", 0, 0);
	if(!file_exist(logdir))
		mkdir(logdir, 0777);
	logfile = ostrcat(logdir, "/imdb-scan.log", 0, 0);
	timen = ostrcat(oitoa(time(NULL)), NULL, 1, 0);

	if(getconfigint("mediadbdebug", NULL) == 1)
	{
		writesys(logfile, "####################################################################", 3); 
		writesys(logfile, "title: ", 2); 
		writesys(logfile, title, 3);
		writesys(logfile, "####################################################################", 3); 
	}

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<movies>Nothing found.</movies>") != NULL)
		{
      debug(133, "<movies>Nothing found.</movies>");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		tmpstr1 = ostrstr(tmpstr, "<movie>");

		while(tmpstr1 != NULL)
		{
			tnode = addtmdb(first, 1, tnode);
			if(tnode == NULL)
			{
				err("no mem");
				free(tmpstr); tmpstr = NULL;
				return *first;
			}
			count++;

			if(ostrstr(tmpstr1, "<name>") != NULL)
				tnode->title = string_resub("<name>", "</name>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<language>") != NULL)
				tnode->language = string_resub("<language>", "</language>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<type>") != NULL)
				tnode->type = string_resub("<type>", "</type>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<original_name>") != NULL)
				tnode->orgname = string_resub("<original_name>", "</original_name>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<score>") != NULL)
				tnode->score = string_resub("<score>", "</score>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<rated>") != NULL)
				tnode->rated = string_resub("<rated>", "</rated>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<released>") != NULL)
				tnode->released = string_resub("<released>", "</released>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<categories>") != NULL)
			{
				char* tmpcat = string_resub("<categories>", "</categories>", tmpstr1, 0);
				char* ptmpcat = ostrstr(tmpcat, "<category ");
				while(ptmpcat != NULL)
				{
					tnode->genre = ostrcat(tnode->genre, string_resub("<category type=\"genre\" name=\"", "\" url=", ptmpcat, 0), 1, 1);
					ptmpcat += 5;
					ptmpcat = ostrstr(ptmpcat, "<category ");

					if(ptmpcat != NULL)
						tnode->genre = ostrcat(tnode->genre, ", ", 1, 0);
				}
				free(tmpcat); tmpcat = NULL;
			}

			if(ostrstr(tmpstr1, "<runtime>") != NULL)
				tnode->runtime = string_resub("<runtime>", "</runtime>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<overview>") != NULL)
				tnode->plot = string_resub("<overview>", "</overview>", tmpstr1, 0);

			if(flag1 != 2 && ostrstr(tmpstr1, "size=\"thumb\"") != NULL)
				tnode->thumb = oregex(".*<image type=\"poster\" url=\".*(http://.*)\" size=\"thumb\".*", tmpstr1);

			if(flag1 != 2 && ostrstr(tmpstr1, "size=\"cover\"") != NULL)
				tnode->cover = oregex(".*<image type=\"poster\" url=\".*(http://.*)\" size=\"cover\".*", tmpstr1);

			if(flag1 != 2 && ostrstr(tmpstr1, "size=\"mid\"") != NULL)
				tnode->postermid = oregex(".*<image type=\"poster\" url=\".*(http://.*)\" size=\"mid\".*", tmpstr1);

			if(flag1 != 2 && ostrstr(tmpstr1, "<images>") != NULL)
			{
				char* tmpcat = string_resub("<images>", "</images>", tmpstr1, 0);
				char* ptmpcat = ostrstr(tmpcat, "<image ");

				while(ptmpcat != NULL)
				{
					tnode->backdrop = ostrcat(tnode->backdrop, string_resub("<image type=\"backdrop\" url=\"", "\" size=", ptmpcat, 0), 1, 1);
					ptmpcat += 5;
					ptmpcat = ostrstr(ptmpcat, "<image ");

					if(ptmpcat != NULL)
					{
						mvicount++;
						tnode->backdrop = ostrcat(tnode->backdrop, "\n", 1, 0);
					}
				}
				free(tmpcat); tmpcat = NULL;
			}

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"original\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/original/.*)\" size=\"original\" width=\"1920\" height=\"1080\".*", tmpstr1);
				
			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"original\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/original/.*)\" size=\"original\" width=\"1280\" height=\"720\".*", tmpstr1);

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"w1280\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/w1280/.*)\" size=\"w1280\" width=\"1280\" height=\"720\".*", tmpstr1);

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"w780\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/w780/.*)\" size=\"poster\" width=\"780\" height=\"439\".*", tmpstr1);

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"original\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/original/.*)\" size=\"original\".*", tmpstr1);

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"w1280\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/w1280/.*)\" size=\"w1280\".*", tmpstr1);

			if(tnode->backdrop == NULL && flag1 != 2 && ostrstr(tmpstr1, "size=\"poster\"") != NULL)
				tnode->backdrop = oregex(".*<image type=\"backdrop\" url=\".*(http://.*/poster/.*)\" size=\"poster\".*", tmpstr1);

				printf("0000022222222222zzzzzzzzzzz\n");

			if(getconfigint("mediadbdebug", NULL) == 1 && tnode->backdrop == NULL)
			{
				log = ostrcat(logdir, "/tmdb_", 0, 0);
				if(tnode->imdbid != NULL)
					log = ostrcat(log, tnode->imdbid, 1, 0);
				else
					log = ostrcat(log, timen, 1, 0);
				
				log = ostrcat(log, "_html", 1, 0);
				writesys(log, tmpstr1, 1);
				free(log), log = NULL;
				
				log = ostrcat(logdir, "/tmdb_", 0, 0);
				if(tnode->imdbid != NULL)
					log = ostrcat(log, tnode->imdbid, 1, 0);
				else
					log = ostrcat(log, timen, 1, 0);

				log = ostrcat(log, "_link", 1, 0);
				writesys(log, tnode->backdrop, 1);
				free(log), log = NULL;
			}


			if(ostrstr(tmpstr1, "<rating>") != NULL)
				tnode->rating = string_resub("<rating>", "</rating>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<votes>") != NULL)
				tnode->votes = string_resub("<votes>", "</votes>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<id>") != NULL)
				tnode->id = string_resub("<id>", "</id>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<imdb_id>") != NULL)
				tnode->imdbid = string_resub("<imdb_id>", "</imdb_id>", tmpstr1, 0);

			if((flag1 == 0 && count == 1) || flag1 == 1)
			{
				savefile = savetmdbpic(tnode->imdbid, tnode->thumb, TMPTMDBPIC1, "_thumb.jpg", flag1);
				free(tnode->thumb);
				tnode->thumb = savefile;

				savefile = savetmdbpic(tnode->imdbid, tnode->cover, TMPTMDBPIC2, "_cover.jpg", flag1);
				free(tnode->cover);
				tnode->cover = savefile;

				savefile = savetmdbpic(tnode->imdbid, tnode->postermid, TMPTMDBPIC3, "_postermid.jpg", flag1);
				free(tnode->postermid);
				tnode->postermid = savefile;

				if(mvicount == 0)
				{
					savefile = savetmdbpic(tnode->imdbid, tnode->backdrop, TMPTMDBPIC4, "_backdrop1.jpg", flag1);
					free(tnode->backdrop);
					tnode->backdrop = savefile;					
				}
			}

			if(flag1 == 1 && tnode->backdrop != NULL && tnode->imdbid != NULL)
			{
				char* tmppath = NULL;
				mvicount = 0;
				char* tmpstr2 = NULL;
				tmpstr2 = ostrcat(tnode->backdrop, NULL, 0, 0);
				struct splitstr* ret1 = NULL;
				int rcount = 0;
				int i = 0;
				ret1 = strsplit(tmpstr2, "\n", &rcount);
				for(i = 0; i < rcount; i++)
				{
					if(ostrstr((&ret1[i])->part, "/original/") != NULL)
					{
						mvicount++;
						debug(133, "load %s\n",(&ret1[i])->part);
	
						if(!ostrncmp("http://", (&ret1[i])->part, 7))
						{
							tmppath = ostrcat("_backdrop", oitoa(mvicount), 0, 1);
							tmppath = ostrcat(tmppath, ".jpg", 1, 0);
							
							savefile = savetmdbpic(tnode->imdbid, (&ret1[i])->part, TMPTMDBPIC4, tmppath, flag1);
							free(tnode->backdrop);
							if(file_exist(savefile))
							{
								tnode->backdrop = savefile;
		
								free(tnode->mvi);
								tnode->mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
								tnode->mvi = ostrcat(tnode->mvi, tnode->imdbid, 1, 0);
								tnode->mvi = ostrcat(tnode->mvi, "_backdrop", 1, 0);
								tnode->mvi = ostrcat(tnode->mvi, oitoa(mvicount), 1, 1);
								tnode->mvi = ostrcat(tnode->mvi, ".mvi", 1, 0);
								
								if((!file_exist(tnode->mvi)) || (flag1 == 2))
								{

									off64_t filesize = getfilesize(tnode->backdrop);
									debug(133, "filesize %lld", filesize);
									
									if(filesize < 1500000)
									{
										char* cmd = NULL;
										cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
										cmd = ostrcat(cmd, tnode->backdrop, 1, 0);
										cmd = ostrcat(cmd, " > /tmp/mediadb.meta 2>&1", 1, 0);
						
										debug(133, "cmd %s", cmd);
										system(cmd);
										free(cmd); cmd = NULL;
	
										char* size = string_newline(command("cat /tmp/mediadb.meta | grep Stream | awk '{print $6}' | cut -d'x' -f1"));
										debug(133, "size %s", size);
										if(size != NULL)
										{
											debug(133, "size %d", atoi(size));
											int picsize = atoi(size);
											unlink("/tmp/mediadb.meta");
		
											if(picsize < 2000)
											{
												debug(133, "size ok %d", picsize);
												cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
												cmd = ostrcat(cmd, tnode->backdrop, 1, 0);
								
												debug(133, "cmd %s", cmd);
												system(cmd);
												free(cmd); cmd = NULL;
		
												if(file_exist("/tmp/backdrop.resize.jpg"))
												{
													if(getconfigint("mediadbdebug", NULL) == 1)
													{
														writesys(logfile, "#############", 3); 
														writesys(logfile, "backdrop: ", 2); 
														writesys(logfile, tnode->backdrop, 2);
														writesys(logfile, " size=(", 2);
														writesys(logfile, size, 2);
														writesys(logfile, ") filesize(", 2);
														writesys(logfile, oitoa(filesize), 2);
														writesys(logfile, ") (", 2);
														writesys(logfile, (&ret1[i])->part, 2);
														writesys(logfile, ")", 3);
														writesys(logfile, "#############", 3); 

														cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg >> ", 1, 0);
														cmd = ostrcat(cmd, logfile, 1, 0);
														cmd = ostrcat(cmd, " 2>&1", 1, 0);
													}
													else
													{
														cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg > /dev/null 2>&1", 1, 0);
													}
													debug(133, "cmd %s", cmd);
													system(cmd);
													free(cmd); cmd = NULL;
													
													if(file_exist("/tmp/backdrop.resize.mpg"))
													{
														cmd = ostrcat(cmd, "mv -f /tmp/backdrop.resize.mpg ", 1, 0);
														cmd = ostrcat(cmd, tnode->mvi, 1, 0);
														debug(133, "cmd %s", cmd);
														system(cmd);
														free(cmd); cmd = NULL;
														
														writesysint("/proc/sys/vm/drop_caches", 3, 0);
														
														if(mvicount == getconfigint("mediadbbackdrop", NULL))
															break;
													}
													else
														mvicount--;
												}
											}
											else
											{
												debug(133, "ERROR Backdrop size to big skipped %d", picsize);

												if(getconfigint("mediadbdebug", NULL) == 1)
												{
													writesys(logfile, "#############", 3); 
													writesys(logfile, "ERROR Backdrop size to big skipped: ", 3); 
													writesys(logfile, tnode->backdrop, 2);
													writesys(logfile, " size=(", 2);
													writesys(logfile, size, 2);
													writesys(logfile, ") filesize(", 2);
													writesys(logfile, oitoa(filesize), 2);
													writesys(logfile, ") (", 2);
													writesys(logfile, (&ret1[i])->part, 2);
													writesys(logfile, ")", 3);
													writesys(logfile, "#############", 3);
												}	
												mvicount--;
											}
										}
										else
										{
											debug(133, "ERROR Backdrop size is NULL skipped %s", size);

											if(getconfigint("mediadbdebug", NULL) == 1)
											{
												writesys(logfile, "#############", 3); 
												writesys(logfile, "ERROR Backdrop size is NULL skipped: ", 3); 
												writesys(logfile, tnode->backdrop, 2);
												writesys(logfile, " size=(", 2);
												writesys(logfile, size, 2);
												writesys(logfile, ") filesize(", 2);
												writesys(logfile, oitoa(filesize), 2);
												writesys(logfile, ") (", 2);
												writesys(logfile, (&ret1[i])->part, 2);
												writesys(logfile, ")", 3);
												writesys(logfile, "#############", 3);
											}

											mvicount--;
										}
										free(size), size = NULL;
										unlink("/tmp/mediadb.meta");
										unlink("/tmp/backdrop.resize.jpg");
										unlink("/tmp/backdrop.resize.mpg");
									}
									else
									{
										debug(133, "ERROR Backdrop filesize to BIG skipped %lld", filesize);
				
										if(getconfigint("mediadbdebug", NULL) == 1)
										{
											writesys(logfile, "#############", 3); 
											writesys(logfile, "ERROR Backdrop filesize to BIG skipped: ", 3); 
											writesys(logfile, tnode->backdrop, 2);
											writesys(logfile, " filesize(", 2);
											writesys(logfile, oitoa(filesize), 2);
											writesys(logfile, ") (", 2);
											writesys(logfile, (&ret1[i])->part, 2);
											writesys(logfile, ")", 3);
											writesys(logfile, "#############", 3);
										}									
									}
								}
								else
									mvicount--;									

								free(tmppath), tmppath = NULL;
							}
							else
								mvicount--;
						}
						else
						{
							log = ostrcat(logdir, "/tmdb_", 0, 0);
							if(tnode->imdbid != NULL)
								log = ostrcat(log, tnode->imdbid, 1, 0);
							else
								log = ostrcat(log, timen, 1, 0);
							
							log = ostrcat(log, "_html", 1, 0);
							
							char* cmd = NULL;
							cmd = command("free");
							log = ostrcat(log, "_error_link", 1, 0);
							cmd = ostrcat(cmd, "\nlink: ", 1, 0);
							cmd = ostrcat(cmd, (&ret1[i])->part, 1, 0);
							writesys(log, cmd, 1);
							free(cmd), cmd = NULL;
							free(log), log = NULL;
							mvicount--;
						}

						if(file_exist(tnode->mvi))
							unlink(tnode->backdrop);
					}
				}
				free(ret1), ret1 = NULL;				
				free(tmpstr2), tmpstr2 = NULL;
			}
			else if(flag1 == 1 && tnode->postermid != NULL && tnode->imdbid != NULL && file_exist(savefile))
			{
				free(tnode->mvi);
				tnode->mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				tnode->mvi = ostrcat(tnode->mvi, tnode->imdbid, 1, 0);
				tnode->mvi = ostrcat(tnode->mvi, "_backdrop1.mvi", 1, 0);
				
				if((!file_exist(tnode->mvi)) || (flag1 == 2))
				{

					off64_t filesize = getfilesize(tnode->backdrop);
					printf("filesize: %lld.\n", filesize);
					
					if(filesize < 1500000)
					{
						char* cmd = NULL;
						cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
						cmd = ostrcat(cmd, tnode->postermid, 1, 0);
						cmd = ostrcat(cmd, " > /tmp/mediadb.meta 2>&1", 1, 0);
	
						debug(133, "cmd %s", cmd);
						system(cmd);
						free(cmd); cmd = NULL;
										
						char* size = string_newline(command("cat /tmp/mediadb.meta | grep Stream | awk '{print $6}' | cut -d'x' -f1"));
						debug(133, "size %s", size);
						if(size != NULL)
						{
							debug(133, "size %d", atoi(size));
							int picsize = atoi(size);
		
							if(picsize < 2000)
							{
								debug(133, "size ok %d", picsize);
												
								cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
								cmd = ostrcat(cmd, tnode->postermid, 1, 0);
				
								debug(133, "cmd %s", cmd);
								system(cmd);
								free(cmd); cmd = NULL;
			
								if(file_exist("/tmp/backdrop.resize.jpg"))
								{
									if(getconfigint("mediadbdebug", NULL) == 1)
									{
										writesys(logfile, "#############", 3); 
										writesys(logfile, "postermid: ", 2); 
										writesys(logfile, tnode->postermid, 2);
										writesys(logfile, " size=(", 2);
										writesys(logfile, size, 2);
										writesys(logfile, ") filesize(", 2);
										writesys(logfile, oitoa(filesize), 2);
										writesys(logfile, ") (", 2);
										writesys(logfile, posterurl, 2);
										writesys(logfile, ")", 3);
										writesys(logfile, "#############", 3); 
	
										cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg >> ", 1, 0);
										cmd = ostrcat(cmd, logfile, 1, 0);
										cmd = ostrcat(cmd, " 2>&1", 1, 0);
									}
									else
									{
										cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg > /dev/null 2>&1", 1, 0);
									}
	
									debug(133, "cmd %s", cmd);
									system(cmd);
									free(cmd); cmd = NULL;
									if(file_exist("/tmp/backdrop.resize.mpg"))
									{					
										cmd = ostrcat(cmd, "mv -f /tmp/backdrop.resize.mpg ", 1, 0);
										cmd = ostrcat(cmd, tnode->mvi, 1, 0);
										debug(133, "cmd %s", cmd);
										system(cmd);
										free(cmd); cmd = NULL;
										
										writesysint("/proc/sys/vm/drop_caches", 3, 0);
									}
								}
							}
							else
							{
								debug(133, "ERROR Postermid size to big skipped %d", picsize);

								if(getconfigint("mediadbdebug", NULL) == 1)
								{
									writesys(logfile, "#############", 3); 
									writesys(logfile, "ERROR Postermid size to big skipped: ", 3); 
									writesys(logfile, tnode->postermid, 2);
									writesys(logfile, " size=(", 2);
									writesys(logfile, size, 2);
									writesys(logfile, ") filesize(", 2);
									writesys(logfile, oitoa(filesize), 2);
									writesys(logfile, ") (", 2);
									writesys(logfile, posterurl, 2);
									writesys(logfile, ")", 3);
									writesys(logfile, "#############", 3);
								}
							}
						}
						else
						{
							debug(133, "ERROR Postermid size is NULL skipped %s", size);

							if(getconfigint("mediadbdebug", NULL) == 1)
							{
								writesys(logfile, "#############", 3); 
								writesys(logfile, "ERROR Postermid size is NULL skipped: ", 3); 
								writesys(logfile, tnode->postermid, 2);
								writesys(logfile, " size=(", 2);
								writesys(logfile, size, 2);
								writesys(logfile, ") filesize(", 2);
								writesys(logfile, oitoa(filesize), 2);
								writesys(logfile, ") (", 2);
								writesys(logfile, posterurl, 2);
								writesys(logfile, ")", 3);
								writesys(logfile, "#############", 3);
							}
						}
						free(size), size = NULL;
						unlink("/tmp/mediadb.meta");
						unlink("/tmp/backdrop.resize.jpg");
						unlink("/tmp/backdrop.resize.mpg");
					}
					else
					{
						debug(133, "ERROR Postermid filesize to BIG skipped %lld", filesize);

						if(getconfigint("mediadbdebug", NULL) == 1)
						{
							writesys(logfile, "#############", 3); 
							writesys(logfile, "ERROR Postermid filesize to BIG skipped: ", 3); 
							writesys(logfile, tnode->postermid, 2);
							writesys(logfile, " filesize(", 2);
							writesys(logfile, oitoa(filesize), 2);
							writesys(logfile, ") (", 2);
							writesys(logfile, posterurl, 2);
							writesys(logfile, ")", 3);
							writesys(logfile, "#############", 3);
						}
					}
				}
			}
					
			if(file_exist(tnode->mvi))
				unlink(tnode->backdrop);

			if(mvicount > 0)
				tnode->mvi = ostrcat(oitoa(mvicount), NULL, 0, 0);
			else
				tnode->mvi = ostrcat("1", NULL, 0, 0);

			tmpstr1 += 5;
			tmpstr1 = ostrstr(tmpstr1, "<movie>");

			debug(133, "----------------------tmdb start----------------------");
			debug(133, "title: %s", tnode->title);
			debug(133, "language: %s", tnode->language);
			debug(133, "type: %s", tnode->type);
			debug(133, "orgname: %s", tnode->orgname);
			debug(133, "score: %s", tnode->score);
			debug(133, "rating: %s", tnode->rating);
			debug(133, "released: %s", tnode->released);
			debug(133, "genre: %s", tnode->genre);
			debug(133, "runtime: %s", tnode->runtime);
			debug(133, "plot: %s", tnode->plot);
			debug(133, "thumb: %s", tnode->thumb);
			debug(133, "cover: %s", tnode->cover);
			debug(133, "postermid: %s", tnode->postermid);
			debug(133, "backdrop: %s", tnode->backdrop);
			debug(133, "mvi: %s", tnode->mvi);
			debug(133, "rating: %s", tnode->rating);
			debug(133, "votes: %s", tnode->votes);
			debug(133, "id: %s", tnode->id);	
			debug(133, "imdbid: %s", tnode->imdbid);
			debug(133, "----------------------tmdb end----------------------");
		}
	}


	free(tmpstr); tmpstr = NULL;
	return *first;
}

//flag 0: only view
//flag 1: can return tmdb node
struct tmdb* screentmdb(char* title, char* skinname, int flag, char* path, char* file)
{
	debug(133, "title: %s",title);
	debug(133, "path: %s",path);
	debug(133, "file: %s",file);	
	char* searchstr = NULL, *tmpstr = NULL;	

	int rcret = 0;
	struct skin* tmdbskin = NULL;

	if(skinname != NULL)
		tmdbskin = getscreen(skinname);
	else
		tmdbskin = getscreen("tmdb");

	struct skin* skin_plot = getscreennode(tmdbskin, "plot");
	struct skin* skin_title = getscreennode(tmdbskin, "title");
	struct skin* skin_orgname = getscreennode(tmdbskin, "orgname");
	struct skin* skin_rating = getscreennode(tmdbskin, "rating");
	struct skin* skin_genre = getscreennode(tmdbskin, "genre");
	struct skin* skin_released = getscreennode(tmdbskin, "released");
	struct skin* skin_cover = getscreennode(tmdbskin, "cover");
	struct skin* skin_votes = getscreennode(tmdbskin, "votes");
	struct skin* b3 = getscreennode(tmdbskin, "b3");
	struct skin* load = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	
	struct tmdb* node = NULL, *retnode = NULL;
	char* search = NULL;

//	setfbtransparent(255);
	status.hangtime = 99999;

	if(flag == 0)
		b3->hidden = YES;
	else
		b3->hidden = NO;
		
	if(title == NULL)
		searchstr = getepgakttitle(NULL);
	else
	{
		int isrec = 0, iscam = 0;
		tmpstr = createshortname(file, &isrec, &iscam, 1);
		if(tmpstr != NULL)
			searchstr = ostrcat(searchstr, tmpstr, 1, 0);
		else
			searchstr = ostrcat(searchstr, title, 1, 0);
	}

	debug(133, "searchstr: %s",searchstr);

	drawscreen(blackscreen, 0, 0);
	drawscreen(load, 0, 0);

	node = gettmdb(&node, searchstr, 0, 0);
	clearscreen(load);
	clearscreen(blackscreen);

start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_orgname, node->orgname);
		changetext(skin_rating, node->rating);
		changetext(skin_genre, node->genre);
		changetext(skin_released, node->released);
		char* tmpstr = NULL;
		tmpstr = ostrcat("( ", node->votes, 0, 0);
		tmpstr = ostrcat(tmpstr, " votes)", 1, 0);		
		changetext(skin_votes, tmpstr);
		free(tmpstr), tmpstr = NULL;
		changepic(skin_cover, TMPTMDBPIC3);
		skin_cover->hidden = NO;
	}
	else
	{
		changetext(skin_plot, "--plot--");
		changetext(skin_title, "--title--");
		changetext(skin_orgname, "--orgname--");
		changetext(skin_rating, "--rating--");
		changetext(skin_genre, "--genre--");
		changetext(skin_released, "--releasetime--");
		changetext(skin_votes, "( --votes-- )");		
		skin_cover->hidden = YES;
	}

	drawscreen(tmdbskin, 0, 0);

	while(1)
	{
		rcret = waitrc(tmdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
		{
			search = textinput("Search", searchstr);
			if(search != NULL)
			{
				freetmdb(&node, 0); node = NULL;
				drawscreen(blackscreen, 0, 0);
				drawscreen(load, 0, 0);

				node = gettmdb(&node, search, 0, 0);
				clearscreen(load);
				clearscreen(blackscreen);
				free(search); search = NULL;
				goto start;
			}
			drawscreen(tmdbskin, 0, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			struct tmdb* tnode = node;
			struct menulist* mlist = NULL, *mbox = NULL;

			while(tnode != NULL)
			{
				addmenulist(&mlist, tnode->title, tnode->imdbid, NULL, 0, 0);
				tnode = tnode->next;
			}

			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 0, 0);
			if(mbox != NULL)
			{
				search = ostrcat(mbox->text, NULL, 0, 0);
				if(search != NULL)
				{
					freetmdb(&node, 0); node = NULL;
					drawscreen(blackscreen, 0, 0);
					drawscreen(load, 0, 0);
					node = gettmdb(&node, search, 1, 0);
					clearscreen(load);
					clearscreen(blackscreen);
					free(search); search = NULL;
					freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
					goto start;
				}
			}
			freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
			drawscreen(tmdbskin, 0, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcyellow", NULL) && flag == 1)
		{
			retnode = node;
			break;
		}

		if(rcret == getrcconfigint("rcyellow", NULL) && path != NULL && file != NULL && node != NULL && node->imdbid != NULL && flag == 2)
		{
			drawscreen(blackscreen, 0, 0);
			drawscreen(load, 0, 0);
			debug(133, "path: %s",path);
			debug(133, "file: %s",file);
			debug(133, "type: 2");
			debug(133, "node->imdbid: %s",node->imdbid);
			addconfigtmp("mediadbscantimeout", "0");
			mediadbfindfilecb(path, file, 0, node->imdbid, 2);
			delconfigtmp("mediadbscantimeout");			
			clearscreen(load);
			clearscreen(blackscreen);
			break;
		}
	}

	if(retnode == NULL)
	{
		freetmdb(&node, 0);
		node = NULL;
	}

	free(searchstr), searchstr = NULL;
//	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(tmdbskin);

	return retnode;
}

#endif
