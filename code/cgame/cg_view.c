/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//
// cg_view.c -- setup all the parameters (position, angle, etc)
// for a 3D rendering
#include "cg_local.h"


/*
=============================================================================

  MODEL TESTING

The viewthing and gun positioning tools from Q2 have been integrated and
enhanced into a single model testing facility.

Model viewing can begin with either "testmodel <modelname>" or "testgun <modelname>".

The names must be the full pathname after the basedir, like 
"models/weapons/v_launch/tris.md3" or "players/male/tris.md3"

Testmodel will create a fake entity 100 units in front of the current view
position, directly facing the viewer.  It will remain immobile, so you can
move around it to view it from different angles.

Testgun will cause the model to follow the player around and supress the real
view weapon model.  The default frame 0 of most guns is completely off screen,
so you will probably have to cycle a couple frames to see it.

"nextframe", "prevframe", "nextskin", and "prevskin" commands will change the
frame or skin of the testmodel.  These are bound to F5, F6, F7, and F8 in
q3default.cfg.

If a gun is being tested, the "gun_x", "gun_y", and "gun_z" variables will let
you adjust the positioning.

Note that none of the model testing features update while the game is paused, so
it may be convenient to test with deathmatch set to 1 so that bringing down the
console doesn't pause the game.

=============================================================================
*/

/*
=================
CG_TestModel_f

Creates an entity in front of the current position, which
can then be moved around
=================
*/
void CG_TestModel_f (void) {
	vec3_t		angles;

	memset( &cg.testModelEntity, 0, sizeof(cg.testModelEntity) );
	if ( trap_Argc() < 2 ) {
		return;
	}

	Q_strncpyz (cg.testModelName, CG_Argv( 1 ), MAX_QPATH );
	cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );

	if ( trap_Argc() == 3 ) {
		cg.testModelEntity.backlerp = atof( CG_Argv( 2 ) );
		cg.testModelEntity.frame = 1;
		cg.testModelEntity.oldframe = 0;
	}
	if (! cg.testModelEntity.hModel ) {
		CG_Printf( "Can't register model\n" );
		return;
	}

	VectorMA( cg.refdef.vieworg, 100, cg.refdef.viewaxis[0], cg.testModelEntity.origin );

	angles[PITCH] = 0;
	angles[YAW] = 180 + cg.refdefViewAngles[1];
	angles[ROLL] = 0;

	AnglesToAxis( angles, cg.testModelEntity.axis );
	cg.testGun = qfalse;
}

/*
=================
CG_TestGun_f

Replaces the current view weapon with the given model
=================
*/
void CG_TestGun_f (void) {
	CG_TestModel_f();
	cg.testGun = qtrue;
	cg.testModelEntity.renderfx = RF_DEPTHHACK | RF_NO_MIRROR | RF_MINLIGHT;
}


void CG_TestModelNextFrame_f (void) {
	cg.testModelEntity.frame++;
	CG_Printf( "frame %i\n", cg.testModelEntity.frame );
}

void CG_TestModelPrevFrame_f (void) {
	cg.testModelEntity.frame--;
	if ( cg.testModelEntity.frame < 0 ) {
		cg.testModelEntity.frame = 0;
	}
	CG_Printf( "frame %i\n", cg.testModelEntity.frame );
}

void CG_TestModelNextSkin_f (void) {
	cg.testModelEntity.skinNum++;
	CG_Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

void CG_TestModelPrevSkin_f (void) {
	cg.testModelEntity.skinNum--;
	if ( cg.testModelEntity.skinNum < 0 ) {
		cg.testModelEntity.skinNum = 0;
	}
	CG_Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

static void CG_AddTestModel (void) {
	int		i;

	// re-register the model, because the level may have changed
	cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );
	if (! cg.testModelEntity.hModel ) {
		CG_Printf ("Can't register model\n");
		return;
	}

	// if testing a gun, set the origin relative to the view origin
	if ( cg.testGun ) {
		VectorCopy( cg.refdef.vieworg, cg.testModelEntity.origin );
		VectorCopy( cg.refdef.viewaxis[0], cg.testModelEntity.axis[0] );
		VectorCopy( cg.refdef.viewaxis[1], cg.testModelEntity.axis[1] );
		VectorCopy( cg.refdef.viewaxis[2], cg.testModelEntity.axis[2] );

		// allow the position to be adjusted
		for (i=0 ; i<3 ; i++) {
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[0][i] * cg_gun_x.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[1][i] * cg_gun_y.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[2][i] * cg_gun_z.value;
		}
	}

	trap_R_AddRefEntityToScene( &cg.testModelEntity );
}



//============================================================================


/*
=================
CG_CalcVrect

Sets the coordinates of the viewport and rendered window
=================
*/
static void CG_CalcVrect (void) {
	int		size;
	float		viewWidth, viewHeight;
	float		viewX, viewY;

	// Viewport size
	viewX = viewY = 0;
	viewWidth = cgs.glconfig.vidWidth;
	viewHeight = cgs.glconfig.vidHeight;

	// Splitscreen viewports
	if (cg.numViewports == 2) {
		if (cg_splitviewVertical.integer) {
			viewWidth *= 0.5f;

			if (cg.viewport == 1) {
				viewX += viewWidth;
			}
		} else {
			viewHeight *= 0.5f;

			if (cg.viewport == 1) {
				viewY += viewHeight;
			}
		}
	} else if (cg.numViewports == 3) {
		if (cg_splitviewVertical.integer) {
			if (cg.viewport == 2) {
				viewWidth *= 0.5f;
				viewX += viewWidth;
			} else {
				viewWidth *= 0.5f;
				viewHeight *= 0.5f;

				if (cg.viewport == 1) {
					viewY += viewHeight;
				}
			}
		} else {
			if (cg.viewport == 2) {
				viewHeight *= 0.5f;
				viewY += viewHeight;
			} else {
				viewWidth *= 0.5f;
				viewHeight *= 0.5f;

				if (cg.viewport == 1) {
					viewX += viewWidth;
				}
			}
		}
	} else if (cg.numViewports > 1 && cg.numViewports <= 4) {
		viewWidth *= 0.5f;
		viewHeight *= 0.5f;

		if (cg.viewport == 1 || cg.viewport == 3) {
			viewX += viewWidth;
		}

		if (cg.viewport == 2 || cg.viewport == 3) {
			viewY += viewHeight;
		}
	}

	// Viewport scale and offset
	cgs.screenXScaleStretch = viewWidth * (1.0/640.0);
	cgs.screenYScaleStretch = viewHeight * (1.0/480.0);
	if ( viewWidth * 480 > viewHeight * 640 ) {
		cgs.screenXScale = viewWidth * (1.0/640.0);
		cgs.screenYScale = viewHeight * (1.0/480.0);
		// wide screen
		cgs.screenXBias = 0.5 * ( viewWidth - ( viewHeight * (640.0/480.0) ) );
		cgs.screenXScale = cgs.screenYScale;
		// no narrow screen
		cgs.screenYBias = 0;
	} else {
		cgs.screenXScale = viewWidth * (1.0/640.0);
		cgs.screenYScale = viewHeight * (1.0/480.0);
		// narrow screen
		cgs.screenYBias = 0.5 * ( viewHeight - ( viewWidth * (480.0/640.0) ) );
		cgs.screenYScale = cgs.screenXScale;
		// no wide screen
		cgs.screenXBias = 0;
	}

	// the intermission should always be full screen
	if ( cg.cur_ps->pm_type == PM_INTERMISSION ) {
		size = 100;
	} else {
		// bound normal viewsize
		if (cg_viewsize.integer < 30) {
			trap_Cvar_Set ("cg_viewsize","30");
			size = 30;
		} else if (cg_viewsize.integer > 100) {
			trap_Cvar_Set ("cg_viewsize","100");
			size = 100;
		} else {
			size = cg_viewsize.integer;
		}
	}

	// Rendered window for drawing world
	cg.refdef.width = viewWidth*size/100;
	cg.refdef.width &= ~1;

	cg.refdef.height = viewHeight*size/100;
	cg.refdef.height &= ~1;

	cg.refdef.x = viewX + (viewWidth - cg.refdef.width)/2;
	cg.refdef.y = viewY + (viewHeight - cg.refdef.height)/2;
}

//==============================================================================


/*
===============
CG_OffsetThirdPersonView

===============
*/
#define	FOCUS_DISTANCE	512
static void CG_OffsetThirdPersonView( void ) {
	vec3_t		forward, right, up;
	vec3_t		view;
	vec3_t		focusAngles;
	trace_t		trace;
	static vec3_t	mins = { -4, -4, -4 };
	static vec3_t	maxs = { 4, 4, 4 };
	vec3_t		focusPoint;
	float		focusDist;
	float		forwardScale, sideScale;

	cg.refdef.vieworg[2] += cg.cur_lc->predictedPlayerState.viewheight;

	VectorCopy( cg.refdefViewAngles, focusAngles );

	// if dead, look at killer
	if ( cg.cur_lc->predictedPlayerState.stats[STAT_HEALTH] <= 0 ) {
		focusAngles[YAW] = cg.cur_lc->predictedPlayerState.stats[STAT_DEAD_YAW];
		cg.refdefViewAngles[YAW] = cg.cur_lc->predictedPlayerState.stats[STAT_DEAD_YAW];
	}

	if ( focusAngles[PITCH] > 45 ) {
		focusAngles[PITCH] = 45;		// don't go too far overhead
	}
	AngleVectors( focusAngles, forward, NULL, NULL );

	VectorMA( cg.refdef.vieworg, FOCUS_DISTANCE, forward, focusPoint );

	VectorCopy( cg.refdef.vieworg, view );

	view[2] += 8;

	cg.refdefViewAngles[PITCH] *= 0.5;

	AngleVectors( cg.refdefViewAngles, forward, right, up );

	forwardScale = cos( cg_thirdPersonAngle[cg.cur_localClientNum].value / 180 * M_PI );
	sideScale = sin( cg_thirdPersonAngle[cg.cur_localClientNum].value / 180 * M_PI );
	VectorMA( view, -cg_thirdPersonRange[cg.cur_localClientNum].value * forwardScale, forward, view );
	VectorMA( view, -cg_thirdPersonRange[cg.cur_localClientNum].value * sideScale, right, view );

	// trace a ray from the origin to the viewpoint to make sure the view isn't
	// in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything

	if (!cg_cameraMode.integer) {
		CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.cur_lc->predictedPlayerState.clientNum, MASK_SOLID );

		if ( trace.fraction != 1.0 ) {
			VectorCopy( trace.endpos, view );
			view[2] += (1.0 - trace.fraction) * 32;
			// try another trace to this position, because a tunnel may have the ceiling
			// close enogh that this is poking out

			CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.cur_lc->predictedPlayerState.clientNum, MASK_SOLID );
			VectorCopy( trace.endpos, view );
		}
	}


	VectorCopy( view, cg.refdef.vieworg );

	// select pitch to look at focus point from vieword
	VectorSubtract( focusPoint, cg.refdef.vieworg, focusPoint );
	focusDist = sqrt( focusPoint[0] * focusPoint[0] + focusPoint[1] * focusPoint[1] );
	if ( focusDist < 1 ) {
		focusDist = 1;	// should never happen
	}
	cg.refdefViewAngles[PITCH] = -180 / M_PI * atan2( focusPoint[2], focusDist );
	cg.refdefViewAngles[YAW] -= cg_thirdPersonAngle[cg.cur_localClientNum].value;
}


// this causes a compiler bug on mac MrC compiler
static void CG_StepOffset( void ) {
	int		timeDelta;
	
	// smooth out stair climbing
	timeDelta = cg.time - cg.cur_lc->stepTime;
	if ( timeDelta < STEP_TIME ) {
		cg.refdef.vieworg[2] -= cg.cur_lc->stepChange 
			* (STEP_TIME - timeDelta) / STEP_TIME;
	}
}

/*
===============
CG_OffsetFirstPersonView

===============
*/
static void CG_OffsetFirstPersonView( void ) {
	float			*origin;
	float			*angles;
	float			bob;
	float			ratio;
	float			delta;
	float			speed;
	float			f;
	vec3_t			predictedVelocity;
	int				timeDelta;
	
	if ( cg.cur_ps->pm_type == PM_INTERMISSION ) {
		return;
	}

	origin = cg.refdef.vieworg;
	angles = cg.refdefViewAngles;

	// if dead, fix the angle and don't add any kick
	if ( cg.cur_ps->stats[STAT_HEALTH] <= 0 ) {
		angles[ROLL] = 40;
		angles[PITCH] = -15;
		angles[YAW] = cg.cur_ps->stats[STAT_DEAD_YAW];
		origin[2] += cg.cur_lc->predictedPlayerState.viewheight;
		return;
	}

	// add angles based on damage kick
	if ( cg.cur_lc->damageTime ) {
		ratio = cg.time - cg.cur_lc->damageTime;
		if ( ratio < DAMAGE_DEFLECT_TIME ) {
			ratio /= DAMAGE_DEFLECT_TIME;
			angles[PITCH] += ratio * cg.cur_lc->v_dmg_pitch;
			angles[ROLL] += ratio * cg.cur_lc->v_dmg_roll;
		} else {
			ratio = 1.0 - ( ratio - DAMAGE_DEFLECT_TIME ) / DAMAGE_RETURN_TIME;
			if ( ratio > 0 ) {
				angles[PITCH] += ratio * cg.cur_lc->v_dmg_pitch;
				angles[ROLL] += ratio * cg.cur_lc->v_dmg_roll;
			}
		}
	}

	// add pitch based on fall kick
#if 0
	ratio = ( cg.time - cg.cur_lc->landTime) / FALL_TIME;
	if (ratio < 0)
		ratio = 0;
	angles[PITCH] += ratio * cg.fall_value;
#endif

	// add angles based on velocity
	VectorCopy( cg.cur_lc->predictedPlayerState.velocity, predictedVelocity );

	delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[0]);
	angles[PITCH] += delta * cg_runpitch.value;
	
	delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[1]);
	angles[ROLL] -= delta * cg_runroll.value;

	// add angles based on bob

	// make sure the bob is visible even at low speeds
	speed = cg.xyspeed > 200 ? cg.xyspeed : 200;

	delta = cg.bobfracsin * cg_bobpitch.value * speed;
	if (cg.cur_lc->predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3;		// crouching
	angles[PITCH] += delta;
	delta = cg.bobfracsin * cg_bobroll.value * speed;
	if (cg.cur_lc->predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3;		// crouching accentuates roll
	if (cg.bobcycle & 1)
		delta = -delta;
	angles[ROLL] += delta;

//===================================

	// add view height
	origin[2] += cg.cur_lc->predictedPlayerState.viewheight;

	// smooth out duck height changes
	timeDelta = cg.time - cg.cur_lc->duckTime;
	if ( timeDelta < DUCK_TIME) {
		cg.refdef.vieworg[2] -= cg.cur_lc->duckChange 
			* (DUCK_TIME - timeDelta) / DUCK_TIME;
	}

	// add bob height
	bob = cg.bobfracsin * cg.xyspeed * cg_bobup.value;
	if (bob > 6) {
		bob = 6;
	}

	origin[2] += bob;


	// add fall height
	delta = cg.time - cg.cur_lc->landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		f = delta / LAND_DEFLECT_TIME;
		cg.refdef.vieworg[2] += cg.cur_lc->landChange * f;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		delta -= LAND_DEFLECT_TIME;
		f = 1.0 - ( delta / LAND_RETURN_TIME );
		cg.refdef.vieworg[2] += cg.cur_lc->landChange * f;
	}

	// add step offset
	CG_StepOffset();

	// pivot the eye based on a neck length
#if 0
	{
#define	NECK_LENGTH		8
	vec3_t			forward, up;
 
	cg.refdef.vieworg[2] -= NECK_LENGTH;
	AngleVectors( cg.refdefViewAngles, forward, NULL, up );
	VectorMA( cg.refdef.vieworg, 3, forward, cg.refdef.vieworg );
	VectorMA( cg.refdef.vieworg, NECK_LENGTH, up, cg.refdef.vieworg );
	}
#endif
}

//======================================================================

void CG_ZoomDown( int localClientNum ) {
	cglc_t *lc = &cg.localClients[localClientNum];

	if ( lc->zoomed ) {
		return;
	}

	lc->zoomed = qtrue;
	lc->zoomTime = cg.time;
}

void CG_ZoomUp( int localClientNum ) {
	cglc_t *lc = &cg.localClients[localClientNum];

	if ( !lc->zoomed ) {
		return;
	}

	lc->zoomed = qfalse;
	lc->zoomTime = cg.time;
}

void CG_ZoomDown_f( void ) { 
	CG_ZoomDown(0);
}

void CG_ZoomUp_f( void ) { 
	CG_ZoomUp(0);
}

void CG_2ZoomDown_f( void ) { 
	CG_ZoomDown(1);
}

void CG_2ZoomUp_f( void ) { 
	CG_ZoomUp(1);
}

void CG_3ZoomDown_f( void ) { 
	CG_ZoomDown(2);
}

void CG_3ZoomUp_f( void ) { 
	CG_ZoomUp(2);
}

void CG_4ZoomDown_f( void ) { 
	CG_ZoomDown(3);
}

void CG_4ZoomUp_f( void ) { 
	CG_ZoomUp(3);
}


/*
====================
CG_CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
#define	WAVE_AMPLITUDE	1
#define	WAVE_FREQUENCY	0.4

static int CG_CalcFov( void ) {
	float	x;
	float	phase;
	float	v;
	int		contents;
	float	fov_x, fov_y;
	float	zoomFov;
	float	f;

	if ( cg.cur_lc->predictedPlayerState.pm_type == PM_INTERMISSION ) {
		// if in intermission, use a fixed value
		cg.fov = fov_x = 90;
	} else {
		// user selectable
		if ( cgs.dmflags & DF_FIXED_FOV ) {
			// dmflag to prevent wide fov for all clients
			fov_x = 90;
		} else {
			fov_x = cg_fov.value;
			if ( fov_x < 1 ) {
				fov_x = 1;
			} else if ( fov_x > 160 ) {
				fov_x = 160;
			}
		}

		cg.fov = fov_x;

		// account for zooms
		zoomFov = cg_zoomFov.value;
		if ( zoomFov < 1 ) {
			zoomFov = 1;
		} else if ( zoomFov > 160 ) {
			zoomFov = 160;
		}

		if ( cg.cur_lc->zoomed ) {
			f = ( cg.time - cg.cur_lc->zoomTime ) / (float)ZOOM_TIME;
			if ( f > 1.0 ) {
				fov_x = zoomFov;
			} else {
				fov_x = fov_x + f * ( zoomFov - fov_x );
			}
		} else {
			f = ( cg.time - cg.cur_lc->zoomTime ) / (float)ZOOM_TIME;
			if ( f <= 1.0 ) {
				fov_x = zoomFov + f * ( fov_x - zoomFov );
			}
		}
	}

	// Do FOV Correction for some viewports
	if ((cg.numViewports == 2) || (cg.numViewports == 3 && cg.viewport == 2)) {
		if (cg_splitviewVertical.integer == 1) {
			// Tall/narrow view
			fov_x *= 0.6f; // 0.5 would be correct, but fov gets real small.
		} else {
			// Short/wide view
			fov_x *= 1.4f; // 1.5 would be correct, but fov gets real big.
		}
	}
	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	// warp if underwater
	contents = CG_PointContents( cg.refdef.vieworg, -1 );
	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ){
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin( phase );
		fov_x += v;
		fov_y -= v;
		cg.refdef.rdflags |= RDF_UNDERWATER;
	}
	else {
		cg.refdef.rdflags &= ~RDF_UNDERWATER;
	}


	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

	if ( !cg.cur_lc->zoomed ) {
		cg.cur_lc->zoomSensitivity = 1;
	} else {
		cg.cur_lc->zoomSensitivity = cg.refdef.fov_y / 75.0;
	}

	return (cg.refdef.rdflags & RDF_UNDERWATER);
}



/*
===============
CG_DamageBlendBlob

===============
*/
static void CG_DamageBlendBlob( void ) {
	int			t;
	int			maxTime;
	refEntity_t		ent;

	if (!cg_blood.integer) {
		return;
	}

	if ( !cg.cur_lc->damageValue ) {
		return;
	}

	//if (cg.cameraMode) {
	//	return;
	//}

	// ragePro systems can't fade blends, so don't obscure the screen
	if ( cgs.glconfig.hardwareType == GLHW_RAGEPRO ) {
		return;
	}

	maxTime = DAMAGE_TIME;
	t = cg.time - cg.cur_lc->damageTime;
	if ( t <= 0 || t >= maxTime ) {
		return;
	}


	memset( &ent, 0, sizeof( ent ) );
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_NO_MIRROR;

	VectorMA( cg.refdef.vieworg, 8, cg.refdef.viewaxis[0], ent.origin );
	VectorMA( ent.origin, cg.cur_lc->damageX * -8, cg.refdef.viewaxis[1], ent.origin );
	VectorMA( ent.origin, cg.cur_lc->damageY * 8, cg.refdef.viewaxis[2], ent.origin );

	ent.radius = cg.cur_lc->damageValue * 3;
	ent.customShader = cgs.media.viewBloodShader;
	ent.shaderRGBA[0] = 255;
	ent.shaderRGBA[1] = 255;
	ent.shaderRGBA[2] = 255;
	ent.shaderRGBA[3] = 200 * ( 1.0 - ((float)t / maxTime) );
	trap_R_AddRefEntityToScene( &ent );
}


/*
===============
CG_CalcViewValues

Sets cg.refdef view values
===============
*/
static int CG_CalcViewValues( void ) {
	playerState_t	*ps;

	memset( &cg.refdef, 0, sizeof( cg.refdef ) );

	// strings for in game rendering
	// Q_strncpyz( cg.refdef.text[0], "Park Ranger", sizeof(cg.refdef.text[0]) );
	// Q_strncpyz( cg.refdef.text[1], "19", sizeof(cg.refdef.text[1]) );

	// calculate size of 3D view
	CG_CalcVrect();

	ps = &cg.cur_lc->predictedPlayerState;
/*
	if (cg.cameraMode) {
		vec3_t origin, angles;
		if (trap_getCameraInfo(cg.time, &origin, &angles)) {
			VectorCopy(origin, cg.refdef.vieworg);
			angles[ROLL] = 0;
			VectorCopy(angles, cg.refdefViewAngles);
			AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );
			return CG_CalcFov();
		} else {
			cg.cameraMode = qfalse;
		}
	}
*/
	// intermission view
	if ( ps->pm_type == PM_INTERMISSION ) {
		VectorCopy( ps->origin, cg.refdef.vieworg );
		VectorCopy( ps->viewangles, cg.refdefViewAngles );
		AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );
		return CG_CalcFov();
	}

	cg.bobcycle = ( ps->bobCycle & 128 ) >> 7;
	cg.bobfracsin = fabs( sin( ( ps->bobCycle & 127 ) / 127.0 * M_PI ) );
	cg.xyspeed = sqrt( ps->velocity[0] * ps->velocity[0] +
		ps->velocity[1] * ps->velocity[1] );


	VectorCopy( ps->origin, cg.refdef.vieworg );
	VectorCopy( ps->viewangles, cg.refdefViewAngles );

	if (cg_cameraOrbit.integer) {
		if (cg.time > cg.nextOrbitTime) {
			cg.nextOrbitTime = cg.time + cg_cameraOrbitDelay.integer;
			cg_thirdPersonAngle[cg.cur_localClientNum].value += cg_cameraOrbit.value;
		}
	}
	// add error decay
	if ( cg_errorDecay.value > 0 ) {
		int		t;
		float	f;

		t = cg.time - cg.cur_lc->predictedErrorTime;
		f = ( cg_errorDecay.value - t ) / cg_errorDecay.value;
		if ( f > 0 && f < 1 ) {
			VectorMA( cg.refdef.vieworg, f, cg.cur_lc->predictedError, cg.refdef.vieworg );
		} else {
			cg.cur_lc->predictedErrorTime = 0;
		}
	}

	if ( cg.cur_lc->renderingThirdPerson ) {
		// back away from character
		CG_OffsetThirdPersonView();
	} else {
		// offset for local bobbing and kicks
		CG_OffsetFirstPersonView();
	}

	// position eye relative to origin
	AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );

	if ( cg.cur_lc->hyperspace ) {
		cg.refdef.rdflags |= RDF_NOWORLDMODEL | RDF_HYPERSPACE;
	}

	// field of view
	return CG_CalcFov();
}


/*
=====================
CG_PowerupTimerSounds
=====================
*/
static void CG_PowerupTimerSounds( void ) {
	int		i;
	int		t;

	// powerup timers going away
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		t = cg.cur_ps->powerups[i];
		if ( t <= cg.time ) {
			continue;
		}
		if ( t - cg.time >= POWERUP_BLINKS * POWERUP_BLINK_TIME ) {
			continue;
		}
		if ( ( t - cg.time ) / POWERUP_BLINK_TIME != ( t - cg.oldTime ) / POWERUP_BLINK_TIME ) {
			trap_S_StartSound( NULL, cg.cur_ps->clientNum, CHAN_ITEM, cgs.media.wearOffSound );
		}
	}
}

/*
=====================
CG_AddBufferedSound
=====================
*/
void CG_AddBufferedSound( sfxHandle_t sfx ) {
	if ( !sfx )
		return;
	cg.soundBuffer[cg.soundBufferIn] = sfx;
	cg.soundBufferIn = (cg.soundBufferIn + 1) % MAX_SOUNDBUFFER;
	if (cg.soundBufferIn == cg.soundBufferOut) {
		cg.soundBufferOut++;
	}
}

/*
=====================
CG_PlayBufferedSounds
=====================
*/
static void CG_PlayBufferedSounds( void ) {
	if ( cg.soundTime < cg.time ) {
		if (cg.soundBufferOut != cg.soundBufferIn && cg.soundBuffer[cg.soundBufferOut]) {
			trap_S_StartLocalSound(cg.soundBuffer[cg.soundBufferOut], CHAN_ANNOUNCER);
			cg.soundBuffer[cg.soundBufferOut] = 0;
			cg.soundBufferOut = (cg.soundBufferOut + 1) % MAX_SOUNDBUFFER;
			cg.soundTime = cg.time + 750;
		}
	}
}

//=========================================================================

/*
**  Frustum code
*/

// some culling bits
typedef struct plane_s {
	vec3_t normal;
	float dist;
} plane_t;

static plane_t frustum[4];

//
//	CG_SetupFrustum
//
void CG_SetupFrustum( void ) {
	int i;
	float xs, xc;
	float ang;

	ang = cg.refdef.fov_x / 180 * M_PI * 0.5f;
	xs = sin( ang );
	xc = cos( ang );

	VectorScale( cg.refdef.viewaxis[0], xs, frustum[0].normal );
	VectorMA( frustum[0].normal, xc, cg.refdef.viewaxis[1], frustum[0].normal );

	VectorScale( cg.refdef.viewaxis[0], xs, frustum[1].normal );
	VectorMA( frustum[1].normal, -xc, cg.refdef.viewaxis[1], frustum[1].normal );

	ang = cg.refdef.fov_y / 180 * M_PI * 0.5f;
	xs = sin( ang );
	xc = cos( ang );

	VectorScale( cg.refdef.viewaxis[0], xs, frustum[2].normal );
	VectorMA( frustum[2].normal, xc, cg.refdef.viewaxis[2], frustum[2].normal );

	VectorScale( cg.refdef.viewaxis[0], xs, frustum[3].normal );
	VectorMA( frustum[3].normal, -xc, cg.refdef.viewaxis[2], frustum[3].normal );

	for ( i = 0 ; i < 4 ; i++ ) {
		frustum[i].dist = DotProduct( cg.refdef.vieworg, frustum[i].normal );
	}
}

//
//	CG_CullPoint - returns true if culled
//
qboolean CG_CullPoint( vec3_t pt ) {
	int i;
	plane_t *frust;

	// check against frustum planes
	for ( i = 0 ; i < 4 ; i++ ) {
		frust = &frustum[i];

		if ( ( DotProduct( pt, frust->normal ) - frust->dist ) < 0 ) {
			return( qtrue );
		}
	}

	return( qfalse );
}

qboolean CG_CullPointAndRadius( const vec3_t pt, vec_t radius ) {
	int i;
	plane_t *frust;

	// check against frustum planes
	for ( i = 0 ; i < 4 ; i++ ) {
		frust = &frustum[i];

		if ( ( DotProduct( pt, frust->normal ) - frust->dist ) < -radius ) {
			return( qtrue );
		}
	}

	return( qfalse );
}

//=========================================================================

/*
=================
CG_DrawActiveFrame

Generates and draws a game scene and status information at the given time.
=================
*/
void CG_DrawActiveFrame( int serverTime, stereoFrame_t stereoView, qboolean demoPlayback ) {
	int		inwater;
	qboolean renderClientViewport[MAX_SPLITVIEW];
	int		i;

	cg.time = serverTime;
	cg.demoPlayback = demoPlayback;

	// update cvars
	CG_UpdateCvars();

	// if we are only updating the screen as a loading
	// pacifier, don't even try to read snapshots
	if ( cg.infoScreenText[0] != 0 ) {
		CG_DrawInformation();
		return;
	}

	// any looped sounds will be respecified as entities
	// are added to the render list
	trap_S_ClearLoopingSounds(qfalse);

	// clear all the render lists
	trap_R_ClearScene();

	// set up cg.snap and possibly cg.nextSnap
	CG_ProcessSnapshots();

	// if we haven't received any snapshots yet, all
	// we can draw is the information screen
	if ( !cg.snap || ( cg.snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) ) {
		CG_DrawInformation();
		return;
	}

	// this counter will be bumped for every valid scene we generate
	cg.clientFrame++;

	// Use single camera/viewport at intermission
	for (i = 0; i < CG_MaxSplitView(); i++) {
		if (cg.snap->lcIndex[i] != -1 && cg.snap->pss[i].pm_type != PM_INTERMISSION) {
			// client present and not at intermission, keep viewports separate.
			break;
		}
	}
	cg.singleCamera = (cg.snap->numPSs > 1) && (i == CG_MaxSplitView());

	cg.numViewports = 0;
	for (i = 0; i < CG_MaxSplitView(); i++) {
		if (cg.snap->lcIndex[i] == -1) {
			renderClientViewport[i] = qfalse;
			continue;
		}
		cg.cur_localClientNum = i;
		cg.cur_lc = &cg.localClients[i];
		cg.cur_ps = &cg.snap->pss[cg.snap->lcIndex[i]];

		// Check if viewport should be drawn.
		if ((cg.singleCamera && cg.numViewports >= 1) || (cg.cur_ps->persistant[PERS_TEAM] == TEAM_SPECTATOR && (cg.cur_ps->pm_flags & PMF_LOCAL_HIDE))) {
			renderClientViewport[i] = qfalse;
		} else {
			cg.numViewports++;
			renderClientViewport[i] = qtrue;
		}

		// let the client system know what our weapon and zoom settings are
		trap_SetUserCmdValue( cg.cur_lc->weaponSelect, cg.cur_lc->zoomSensitivity, cg.cur_localClientNum );

		// update cg.predictedPlayerState
		CG_PredictPlayerState();
	}

	// If all local clients dropped out from playing still draw main local client.
	if (cg.numViewports == 0) {
		cg.numViewports = 1;
		renderClientViewport[0] = qtrue;
	}

	// play lead change annoucement and time/frag limit warnings
	CG_CheckGameSounds();

	for (i = 0, cg.viewport = -1; i < CG_MaxSplitView(); i++) {
		if (!renderClientViewport[i]) {
			continue;
		}
		cg.viewport++;
		cg.cur_localClientNum = i;
		cg.cur_lc = &cg.localClients[i];
		cg.cur_ps = &cg.snap->pss[cg.snap->lcIndex[i]];

		// decide on third person view
		cg.cur_lc->renderingThirdPerson = cg_thirdPerson[cg.cur_localClientNum].integer || (cg.cur_ps->stats[STAT_HEALTH] <= 0);

		CG_PB_ClearPolyBuffers();

		// build cg.refdef
		inwater = CG_CalcViewValues();
		CG_SetupFrustum();

		// first person blend blobs, done after AnglesToAxis
		if ( !cg.cur_lc->renderingThirdPerson ) {
			CG_DamageBlendBlob();
		}

		// build the render lists
		if ( !cg.cur_lc->hyperspace ) {
			CG_AddPacketEntities();			// adter calcViewValues, so predicted player state is correct
			CG_AddMarks();
			CG_AddParticles ();
			CG_AddLocalEntities();
			CG_AddAtmosphericEffects();
		}
		CG_AddViewWeapon( &cg.cur_lc->predictedPlayerState );

		// add buffered sounds
		CG_PlayBufferedSounds();

#ifdef MISSIONPACK
		// play buffered voice chats
		CG_PlayBufferedVoiceChats();
#endif

		// finish up the rest of the refdef
		if ( cg.testModelEntity.hModel ) {
			CG_AddTestModel();
		}
		cg.refdef.time = cg.time;
		memcpy( cg.refdef.areamask, cg.snap->areamask, sizeof( cg.refdef.areamask ) );

		// warning sounds when powerup is wearing off
		CG_PowerupTimerSounds();

		// update audio positions
		trap_S_Respatialize( cg.cur_ps->clientNum, cg.refdef.vieworg, cg.refdef.viewaxis, inwater, !cg.cur_lc->renderingThirdPerson );

		// make sure the lagometerSample and frame timing isn't done twice when in stereo
		if ( stereoView != STEREO_RIGHT && cg.viewport == 0 ) {
			cg.frametime = cg.time - cg.oldTime;
			if ( cg.frametime < 0 ) {
				cg.frametime = 0;
			}
			cg.oldTime = cg.time;
			CG_AddLagometerFrameInfo();
		}
		if (cg_timescale.value != cg_timescaleFadeEnd.value) {
			if (cg_timescale.value < cg_timescaleFadeEnd.value) {
				cg_timescale.value += cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
				if (cg_timescale.value > cg_timescaleFadeEnd.value)
					cg_timescale.value = cg_timescaleFadeEnd.value;
			}
			else {
				cg_timescale.value -= cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
				if (cg_timescale.value < cg_timescaleFadeEnd.value)
					cg_timescale.value = cg_timescaleFadeEnd.value;
			}
			if (cg_timescaleFadeSpeed.value) {
				trap_Cvar_Set("timescale", va("%f", cg_timescale.value));
			}
		}

		// actually issue the rendering calls
		CG_DrawActive( stereoView );
	}

	// load any models that have been deferred if a scoreboard is shown
	if ( !CG_AnyScoreboardShowing() ) {
		cg.deferredPlayerLoading = 0;
	} else if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}

	if (cg.numViewports != 1) {
		// Setup single viewport
		cg.numViewports = 1;
		cg.viewport = 0;

		// calculate size of viewport
		CG_CalcVrect();
	}

	// Not drawing single client view.
	cg.cur_lc = NULL;
	cg.cur_ps = NULL;
	cg.cur_localClientNum = -1;

	// Remove expired console lines
	if( cg.consoleLines[ 0 ].time + cg_consoleLatency.integer < cg.time && cg_consoleLatency.integer > 0 ) {
		CG_RemoveNotifyLine( );
	}

	// Draw over all viewports
	CG_DrawScreen2D( stereoView );

	if ( cg_stats.integer ) {
		CG_Printf( "cg.clientFrame:%i\n", cg.clientFrame );
	}


}

