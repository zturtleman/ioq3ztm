There are various sound files played when a CTF flag is picked up, captured, returned, or a base is attacked in Team Arena's Overload game mode. These sounds are for either "your team" or "enemy team". These do not work well in splitscreen, and in the case of CTF flags being taken no sound is played if there are local players on the red and blue teams.

You should add new sounds to fix the issues, it's up to you to decide what sounds you want / when you want them.

In code/cgame/cg\_event.c search for "EV\_GLOBAL\_TEAM\_SOUND", you should find the following:
```c

case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
```

First there is a loop that check what teams the local players on on. Then there is a note about the sound issue this is a guide to solve. Feel free to look over the following `"case GTS_*"` code and ZTM: NOTE: comments about sound issues.

## Minor ##
Currently "your team" sound effects for red/blue flag capture and red/blue flag return are always played if splitscreen with players on both red and blue teams.

You may want to make GTS\_RED\_CAPTURE, GTS\_BLUE\_CAPTURE, GTS\_RED\_RETURN, and GTS\_BLUE\_RETURN always play the same sound regardless of which teams the local players are on to avoid confusion.

Flag captures also have GTS\_REDTEAM\_SCORED, GTS\_TEAMS\_TIED, or GTS\_REDTEAM\_LEADS sent to client (or blue versions) which have announcer saying what happened, so it's pretty minor.

The flag return sound effects are played under the announcer saying "red flag returns" or "blue flag returns", so it's pretty minor.

## Major ##
These aren't "might be confusing". The announcer is non-specific/lies or silent. However they cannot be fixed without adding new sounds. How they are fixed is up to you.

### CTF / 1FCTF ###
No sound is played when a CTF flag is taken by a network player in splitscreen with local players on both the red and blue teams.

If a local splitscreen player picked up the flag, then "You have the flag!" is played in CG\_CheckLocalSounds. But if a network player picked up the flag, than "your team" or "enemy team" picked up the flag is played, unless in splitscreen with local players on both the red and blue teams (as there is no correct sound to play).

I would suggest adding "Red team has the blue flag!" and "Blue team has the red flag!" sounds for CTF and "Red team has the flag!" and "Blue team has the flag!" sounds for 1 Flag CTF. You could get by with just the 1 flag CTF sounds if you want.

In code/cgame/cg\_local.h search for enemyTookYourFlagSound and add the following lines that are marked as new:
```c

sfxHandle_t	enemyTookYourFlagSound;
sfxHandle_t yourTeamTookEnemyFlagSound;
sfxHandle_t	redTeamTookBlueFlagSound; // new
sfxHandle_t	blueTeamTookRedFlagSound; // new
sfxHandle_t	youHaveFlagSound;
#ifdef MISSIONPACK
sfxHandle_t	enemyTookTheFlagSound;
sfxHandle_t yourTeamTookTheFlagSound;
sfxHandle_t	redTeamTookTheFlagSound; // new
sfxHandle_t	blueTeamTookTheFlagSound; // new
sfxHandle_t yourBaseIsUnderAttackSound;
#endif
```

In code/cgame/cg\_main.c again search for enemyTookYourFlagSound and add the last two lines below:
```c

cgs.media.enemyTookYourFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_enemy_flag.wav", qtrue );
cgs.media.yourTeamTookEnemyFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_team_flag.wav", qtrue );
cgs.media.redTeamTookBlueFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_red_flag.wav", qtrue );
cgs.media.blueTeamTookRedFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_blue_flag.wav", qtrue );
```

In code/cgame/cg\_main.c, down a few lines, add the last two lines below:
```c

cgs.media.yourTeamTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_team_1flag.wav", qtrue );
cgs.media.enemyTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_enemy_1flag.wav", qtrue );
cgs.media.redTeamTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_red_1flag.wav", qtrue );
cgs.media.blueTeamTookTheFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_blue_1flag.wav", qtrue );
```

In code/cgame/cg\_event.c search for GTS\_RED\_TAKEN: **This is the current code:**
```

case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
/* if this player picked up the flag then a sound is played in CG_CheckLocalSounds */
if (localHasBlue || localHasNeutral) {
}
else if (!(redTeam && blueTeam)) {
if (blueTeam) {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
}
else if (redTeam) {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
}
} else {
// ZTM: NOTE: There are local players on both teams, so have no correct sound to play. New games should fix this.
}
```

If you want to continue to use the "your team" and "enemy team" sounds, I recommend the following (new code not indented so you can see the changes):
```

case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
/* if this player picked up the flag then a sound is played in CG_CheckLocalSounds */
if (localHasBlue || localHasNeutral) {
}
else if (!(redTeam && blueTeam)) {
if (blueTeam) {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
}
else if (redTeam) {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
}
} else {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.redTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.redTeamTookBlueFlagSound );
}
```

If you don't care to continue to use the "your team" and "enemy team" sounds (which might seem inconsistent with per-team sounds):  **_I think this is the best option, though it does change the game for non-splitscreen._**
```

case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
/* if this player picked up the flag then a sound is played in CG_CheckLocalSounds */
if (localHasBlue || localHasNeutral) {
} else {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.redTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound( cgs.media.redTeamTookBlueFlagSound );
}
```

If you find specifying the flag color in CTF unnecessary, feel free to do this:
```

} else {
CG_AddBufferedSound( cgs.media.redTeamTookTheFlagSound );
}
```

Remember to update GTS\_BLUE\_TAKEN to match the changes to GTS\_RED\_TAKEN. (Using blueTeamTookTheFlagSound and blueTeamTookRedFlagSound.)

#### Conclusion ####
There are many ways to do it, feel free to do it however or leave it as is, at least you know of the bug.

Feel free to remove the "ZTM: NOTE:" comments about the sound issues after you fix them.

If you don't use yourTeamTookEnemyFlagSound, enemyTookYourFlagSound, yourTeamTookTheFlagSound, or enemyTookTheFlagSound feel free to remove them from cg\_local.h and cg\_main.c.

### You Have The Flag ###
"You have the flag!" might be confusing in splitscreen. This probably pretty minor. A player probably knows if they touch the flag or can take a quick glance at the HUD to check.

But if you're interested in changing it, here some suggestions.

Open code/cgame/cg\_playerstate.c and search for youHaveFlag. **This is the current code:**
```c

// check for flag pickup
if ( cgs.gametype > GT_TEAM ) {
if ((ps->powerups[PW_REDFLAG] != ops->powerups[PW_REDFLAG] && ps->powerups[PW_REDFLAG]) ||
(ps->powerups[PW_BLUEFLAG] != ops->powerups[PW_BLUEFLAG] && ps->powerups[PW_BLUEFLAG]) ||
(ps->powerups[PW_NEUTRALFLAG] != ops->powerups[PW_NEUTRALFLAG] && ps->powerups[PW_NEUTRALFLAG]) )
{
trap_S_StartLocalSound( cgs.media.youHaveFlagSound, CHAN_ANNOUNCER );
}
}
```

#### Use Team Pickup Sounds In Splitscreen ####
If you've added team flag pickup sounds, from farther up on this page, you could use the new team flag taken sounds when there is more than 1 player. (based on code from [ioid3](http://ioid3.googlecode.com))
```c

// check for flag pickup
if ( cgs.gametype > GT_TEAM ) {
if ((ps->powerups[PW_REDFLAG] != ops->powerups[PW_REDFLAG] && ps->powerups[PW_REDFLAG]) ||
(ps->powerups[PW_BLUEFLAG] != ops->powerups[PW_BLUEFLAG] && ps->powerups[PW_BLUEFLAG]) ||
(ps->powerups[PW_NEUTRALFLAG] != ops->powerups[PW_NEUTRALFLAG] && ps->powerups[PW_NEUTRALFLAG]) )
{
if (cg.snap->numPSs > 1) {
if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.blueTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound(cgs.media.blueTeamTookRedFlagSound);
} else {
#ifdef MISSIONPACK
if (cgs.gametype == GT_1FCTF)
CG_AddBufferedSound( cgs.media.redTeamTookTheFlagSound );
else
#endif
CG_AddBufferedSound(cgs.media.redTeamTookBlueFlagSound);
}
}
else
trap_S_StartLocalSound( cgs.media.youHaveFlagSound, CHAN_ANNOUNCER );
}
}
```

Feel free to adjust which sounds are played based on what you did for team flag taken sounds. Examples: using redTeamTookTheFlagSound instead of redTeamTookBlueFlagSound.

#### Per-player Sounds ####
Another idea is having "Player # has the flag!" sounds for each local player.

```c

// check for flag pickup
if ( cgs.gametype > GT_TEAM ) {
if ((ps->powerups[PW_REDFLAG] != ops->powerups[PW_REDFLAG] && ps->powerups[PW_REDFLAG]) ||
(ps->powerups[PW_BLUEFLAG] != ops->powerups[PW_BLUEFLAG] && ps->powerups[PW_BLUEFLAG]) ||
(ps->powerups[PW_NEUTRALFLAG] != ops->powerups[PW_NEUTRALFLAG] && ps->powerups[PW_NEUTRALFLAG]) )
{
if (cg.snap->numPSs > 1) {
trap_S_StartLocalSound( cgs.media.playerHasFlagSound[cg.cur_localClientNum], CHAN_ANNOUNCER );
}
else
trap_S_StartLocalSound( cgs.media.youHaveFlagSound, CHAN_ANNOUNCER );
}
}
```

In code/cgame/cg\_local.h search for youHaveFlagSound and add last line below.
```c

sfxHandle_t	youHaveFlagSound;
sfxHandle_t	playerHasFlagSound[MAX_SPLITVIEW];
```

In code/cgame/cg\_main.c again search for youHaveFlagSound and add the following lines before it (Note: cgs.media.youHaveFlagSound = ... code exists twice add this below each)
```c

cgs.media.youHaveFlagSound = trap_S_RegisterSound( "sound/teamplay/voc_you_flag.wav", qtrue );
for ( i = 0; i < CG_MaxSplitView(); i++ ) {
cgs.media.playerHasFlagSound[i] = trap_S_RegisterSound( va( "sound/teamplay/voc_player%d_flag.wav", i+1 ), qtrue );
}
```

Now the following sounds will be used in splitscreen:
  * sound/teamplay/voc\_player1\_flag.wav
  * sound/teamplay/voc\_player2\_flag.wav
  * sound/teamplay/voc\_player3\_flag.wav
  * sound/teamplay/voc\_player4\_flag.wav

And sound/teamplay/voc\_you\_flag.wav will still be used in non-splitscreen.

### Overload ###
"Your base is under attack" sound is always played in Team Arena's Overload game mode in splitscreen with local players on both the red and blue teams. Players do not know _which_ team's base is attacked!

I would suggest adding "Red base is under attack!" and "Blue base is under attack!" sounds.

In code/cgame/cg\_local.h search for yourBaseIsUnderAttackSound and add the following 2 lines after it:
```c

sfxHandle_t yourBaseIsUnderAttackSound;
sfxHandle_t redBaseIsUnderAttackSound;
sfxHandle_t blueBaseIsUnderAttackSound;
```

In code/cgame/cg\_main.c again search for yourBaseIsUnderAttackSound and add the following 2 lines after it:
```c

cgs.media.yourBaseIsUnderAttackSound = trap_S_RegisterSound( "sound/teamplay/voc_base_attack.wav", qtrue );
cgs.media.redBaseIsUnderAttackSound = trap_S_RegisterSound( "sound/teamplay/voc_red_base_attack.wav", qtrue );
cgs.media.blueBaseIsUnderAttackSound = trap_S_RegisterSound( "sound/teamplay/voc_blue_base_attack.wav", qtrue );
```

In code/cgame/cg\_event.c search for GTS\_REDOBELISK\_ATTACKED. **This is the current code:**
```c

case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
if (redTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
}
break;
case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
if (blueTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
}
break;
```

You could use the following to play red/blue version if there are players on both teams (new code not indented so you can see the changes):
```c

case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
if (redTeam && blueTeam) {
CG_AddBufferedSound( cgs.media.redBaseIsUnderAttackSound );
} else
if (redTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
}
break;
case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
if (redTeam && blueTeam) {
CG_AddBufferedSound( cgs.media.blueBaseIsUnderAttackSound );
} else
if (blueTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
}
break;
```

Or replacing using of cgs.media.yourBaseIsUnderAttackSound with red and blue versions. Though I think this might lead to some confusion, like "why does it only tell me when my team is attacked?"

```c

case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
if (redTeam) {
CG_AddBufferedSound( cgs.media.redBaseIsUnderAttackSound );
}
break;
case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
if (blueTeam) {
CG_AddBufferedSound( cgs.media.blueBaseIsUnderAttackSound );
}
break;
```

You could opt to always play the red and blue versions even if no local player is on that team, so you know when fellow players are attacking the other team. Also makes it so spectator hear base under attack sounds. **_I think this is the best option, though it does change the game for non-splitscreen._**

```c

case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
CG_AddBufferedSound( cgs.media.redBaseIsUnderAttackSound );
break;
case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
CG_AddBufferedSound( cgs.media.blueBaseIsUnderAttackSound );
break;
```

If you still want to use yourBaseIsUnderAttackSound, see below. Though it might be strange to use "your team" and whatever the color of the other team is. Could add an additional "enemy base under attack" for when don't have any local players on that team but have some on the other team...
```c

case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
if (redTeam && !blueTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
//} else if (blueTeam && !redTeam) {
// play enemy base under attack
} else {
CG_AddBufferedSound( cgs.media.redBaseIsUnderAttackSound );
}
break;
case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
if (blueTeam && !redTeam) {
CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
//} else if (redTeam && !blueTeam) {
// play enemy base under attack
} else {
CG_AddBufferedSound( cgs.media.blueBaseIsUnderAttackSound );
}
break;
```

#### Conclusion ####
There are many ways to do it, feel free to do it however or leave it as is, at least you know of the bug.

Feel free to remove the "ZTM: NOTE:" comments about the sound issues after you fix them.

If you don't use yourBaseIsUnderAttackSound, feel free to remove it from cg\_local.h and cg\_main.c.