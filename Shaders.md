New shader features not found in Quake III Arena 1.32b or ioquake3.

## General Keywords ##
General Keywords are outside shader stages.

### implicitMap ###
Added from WolfET.

### implicitMask ###
Added from WolfET.

### implicitBlend ###
Added from WolfET.

### distancecull ###
distancecull is used for foliage. Added from WolfET.

Usage: distancecull <opaque distance> <transparent distance> <alpha threshold>

NOTE: Foliage is currently not supported by Rend2.


## Per-Stage Keywords ##
### lightmap ###
NOTE: Currently not supported by Rend2. Added from WolfET.

Usage: lightmap < string >

  * Example: lightmap path/to/image
    * Custom lightmap.
  * Example: lightmap $dlight
    * Uses image for dynamic lights.
  * Example: lightmap $lightmap
    * Uses lightmap set in map BSP.


## Waveform Types ##
### Noise ###
Waveform is random values between min and max. It's a discontinuous function.

Noise keyword is recognized by Quake III, but isn't usable. It's fixed in Spearmint using code from ioSTVEF.

### Random ###
Waveform will be either on or off, randomly, each frame. It's a discontinuous function. Added from ioSTVEF.