/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "xos4 Terminus:size=10" };
static const char dmenufont[]       = "xos4 Terminus:size=10";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#333333";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#18381a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  "#9ccc9c"  },
	[SchemeTitle]  = { col_gray4, col_gray2,  col_gray2  },
	[SchemeTitleSel]  = { "#FFFFFF", col_cyan,  col_gray2  },
};

/* tagging */
static const char *tags[] = { "1:term", "2:www", "3:dev", "4:docs", "5:irc", "6:file", "7:misc", "8:desk" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     		NULL,       NULL,       0,            1,           -1 },
	{ "Chromium",		NULL,       NULL,       1 << 1,       0,           -1 },
//	{ "Zathura",  		NULL,       NULL,       1 << 4,       0,           -1 },
	{ "code-oss",           NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Emacs",    		NULL,       NULL,       1 << 2,       0,           -1 },
	{ "TelegramDesktop",    NULL,       NULL,       1 << 4,       0,           -1 },
	{ "discord",    	NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Pcmanfm",  		NULL,	    NULL,       1 << 5,       0,           -1 },
	{ "KeePassXC",  	NULL,	    NULL,       1 << 7,       0,           -1 },
//	{ "Firefox",  		NULL, 	    NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-p", "exec:", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *slockcmd[] = { "slock",  NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
        { MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
        { MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,			XK_space,  focusmaster,    {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ Mod1Mask|ShiftMask,           XK_h,      setmfact,       {.f = -0.05} },
	{ Mod1Mask|ShiftMask,           XK_l,      setmfact,       {.f = +0.05} },
	{ Mod1Mask|ShiftMask,           XK_j,      setcfact,       {.f = +0.25} },
	{ Mod1Mask|ShiftMask,           XK_k,      setcfact,       {.f = -0.25} },
	{ Mod1Mask|ShiftMask,           XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	/*{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },*/
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,	   spawn,	   {.v = slockcmd } },
	{ MODKEY|ControlMask,           XK_Right,           	view_adjacent,  { .i = +1 } },
	{ MODKEY|ControlMask,           XK_Left,           	view_adjacent,  { .i = -1 } },
	{ 0,                            XK_Print,  spawn,          SHCMD("maim ~/Pictures/Screenshots/$(date +%Y-%m-%d-%H%M%S)_scrot.png ; notify-send 'Screen captured'") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("maim -s ~/Pictures/Screenshots/$(date +%Y-%m-%d-%H%M%S)_scrot.png -u ; notify-send 'Screen captured'") },
	{ MODKEY,                            XK_Print,  spawn,          SHCMD("maim | xclip -selection clipboard -t image/png ; notify-send 'Screen captured and copied to clipboard'") },
	{ MODKEY|ShiftMask,                    XK_Print,  spawn,          SHCMD("maim -s -u | xclip -selection clipboard -t image/png ; notify-send 'Screen captured and copied to clipboard'") },
        { 0,              XF86XK_AudioRaiseVolume, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +1000 ; notify-send -h string:x-dunst-stack-tag:test Volume: $(pactl list sinks | awk '/Volume:/ { print $5 }' | tail -n+$((sink_nr+1)) | head -n1 | cut -d '%' -f 1)%") },
        { 0,              XF86XK_AudioLowerVolume, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -1000 ; notify-send -h string:x-dunst-stack-tag:test Volume: $(pactl list sinks | awk '/Volume:/ { print $5 }' | tail -n+$((sink_nr+1)) | head -n1 | cut -d '%' -f 1)%")  },
        { 0,              XF86XK_AudioMute,        spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle ; notify-send 'Volume: Mute'") },
	{ MODKEY,              XK_F3, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +1000 ; notify-send -h string:x-dunst-stack-tag:test Volume: $(pactl list sinks | awk '/Volume:/ { print $5 }' | tail -n+$((sink_nr+1)) | head -n1 | cut -d '%' -f 1)%") },
        { MODKEY,              XK_F2, spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -1000 ; notify-send -h string:x-dunst-stack-tag:test Volume: $(pactl list sinks | awk '/Volume:/ { print $5 }' | tail -n+$((sink_nr+1)) | head -n1 | cut -d '%' -f 1)%")  },
        { MODKEY,              XK_F1,        spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle ; notify-send 'Volume: Mute'") },
	{ 0,              XF86XK_MonBrightnessUp,  spawn,          SHCMD("xbacklight -inc 10; notify-send -h string:x-dunst-stack-tag:test Brightness: $(xbacklight -get | awk '{print int($1)}')%") },
	{ 0,              XF86XK_MonBrightnessDown,spawn,          SHCMD("xbacklight -dec 10; notify-send -h string:x-dunst-stack-tag:test Brightness: $(xbacklight -get | awk '{print int($1)}')%") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

