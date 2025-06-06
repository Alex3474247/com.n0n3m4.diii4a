/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/
#include "../server/exe_headers.h"

#include "tr_local.h"

/*

  for a projection shadow:

  point[x] += light vector * ( z - shadow plane )
  point[y] +=
  point[z] = shadow plane

  1 0 light[x] / light[z]

*/

#ifdef _GLES //karin: optmize shadow volume render
#define _STENCIL_SHADOW_OPT
#endif

#define _STENCIL_REVERSE

typedef struct {
	int		i2;
	int		facing;
} edgeDef_t;

#define	MAX_EDGE_DEFS	32

static	edgeDef_t	edgeDefs[SHADER_MAX_VERTEXES][MAX_EDGE_DEFS];
static	int			numEdgeDefs[SHADER_MAX_VERTEXES];
static	int			facing[SHADER_MAX_INDEXES/3];
#ifdef _STENCIL_SHADOW_OPT //karin: use more memory for shadow volume and using index by glDrawElements
// sil edge triangles indexes
static glIndex_t indexes[6*MAX_EDGE_DEFS*SHADER_MAX_VERTEXES];
// num of sil edge triangles indexes
static int idx = 0;
// front and far cap triangles indexes
static glIndex_t cap_indexes[SHADER_MAX_INDEXES * 2];
// num of front and far triangles indexes
static int cap_idx = 0;
// shadow volume indexes generated
static qboolean shadow_volume_generated = qfalse;
// shadow vertexes(front and far, not use tess object)
static	vec3_t		shadowXyz[SHADER_MAX_VERTEXES * 2];
#else
static	vec3_t		shadowXyz[SHADER_MAX_VERTEXES];
#endif


void R_AddEdgeDef( int i1, int i2, int facing ) {
	int		c;

	c = numEdgeDefs[ i1 ];
	if ( c == MAX_EDGE_DEFS ) {
		return;		// overflow
	}
	edgeDefs[ i1 ][ c ].i2 = i2;
	edgeDefs[ i1 ][ c ].facing = facing;

	numEdgeDefs[ i1 ]++;
}

#ifdef _STENCIL_SHADOW_OPT //karin: render shadow volume's sil edge and front/far caps
static void R_RenderShadowVolume( void )
{
	if(idx > 0)
	{
		GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
		GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
		GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

		if (!glva)
			qglEnableClientState( GL_VERTEX_ARRAY );
		if (gltca)
			qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglDisableClientState( GL_COLOR_ARRAY );

		qglVertexPointer(3, GL_FLOAT, 0, shadowXyz);
		qglDrawElements(GL_TRIANGLES, idx, GL_INDEX_TYPE, indexes);
		if(cap_idx > 0)
		{
			qglDrawElements(GL_TRIANGLES, cap_idx, GL_INDEX_TYPE, cap_indexes);
		}
		if (!glva)
			qglDisableClientState( GL_VERTEX_ARRAY );
		if (gltca)
			qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglEnableClientState( GL_COLOR_ARRAY );

	}
}
#endif

void R_RenderShadowEdges( void ) {
	int		i;
	int		c;
	int		j;
	int		i2;
	//int		c_edges, c_rejected;
#if 0
	int		c2, k;
	int		hit[2];
#endif
#ifdef _STENCIL_REVERSE
	int		numTris;
	int		o1, o2, o3;
#endif
#ifdef _STENCIL_SHADOW_OPT //karin: if shadow volume generated, only draw it; else regenerate
	if(shadow_volume_generated)
	{
		R_RenderShadowVolume();
		return;
	}
	idx = 0;
	cap_idx = 0;
#endif

	// an edge is NOT a silhouette edge if its face doesn't face the light,
	// or if it has a reverse paired edge that also faces the light.
	// A well behaved polyhedron would have exactly two faces for each edge,
	// but lots of models have dangling edges or overfanned edges
#if 0
	c_edges = 0;
	c_rejected = 0;
#endif

	for ( i = 0 ; i < tess.numVertexes ; i++ ) {
		c = numEdgeDefs[ i ];
		for ( j = 0 ; j < c ; j++ ) {
			if ( !edgeDefs[ i ][ j ].facing ) {
				continue;
			}

			//with this system we can still get edges shared by more than 2 tris which
			//produces artifacts including seeing the shadow through walls. So for now
			//we are going to render all edges even though it is a tiny bit slower. -rww
#if 1
			i2 = edgeDefs[ i ][ j ].i2;
#ifdef _GLES //karin: glBegin/glEnd
#ifdef _STENCIL_SHADOW_OPT //karin: only record GL_TRIANGLES indexes of sil edge
			indexes[idx++] = i;
			indexes[idx++] = i + SHADER_MAX_VERTEXES;
			indexes[idx++] = i2;
			indexes[idx++] = i2;
			indexes[idx++] = i + SHADER_MAX_VERTEXES;
			indexes[idx++] = i2 + SHADER_MAX_VERTEXES;
#else
            {
                GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
                GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
                GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

                if (!glva)
                    qglEnableClientState( GL_VERTEX_ARRAY );
                if (gltca)
                    qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
                if (glca)
                    qglDisableClientState( GL_COLOR_ARRAY );

                GLfloat vs[] = {
                    tess.xyz[ i ][0],tess.xyz[ i ][1],tess.xyz[ i ][2],
                    shadowXyz[ i ][0],shadowXyz[ i ][1],shadowXyz[ i ][2],
                    tess.xyz[ i2 ][0],tess.xyz[ i2 ][1],tess.xyz[ i2 ][2],
                    shadowXyz[ i2 ][0],shadowXyz[ i2 ][1],shadowXyz[ i2 ][2],
                };
                qglVertexPointer(3, GL_FLOAT, 0, vs);
                qglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                if (!glva)
                    qglDisableClientState( GL_VERTEX_ARRAY );
                if (gltca)
                    qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
                if (glca)
                    qglEnableClientState( GL_COLOR_ARRAY );
            }
#endif
#else
			qglBegin( GL_TRIANGLE_STRIP );
				qglVertex3fv( tess.xyz[ i ] );
				qglVertex3fv( shadowXyz[ i ] );
				qglVertex3fv( tess.xyz[ i2 ] );
				qglVertex3fv( shadowXyz[ i2 ] );
			qglEnd();
#endif
#else
			hit[0] = 0;
			hit[1] = 0;

			i2 = edgeDefs[ i ][ j ].i2;
			c2 = numEdgeDefs[ i2 ];
			for ( k = 0 ; k < c2 ; k++ ) {
				if ( edgeDefs[ i2 ][ k ].i2 == i ) {
					hit[ edgeDefs[ i2 ][ k ].facing ]++;
				}
			}

			// if it doesn't share the edge with another front facing
			// triangle, it is a sil edge
			if ( hit[ 1 ] == 0 ) {
				qglBegin( GL_TRIANGLE_STRIP );
				qglVertex3fv( tess.xyz[ i ] );
				qglVertex3fv( shadowXyz[ i ] );
				qglVertex3fv( tess.xyz[ i2 ] );
				qglVertex3fv( shadowXyz[ i2 ] );
				qglEnd();
				c_edges++;
			} else {
				c_rejected++;
			}
#endif
		}
	}

#ifdef _STENCIL_REVERSE
	//Carmack Reverse<tm> method requires that volumes
	//be capped properly -rww
	numTris = tess.numIndexes / 3;

	for ( i = 0 ; i < numTris ; i++ )
	{
		if ( !facing[i] )
		{
			continue;
		}

		o1 = tess.indexes[ i*3 + 0 ];
		o2 = tess.indexes[ i*3 + 1 ];
		o3 = tess.indexes[ i*3 + 2 ];

#ifdef _GLES //karin: glBegin/glEnd
#ifdef _STENCIL_SHADOW_OPT //karin: only record GL_TRIANGLES indexes of front and far caps
		cap_indexes[cap_idx++] = o1;
		cap_indexes[cap_idx++] = o2;
		cap_indexes[cap_idx++] = o3;
		cap_indexes[cap_idx++] = o3 + SHADER_MAX_VERTEXES;
		cap_indexes[cap_idx++] = o2 + SHADER_MAX_VERTEXES;
		cap_indexes[cap_idx++] = o1 + SHADER_MAX_VERTEXES;
#else
		{
			GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
			GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
			GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

			if (!glva)
				qglEnableClientState( GL_VERTEX_ARRAY );
			if (gltca)
				qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
			if (glca)
				qglDisableClientState( GL_COLOR_ARRAY );

			glIndex_t is[] = {
				(glIndex_t)o1, (glIndex_t)o2, (glIndex_t)o3,
			};
			qglVertexPointer(3, GL_FLOAT, 0, &tess.xyz[0]);
			qglDrawElements(GL_TRIANGLES, 3, GL_INDEX_TYPE, is);

			glIndex_t sis[] = {
				(glIndex_t)o3, (glIndex_t)o2, (glIndex_t)o1,
			};
			qglVertexPointer(3, GL_FLOAT, 0, shadowXyz);
			qglDrawElements(GL_TRIANGLES, 3, GL_INDEX_TYPE, sis);
			if (!glva)
				qglDisableClientState( GL_VERTEX_ARRAY );
			if (gltca)
				qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
			if (glca)
				qglEnableClientState( GL_COLOR_ARRAY );
		}
#endif
#else
		qglBegin(GL_TRIANGLES);
			qglVertex3fv(tess.xyz[o1]);
			qglVertex3fv(tess.xyz[o2]);
			qglVertex3fv(tess.xyz[o3]);
		qglEnd();
		qglBegin(GL_TRIANGLES);
			qglVertex3fv(shadowXyz[o3]);
			qglVertex3fv(shadowXyz[o2]);
			qglVertex3fv(shadowXyz[o1]);
		qglEnd();
#endif
	}

#ifdef _STENCIL_SHADOW_OPT //karin: first draw it and mark shadow volume generated
	R_RenderShadowVolume();
	shadow_volume_generated = qtrue;
#endif
#endif
}

//#define _DEBUG_STENCIL_SHADOWS

/*
=================
RB_ShadowTessEnd

triangleFromEdge[ v1 ][ v2 ]


  set triangle from edge( v1, v2, tri )
  if ( facing[ triangleFromEdge[ v1 ][ v2 ] ] && !facing[ triangleFromEdge[ v2 ][ v1 ] ) {
  }
=================
*/
void RB_DoShadowTessEnd( vec3_t lightPos );
void RB_ShadowTessEnd( void )
{
#if 0
	if (backEnd.currentEntity &&
		(backEnd.currentEntity->directedLight[0] ||
			backEnd.currentEntity->directedLight[1] ||
			backEnd.currentEntity->directedLight[2]))
	{ //an ent that has its light set for it
		RB_DoShadowTessEnd(NULL);
		return;
	}

//	if (!tess.dlightBits)
//	{
//		return;
//	}

	int i = 0;
	dlight_t *dl;

	R_TransformDlights( backEnd.refdef.num_dlights, backEnd.refdef.dlights, &backEnd.ori );
/*	while (i < tr.refdef.num_dlights)
	{
		if (tess.dlightBits & (1 << i))
		{
			dl = &tr.refdef.dlights[i];

			RB_DoShadowTessEnd(dl->transformed);
		}

		i++;
	}
	*/
			dl = &tr.refdef.dlights[0];

			RB_DoShadowTessEnd(dl->transformed);

#else //old ents-only way
	RB_DoShadowTessEnd(NULL);
#endif
}

void RB_DoShadowTessEnd( vec3_t lightPos )
{
	int		i;
	int		numTris;
	vec3_t	lightDir;

	if ( glConfig.stencilBits < 4 ) {
		return;
	}

#if 1 //controlled method - try to keep shadows in range so they don't show through so much -rww
	vec3_t	worldxyz;
	vec3_t	entLight;
	float	groundDist;

	VectorCopy( backEnd.currentEntity->lightDir, entLight );
	entLight[2] = 0.0f;
	VectorNormalize(entLight);

	//Oh well, just cast them straight down no matter what onto the ground plane.
	//This presets no chance of screwups and still looks better than a stupid
	//shader blob.
	VectorSet(lightDir, entLight[0]*0.3f, entLight[1]*0.3f, 1.0f);
	// project vertexes away from light direction
	for ( i = 0 ; i < tess.numVertexes ; i++ ) {
		//add or.origin to vert xyz to end up with world oriented coord, then figure
		//out the ground pos for the vert to project the shadow volume to
		VectorAdd(tess.xyz[i], backEnd.ori.origin, worldxyz);
		groundDist = worldxyz[2] - backEnd.currentEntity->e.shadowPlane;
		groundDist += 16.0f; //fudge factor
#ifdef _STENCIL_SHADOW_OPT //karin: all vertexes save to shadowXyz, not use tess object
		memcpy(shadowXyz[i], tess.xyz[i], sizeof(vec3_t));
		VectorMA( tess.xyz[i], -groundDist, lightDir, shadowXyz[SHADER_MAX_VERTEXES + i] );
#else
		VectorMA( tess.xyz[i], -groundDist, lightDir, shadowXyz[i] );
#endif
	}
#else
	if (lightPos)
	{
		for ( i = 0 ; i < tess.numVertexes ; i++ )
		{
			shadowXyz[i][0] = tess.xyz[i][0]+(( tess.xyz[i][0]-lightPos[0] )*128.0f);
			shadowXyz[i][1] = tess.xyz[i][1]+(( tess.xyz[i][1]-lightPos[1] )*128.0f);
			shadowXyz[i][2] = tess.xyz[i][2]+(( tess.xyz[i][2]-lightPos[2] )*128.0f);
		}
	}
	else
	{
		VectorCopy( backEnd.currentEntity->lightDir, lightDir );

		// project vertexes away from light direction
		for ( i = 0 ; i < tess.numVertexes ; i++ ) {
			VectorMA( tess.xyz[i], -512, lightDir, shadowXyz[i] );
		}
	}
#endif
	// decide which triangles face the light
	memset( numEdgeDefs, 0, 4 * tess.numVertexes );

	numTris = tess.numIndexes / 3;
	for ( i = 0 ; i < numTris ; i++ ) {
		int		i1, i2, i3;
		vec3_t	d1, d2, normal;
		float	*v1, *v2, *v3;
		float	d;

		i1 = tess.indexes[ i*3 + 0 ];
		i2 = tess.indexes[ i*3 + 1 ];
		i3 = tess.indexes[ i*3 + 2 ];

		v1 = tess.xyz[ i1 ];
		v2 = tess.xyz[ i2 ];
		v3 = tess.xyz[ i3 ];

		if (!lightPos)
		{
			VectorSubtract( v2, v1, d1 );
			VectorSubtract( v3, v1, d2 );
			CrossProduct( d1, d2, normal );

			d = DotProduct( normal, lightDir );
		}
		else
		{
			float planeEq[4];
			planeEq[0] = v1[1]*(v2[2]-v3[2]) + v2[1]*(v3[2]-v1[2]) + v3[1]*(v1[2]-v2[2]);
			planeEq[1] = v1[2]*(v2[0]-v3[0]) + v2[2]*(v3[0]-v1[0]) + v3[2]*(v1[0]-v2[0]);
			planeEq[2] = v1[0]*(v2[1]-v3[1]) + v2[0]*(v3[1]-v1[1]) + v3[0]*(v1[1]-v2[1]);
			planeEq[3] = -( v1[0]*( v2[1]*v3[2] - v3[1]*v2[2] ) +
						v2[0]*(v3[1]*v1[2] - v1[1]*v3[2]) +
						v3[0]*(v1[1]*v2[2] - v2[1]*v1[2]) );

			d = planeEq[0]*lightPos[0]+
				planeEq[1]*lightPos[1]+
				planeEq[2]*lightPos[2]+
				planeEq[3];
		}

		if ( d > 0 ) {
			facing[ i ] = 1;
		} else {
			facing[ i ] = 0;
		}

		// create the edges
		R_AddEdgeDef( i1, i2, facing[ i ] );
		R_AddEdgeDef( i2, i3, facing[ i ] );
		R_AddEdgeDef( i3, i1, facing[ i ] );
	}

	GL_Bind( tr.whiteImage );
	//qglEnable( GL_CULL_FACE );
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO );

#ifndef _DEBUG_STENCIL_SHADOWS
	qglColor3f( 0.2f, 0.2f, 0.2f );

	// don't write to the color buffer
	qglColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

	qglEnable( GL_STENCIL_TEST );
	qglStencilFunc( GL_ALWAYS, 1, 255 );
#else
	qglColor3f( 1.0f, 0.0f, 0.0f );
	qglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//qglDisable(GL_DEPTH_TEST);
#endif

#ifdef _STENCIL_REVERSE
#ifdef _STENCIL_SHADOW_OPT //karin: request regenerate shadow volume
	shadow_volume_generated = qfalse;
#endif
	qglDepthFunc(GL_LESS);

	//now using the Carmack Reverse<tm> -rww
	if (glConfig.doStencilShadowsInOneDrawcall)
	{
		GL_Cull(CT_TWO_SIDED);
		qglStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		qglStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		R_RenderShadowEdges();
		qglDisable(GL_STENCIL_TEST);
	}
	else
	{
		GL_Cull(CT_FRONT_SIDED);
		qglStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

		R_RenderShadowEdges();

		GL_Cull(CT_BACK_SIDED);
		qglStencilOp(GL_KEEP, GL_DECR, GL_KEEP);

		R_RenderShadowEdges();
		qglDisable(GL_STENCIL_TEST); //karin: add
	}

	qglDepthFunc(GL_LEQUAL);
#else
	// mirrors have the culling order reversed
	if ( backEnd.viewParms.isMirror ) {
		qglCullFace( GL_FRONT );
		qglStencilOp( GL_KEEP, GL_KEEP, GL_INCR );

		R_RenderShadowEdges();

		qglCullFace( GL_BACK );
		qglStencilOp( GL_KEEP, GL_KEEP, GL_DECR );

		R_RenderShadowEdges();
	} else {
		qglCullFace( GL_BACK );
		qglStencilOp( GL_KEEP, GL_KEEP, GL_INCR );

		R_RenderShadowEdges();

		qglCullFace( GL_FRONT );
		qglStencilOp( GL_KEEP, GL_KEEP, GL_DECR );

		R_RenderShadowEdges();
	}
#endif

	// reenable writing to the color buffer
	qglColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

#ifdef _DEBUG_STENCIL_SHADOWS
	qglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}


/*
=================
RB_ShadowFinish

Darken everything that is is a shadow volume.
We have to delay this until everything has been shadowed,
because otherwise shadows from different body parts would
overlap and double darken.
=================
*/
void RB_ShadowFinish( void ) {
	if ( r_shadows->integer != 2 ) {
		return;
	}
	if ( glConfig.stencilBits < 4 ) {
		return;
	}

#ifdef _DEBUG_STENCIL_SHADOWS
	return;
#endif

	qglEnable( GL_STENCIL_TEST );
	qglStencilFunc( GL_NOTEQUAL, 0, 255 );

	qglStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

	bool planeZeroBack = false;
	if (qglIsEnabled(GL_CLIP_PLANE0))
	{
		planeZeroBack = true;
		qglDisable (GL_CLIP_PLANE0);
	}
	GL_Cull(CT_TWO_SIDED);
	//qglDisable (GL_CULL_FACE);

	GL_Bind( tr.whiteImage );

	qglPushMatrix();
    qglLoadIdentity ();

//	qglColor3f( 0.6f, 0.6f, 0.6f );
//	GL_State( GLS_DEPTHMASK_TRUE | GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO );

//	qglColor3f( 1, 0, 0 );
//	GL_State( GLS_DEPTHMASK_TRUE | GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO );

	qglColor4f( 0.0f, 0.0f, 0.0f, 0.5f );
	//GL_State( GLS_DEPTHMASK_TRUE | GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA );
	GL_State( GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA );

#ifdef _GLES //karin: glBegin/glEnd
	{
		GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
		GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
		GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

		if (!glva)
			qglEnableClientState( GL_VERTEX_ARRAY );
		if (gltca)
			qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglDisableClientState( GL_COLOR_ARRAY );

		GLfloat vs[] = {
			-100.0f,  100.0f, -10.0f,
			100.0f,  100.0f, -10.0f,
			100.0f, -100.0f, -10.0f,
			-100.0f, -100.0f, -10.0f
		};
		qglVertexPointer(3, GL_FLOAT, 0, vs);
		qglDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (!glva)
			qglDisableClientState( GL_VERTEX_ARRAY );
		if (gltca)
			qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglEnableClientState( GL_COLOR_ARRAY );
	}
#else
	qglBegin( GL_QUADS );
	qglVertex3f( -100, 100, -10 );
	qglVertex3f( 100, 100, -10 );
	qglVertex3f( 100, -100, -10 );
	qglVertex3f( -100, -100, -10 );
	qglEnd ();
#endif

	qglColor4f(1,1,1,1);
	qglDisable( GL_STENCIL_TEST );
	if (planeZeroBack)
	{
		qglEnable (GL_CLIP_PLANE0);
	}
	qglPopMatrix();
}


/*
=================
RB_ProjectionShadowDeform

=================
*/
void RB_ProjectionShadowDeform( void ) {
	float	*xyz;
	int		i;
	float	h;
	vec3_t	ground;
	vec3_t	light;
	float	groundDist;
	float	d;
	vec3_t	lightDir;

	xyz = ( float * ) tess.xyz;

	ground[0] = backEnd.ori.axis[0][2];
	ground[1] = backEnd.ori.axis[1][2];
	ground[2] = backEnd.ori.axis[2][2];

	groundDist = backEnd.ori.origin[2] - backEnd.currentEntity->e.shadowPlane;

	VectorCopy( backEnd.currentEntity->lightDir, lightDir );
	d = DotProduct( lightDir, ground );
	// don't let the shadows get too long or go negative
	if ( d < 0.5 ) {
		VectorMA( lightDir, (0.5 - d), ground, lightDir );
		d = DotProduct( lightDir, ground );
	}
	d = 1.0 / d;

	light[0] = lightDir[0] * d;
	light[1] = lightDir[1] * d;
	light[2] = lightDir[2] * d;

	for ( i = 0; i < tess.numVertexes; i++, xyz += 4 ) {
		h = DotProduct( xyz, ground ) + groundDist;

		xyz[0] -= light[0] * h;
		xyz[1] -= light[1] * h;
		xyz[2] -= light[2] * h;
	}
}

//update tr.screenImage
void RB_CaptureScreenImage(void)
{
	int radX = 2048;
	int radY = 2048;
	int x = glConfig.vidWidth/2;
	int y = glConfig.vidHeight/2;
	int cX, cY;

	GL_Bind( tr.screenImage );
	//using this method, we could pixel-filter the texture and all sorts of crazy stuff.
	//but, it is slow as hell.
	/*
	static byte *tmp = NULL;
	if (!tmp)
	{
		tmp = (byte *)R_Malloc((sizeof(byte)*4)*(glConfig.vidWidth*glConfig.vidHeight), TAG_ICARUS, qtrue);
	}
	qglReadPixels(0, 0, glConfig.vidWidth, glConfig.vidHeight, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
	qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
	*/

	if (radX > glConfig.maxTextureSize)
	{
		radX = glConfig.maxTextureSize;
	}
	if (radY > glConfig.maxTextureSize)
	{
		radY = glConfig.maxTextureSize;
	}

	while (glConfig.vidWidth < radX)
	{
		radX /= 2;
	}
	while (glConfig.vidHeight < radY)
	{
		radY /= 2;
	}

	cX = x-(radX/2);
	cY = y-(radY/2);

	if (cX+radX > glConfig.vidWidth)
	{ //would it go off screen?
		cX = glConfig.vidWidth-radX;
	}
	else if (cX < 0)
	{ //cap it off at 0
		cX = 0;
	}

	if (cY+radY > glConfig.vidHeight)
	{ //would it go off screen?
		cY = glConfig.vidHeight-radY;
	}
	else if (cY < 0)
	{ //cap it off at 0
		cY = 0;
	}

#ifdef _GLES //karin: GL_RGBA16
	qglCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cX, cY, radX, radY, 0);
#else
	qglCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, cX, cY, radX, radY, 0);
#endif
}


//yeah.. not really shadow-related.. but it's stencil-related. -rww
float tr_distortionAlpha = 1.0f; //opaque
float tr_distortionStretch = 0.0f; //no stretch override
qboolean tr_distortionPrePost = qfalse; //capture before postrender phase?
qboolean tr_distortionNegate = qfalse; //negative blend mode
void RB_DistortionFill(void)
{
	float alpha = tr_distortionAlpha;
	float spost = 0.0f;
	float spost2 = 0.0f;

	if ( glConfig.stencilBits < 4 )
	{
		return;
	}

	//ok, cap the stupid thing now I guess
	if (!tr_distortionPrePost)
	{
		RB_CaptureScreenImage();
	}

	qglEnable(GL_STENCIL_TEST);
	qglStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFF);
	qglStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	qglDisable (GL_CLIP_PLANE0);
	GL_Cull( CT_TWO_SIDED );

	//reset the view matrices and go into ortho mode
	qglMatrixMode(GL_PROJECTION);
	qglPushMatrix();
	qglLoadIdentity();
	qglOrtho(0, glConfig.vidWidth, glConfig.vidHeight, 32, -1, 1);
	qglMatrixMode(GL_MODELVIEW);
	qglPushMatrix();
	qglLoadIdentity();

	if (tr_distortionStretch)
	{ //override
		spost = tr_distortionStretch;
		spost2 = tr_distortionStretch;
	}
	else
	{ //do slow stretchy effect
		spost = sin(tr.refdef.time*0.0005f);
		if (spost < 0.0f)
		{
			spost = -spost;
		}
		spost *= 0.2f;

		spost2 = sin(tr.refdef.time*0.0005f);
		if (spost2 < 0.0f)
		{
			spost2 = -spost2;
		}
		spost2 *= 0.08f;
	}

	if (alpha != 1.0f)
	{ //blend
		GL_State(GLS_SRCBLEND_SRC_ALPHA|GLS_DSTBLEND_SRC_ALPHA);
	}
	else
	{ //be sure to reset the draw state
		GL_State(0);
	}

#ifdef _GLES //karin: glBegin/glEnd
	{
		GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
		GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
		GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

		if (!glva)
			qglEnableClientState( GL_VERTEX_ARRAY );
		if (!gltca)
			qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglDisableClientState( GL_COLOR_ARRAY );

		qglColor4f(1.0f, 1.0f, 1.0f, alpha);
		GLfloat vs[] = {
			0.0f+spost2, 1.0f-spost,
			0.0f, 0.0f,

			0.0f+spost2, 0.0f+spost,
			0.0f, (float)glConfig.vidHeight,

			1.0f-spost2, 0.0f+spost,
			(float)glConfig.vidWidth, (float)glConfig.vidHeight,

			1.0f-spost2, 1.0f-spost,
			(float)glConfig.vidWidth, 0.0f,
		};
		qglVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, vs + 2);
		qglTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, vs);
		qglDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (!glva)
			qglDisableClientState( GL_VERTEX_ARRAY );
		if (!gltca)
			qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglEnableClientState( GL_COLOR_ARRAY );
	}
#else
	qglBegin(GL_QUADS);
		qglColor4f(1.0f, 1.0f, 1.0f, alpha);
		qglTexCoord2f(0+spost2, 1-spost);
		qglVertex2f(0, 0);

		qglTexCoord2f(0+spost2, 0+spost);
		qglVertex2f(0, glConfig.vidHeight);

		qglTexCoord2f(1-spost2, 0+spost);
		qglVertex2f(glConfig.vidWidth, glConfig.vidHeight);

		qglTexCoord2f(1-spost2, 1-spost);
		qglVertex2f(glConfig.vidWidth, 0);
	qglEnd();
#endif

	if (tr_distortionAlpha == 1.0f && tr_distortionStretch == 0.0f)
	{ //no overrides
		if (tr_distortionNegate)
		{ //probably the crazy alternate saber trail
			alpha = 0.8f;
			GL_State(GLS_SRCBLEND_ZERO|GLS_DSTBLEND_ONE_MINUS_SRC_COLOR);
		}
		else
		{
			alpha = 0.5f;
			GL_State(GLS_SRCBLEND_SRC_ALPHA|GLS_DSTBLEND_SRC_ALPHA);
		}

		spost = sin(tr.refdef.time*0.0008f);
		if (spost < 0.0f)
		{
			spost = -spost;
		}
		spost *= 0.08f;

		spost2 = sin(tr.refdef.time*0.0008f);
		if (spost2 < 0.0f)
		{
			spost2 = -spost2;
		}
		spost2 *= 0.2f;

#ifdef _GLES //karin: glBegin/glEnd
	{
		GLboolean glva = qglIsEnabled(GL_VERTEX_ARRAY);
		GLboolean gltca = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
		GLboolean glca = qglIsEnabled(GL_VERTEX_ARRAY);

		if (!glva)
			qglEnableClientState( GL_VERTEX_ARRAY );
		if (!gltca)
			qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglDisableClientState( GL_COLOR_ARRAY );

		qglColor4f(1.0f, 1.0f, 1.0f, alpha);
		GLfloat vs[] = {
			0.0f+spost2, 1.0f-spost,
			0.0f, 0.0f,

			0.0f+spost2, 0.0f+spost,
			0.0f, (float)glConfig.vidHeight,

			1.0f-spost2, 0.0f+spost,
			(float)glConfig.vidWidth, (float)glConfig.vidHeight,

			1.0f-spost2, 1.0f-spost,
			(float)glConfig.vidWidth, 0.0f,
		};
		qglVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, vs + 2);
		qglTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, vs);
		qglDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (!glva)
			qglDisableClientState( GL_VERTEX_ARRAY );
		if (!gltca)
			qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
		if (glca)
			qglEnableClientState( GL_COLOR_ARRAY );
	}
#else
		qglBegin(GL_QUADS);
			qglColor4f(1.0f, 1.0f, 1.0f, alpha);
			qglTexCoord2f(0+spost2, 1-spost);
			qglVertex2f(0, 0);

			qglTexCoord2f(0+spost2, 0+spost);
			qglVertex2f(0, glConfig.vidHeight);

			qglTexCoord2f(1-spost2, 0+spost);
			qglVertex2f(glConfig.vidWidth, glConfig.vidHeight);

			qglTexCoord2f(1-spost2, 1-spost);
			qglVertex2f(glConfig.vidWidth, 0);
		qglEnd();
#endif
	}

	//pop the view matrices back
	qglMatrixMode(GL_PROJECTION);
	qglPopMatrix();
	qglMatrixMode(GL_MODELVIEW);
	qglPopMatrix();

	qglDisable( GL_STENCIL_TEST );
}
