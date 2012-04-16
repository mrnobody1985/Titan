#ifndef IMDB_H
#define IMDB_H

#define TMPIMDBPIC "/tmp/tmpimdb.jpg"

struct imdb
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	char* runtime;
	char* rating;
	char* votes;
	char* id;
};

void freeimdb(struct imdb* node)
{
	if(node == NULL) return;

	free(node->title); node->title = NULL;
	free(node->year); node->year = NULL;
	free(node->rated); node->rated = NULL;
	free(node->released); node->released = NULL;
	free(node->genre); node->genre = NULL;
	free(node->director); node->director = NULL;
	free(node->writer); node->writer = NULL;
	free(node->actors); node->actors = NULL;
	free(node->plot); node->plot = NULL;
	free(node->poster); node->poster = NULL;
	free(node->runtime); node->runtime = NULL;
	free(node->rating); node->rating = NULL;
	free(node->votes); node->votes = NULL;
	free(node->id); node->id = NULL;
	unlink(TMPIMDBPIC);

	free(node); node = NULL;
}

struct imdb* getimdb(char* title)
{
	struct imdb* imdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;

	tmpsearch = ostrcat("?i=&t=", title, 0, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.imdbapi.com", tmpsearch, 80, NULL, NULL, NULL, 0);
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		imdb = (struct imdb*)malloc(sizeof(struct imdb));
		if(imdb == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		memset(imdb, 0, sizeof(struct imdb));

		imdb->title = ostrcat(getxmlentry(tmpstr, "\"Title\":"), NULL, 0, 0);
		imdb->year = ostrcat(getxmlentry(tmpstr, "\"Year\":"), NULL, 0, 0);
		imdb->rated = ostrcat(getxmlentry(tmpstr, "\"Rated\":"), NULL, 0, 0);
		imdb->released = ostrcat(getxmlentry(tmpstr, "\"Released\":"), NULL, 0, 0);
		imdb->genre = ostrcat(getxmlentry(tmpstr, "\"Genre\":"), NULL, 0, 0);
		imdb->director = ostrcat(getxmlentry(tmpstr, "\"Director\":"), NULL, 0, 0);
		imdb->writer = ostrcat(getxmlentry(tmpstr, "\"Writer\":"), NULL, 0, 0);
		imdb->actors = ostrcat(getxmlentry(tmpstr, "\"Actors\":"), NULL, 0, 0);
		imdb->plot = ostrcat(getxmlentry(tmpstr, "\"Plot\":"), NULL, 0, 0);
		imdb->poster = ostrcat(getxmlentry(tmpstr, "\"Poster\":"), NULL, 0, 0);
		imdb->runtime = ostrcat(getxmlentry(tmpstr, "\"Runtime\":"), NULL, 0, 0);
		imdb->rating = ostrcat(getxmlentry(tmpstr, "\"Rating\":"), NULL, 0, 0);
		imdb->votes = ostrcat(getxmlentry(tmpstr, "\"Votes\":"), NULL, 0, 0);
		imdb->id = ostrcat(getxmlentry(tmpstr, "\"ID\":"), NULL, 0, 0);

		if(imdb->poster != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", imdb->poster, 0);

			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}

			gethttp(ip, path, 80, TMPIMDBPIC, NULL, NULL, 0);
			free(ip); ip = NULL;

		}
		free(tmpstr); tmpstr = NULL;
	}

	return imdb;
}

void screenimdb(char* title)
{
	int rcret = 0;
	struct skin* imdbskin = getscreen("imdb");
	struct skin* skin_plot = getscreennode(imdbskin, "plot");
	struct skin* skin_title = getscreennode(imdbskin, "title");
	struct skin* skin_director = getscreennode(imdbskin, "director");
	struct skin* skin_writers = getscreennode(imdbskin, "writers");
	struct skin* skin_genre = getscreennode(imdbskin, "genre");
	struct skin* skin_releasetime = getscreennode(imdbskin, "releasetime");
	struct skin* skin_bigcover = getscreennode(imdbskin, "bigcover");
	struct skin* skin_cover = getscreennode(imdbskin, "cover");
	struct skin* skin_actors = getscreennode(imdbskin, "actors");
	struct imdb* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	node = getimdb(title);
start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_director, node->director);
		changetext(skin_writers, node->writer);
		changetext(skin_genre, node->genre);
		changetext(skin_releasetime, node->released);
		changetext(skin_actors, node->actors);
		changepic(skin_cover, TMPIMDBPIC);
	}

	drawscreen(imdbskin, 0);

	while(1)
	{
		rcret = waitrc(imdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
                {
                        search = textinput("Search", NULL);
                        if(search != NULL)
                        {
				freeimdb(node); node = NULL;
				node = getimdb(search);
                                free(search); search = NULL;
				goto start;
                        }
                        drawscreen(imdbskin, 0);
                        continue;
                }
	}

	freeimdb(node); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbskin);
}

#endif
