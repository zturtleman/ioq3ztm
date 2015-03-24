# Spearmint 1.0 Roadmap #
Spearmint 1.0 is intended to support Quake 3 (including OpenArena and ioquake3-compatible derivative games), Tremulous, World of Padman, and Turtle Arena engine features. Games/mods will have to be ported to Spearmint to be playable. Some features will be implemented differently in Spearmint than the games previously mentioned.

Spearmint 1.0 may have partial support for RTCW and WolfET engine features, support for them will be considered as development continues.

Roadmap subject to change. Spearmint 1.0 ETA June 2013.

May switch to SDL 2.0 if it is released before Spearmint 1.0.

## Improve Mod Support ##
  * Improve mod API handling.
    * DONE: Don't allow vms to make syscalls when engine asks for API version. (Engine don't know if it's compatible yet.)
    * Add API version to game and cgame.
    * Add major and minor numbers for API. Major changes when API compatiblity to broken, minor is increased when API is extended in ways able to still run pervious minor versions.
  * DONE: Move game-specific cvars out of client into cgame/ui.
  * Make playerState\_t and entityState\_t modable, included networking code in code/qcommon/msg.c
  * Add support for (most if not all) features added in Tremulous and Turtle Arena.

## Improve Handling Multiple Separate Games ##
Currently ioq3-games have to rebuild the engine just to change these.

  * Add support for games to set window title (full and mini).
  * Add support for games to set window icon.
    * Note: SDL 1.2.x only supports 32x32, but SDL 2.0 adds support for larger icons.
  * Add support for games to set com\_gamename

---

  * Add support for games to set config dir within .spearmint dir (e.g. .spearmint/game-dir/mod-dir)?
    * (Currently can't have multiple mods with the same name, so not sure reason to do this?)

---

  * Need a way to tell what game (and mod) a mods is based on, so com\_basegame and fs\_basegame can be set correctly when a mod is loaded. And so it's possible list mods only for a specific game?

## Extend Base Game (Q3A) ##
As all games have to be ported to Spearmint, it's diffidently worth improving the central core they share.

  * DONE: Use clip region feature added from Tremulous.
  * DONE: Added "addbot random" to add a random bot. Also added to bot select in UI.
  * Add support for "!gametype" entity key for specifying which gametypes to not spawn an entity. (From OpenArena.)

Possibly:
  * Add func\_breakable for destroyable brushes. (There are various different games which add this, all different of course.)
  * Breakable objects, misc\_gamemodel? Based on code from WolfET.
  * Ladder support (though, quake3 players lack animations for climbing ladders).

I'll probably add more sound entity keys from EntityPlus and Turtle Arena.

## Input ##
  * Touch-screen controls (added in cgame or ui so mods can control them).

## Sound ##
  * Add support multiple listeners, mainly for splitscreen.
  * Add support for sound shaders similar to Doom 3.
  * Add support for playing Opus sounds.
  * Use Opus, instead of Speex, for VOIP.

## Video ##
  * Add support for Ogg Theora playback (with Vorbis or Opus audio).

## Models ##
  * Add support for loading models in game to get tag locations.
  * Add support for modifying IQM skeletons in cgame, ui, and game for rendering / getting tags.

## i10n ##
  * Add support for UTF-8 in game chat messages, only usable with games that use TrueType fonts.
  * Possibly: Support for translating server/client/game/cgame/ui using textget will be considered.

## Platforms ##
  * Add support for running client on Android.
  * Add support for running client on Ouya.

## Documentation ##
  * Document how to create a 'game' and 'mod' running on unmodified Spearmint.
  * Document new/changed mod API functions (might try to document them all).
  * Document new entity types and new entity keys.
  * Document new shader stuff, like the new waveform types noise and random.
  * Document sound shader system.
  * Create credits list.