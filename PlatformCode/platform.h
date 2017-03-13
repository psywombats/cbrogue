#include "Rogue.h"

struct brogueConsole {
	void (*gameLoop)();
	bool (*pauseForMilliseconds)(short milliseconds);
	void (*nextKeyOrMouseEvent)(rogueEvent *returnEvent, bool textInput, bool colorsDance);
	void (*plotChar)(uchar, short, short, short, short, short, short, short, short);
	void (*remap)(const char *, const char *);
	bool (*modifierHeld)(int modifier);
};

void loadKeymap();

#ifdef BROGUE_TCOD
extern struct brogueConsole tcodConsole;
#endif

#ifdef BROGUE_CURSES
extern struct brogueConsole cursesConsole;
#endif

extern struct brogueConsole currentConsole;
extern bool serverMode;
extern bool noMenu;

