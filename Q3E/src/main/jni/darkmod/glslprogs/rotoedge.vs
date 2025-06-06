#version 320 es
/*****************************************************************************
The Dark Mod GPL Source Code

This file is part of the The Dark Mod Source Code, originally based
on the Doom 3 GPL Source Code as published in 2011.

The Dark Mod Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Project: The Dark Mod (http://www.thedarkmod.com/)

******************************************************************************/

precision highp float;
precision highp int;
// !!ARBvp1.0 OPTION ARB_position_invariant ;

#pragma tdm_include "tdm_transform.glsl"

in vec4 attr_Position;
out vec4 var_tc0;
uniform vec4 u_localParam0;

void main() {
	
	//from sobel filter by John Rittenhouse
	vec4 size = vec4(0.0016);                                                                           //PARAM size = 0.0016;
	var_tc0 = (size) * (u_localParam0);                                                                 //MUL	result.texcoord[0], size, program.local[0];
	
	gl_Position = tdm_transform(attr_Position);
}
