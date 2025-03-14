!!ver 110
!!permu FOG
!!samps diffuse=0

#include "sys/defs.h"
#include "sys/fog.h"

varying vec2 tex_c;

#ifdef VERTEX_SHADER
void main ()
{
	tex_c = v_texcoord;
	gl_Position = ftetransform();
}
#endif

#ifdef FRAGMENT_SHADER
void main ()
{
	vec4 diffuse_f = texture2D( s_diffuse, fract(tex_c) );
	gl_FragColor = fog4( diffuse_f );
}
#endif
