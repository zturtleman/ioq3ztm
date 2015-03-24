If you're making a new game using Spearmint, these are some suggested changes you may want to make.

## Engine ##
If you're game isn't going to be a mod (only changing game, cgame, and ui), you should probably rebrand the engine.

  * Change settings in Makefile. (BASEGAME, VERSION, CLIENT, SERVER, etc)
  * Change defines at the top of code/qcommon/q\_shared.h
  * Optionally add pk3 checksums at top of code/qcommon/files.c for proper pure support

In code/qcommon/q\_shared.h, near the top of the file is the following:
```c

#define PRODUCT_NAME			"Spearmint"
#define BASEGAME			"baseq3"
#define CLIENT_WINDOW_TITLE     	"Spearmint"
#define CLIENT_WINDOW_MIN_TITLE 	"Spearmint"
#define HOMEPATH_NAME_UNIX		".spearmint"
#define HOMEPATH_NAME_WIN		"Spearmint"
#define HOMEPATH_NAME_MACOSX		HOMEPATH_NAME_WIN
#define GAMENAME_FOR_MASTER		"Spearmint"	// must NOT contain whitespace

#ifndef PRODUCT_VERSION
#define PRODUCT_VERSION "1.36"
#endif
```

BASEGAME is the default loaded mod. You should probably keep it in sync with BASEGAME in Makefile.

Changing GAMENAME\_FOR\_MASTER will make sure your game is separated from other games in the server browser.

PRODUCT\_VERSION here is only used by Code::Blocks and MSVC projects. You should keep it in sync with PRODUCT\_VERSION in Makefile.

## Cgame ##
You may want to add new CTF/1FCTF/Overload sounds for splitscreen, see NewSounds page.