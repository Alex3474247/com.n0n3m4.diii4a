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

uniform sampler2D u_texture;

in vec2 var_TexCoord0;
in vec4 var_Color;

out vec4 FragColor;

void main() {
	FragColor = texture(u_texture, var_TexCoord0) * var_Color;
}
