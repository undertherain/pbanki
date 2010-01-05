iconfig *cfgMain;

static char *menuLayoutVariants[] = { "100", "150", "200", "300", NULL };
static char *menuAnswerControls[] = { "buttons", "crosspiece", NULL };

iconfigedit configEditor[] = 
{
	{ CFG_CHOICE, NULL, "Layout", "set front field height", "layout", "300", menuLayoutVariants, NULL },
	{ CFG_INDEX, NULL, "Answer", "buttons or cross control", "answer_style", "0", menuAnswerControls, NULL },
	{ CFG_FONT, NULL, "Front Font", "font of front field", "front_font", DEFAULTFONT",50", NULL, NULL },
	{ CFG_FONT, NULL, "Back Font", "font of back field", "back_font", DEFAULTFONT",40", NULL, NULL },
	{ 0, NULL, NULL, NULL, NULL, NULL, NULL}
};


ifont *ReadFont(iconfig *cfg, char *name, char *deflt)
{
	char *fname, buf[256];
	int size;
	strcpy(buf, ReadString(cfg, name, deflt));
	fname = strtok(buf, ",");
	sscanf(fname+strlen(fname)+1, "%d", &size);
	ifont *fnt = OpenFont(fname, size, 0);
	return fnt;
}
