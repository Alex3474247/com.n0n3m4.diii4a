/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../idlib/precompiled.h"
#pragma hdrstop

#include "tr_local.h"
#include "Model_local.h"
#include "Model_ase.h"
#include "Model_lwo.h"
#include "Model_ma.h"

idCVar idRenderModelStatic::r_mergeModelSurfaces("r_mergeModelSurfaces", "1", CVAR_BOOL|CVAR_RENDERER, "combine model surfaces with the same material");
idCVar idRenderModelStatic::r_slopVertex("r_slopVertex", "0.01", CVAR_RENDERER, "merge xyz coordinates this far apart");
idCVar idRenderModelStatic::r_slopTexCoord("r_slopTexCoord", "0.001", CVAR_RENDERER, "merge texture coordinates this far apart");
idCVar idRenderModelStatic::r_slopNormal("r_slopNormal", "0.02", CVAR_RENDERER, "merge normals that dot less than this");

/*
================
idRenderModelStatic::idRenderModelStatic
================
*/
idRenderModelStatic::idRenderModelStatic()
{
	name = "<undefined>";
	bounds.Clear();
	lastModifiedFrame = 0;
	lastArchivedFrame = 0;
	overlaysAdded = 0;
	shadowHull = NULL;
	isStaticWorldModel = false;
	defaulted = false;
	purged = false;
	fastLoad = false;
	reloadable = true;
	levelLoadReferenced = false;
	timeStamp = 0;
#ifdef _RAVEN //karin: portal sky
	hasSky = false;
#endif
}

/*
================
idRenderModelStatic::~idRenderModelStatic
================
*/
idRenderModelStatic::~idRenderModelStatic()
{
	PurgeModel();
}

/*
==============
idRenderModelStatic::Print
==============
*/
void idRenderModelStatic::Print() const
{
	common->Printf("%s\n", name.c_str());
	common->Printf("Static model.\n");
	common->Printf("bounds: (%f %f %f) to (%f %f %f)\n",
	               bounds[0][0], bounds[0][1], bounds[0][2],
	               bounds[1][0], bounds[1][1], bounds[1][2]);

	common->Printf("    verts  tris material\n");

	for (int i = 0 ; i < NumSurfaces() ; i++) {
		const modelSurface_t	*surf = Surface(i);

		srfTriangles_t *tri = surf->geometry;
		const idMaterial *material = surf->shader;

		if (!tri) {
			common->Printf("%2i: %s, NULL surface geometry\n", i, material->GetName());
			continue;
		}

		common->Printf("%2i: %5i %5i %s", i, tri->numVerts, tri->numIndexes / 3, material->GetName());

		if (tri->generateNormals) {
			common->Printf(" (smoothed)\n");
		} else {
			common->Printf("\n");
		}
	}
}

/*
==============
idRenderModelStatic::Memory
==============
*/
int idRenderModelStatic::Memory() const
{
	int	totalBytes = 0;

	totalBytes += sizeof(*this);
	totalBytes += name.DynamicMemoryUsed();
	totalBytes += surfaces.MemoryUsed();

	if (shadowHull) {
		totalBytes += R_TriSurfMemory(shadowHull);
	}

	for (int j = 0 ; j < NumSurfaces() ; j++) {
		const modelSurface_t	*surf = Surface(j);

		if (!surf->geometry) {
			continue;
		}

		totalBytes += R_TriSurfMemory(surf->geometry);
	}

	return totalBytes;
}

/*
==============
idRenderModelStatic::List
==============
*/
void idRenderModelStatic::List() const
{
	int	totalTris = 0;
	int	totalVerts = 0;
	int	totalBytes = 0;

	totalBytes = Memory();

	char	closed = 'C';

	for (int j = 0 ; j < NumSurfaces() ; j++) {
		const modelSurface_t	*surf = Surface(j);

		if (!surf->geometry) {
			continue;
		}

		if (!surf->geometry->perfectHull) {
			closed = ' ';
		}

		totalTris += surf->geometry->numIndexes / 3;
		totalVerts += surf->geometry->numVerts;
	}

	common->Printf("%c%4ik %3i %4i %4i %s", closed, totalBytes/1024, NumSurfaces(), totalVerts, totalTris, Name());

	if (IsDynamicModel() == DM_CACHED) {
		common->Printf(" (DM_CACHED)");
	}

	if (IsDynamicModel() == DM_CONTINUOUS) {
		common->Printf(" (DM_CONTINUOUS)");
	}

	if (defaulted) {
		common->Printf(" (DEFAULTED)");
	}

	if (bounds[0][0] >= bounds[1][0]) {
		common->Printf(" (EMPTY BOUNDS)");
	}

	if (bounds[1][0] - bounds[0][0] > 100000) {
		common->Printf(" (HUGE BOUNDS)");
	}

	common->Printf("\n");
}

/*
================
idRenderModelStatic::IsDefaultModel
================
*/
bool idRenderModelStatic::IsDefaultModel() const
{
	return defaulted;
}

/*
================
AddCubeFace
================
*/
static void AddCubeFace(srfTriangles_t *tri, idVec3 v1, idVec3 v2, idVec3 v3, idVec3 v4)
{
	tri->verts[tri->numVerts+0].Clear();
	tri->verts[tri->numVerts+0].xyz = v1 * 8;
	tri->verts[tri->numVerts+0].st[0] = 0;
	tri->verts[tri->numVerts+0].st[1] = 0;

	tri->verts[tri->numVerts+1].Clear();
	tri->verts[tri->numVerts+1].xyz = v2 * 8;
	tri->verts[tri->numVerts+1].st[0] = 1;
	tri->verts[tri->numVerts+1].st[1] = 0;

	tri->verts[tri->numVerts+2].Clear();
	tri->verts[tri->numVerts+2].xyz = v3 * 8;
	tri->verts[tri->numVerts+2].st[0] = 1;
	tri->verts[tri->numVerts+2].st[1] = 1;

	tri->verts[tri->numVerts+3].Clear();
	tri->verts[tri->numVerts+3].xyz = v4 * 8;
	tri->verts[tri->numVerts+3].st[0] = 0;
	tri->verts[tri->numVerts+3].st[1] = 1;

	tri->indexes[tri->numIndexes+0] = tri->numVerts + 0;
	tri->indexes[tri->numIndexes+1] = tri->numVerts + 1;
	tri->indexes[tri->numIndexes+2] = tri->numVerts + 2;
	tri->indexes[tri->numIndexes+3] = tri->numVerts + 0;
	tri->indexes[tri->numIndexes+4] = tri->numVerts + 2;
	tri->indexes[tri->numIndexes+5] = tri->numVerts + 3;

	tri->numVerts += 4;
	tri->numIndexes += 6;
}

/*
================
idRenderModelStatic::MakeDefaultModel
================
*/
void idRenderModelStatic::MakeDefaultModel()
{

	defaulted = true;

	// throw out any surfaces we already have
	PurgeModel();

	// create one new surface
	modelSurface_t	surf;

	srfTriangles_t *tri = R_AllocStaticTriSurf();

	surf.shader = tr.defaultMaterial;
	surf.geometry = tri;

	R_AllocStaticTriSurfVerts(tri, 24);
	R_AllocStaticTriSurfIndexes(tri, 36);

	AddCubeFace(tri, idVec3(-1, 1, 1), idVec3(1, 1, 1), idVec3(1, -1, 1), idVec3(-1, -1, 1));
	AddCubeFace(tri, idVec3(-1, 1, -1), idVec3(-1, -1, -1), idVec3(1, -1, -1), idVec3(1, 1, -1));

	AddCubeFace(tri, idVec3(1, -1, 1), idVec3(1, 1, 1), idVec3(1, 1, -1), idVec3(1, -1, -1));
	AddCubeFace(tri, idVec3(-1, -1, 1), idVec3(-1, -1, -1), idVec3(-1, 1, -1), idVec3(-1, 1, 1));

	AddCubeFace(tri, idVec3(-1, -1, 1), idVec3(1, -1, 1), idVec3(1, -1, -1), idVec3(-1, -1, -1));
	AddCubeFace(tri, idVec3(-1, 1, 1), idVec3(-1, 1, -1), idVec3(1, 1, -1), idVec3(1, 1, 1));

	tri->generateNormals = true;

	AddSurface(surf);
	FinishSurfaces();
}

/*
================
idRenderModelStatic::PartialInitFromFile
================
*/
void idRenderModelStatic::PartialInitFromFile(const char *fileName)
{
	fastLoad = true;
	InitFromFile(fileName);
}

/*
================
idRenderModelStatic::InitFromFile
================
*/
void idRenderModelStatic::InitFromFile(const char *fileName)
{
	bool loaded;
	idStr extension;

	InitEmpty(fileName);

	// FIXME: load new .proc map format

	name.ExtractFileExtension(extension);

	if (extension.Icmp("ase") == 0) {
		loaded		= LoadASE(name);
		reloadable	= true;
	} else if (extension.Icmp("lwo") == 0) {
		loaded		= LoadLWO(name);
		reloadable	= true;
	} else if (extension.Icmp("flt") == 0) {
		loaded		= LoadFLT(name);
		reloadable	= true;
	} else if (extension.Icmp("ma") == 0) {
		loaded		= LoadMA(name);
		reloadable	= true;
#ifdef _MODEL_OBJ
	} else if (extension.Icmp("obj") == 0) {
		loaded		= LoadOBJ(name);
		reloadable	= true;
#endif
#ifdef _MODEL_DAE
	} else if (extension.Icmp("dae") == 0) {
		loaded		= LoadDAE(name);
		reloadable	= true;
#endif
	} else {
		common->Warning("idRenderModelStatic::InitFromFile: unknown type for model: \'%s\'", name.c_str());
		loaded		= false;
	}

	if (!loaded) {
		common->Warning("Couldn't load model: '%s'", name.c_str());
		MakeDefaultModel();
		return;
	}

	// it is now available for use
	purged = false;

	// create the bounds for culling and dynamic surface creation
	FinishSurfaces();
}

/*
================
idRenderModelStatic::LoadModel
================
*/
void idRenderModelStatic::LoadModel()
{
	PurgeModel();
	InitFromFile(name);
}

/*
================
idRenderModelStatic::InitEmpty
================
*/
void idRenderModelStatic::InitEmpty(const char *fileName)
{
	// model names of the form _area* are static parts of the
	// world, and have already been considered for optimized shadows
	// other model names are inline entity models, and need to be
	// shadowed normally
	if (!idStr::Cmpn(fileName, "_area", 5)) {
		isStaticWorldModel = true;
	} else {
		isStaticWorldModel = false;
	}

	name = fileName;
	reloadable = false;	// if it didn't come from a file, we can't reload it
	PurgeModel();
	purged = false;
	bounds.Zero();
}

/*
================
idRenderModelStatic::AddSurface
================
*/
void idRenderModelStatic::AddSurface(modelSurface_t surface)
{
	surfaces.Append(surface);

	if (surface.geometry) {
		bounds += surface.geometry->bounds;
	}
}

/*
================
idRenderModelStatic::Name
================
*/
const char *idRenderModelStatic::Name() const
{
	return name;
}

/*
================
idRenderModelStatic::Timestamp
================
*/
ID_TIME_T idRenderModelStatic::Timestamp() const
{
	return timeStamp;
}

/*
================
idRenderModelStatic::NumSurfaces
================
*/
int idRenderModelStatic::NumSurfaces() const
{
	return surfaces.Num();
}

/*
================
idRenderModelStatic::NumBaseSurfaces
================
*/
int idRenderModelStatic::NumBaseSurfaces() const
{
	return surfaces.Num() - overlaysAdded;
}

/*
================
idRenderModelStatic::Surface
================
*/
const modelSurface_t *idRenderModelStatic::Surface(int surfaceNum) const
{
	return &surfaces[surfaceNum];
}

/*
================
idRenderModelStatic::AllocSurfaceTriangles
================
*/
srfTriangles_t *idRenderModelStatic::AllocSurfaceTriangles(int numVerts, int numIndexes) const
{
	srfTriangles_t *tri = R_AllocStaticTriSurf();
	R_AllocStaticTriSurfVerts(tri, numVerts);
	R_AllocStaticTriSurfIndexes(tri, numIndexes);
	return tri;
}

/*
================
idRenderModelStatic::FreeSurfaceTriangles
================
*/
void idRenderModelStatic::FreeSurfaceTriangles(srfTriangles_t *tris) const
{
	R_FreeStaticTriSurf(tris);
}

/*
================
idRenderModelStatic::ShadowHull
================
*/
srfTriangles_t *idRenderModelStatic::ShadowHull() const
{
	return shadowHull;
}

/*
================
idRenderModelStatic::IsStaticWorldModel
================
*/
bool idRenderModelStatic::IsStaticWorldModel() const
{
	return isStaticWorldModel;
}

/*
================
idRenderModelStatic::IsDynamicModel
================
*/
dynamicModel_t idRenderModelStatic::IsDynamicModel() const
{
	// dynamic subclasses will override this
	return DM_STATIC;
}

/*
================
idRenderModelStatic::IsReloadable
================
*/
bool idRenderModelStatic::IsReloadable() const
{
	return reloadable;
}

/*
================
idRenderModelStatic::Bounds
================
*/
idBounds idRenderModelStatic::Bounds(const struct renderEntity_s *mdef) const
{
	return bounds;
}

/*
================
idRenderModelStatic::DepthHack
================
*/
float idRenderModelStatic::DepthHack() const
{
	return 0.0f;
}

/*
================
idRenderModelStatic::InstantiateDynamicModel
================
*/
idRenderModel *idRenderModelStatic::InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel)
{
	if (cachedModel) {
		delete cachedModel;
		cachedModel = NULL;
	}

	common->Error("InstantiateDynamicModel called on static model '%s'", name.c_str());
	return NULL;
}

/*
================
idRenderModelStatic::NumJoints
================
*/
int idRenderModelStatic::NumJoints(void) const
{
	return 0;
}

/*
================
idRenderModelStatic::GetJoints
================
*/
const idMD5Joint *idRenderModelStatic::GetJoints(void) const
{
	return NULL;
}

/*
================
idRenderModelStatic::GetJointHandle
================
*/
jointHandle_t idRenderModelStatic::GetJointHandle(const char *name) const
{
	return INVALID_JOINT;
}

/*
================
idRenderModelStatic::GetJointName
================
*/
const char *idRenderModelStatic::GetJointName(jointHandle_t handle) const
{
	return "";
}

/*
================
idRenderModelStatic::GetDefaultPose
================
*/
const idJointQuat *idRenderModelStatic::GetDefaultPose(void) const
{
	return NULL;
}

/*
================
idRenderModelStatic::NearestJoint
================
*/
int idRenderModelStatic::NearestJoint(int surfaceNum, int a, int b, int c) const
{
	return INVALID_JOINT;
}


//=====================================================================


/*
================
idRenderModelStatic::FinishSurfaces

The mergeShadows option allows surfaces with different textures to share
silhouette edges for shadow calculation, instead of leaving shared edges
hanging.

If any of the original shaders have the noSelfShadow flag set, the surfaces
can't be merged, because they will need to be drawn in different order.

If there is only one surface, a separate merged surface won't be generated.

A model with multiple surfaces can't later have a skinned shader change the
state of the noSelfShadow flag.

-----------------

Creates mirrored copies of two sided surfaces with normal maps, which would
otherwise light funny.

Extends the bounds of deformed surfaces so they don't cull incorrectly at screen edges.

================
*/
void idRenderModelStatic::FinishSurfaces()
{
	int			i;
	int			totalVerts, totalIndexes;

	purged = false;

	// make sure we don't have a huge bounds even if we don't finish everything
	bounds.Zero();

	if (surfaces.Num() == 0) {
		return;
	}

	// renderBump doesn't care about most of this
	if (fastLoad) {
		bounds.Zero();

		for (i = 0 ; i < surfaces.Num() ; i++) {
			const modelSurface_t	*surf = &surfaces[i];

			R_BoundTriSurf(surf->geometry);
			bounds.AddBounds(surf->geometry->bounds);
		}

		return;
	}

	// cleanup all the final surfaces, but don't create sil edges
	totalVerts = 0;
	totalIndexes = 0;

	// decide if we are going to merge all the surfaces into one shadower
	int	numOriginalSurfaces = surfaces.Num();

	// make sure there aren't any NULL shaders or geometry
	for (i = 0 ; i < numOriginalSurfaces ; i++) {
		const modelSurface_t	*surf = &surfaces[i];

		if (surf->geometry == NULL || surf->shader == NULL) {
			MakeDefaultModel();
			common->Error("Model %s, surface %i had NULL geometry", name.c_str(), i);
		}

		if (surf->shader == NULL) {
			MakeDefaultModel();
			common->Error("Model %s, surface %i had NULL shader", name.c_str(), i);
		}
	}

	// duplicate and reverse triangles for two sided bump mapped surfaces
	// note that this won't catch surfaces that have their shaders dynamically
	// changed, and won't work with animated models.
	// It is better to create completely separate surfaces, rather than
	// add vertexes and indexes to the existing surface, because the
	// tangent generation wouldn't like the acute shared edges
	for (i = 0 ; i < numOriginalSurfaces ; i++) {
		const modelSurface_t	*surf = &surfaces[i];

		if (surf->shader->ShouldCreateBackSides()) {
			srfTriangles_t *newTri;

			newTri = R_CopyStaticTriSurf(surf->geometry);
			R_ReverseTriangles(newTri);

			modelSurface_t	newSurf;

			newSurf.shader = surf->shader;
			newSurf.geometry = newTri;

			AddSurface(newSurf);
		}
	}

	// clean the surfaces
	for (i = 0 ; i < surfaces.Num() ; i++) {
		const modelSurface_t	*surf = &surfaces[i];

		R_CleanupTriangles(surf->geometry, surf->geometry->generateNormals, true, surf->shader->UseUnsmoothedTangents());

		if (surf->shader->SurfaceCastsShadow()) {
			totalVerts += surf->geometry->numVerts;
			totalIndexes += surf->geometry->numIndexes;
		}
	}

	// add up the total surface area for development information
	for (i = 0 ; i < surfaces.Num() ; i++) {
		const modelSurface_t	*surf = &surfaces[i];
		srfTriangles_t	*tri = surf->geometry;

		for (int j = 0 ; j < tri->numIndexes ; j += 3) {
			float	area = idWinding::TriangleArea(tri->verts[tri->indexes[j]].xyz,
			                                       tri->verts[tri->indexes[j+1]].xyz,  tri->verts[tri->indexes[j+2]].xyz);
			const_cast<idMaterial *>(surf->shader)->AddToSurfaceArea(area);
		}
	}

	// calculate the bounds
	if (surfaces.Num() == 0) {
		bounds.Zero();
	} else {
		bounds.Clear();

		for (i = 0 ; i < surfaces.Num() ; i++) {
			modelSurface_t	*surf = &surfaces[i];

			// if the surface has a deformation, increase the bounds
			// the amount here is somewhat arbitrary, designed to handle
			// autosprites and flares, but could be done better with exact
			// deformation information.
			// Note that this doesn't handle deformations that are skinned in
			// at run time...
			if (surf->shader->Deform() != DFRM_NONE) {
				srfTriangles_t	*tri = surf->geometry;
				idVec3	mid = (tri->bounds[1] + tri->bounds[0]) * 0.5f;
				float	radius = (tri->bounds[0] - mid).Length();
				radius += 20.0f;

				tri->bounds[0][0] = mid[0] - radius;
				tri->bounds[0][1] = mid[1] - radius;
				tri->bounds[0][2] = mid[2] - radius;

				tri->bounds[1][0] = mid[0] + radius;
				tri->bounds[1][1] = mid[1] + radius;
				tri->bounds[1][2] = mid[2] + radius;
			}

			// add to the model bounds
			bounds.AddBounds(surf->geometry->bounds);

		}
	}
}

/*
=================
idRenderModelStatic::ConvertASEToModelSurfaces
=================
*/
typedef struct matchVert_s {
	struct matchVert_s	*next;
	int		v, tv;
	byte	color[4];
	idVec3	normal;
} matchVert_t;

bool idRenderModelStatic::ConvertASEToModelSurfaces(const struct aseModel_s *ase)
{
	aseObject_t 	*object;
	aseMesh_t 		*mesh;
	aseMaterial_t 	*material;
	const idMaterial *im1, *im2;
	srfTriangles_t *tri;
	int				objectNum;
	int				i, j, k;
	int				v, tv;
	int 			*vRemap;
	int 			*tvRemap;
	matchVert_t 	*mvTable;	// all of the match verts
	matchVert_t 	**mvHash;		// points inside mvTable for each xyz index
	matchVert_t 	*lastmv;
	matchVert_t 	*mv;
	idVec3			normal;
	float			uOffset, vOffset, textureSin, textureCos;
	float			uTiling, vTiling;
	int 			*mergeTo;
	byte 			*color;
	static byte	identityColor[4] = { 255, 255, 255, 255 };
	modelSurface_t	surf, *modelSurf;

	if (!ase) {
		return false;
	}

	if (ase->objects.Num() < 1) {
		return false;
	}

	timeStamp = ase->timeStamp;

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to mege them together where possible
	// meaning that this->NumSurfaces() <= ase->objects.currentElements
	mergeTo = (int *)_alloca(ase->objects.Num() * sizeof(*mergeTo));
	surf.geometry = NULL;

	if (ase->materials.Num() == 0) {
		// if we don't have any materials, dump everything into a single surface
		surf.shader = tr.defaultMaterial;
		surf.id = 0;
		this->AddSurface(surf);

		for (i = 0 ; i < ase->objects.Num() ; i++) {
			mergeTo[i] = 0;
		}
	} else if (!r_mergeModelSurfaces.GetBool()) {
		// don't merge any
		for (i = 0 ; i < ase->objects.Num() ; i++) {
			mergeTo[i] = i;
			object = ase->objects[i];
			material = ase->materials[object->materialRef];
			surf.shader = declManager->FindMaterial(material->name);
			surf.id = this->NumSurfaces();
			this->AddSurface(surf);
		}
	} else {
		// search for material matches
		for (i = 0 ; i < ase->objects.Num() ; i++) {
			object = ase->objects[i];
			material = ase->materials[object->materialRef];
			im1 = declManager->FindMaterial(material->name);

			if (im1->IsDiscrete()) {
				// flares, autosprites, etc
				j = this->NumSurfaces();
			} else {
				for (j = 0 ; j < this->NumSurfaces() ; j++) {
					modelSurf = &this->surfaces[j];
					im2 = modelSurf->shader;

					if (im1 == im2) {
						// merge this
						mergeTo[i] = j;
						break;
					}
				}
			}

			if (j == this->NumSurfaces()) {
				// didn't merge
				mergeTo[i] = j;
				surf.shader = im1;
				surf.id = this->NumSurfaces();
				this->AddSurface(surf);
			}
		}
	}

	idVectorSubset<idVec3, 3> vertexSubset;
	idVectorSubset<idVec2, 2> texCoordSubset;

	// build the surfaces
	for (objectNum = 0 ; objectNum < ase->objects.Num() ; objectNum++) {
		object = ase->objects[objectNum];
		mesh = &object->mesh;
#if 1 //k 2025
        material = (object->materialRef < ase->materials.Num()) ? ase->materials[object->materialRef] : NULL;
        im1 = declManager->FindMaterial( material ? material->name : NULL );
#else
		material = ase->materials[object->materialRef];
		im1 = declManager->FindMaterial(material->name);
#endif

		bool normalsParsed = mesh->normalsParsed;

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		const char *rb = im1->GetRenderBump();

		if (rb && rb[0]) {
			normalsParsed = false;
		}

		// It seems like the tools our artists are using often generate
		// verts and texcoords slightly separated that should be merged
		// note that we really should combine the surfaces with common materials
		// before doing this operation, because we can miss a slop combination
		// if they are in different surfaces

		vRemap = (int *)R_StaticAlloc(mesh->numVertexes * sizeof(vRemap[0]));

		if (fastLoad) {
			// renderbump doesn't care about vertex count
			for (j = 0; j < mesh->numVertexes; j++) {
				vRemap[j] = j;
			}
		} else {
			float vertexEpsilon = r_slopVertex.GetFloat();
			float expand = 2 * 32 * vertexEpsilon;
			idVec3 mins, maxs;

			SIMDProcessor->MinMax(mins, maxs, mesh->vertexes, mesh->numVertexes);
			mins -= idVec3(expand, expand, expand);
			maxs += idVec3(expand, expand, expand);
			vertexSubset.Init(mins, maxs, 32, 1024);

			for (j = 0; j < mesh->numVertexes; j++) {
				vRemap[j] = vertexSubset.FindVector(mesh->vertexes, j, vertexEpsilon);
			}
		}

		tvRemap = (int *)R_StaticAlloc(mesh->numTVertexes * sizeof(tvRemap[0]));

		if (fastLoad) {
			// renderbump doesn't care about vertex count
			for (j = 0; j < mesh->numTVertexes; j++) {
				tvRemap[j] = j;
			}
		} else {
			float texCoordEpsilon = r_slopTexCoord.GetFloat();
			float expand = 2 * 32 * texCoordEpsilon;
			idVec2 mins, maxs;

			SIMDProcessor->MinMax(mins, maxs, mesh->tvertexes, mesh->numTVertexes);
			mins -= idVec2(expand, expand);
			maxs += idVec2(expand, expand);
			texCoordSubset.Init(mins, maxs, 32, 1024);

			for (j = 0; j < mesh->numTVertexes; j++) {
				tvRemap[j] = texCoordSubset.FindVector(mesh->tvertexes, j, texCoordEpsilon);
			}
		}

		// we need to find out how many unique vertex / texcoord combinations
		// there are, because ASE tracks them separately but we need them unified

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = (matchVert_t *)R_ClearedStaticAlloc(mesh->numFaces * 3 * sizeof(mvTable[0]));

		// we will have a hash chain based on the xyz values
		mvHash = (matchVert_t **)R_ClearedStaticAlloc(mesh->numVertexes * sizeof(mvHash[0]));

		// allocate triangle surface
		tri = R_AllocStaticTriSurf();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes(tri, mesh->numFaces * 3);
		tri->generateNormals = !normalsParsed;

		// init default normal, color and tex coord index
		normal.Zero();
		color = identityColor;
		tv = 0;

		// find all the unique combinations
		float normalEpsilon = 1.0f - r_slopNormal.GetFloat();

		for (j = 0; j < mesh->numFaces; j++) {
			for (k = 0; k < 3; k++) {
				v = mesh->faces[j].vertexNum[k];

				if (v < 0 || v >= mesh->numVertexes) {
					common->Error("ConvertASEToModelSurfaces: bad vertex index in ASE file %s", name.c_str());
				}

				// collapse the position if it was slightly offset
				v = vRemap[v];

				// we may or may not have texcoords to compare
				if (mesh->numTVFaces == mesh->numFaces && mesh->numTVertexes != 0) {
					tv = mesh->faces[j].tVertexNum[k];

					if (tv < 0 || tv >= mesh->numTVertexes) {
						common->Error("ConvertASEToModelSurfaces: bad tex coord index in ASE file %s", name.c_str());
					}

					// collapse the tex coord if it was slightly offset
					tv = tvRemap[tv];
				}

				// we may or may not have normals to compare
				if (normalsParsed) {
					normal = mesh->faces[j].vertexNormals[k];
				}

				// we may or may not have colors to compare
				if (mesh->colorsParsed) {
					color = mesh->faces[j].vertexColors[k];
				}

				// find a matching vert
				for (lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next) {
					if (mv->tv != tv) {
						continue;
					}

					if (*(unsigned *)mv->color != *(unsigned *)color) {
						continue;
					}

					if (!normalsParsed) {
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}

					if (mv->normal * normal > normalEpsilon) {
						break;		// we already have this one
					}
				}

				if (!mv) {
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*(unsigned *)mv->color = *(unsigned *)color;
					mv->next = NULL;

					if (lastmv) {
						lastmv->next = mv;
					} else {
						mvHash[v] = mv;
					}

					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if (tri->numIndexes > mesh->numFaces * 3) {
			common->FatalError("ConvertASEToModelSurfaces: index miscount in ASE file %s", name.c_str());
		}

		if (tri->numVerts > mesh->numFaces * 3) {
			common->FatalError("ConvertASEToModelSurfaces: vertex miscount in ASE file %s", name.c_str());
		}

		// an ASE allows the texture coordinates to be scaled, translated, and rotated
		if (ase->materials.Num() == 0) {
			uOffset = vOffset = 0.0f;
			uTiling = vTiling = 1.0f;
			textureSin = 0.0f;
			textureCos = 1.0f;
		} else {
			material = ase->materials[object->materialRef];
			uOffset = -material->uOffset;
			vOffset = material->vOffset;
			uTiling = material->uTiling;
			vTiling = material->vTiling;
			textureSin = idMath::Sin(material->angle);
			textureCos = idMath::Cos(material->angle);
		}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts(tri, tri->numVerts);

		for (j = 0; j < tri->numVerts; j++) {
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = mesh->vertexes[ mv->v ];
			tri->verts[ j ].normal = mv->normal;
			*(unsigned *)tri->verts[j].color = *(unsigned *)mv->color;

			if (mesh->numTVFaces == mesh->numFaces && mesh->numTVertexes != 0) {
				const idVec2 &tv = mesh->tvertexes[ mv->tv ];
				float u = tv.x * uTiling + uOffset;
				float v = tv.y * vTiling + vOffset;
				tri->verts[ j ].st[0] = u * textureCos + v * textureSin;
				tri->verts[ j ].st[1] = u * -textureSin + v * textureCos;
			}
		}

		R_StaticFree(mvTable);
		R_StaticFree(mvHash);
		R_StaticFree(tvRemap);
		R_StaticFree(vRemap);

		// see if we need to merge with a previous surface of the same material
		modelSurf = &this->surfaces[mergeTo[ objectNum ]];
		srfTriangles_t	*mergeTri = modelSurf->geometry;

		if (!mergeTri) {
			modelSurf->geometry = tri;
		} else {
			modelSurf->geometry = R_MergeTriangles(mergeTri, tri);
			R_FreeStaticTriSurf(tri);
			R_FreeStaticTriSurf(mergeTri);
		}
	}

	return true;
}

/*
=================
idRenderModelStatic::ConvertLWOToModelSurfaces
=================
*/
bool idRenderModelStatic::ConvertLWOToModelSurfaces(const struct st_lwObject *lwo)
{
	const idMaterial *im1, *im2;
	srfTriangles_t	*tri;
	lwSurface 		*lwoSurf;
	int				numTVertexes;
	int				i, j, k;
	int				v, tv;
	idVec3 		*vList;
	int 			*vRemap;
	idVec2 		*tvList;
	int 			*tvRemap;
	matchVert_t 	*mvTable;	// all of the match verts
	matchVert_t 	**mvHash;		// points inside mvTable for each xyz index
	matchVert_t 	*lastmv;
	matchVert_t 	*mv;
	idVec3			normal;
	int 			*mergeTo;
	byte			color[4];
	modelSurface_t	surf, *modelSurf;

	if (!lwo) {
		return false;
	}

	if (lwo->surf == NULL) {
		return false;
	}

	timeStamp = lwo->timeStamp;

	// count the number of surfaces
	i = 0;

	for (lwoSurf = lwo->surf; lwoSurf; lwoSurf = lwoSurf->next) {
		i++;
	}

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to merge them together where possible
	mergeTo = (int *)_alloca(i * sizeof(mergeTo[0]));
	memset(&surf, 0, sizeof(surf));

	if (!r_mergeModelSurfaces.GetBool()) {
		// don't merge any
		for (lwoSurf = lwo->surf, i = 0; lwoSurf; lwoSurf = lwoSurf->next, i++) {
			mergeTo[i] = i;
			surf.shader = declManager->FindMaterial(lwoSurf->name);
			surf.id = this->NumSurfaces();
			this->AddSurface(surf);
		}
	} else {
		// search for material matches
		for (lwoSurf = lwo->surf, i = 0; lwoSurf; lwoSurf = lwoSurf->next, i++) {
			im1 = declManager->FindMaterial(lwoSurf->name);

			if (im1->IsDiscrete()) {
				// flares, autosprites, etc
				j = this->NumSurfaces();
			} else {
				for (j = 0 ; j < this->NumSurfaces() ; j++) {
					modelSurf = &this->surfaces[j];
					im2 = modelSurf->shader;

					if (im1 == im2) {
						// merge this
						mergeTo[i] = j;
						break;
					}
				}
			}

			if (j == this->NumSurfaces()) {
				// didn't merge
				mergeTo[i] = j;
				surf.shader = im1;
				surf.id = this->NumSurfaces();
				this->AddSurface(surf);
			}
		}
	}

	idVectorSubset<idVec3, 3> vertexSubset;
	idVectorSubset<idVec2, 2> texCoordSubset;

	// we only ever use the first layer
	lwLayer *layer = lwo->layer;

	// vertex positions
	if (layer->point.count <= 0) {
		common->Warning("ConvertLWOToModelSurfaces: model \'%s\' has bad or missing vertex data", name.c_str());
		return false;
	}

	vList = (idVec3 *)R_StaticAlloc(layer->point.count * sizeof(vList[0]));

	for (j = 0; j < layer->point.count; j++) {
		vList[j].x = layer->point.pt[j].pos[0];
		vList[j].y = layer->point.pt[j].pos[2];
		vList[j].z = layer->point.pt[j].pos[1];
	}

	// vertex texture coords
	numTVertexes = 0;

	if (layer->nvmaps) {
		for (lwVMap *vm = layer->vmap; vm; vm = vm->next) {
			if (vm->type == LWID_('T','X','U','V')) {
				numTVertexes += vm->nverts;
			}
		}
	}

	if (numTVertexes) {
		tvList = (idVec2 *)Mem_Alloc(numTVertexes * sizeof(tvList[0]));
		int offset = 0;

		for (lwVMap *vm = layer->vmap; vm; vm = vm->next) {
			if (vm->type == LWID_('T','X','U','V')) {
				vm->offset = offset;

				for (k = 0; k < vm->nverts; k++) {
					tvList[k + offset].x = vm->val[k][0];
					tvList[k + offset].y = 1.0f - vm->val[k][1];	// invert the t
				}

				offset += vm->nverts;
			}
		}
	} else {
		common->Warning("ConvertLWOToModelSurfaces: model \'%s\' has bad or missing uv data", name.c_str());
		numTVertexes = 1;
		tvList = (idVec2 *)Mem_ClearedAlloc(numTVertexes * sizeof(tvList[0]));
	}

	// It seems like the tools our artists are using often generate
	// verts and texcoords slightly separated that should be merged
	// note that we really should combine the surfaces with common materials
	// before doing this operation, because we can miss a slop combination
	// if they are in different surfaces

	vRemap = (int *)R_StaticAlloc(layer->point.count * sizeof(vRemap[0]));

	if (fastLoad) {
		// renderbump doesn't care about vertex count
		for (j = 0; j < layer->point.count; j++) {
			vRemap[j] = j;
		}
	} else {
		float vertexEpsilon = r_slopVertex.GetFloat();
		float expand = 2 * 32 * vertexEpsilon;
		idVec3 mins, maxs;

		SIMDProcessor->MinMax(mins, maxs, vList, layer->point.count);
		mins -= idVec3(expand, expand, expand);
		maxs += idVec3(expand, expand, expand);
		vertexSubset.Init(mins, maxs, 32, 1024);

		for (j = 0; j < layer->point.count; j++) {
			vRemap[j] = vertexSubset.FindVector(vList, j, vertexEpsilon);
		}
	}

	tvRemap = (int *)R_StaticAlloc(numTVertexes * sizeof(tvRemap[0]));

	if (fastLoad) {
		// renderbump doesn't care about vertex count
		for (j = 0; j < numTVertexes; j++) {
			tvRemap[j] = j;
		}
	} else {
		float texCoordEpsilon = r_slopTexCoord.GetFloat();
		float expand = 2 * 32 * texCoordEpsilon;
		idVec2 mins, maxs;

		SIMDProcessor->MinMax(mins, maxs, tvList, numTVertexes);
		mins -= idVec2(expand, expand);
		maxs += idVec2(expand, expand);
		texCoordSubset.Init(mins, maxs, 32, 1024);

		for (j = 0; j < numTVertexes; j++) {
			tvRemap[j] = texCoordSubset.FindVector(tvList, j, texCoordEpsilon);
		}
	}

	// build the surfaces
	for (lwoSurf = lwo->surf, i = 0; lwoSurf; lwoSurf = lwoSurf->next, i++) {
		im1 = declManager->FindMaterial(lwoSurf->name);

		bool normalsParsed = true;

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		const char *rb = im1->GetRenderBump();

		if (rb && rb[0]) {
			normalsParsed = false;
		}

		// we need to find out how many unique vertex / texcoord combinations there are

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = (matchVert_t *)R_ClearedStaticAlloc(layer->polygon.count * 3 * sizeof(mvTable[0]));

		// we will have a hash chain based on the xyz values
		mvHash = (matchVert_t **)R_ClearedStaticAlloc(layer->point.count * sizeof(mvHash[0]));

		// allocate triangle surface
		tri = R_AllocStaticTriSurf();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes(tri, layer->polygon.count * 3);
		tri->generateNormals = !normalsParsed;

		// find all the unique combinations
		float	normalEpsilon;

		if (fastLoad) {
			normalEpsilon = 1.0f;	// don't merge unless completely exact
		} else {
			normalEpsilon = 1.0f - r_slopNormal.GetFloat();
		}

		for (j = 0; j < layer->polygon.count; j++) {
			lwPolygon *poly = &layer->polygon.pol[j];

			if (poly->surf != lwoSurf) {
				continue;
			}

			if (poly->nverts != 3) {
				common->Warning("ConvertLWOToModelSurfaces: model %s has too many verts for a poly! Make sure you triplet it down", name.c_str());
				continue;
			}

			for (k = 0; k < 3; k++) {

				v = vRemap[poly->v[k].index];

				normal.x = poly->v[k].norm[0];
				normal.y = poly->v[k].norm[2];
				normal.z = poly->v[k].norm[1];

				// LWO models aren't all that pretty when it comes down to the floating point values they store
				normal.FixDegenerateNormal();

				tv = 0;

				color[0] = lwoSurf->color.rgb[0] * 255;
				color[1] = lwoSurf->color.rgb[1] * 255;
				color[2] = lwoSurf->color.rgb[2] * 255;
				color[3] = 255;

				// first set attributes from the vertex
				lwPoint	*pt = &layer->point.pt[poly->v[k].index];
				int nvm;

				for (nvm = 0; nvm < pt->nvmaps; nvm++) {
					lwVMapPt *vm = &pt->vm[nvm];

					if (vm->vmap->type == LWID_('T','X','U','V')) {
						tv = tvRemap[vm->index + vm->vmap->offset];
					}

					if (vm->vmap->type == LWID_('R','G','B','A')) {
						for (int chan = 0; chan < 4; chan++) {
							color[chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}

				// then override with polygon attributes
				for (nvm = 0; nvm < poly->v[k].nvmaps; nvm++) {
					lwVMapPt *vm = &poly->v[k].vm[nvm];

					if (vm->vmap->type == LWID_('T','X','U','V')) {
						tv = tvRemap[vm->index + vm->vmap->offset];
					}

					if (vm->vmap->type == LWID_('R','G','B','A')) {
						for (int chan = 0; chan < 4; chan++) {
							color[chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}

				// find a matching vert
				for (lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next) {
					if (mv->tv != tv) {
						continue;
					}

					if (*(unsigned *)mv->color != *(unsigned *)color) {
						continue;
					}

					if (!normalsParsed) {
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}

					if (mv->normal * normal > normalEpsilon) {
						break;		// we already have this one
					}
				}

				if (!mv) {
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*(unsigned *)mv->color = *(unsigned *)color;
					mv->next = NULL;

					if (lastmv) {
						lastmv->next = mv;
					} else {
						mvHash[v] = mv;
					}

					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if (tri->numIndexes > layer->polygon.count * 3) {
			common->FatalError("ConvertLWOToModelSurfaces: index miscount in LWO file %s", name.c_str());
		}

		if (tri->numVerts > layer->polygon.count * 3) {
			common->FatalError("ConvertLWOToModelSurfaces: vertex miscount in LWO file %s", name.c_str());
		}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts(tri, tri->numVerts);

		for (j = 0; j < tri->numVerts; j++) {
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = vList[ mv->v ];
			tri->verts[ j ].st = tvList[ mv->tv ];
			tri->verts[ j ].normal = mv->normal;
			*(unsigned *)tri->verts[j].color = *(unsigned *)mv->color;
		}

		R_StaticFree(mvTable);
		R_StaticFree(mvHash);

		// see if we need to merge with a previous surface of the same material
		modelSurf = &this->surfaces[mergeTo[ i ]];
		srfTriangles_t	*mergeTri = modelSurf->geometry;

		if (!mergeTri) {
			modelSurf->geometry = tri;
		} else {
			modelSurf->geometry = R_MergeTriangles(mergeTri, tri);
			R_FreeStaticTriSurf(tri);
			R_FreeStaticTriSurf(mergeTri);
		}
	}

	R_StaticFree(tvRemap);
	R_StaticFree(vRemap);
	R_StaticFree(tvList);
	R_StaticFree(vList);

	return true;
}

/*
=================
idRenderModelStatic::ConvertLWOToASE
=================
*/
struct aseModel_s *idRenderModelStatic::ConvertLWOToASE(const struct st_lwObject *obj, const char *fileName) {
	int j, k;
	aseModel_t *ase;

	if (!obj) {
		return NULL;
	}

	// NOTE: using new operator because aseModel_t contains idList class objects
	ase = new aseModel_t;
	ase->timeStamp = obj->timeStamp;
	ase->objects.Resize(obj->nlayers, obj->nlayers);

	int materialRef = 0;

	for (lwSurface *surf = obj->surf; surf; surf = surf->next) {

		aseMaterial_t *mat = (aseMaterial_t *)Mem_ClearedAlloc(sizeof(*mat));
		strcpy(mat->name, surf->name);
		mat->uTiling = mat->vTiling = 1;
		mat->angle = mat->uOffset = mat->vOffset = 0;
		ase->materials.Append(mat);

		lwLayer *layer = obj->layer;

		aseObject_t *object = (aseObject_t *)Mem_ClearedAlloc(sizeof(*object));
		object->materialRef = materialRef++;

		aseMesh_t *mesh = &object->mesh;
		ase->objects.Append(object);

		mesh->numFaces = layer->polygon.count;
		mesh->numTVFaces = mesh->numFaces;
		mesh->faces = (aseFace_t *)Mem_Alloc(mesh->numFaces  * sizeof(mesh->faces[0]));

		mesh->numVertexes = layer->point.count;
		mesh->vertexes = (idVec3 *)Mem_Alloc(mesh->numVertexes * sizeof(mesh->vertexes[0]));

		// vertex positions
		if (layer->point.count <= 0) {
			common->Warning("ConvertLWOToASE: model \'%s\' has bad or missing vertex data", name.c_str());
		}

		for (j = 0; j < layer->point.count; j++) {
			mesh->vertexes[j].x = layer->point.pt[j].pos[0];
			mesh->vertexes[j].y = layer->point.pt[j].pos[2];
			mesh->vertexes[j].z = layer->point.pt[j].pos[1];
		}

		// vertex texture coords
		mesh->numTVertexes = 0;

		if (layer->nvmaps) {
			for (lwVMap *vm = layer->vmap; vm; vm = vm->next) {
				if (vm->type == LWID_('T','X','U','V')) {
					mesh->numTVertexes += vm->nverts;
				}
			}
		}

		if (mesh->numTVertexes) {
			mesh->tvertexes = (idVec2 *)Mem_Alloc(mesh->numTVertexes * sizeof(mesh->tvertexes[0]));
			int offset = 0;

			for (lwVMap *vm = layer->vmap; vm; vm = vm->next) {
				if (vm->type == LWID_('T','X','U','V')) {
					vm->offset = offset;

					for (k = 0; k < vm->nverts; k++) {
						mesh->tvertexes[k + offset].x = vm->val[k][0];
						mesh->tvertexes[k + offset].y = 1.0f - vm->val[k][1];	// invert the t
					}

					offset += vm->nverts;
				}
			}
		} else {
			common->Warning("ConvertLWOToASE: model \'%s\' has bad or missing uv data", fileName);
			mesh->numTVertexes = 1;
			mesh->tvertexes = (idVec2 *)Mem_ClearedAlloc(mesh->numTVertexes * sizeof(mesh->tvertexes[0]));
		}

		mesh->normalsParsed = true;
		mesh->colorsParsed = true;	// because we are falling back to the surface color

		// triangles
		int faceIndex = 0;

		for (j = 0; j < layer->polygon.count; j++) {
			lwPolygon *poly = &layer->polygon.pol[j];

			if (poly->surf != surf) {
				continue;
			}

			if (poly->nverts != 3) {
				common->Warning("ConvertLWOToASE: model %s has too many verts for a poly! Make sure you triplet it down", fileName);
				continue;
			}

			mesh->faces[faceIndex].faceNormal.x = poly->norm[0];
			mesh->faces[faceIndex].faceNormal.y = poly->norm[2];
			mesh->faces[faceIndex].faceNormal.z = poly->norm[1];

			for (k = 0; k < 3; k++) {

				mesh->faces[faceIndex].vertexNum[k] = poly->v[k].index;

				mesh->faces[faceIndex].vertexNormals[k].x = poly->v[k].norm[0];
				mesh->faces[faceIndex].vertexNormals[k].y = poly->v[k].norm[2];
				mesh->faces[faceIndex].vertexNormals[k].z = poly->v[k].norm[1];

				// complete fallbacks
				mesh->faces[faceIndex].tVertexNum[k] = 0;

				mesh->faces[faceIndex].vertexColors[k][0] = surf->color.rgb[0] * 255;
				mesh->faces[faceIndex].vertexColors[k][1] = surf->color.rgb[1] * 255;
				mesh->faces[faceIndex].vertexColors[k][2] = surf->color.rgb[2] * 255;
				mesh->faces[faceIndex].vertexColors[k][3] = 255;

				// first set attributes from the vertex
				lwPoint	*pt = &layer->point.pt[poly->v[k].index];
				int nvm;

				for (nvm = 0; nvm < pt->nvmaps; nvm++) {
					lwVMapPt *vm = &pt->vm[nvm];

					if (vm->vmap->type == LWID_('T','X','U','V')) {
						mesh->faces[faceIndex].tVertexNum[k] = vm->index + vm->vmap->offset;
					}

					if (vm->vmap->type == LWID_('R','G','B','A')) {
						for (int chan = 0; chan < 4; chan++) {
							mesh->faces[faceIndex].vertexColors[k][chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}

				// then override with polygon attributes
				for (nvm = 0; nvm < poly->v[k].nvmaps; nvm++) {
					lwVMapPt *vm = &poly->v[k].vm[nvm];

					if (vm->vmap->type == LWID_('T','X','U','V')) {
						mesh->faces[faceIndex].tVertexNum[k] = vm->index + vm->vmap->offset;
					}

					if (vm->vmap->type == LWID_('R','G','B','A')) {
						for (int chan = 0; chan < 4; chan++) {
							mesh->faces[faceIndex].vertexColors[k][chan] = 255 * vm->vmap->val[vm->index][chan];
						}
					}
				}
			}

			faceIndex++;
		}

		mesh->numFaces = faceIndex;
		mesh->numTVFaces = faceIndex;

		aseFace_t *newFaces = (aseFace_t *)Mem_Alloc(mesh->numFaces * sizeof(mesh->faces[0]));
		memcpy(newFaces, mesh->faces, sizeof(mesh->faces[0]) * mesh->numFaces);
		Mem_Free(mesh->faces);
		mesh->faces = newFaces;
	}

	return ase;
}

/*
=================
idRenderModelStatic::ConvertMAToModelSurfaces
=================
*/
bool idRenderModelStatic::ConvertMAToModelSurfaces(const struct maModel_s *ma)
{

	maObject_t 	*object;
	maMesh_t 		*mesh;
	maMaterial_t 	*material;

	const idMaterial *im1, *im2;
	srfTriangles_t *tri;
	int				objectNum;
	int				i, j, k;
	int				v, tv;
	int 			*vRemap;
	int 			*tvRemap;
	matchVert_t 	*mvTable;	// all of the match verts
	matchVert_t 	**mvHash;		// points inside mvTable for each xyz index
	matchVert_t 	*lastmv;
	matchVert_t 	*mv;
	idVec3			normal;
	float			uOffset, vOffset, textureSin, textureCos;
	float			uTiling, vTiling;
	int 			*mergeTo;
	byte 			*color;
	static byte	identityColor[4] = { 255, 255, 255, 255 };
	modelSurface_t	surf, *modelSurf;

	if (!ma) {
		return false;
	}

	if (ma->objects.Num() < 1) {
		return false;
	}

	timeStamp = ma->timeStamp;

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to mege them together where possible
	// meaning that this->NumSurfaces() <= ma->objects.currentElements
	mergeTo = (int *)_alloca(ma->objects.Num() * sizeof(*mergeTo));

	surf.geometry = NULL;

	if (ma->materials.Num() == 0) {
		// if we don't have any materials, dump everything into a single surface
		surf.shader = tr.defaultMaterial;
		surf.id = 0;
		this->AddSurface(surf);

		for (i = 0 ; i < ma->objects.Num() ; i++) {
			mergeTo[i] = 0;
		}
	} else if (!r_mergeModelSurfaces.GetBool()) {
		// don't merge any
		for (i = 0 ; i < ma->objects.Num() ; i++) {
			mergeTo[i] = i;
			object = ma->objects[i];

			if (object->materialRef >= 0) {
				material = ma->materials[object->materialRef];
				surf.shader = declManager->FindMaterial(material->name);
			} else {
				surf.shader = tr.defaultMaterial;
			}

			surf.id = this->NumSurfaces();
			this->AddSurface(surf);
		}
	} else {
		// search for material matches
		for (i = 0 ; i < ma->objects.Num() ; i++) {
			object = ma->objects[i];

			if (object->materialRef >= 0) {
				material = ma->materials[object->materialRef];
				im1 = declManager->FindMaterial(material->name);
			} else {
				im1 = tr.defaultMaterial;
			}

			if (im1->IsDiscrete()) {
				// flares, autosprites, etc
				j = this->NumSurfaces();
			} else {
				for (j = 0 ; j < this->NumSurfaces() ; j++) {
					modelSurf = &this->surfaces[j];
					im2 = modelSurf->shader;

					if (im1 == im2) {
						// merge this
						mergeTo[i] = j;
						break;
					}
				}
			}

			if (j == this->NumSurfaces()) {
				// didn't merge
				mergeTo[i] = j;
				surf.shader = im1;
				surf.id = this->NumSurfaces();
				this->AddSurface(surf);
			}
		}
	}

	idVectorSubset<idVec3, 3> vertexSubset;
	idVectorSubset<idVec2, 2> texCoordSubset;

	// build the surfaces
	for (objectNum = 0 ; objectNum < ma->objects.Num() ; objectNum++) {
		object = ma->objects[objectNum];
		mesh = &object->mesh;

		if (object->materialRef >= 0) {
			material = ma->materials[object->materialRef];
			im1 = declManager->FindMaterial(material->name);
		} else {
			im1 = tr.defaultMaterial;
		}

		bool normalsParsed = mesh->normalsParsed;

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		const char *rb = im1->GetRenderBump();

		if (rb && rb[0]) {
			normalsParsed = false;
		}

		// It seems like the tools our artists are using often generate
		// verts and texcoords slightly separated that should be merged
		// note that we really should combine the surfaces with common materials
		// before doing this operation, because we can miss a slop combination
		// if they are in different surfaces

		vRemap = (int *)R_StaticAlloc(mesh->numVertexes * sizeof(vRemap[0]));

		if (fastLoad) {
			// renderbump doesn't care about vertex count
			for (j = 0; j < mesh->numVertexes; j++) {
				vRemap[j] = j;
			}
		} else {
			float vertexEpsilon = r_slopVertex.GetFloat();
			float expand = 2 * 32 * vertexEpsilon;
			idVec3 mins, maxs;

			SIMDProcessor->MinMax(mins, maxs, mesh->vertexes, mesh->numVertexes);
			mins -= idVec3(expand, expand, expand);
			maxs += idVec3(expand, expand, expand);
			vertexSubset.Init(mins, maxs, 32, 1024);

			for (j = 0; j < mesh->numVertexes; j++) {
				vRemap[j] = vertexSubset.FindVector(mesh->vertexes, j, vertexEpsilon);
			}
		}

		tvRemap = (int *)R_StaticAlloc(mesh->numTVertexes * sizeof(tvRemap[0]));

		if (fastLoad) {
			// renderbump doesn't care about vertex count
			for (j = 0; j < mesh->numTVertexes; j++) {
				tvRemap[j] = j;
			}
		} else {
			float texCoordEpsilon = r_slopTexCoord.GetFloat();
			float expand = 2 * 32 * texCoordEpsilon;
			idVec2 mins, maxs;

			SIMDProcessor->MinMax(mins, maxs, mesh->tvertexes, mesh->numTVertexes);
			mins -= idVec2(expand, expand);
			maxs += idVec2(expand, expand);
			texCoordSubset.Init(mins, maxs, 32, 1024);

			for (j = 0; j < mesh->numTVertexes; j++) {
				tvRemap[j] = texCoordSubset.FindVector(mesh->tvertexes, j, texCoordEpsilon);
			}
		}

		// we need to find out how many unique vertex / texcoord / color combinations
		// there are, because MA tracks them separately but we need them unified

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = (matchVert_t *)R_ClearedStaticAlloc(mesh->numFaces * 3 * sizeof(mvTable[0]));

		// we will have a hash chain based on the xyz values
		mvHash = (matchVert_t **)R_ClearedStaticAlloc(mesh->numVertexes * sizeof(mvHash[0]));

		// allocate triangle surface
		tri = R_AllocStaticTriSurf();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes(tri, mesh->numFaces * 3);
		tri->generateNormals = !normalsParsed;

		// init default normal, color and tex coord index
		normal.Zero();
		color = identityColor;
		tv = 0;

		// find all the unique combinations
		float normalEpsilon = 1.0f - r_slopNormal.GetFloat();

		for (j = 0; j < mesh->numFaces; j++) {
			for (k = 0; k < 3; k++) {
				v = mesh->faces[j].vertexNum[k];

				if (v < 0 || v >= mesh->numVertexes) {
					common->Error("ConvertMAToModelSurfaces: bad vertex index in MA file %s", name.c_str());
				}

				// collapse the position if it was slightly offset
				v = vRemap[v];

				// we may or may not have texcoords to compare
				if (mesh->numTVertexes != 0) {
					tv = mesh->faces[j].tVertexNum[k];

					if (tv < 0 || tv >= mesh->numTVertexes) {
						common->Error("ConvertMAToModelSurfaces: bad tex coord index in MA file %s", name.c_str());
					}

					// collapse the tex coord if it was slightly offset
					tv = tvRemap[tv];
				}

				// we may or may not have normals to compare
				if (normalsParsed) {
					normal = mesh->faces[j].vertexNormals[k];
				}

				//BSM: Todo: Fix the vertex colors
				// we may or may not have colors to compare
				if (mesh->faces[j].vertexColors[k] != -1 && mesh->faces[j].vertexColors[k] != -999) {

					color = &mesh->colors[mesh->faces[j].vertexColors[k]*4];
				}

				// find a matching vert
				for (lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next) {
					if (mv->tv != tv) {
						continue;
					}

					if (*(unsigned *)mv->color != *(unsigned *)color) {
						continue;
					}

					if (!normalsParsed) {
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}

					if (mv->normal * normal > normalEpsilon) {
						break;		// we already have this one
					}
				}

				if (!mv) {
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*(unsigned *)mv->color = *(unsigned *)color;
					mv->next = NULL;

					if (lastmv) {
						lastmv->next = mv;
					} else {
						mvHash[v] = mv;
					}

					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if (tri->numIndexes > mesh->numFaces * 3) {
			common->FatalError("ConvertMAToModelSurfaces: index miscount in MA file %s", name.c_str());
		}

		if (tri->numVerts > mesh->numFaces * 3) {
			common->FatalError("ConvertMAToModelSurfaces: vertex miscount in MA file %s", name.c_str());
		}

		// an MA allows the texture coordinates to be scaled, translated, and rotated
		//BSM: Todo: Does Maya support this and if so how
		//if ( ase->materials.Num() == 0 ) {
		uOffset = vOffset = 0.0f;
		uTiling = vTiling = 1.0f;
		textureSin = 0.0f;
		textureCos = 1.0f;
		//} else {
		//	material = ase->materials[object->materialRef];
		//	uOffset = -material->uOffset;
		//	vOffset = material->vOffset;
		//	uTiling = material->uTiling;
		//	vTiling = material->vTiling;
		//	textureSin = idMath::Sin( material->angle );
		//	textureCos = idMath::Cos( material->angle );
		//}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts(tri, tri->numVerts);

		for (j = 0; j < tri->numVerts; j++) {
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = mesh->vertexes[ mv->v ];
			tri->verts[ j ].normal = mv->normal;
			*(unsigned *)tri->verts[j].color = *(unsigned *)mv->color;

			if (mesh->numTVertexes != 0) {
				const idVec2 &tv = mesh->tvertexes[ mv->tv ];
				float u = tv.x * uTiling + uOffset;
				float v = tv.y * vTiling + vOffset;
				tri->verts[ j ].st[0] = u * textureCos + v * textureSin;
				tri->verts[ j ].st[1] = u * -textureSin + v * textureCos;
			}
		}

		R_StaticFree(mvTable);
		R_StaticFree(mvHash);
		R_StaticFree(tvRemap);
		R_StaticFree(vRemap);

		// see if we need to merge with a previous surface of the same material
		modelSurf = &this->surfaces[mergeTo[ objectNum ]];
		srfTriangles_t	*mergeTri = modelSurf->geometry;

		if (!mergeTri) {
			modelSurf->geometry = tri;
		} else {
			modelSurf->geometry = R_MergeTriangles(mergeTri, tri);
			R_FreeStaticTriSurf(tri);
			R_FreeStaticTriSurf(mergeTri);
		}
	}

	return true;
}

/*
=================
idRenderModelStatic::LoadASE
=================
*/
bool idRenderModelStatic::LoadASE(const char *fileName)
{
	aseModel_t *ase;

	ase = ASE_Load(fileName);

	if (ase == NULL) {
		return false;
	}

	ConvertASEToModelSurfaces(ase);

	ASE_Free(ase);

	return true;
}

/*
=================
idRenderModelStatic::LoadLWO
=================
*/
bool idRenderModelStatic::LoadLWO(const char *fileName)
{
	unsigned int failID;
	int failPos;
	lwObject *lwo;

	lwo = lwGetObject(fileName, &failID, &failPos);

	if (lwo == NULL) {
		return false;
	}

	ConvertLWOToModelSurfaces(lwo);

	lwFreeObject(lwo);

	return true;
}

/*
=================
idRenderModelStatic::LoadMA
=================
*/
bool idRenderModelStatic::LoadMA(const char *fileName)
{
	maModel_t *ma;

	ma = MA_Load(fileName);

	if (ma == NULL) {
		return false;
	}

	ConvertMAToModelSurfaces(ma);

	MA_Free(ma);

	return true;
}

/*
=================
idRenderModelStatic::LoadFLT

USGS height map data for megaTexture experiments
=================
*/
bool idRenderModelStatic::LoadFLT(const char *fileName)
{
	float	*data;
	int		len;

	len = fileSystem->ReadFile(fileName, (void **)&data);

	if (len <= 0) {
		return false;
	}

	int	size = sqrt(len / 4.0f);

	// bound the altitudes
	float min = 9999999;
	float max = -9999999;

	for (int i = 0 ; i < len/4 ; i++) {
		data[i] = BigFloat(data[i]);

		if (data[i] == -9999) {
			data[i] = 0;		// unscanned areas
		}

		if (data[i] < min) {
			min = data[i];
		}

		if (data[i] > max) {
			max = data[i];
		}
	}

#if 1
	// write out a gray scale height map
	byte	*image = (byte *)R_StaticAlloc(len);
	byte	*image_p = image;

	for (int i = 0 ; i < len/4 ; i++) {
		float v = (data[i] - min) / (max - min);
		image_p[0] =
		        image_p[1] =
		                image_p[2] = v * 255;
		image_p[3] = 255;
		image_p += 4;
	}

	idStr	tgaName = fileName;
	tgaName.StripFileExtension();
	tgaName += ".tga";
	R_WriteTGA(tgaName.c_str(), image, size, size, false);
	R_StaticFree(image);
//return false;
#endif

	// find the island above sea level
	int	minX, maxX, minY, maxY;
	{
		int	i;

		for (minX = 0 ; minX < size ; minX++) {
			for (i = 0 ; i < size ; i++) {
				if (data[i*size + minX] > 1.0) {
					break;
				}
			}

			if (i != size) {
				break;
			}
		}

		for (maxX = size-1 ; maxX > 0 ; maxX--) {
			for (i = 0 ; i < size ; i++) {
				if (data[i*size + maxX] > 1.0) {
					break;
				}
			}

			if (i != size) {
				break;
			}
		}

		for (minY = 0 ; minY < size ; minY++) {
			for (i = 0 ; i < size ; i++) {
				if (data[minY*size + i] > 1.0) {
					break;
				}
			}

			if (i != size) {
				break;
			}
		}

		for (maxY = size-1 ; maxY < size ; maxY--) {
			for (i = 0 ; i < size ; i++) {
				if (data[maxY*size + i] > 1.0) {
					break;
				}
			}

			if (i != size) {
				break;
			}
		}
	}

	int	width = maxX - minX + 1;
	int height = maxY - minY + 1;

//width /= 2;
	// allocate triangle surface
	srfTriangles_t *tri = R_AllocStaticTriSurf();
	tri->numVerts = width * height;
	tri->numIndexes = (width-1) * (height-1) * 6;

	fastLoad = true;		// don't do all the sil processing

	R_AllocStaticTriSurfIndexes(tri, tri->numIndexes);
	R_AllocStaticTriSurfVerts(tri, tri->numVerts);

	for (int i = 0 ; i < height ; i++) {
		for (int j = 0; j < width ; j++) {
			int		v = i * width + j;
			tri->verts[ v ].Clear();
			tri->verts[ v ].xyz[0] = j * 10;	// each sample is 10 meters
			tri->verts[ v ].xyz[1] = -i * 10;
			tri->verts[ v ].xyz[2] = data[(minY+i)*size+minX+j];	// height is in meters
			tri->verts[ v ].st[0] = (float) j / (width-1);
			tri->verts[ v ].st[1] = 1.0 - ((float) i / (height-1));
		}
	}

	for (int i = 0 ; i < height-1 ; i++) {
		for (int j = 0; j < width-1 ; j++) {
			int	v = (i * (width-1) + j) * 6;
#if 0
			tri->indexes[ v + 0 ] = i * width + j;
			tri->indexes[ v + 1 ] = (i+1) * width + j;
			tri->indexes[ v + 2 ] = (i+1) * width + j + 1;
			tri->indexes[ v + 3 ] = i * width + j;
			tri->indexes[ v + 4 ] = (i+1) * width + j + 1;
			tri->indexes[ v + 5 ] = i * width + j + 1;
#else
			tri->indexes[ v + 0 ] = i * width + j;
			tri->indexes[ v + 1 ] = i * width + j + 1;
			tri->indexes[ v + 2 ] = (i+1) * width + j + 1;
			tri->indexes[ v + 3 ] = i * width + j;
			tri->indexes[ v + 4 ] = (i+1) * width + j + 1;
			tri->indexes[ v + 5 ] = (i+1) * width + j;
#endif
		}
	}

	fileSystem->FreeFile(data);

	modelSurface_t	surface;

	surface.geometry = tri;
	surface.id = 0;
	surface.shader = tr.defaultMaterial; // declManager->FindMaterial( "shaderDemos/megaTexture" );

	this->AddSurface(surface);

	return true;
}


//=============================================================================

/*
================
idRenderModelStatic::PurgeModel
================
*/
void idRenderModelStatic::PurgeModel()
{
	int		i;
	modelSurface_t	*surf;

	for (i = 0 ; i < surfaces.Num() ; i++) {
		surf = &surfaces[i];

		if (surf->geometry) {
			R_FreeStaticTriSurf(surf->geometry);
		}
	}

	surfaces.Clear();

	purged = true;
}

/*
==============
idRenderModelStatic::FreeVertexCache

We are about to restart the vertex cache, so dump everything
==============
*/
void idRenderModelStatic::FreeVertexCache(void)
{
	for (int j = 0 ; j < surfaces.Num() ; j++) {
		srfTriangles_t *tri = surfaces[j].geometry;

		if (!tri) {
			continue;
		}

		if (tri->ambientCache) {
			vertexCache.Free(tri->ambientCache);
			tri->ambientCache = NULL;
		}

		// static shadows may be present
		if (tri->shadowCache) {
			vertexCache.Free(tri->shadowCache);
			tri->shadowCache = NULL;
		}
	}
}

/*
================
idRenderModelStatic::ReadFromDemoFile
================
*/
void idRenderModelStatic::ReadFromDemoFile(class idDemoFile *f)
{
	PurgeModel();

	InitEmpty(f->ReadHashString());

	int i, j, numSurfaces;
	f->ReadInt(numSurfaces);

	for (i = 0 ; i < numSurfaces ; i++) {
		modelSurface_t	surf;

		surf.shader = declManager->FindMaterial(f->ReadHashString());

		srfTriangles_t	*tri = R_AllocStaticTriSurf();

		f->ReadInt(tri->numIndexes);
		R_AllocStaticTriSurfIndexes(tri, tri->numIndexes);

		for (j = 0; j < tri->numIndexes; ++j)
			f->ReadInt((int &)tri->indexes[j]);

		f->ReadInt(tri->numVerts);
		R_AllocStaticTriSurfVerts(tri, tri->numVerts);

		for (j = 0; j < tri->numVerts; ++j) {
			f->ReadVec3(tri->verts[j].xyz);
			f->ReadVec2(tri->verts[j].st);
			f->ReadVec3(tri->verts[j].normal);
			f->ReadVec3(tri->verts[j].tangents[0]);
			f->ReadVec3(tri->verts[j].tangents[1]);
			f->ReadUnsignedChar(tri->verts[j].color[0]);
			f->ReadUnsignedChar(tri->verts[j].color[1]);
			f->ReadUnsignedChar(tri->verts[j].color[2]);
			f->ReadUnsignedChar(tri->verts[j].color[3]);
		}

		surf.geometry = tri;

		this->AddSurface(surf);
	}

	this->FinishSurfaces();
}

/*
================
idRenderModelStatic::WriteToDemoFile
================
*/
void idRenderModelStatic::WriteToDemoFile(class idDemoFile *f)
{
	int	data[1];

	// note that it has been updated
	lastArchivedFrame = tr.frameCount;

	data[0] = DC_DEFINE_MODEL;
	f->WriteInt(data[0]);
	f->WriteHashString(this->Name());

	int i, j, iData = surfaces.Num();
	f->WriteInt(iData);

	for (i = 0 ; i < surfaces.Num() ; i++) {
		const modelSurface_t	*surf = &surfaces[i];

		f->WriteHashString(surf->shader->GetName());

		srfTriangles_t *tri = surf->geometry;
		f->WriteInt(tri->numIndexes);

		for (j = 0; j < tri->numIndexes; ++j)
			f->WriteInt((int &)tri->indexes[j]);

		f->WriteInt(tri->numVerts);

		for (j = 0; j < tri->numVerts; ++j) {
			f->WriteVec3(tri->verts[j].xyz);
			f->WriteVec2(tri->verts[j].st);
			f->WriteVec3(tri->verts[j].normal);
			f->WriteVec3(tri->verts[j].tangents[0]);
			f->WriteVec3(tri->verts[j].tangents[1]);
			f->WriteUnsignedChar(tri->verts[j].color[0]);
			f->WriteUnsignedChar(tri->verts[j].color[1]);
			f->WriteUnsignedChar(tri->verts[j].color[2]);
			f->WriteUnsignedChar(tri->verts[j].color[3]);
		}
	}
}

/*
================
idRenderModelStatic::IsLoaded
================
*/
bool idRenderModelStatic::IsLoaded(void) const
{
	return !purged;
}

/*
================
idRenderModelStatic::SetLevelLoadReferenced
================
*/
void idRenderModelStatic::SetLevelLoadReferenced(bool referenced)
{
	levelLoadReferenced = referenced;
}

/*
================
idRenderModelStatic::IsLevelLoadReferenced
================
*/
bool idRenderModelStatic::IsLevelLoadReferenced(void)
{
	return levelLoadReferenced;
}

/*
=================
idRenderModelStatic::TouchData
=================
*/
void idRenderModelStatic::TouchData(void)
{
	for (int i = 0 ; i < surfaces.Num() ; i++) {
		const modelSurface_t	*surf = &surfaces[i];

		// re-find the material to make sure it gets added to the
		// level keep list
		declManager->FindMaterial(surf->shader->GetName());
	}
}

/*
=================
idRenderModelStatic::DeleteSurfaceWithId
=================
*/
bool idRenderModelStatic::DeleteSurfaceWithId(int id)
{
	int i;

	for (i = 0; i < surfaces.Num(); i++) {
		if (surfaces[i].id == id) {
			R_FreeStaticTriSurf(surfaces[i].geometry);
			surfaces.RemoveIndex(i);
			return true;
		}
	}

	return false;
}

/*
=================
idRenderModelStatic::DeleteSurfacesWithNegativeId
=================
*/
void idRenderModelStatic::DeleteSurfacesWithNegativeId(void)
{
	int i;

	for (i = 0; i < surfaces.Num(); i++) {
		if (surfaces[i].id < 0) {
			R_FreeStaticTriSurf(surfaces[i].geometry);
			surfaces.RemoveIndex(i);
			i--;
		}
	}
}

/*
=================
idRenderModelStatic::FindSurfaceWithId
=================
*/
bool idRenderModelStatic::FindSurfaceWithId(int id, int &surfaceNum)
{
	int i;

	for (i = 0; i < surfaces.Num(); i++) {
		if (surfaces[i].id == id) {
			surfaceNum = i;
			return true;
		}
	}

	return false;
}

#ifdef _RAVEN //k: for ShowSurface/HideSurface, static model using surfaces index as mask: 1 << index, name is shader material name
int idRenderModelStatic::GetSurfaceMask(const char *name) const
{
	int i;
	const modelSurface_t *surf;
	if(!name || !name[0])
		return 0;
	for(i = 0; i < surfaces.Num(); i++)
	{
		if(i > 31) //k: greate than int bits, it should be happened, but if happen, return 0 for do nothing
			break;
		surf = &surfaces[i];
		if(!surf || !surf->shader)
			continue;
		if(!idStr::Icmp(name, surf->shader->GetName()))
			return SUPPRESS_SURFACE_MASK(i);
	}
	return 0;
}
#endif

#ifdef _MODEL_OBJ
#include "model/Model_obj.h"
#include "model/Model_obj.cpp"


/*
=================
idRenderModelStatic::LoadOBJ
=================
*/
bool idRenderModelStatic::LoadOBJ( const char* fileName)
{
	objModel_t* obj;

	obj = OBJ_Load( fileName );
	if( obj == NULL )
	{
		return false;
	}

	ConvertOBJToModelSurfaces( obj );

	OBJ_Free( obj );

	return true;
}

/*
=================
idRenderModelStatic::ConvertOBJToModelSurfaces
=================
*/
bool idRenderModelStatic::ConvertOBJToModelSurfaces( const objModel_t* model )
{
	objObject_t* 	mesh;
	const idMaterial* im1, *im2;
	srfTriangles_t* tri;
	int				objectNum;
	int				i, j;
	int				v, tv;
	int* 			vRemap;
	int* 			tvRemap;
	matchVert_t* 	mvTable;	// all of the match verts
	matchVert_t** 	mvHash;		// points inside mvTable for each xyz index
	matchVert_t* 	lastmv;
	matchVert_t* 	mv;
	idVec3			normal;
	int* 			mergeTo;
	byte* 			color;
	static byte	identityColor[4] = { 255, 255, 255, 255 };
	modelSurface_t	surf, *modelSurf;

	if( !model )
	{
		return false;
	}
	if( model->objects.Num() < 1 )
	{
		return false;
	}

	timeStamp = model->timeStamp;

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to mege them together where possible
	// meaning that this->NumSurfaces() <= ase->objects.currentElements
	mergeTo = ( int* )_alloca( model->objects.Num() * sizeof( *mergeTo ) );
	surf.geometry = NULL;
	/*
	if( model->materials.Num() == 0 )
	{
		// if we don't have any materials, dump everything into a single surface
		surf.shader = tr.defaultMaterial;
		surf.id = 0;
		this->AddSurface( surf );
		for( i = 0; i < ase->objects.Num(); i++ )
		{
			mergeTo[i] = 0;
		}
	}
	else
	*/
	if( !r_mergeModelSurfaces.GetBool() )
	{
		// don't merge any
		for( i = 0; i < model->objects.Num(); i++ )
		{
			mergeTo[i] = i;
			mesh = model->objects[i];

			surf.shader = declManager->FindMaterial( mesh->material );
			surf.id = this->NumSurfaces();
			this->AddSurface( surf );
		}
	}
	else
	{
		// search for material matches
		for( i = 0; i < model->objects.Num(); i++ )
		{
			mesh = model->objects[i];

			im1 = declManager->FindMaterial( mesh->material );
			if( im1->IsDiscrete() )
			{
				// flares, autosprites, etc
				j = this->NumSurfaces();
			}
			else
			{
				for( j = 0; j < this->NumSurfaces(); j++ )
				{
					modelSurf = &this->surfaces[j];
					im2 = modelSurf->shader;
					if( im1 == im2 )
					{
						// merge this
						mergeTo[i] = j;
						break;
					}
				}
			}
			if( j == this->NumSurfaces() )
			{
				// didn't merge
				mergeTo[i] = j;
				surf.shader = im1;
				surf.id = this->NumSurfaces();
				this->AddSurface( surf );
			}
		}
	}

	idVectorSubset<idVec3, 3> vertexSubset;
	idVectorSubset<idVec2, 2> texCoordSubset;

	// build the surfaces
	for( objectNum = 0; objectNum < model->objects.Num(); objectNum++ )
	{
		mesh = model->objects[objectNum];

		im1 = declManager->FindMaterial( mesh->material );

		bool normalsParsed = mesh->normals.Num();

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		const char* rb = im1->GetRenderBump();
		if( rb != NULL && rb[0] != '\0' )
		{
			normalsParsed = false;
		}

		// It seems like the tools our artists are using often generate
		// verts and texcoords slightly separated that should be merged
		// note that we really should combine the surfaces with common materials
		// before doing this operation, because we can miss a slop combination
		// if they are in different surfaces

		vRemap = ( int* )R_StaticAlloc( mesh->vertexes.Num() * sizeof( vRemap[0] ) );

		if( fastLoad )
		{
			// renderbump doesn't care about vertex count
			for( j = 0; j < mesh->vertexes.Num(); j++ )
			{
				vRemap[j] = j;
			}
		}
		else
		{
			float vertexEpsilon = r_slopVertex.GetFloat();
			float expand = 2 * 32 * vertexEpsilon;
			idVec3 mins, maxs;

			SIMDProcessor->MinMax( mins, maxs, &mesh->vertexes[0], mesh->vertexes.Num() );

			mins -= idVec3( expand, expand, expand );
			maxs += idVec3( expand, expand, expand );

			vertexSubset.Init( mins, maxs, 32, 1024 );
			for( j = 0; j < mesh->vertexes.Num(); j++ )
			{
				vRemap[j] = vertexSubset.FindVector( &mesh->vertexes[0], j, vertexEpsilon );
			}
		}

		tvRemap = ( int* )R_StaticAlloc( mesh->texcoords.Num() * sizeof( tvRemap[0] ) );

		if( fastLoad )
		{
			// renderbump doesn't care about vertex count
			for( j = 0; j < mesh->texcoords.Num(); j++ )
			{
				tvRemap[j] = j;
			}
		}
		else
		{
			float texCoordEpsilon = r_slopTexCoord.GetFloat();
			float expand = 2 * 32 * texCoordEpsilon;
			idVec2 mins, maxs;

			SIMDProcessor->MinMax( mins, maxs, &mesh->texcoords[0], mesh->texcoords.Num() );

			mins -= idVec2( expand, expand );
			maxs += idVec2( expand, expand );

			texCoordSubset.Init( mins, maxs, 32, 1024 );

			for( j = 0; j < mesh->texcoords.Num(); j++ )
			{
				tvRemap[j] = texCoordSubset.FindVector( &mesh->texcoords[0], j, texCoordEpsilon );
			}
		}

		// we need to find out how many unique vertex / texcoord combinations
		// there are, because ASE tracks them separately but we need them unified

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = ( matchVert_t* )R_ClearedStaticAlloc( mesh->indexes.Num() * sizeof( mvTable[0] ) );

		// we will have a hash chain based on the xyz values
		mvHash = ( matchVert_t** )R_ClearedStaticAlloc( mesh->vertexes.Num() * sizeof( mvHash[0] ) );

		// allocate triangle surface
		tri = R_AllocStaticTriSurf();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes( tri, mesh->indexes.Num() );
		tri->generateNormals = !normalsParsed;

		// init default normal, color and tex coord index
		normal.Zero();
		color = identityColor;
		tv = 0;

		// find all the unique combinations
		float normalEpsilon = 1.0f - r_slopNormal.GetFloat();

		for( j = 0; j < ( mesh->indexes.Num() / 3 ); j++ )
		{
			// construct triangles in reverse order
			for( int k = 2; k >= 0; k-- )
			{
				int index = j * 3 + k;

				if( index < 0 || index >= mesh->vertexes.Num() )
				{
					common->Error( "ConvertOBJToModelSurfaces: bad vertex index in ASE file %s", name.c_str() );
				}

				// collapse the position if it was slightly offset
				v = vRemap[index];

				// we may or may not have texcoords to compare
				if( mesh->texcoords.Num() != 0 )
				{
					tv = index;
					//tv = mesh->faces[j].tVertexNum[k];

					if( tv < 0 || tv >= mesh->texcoords.Num() )
					{
						common->Error( "ConvertOBJToModelSurfaces: bad tex coord index in ASE file %s", name.c_str() );
					}
					// collapse the tex coord if it was slightly offset
					tv = tvRemap[tv];
				}

				// we may or may not have normals to compare
				if( normalsParsed )
				{
					normal = mesh->normals[ index ];
				}

				// we may or may not have colors to compare
				/*
				if( mesh->colorsParsed )
				{
					color = mesh->faces[j].vertexColors[k];
				}
				*/

				// find a matching vert
				for( lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next )
				{
					if( mv->tv != tv )
					{
						continue;
					}
					if( *( unsigned* )mv->color != *( unsigned* )color )
					{
						continue;
					}
					if( !normalsParsed )
					{
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}
					if( mv->normal * normal > normalEpsilon )
					{
						break;		// we already have this one
					}
				}
				if( !mv )
				{
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*( unsigned* )mv->color = *( unsigned* )color;
					mv->next = NULL;
					if( lastmv )
					{
						lastmv->next = mv;
					}
					else
					{
						mvHash[v] = mv;
					}
					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if( tri->numIndexes > mesh->indexes.Num() )
		{
			common->FatalError( "ConvertOBJToModelSurfaces: index miscount in OBJ file %s", name.c_str() );
		}
		if( tri->numVerts > mesh->indexes.Num() )
		{
			common->FatalError( "ConvertOBJToModelSurfaces: vertex miscount in OBJ file %s", name.c_str() );
		}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts( tri, tri->numVerts );

		for( j = 0; j < tri->numVerts; j++ )
		{
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = mesh->vertexes[ mv->v ];
			tri->verts[ j ].normal = ( mv->normal );

			*( unsigned* )tri->verts[j].color = *( unsigned* )mv->color;

			if( mesh->texcoords.Num() != 0 )
			{
				const idVec2& tv = mesh->texcoords[ mv->tv ];
				float u = tv.x;
				float v = tv.y;
				tri->verts[j].st = idVec2( u, v );
			}
		}

		R_StaticFree( mvTable );
		R_StaticFree( mvHash );
		R_StaticFree( tvRemap );
		R_StaticFree( vRemap );

		// see if we need to merge with a previous surface of the same material
		modelSurf = &this->surfaces[mergeTo[ objectNum ]];
		srfTriangles_t*	mergeTri = modelSurf->geometry;
		if( !mergeTri )
		{
			modelSurf->geometry = tri;
		}
		else
		{
			modelSurf->geometry = R_MergeTriangles( mergeTri, tri );
			R_FreeStaticTriSurf( tri );
			R_FreeStaticTriSurf( mergeTri );
		}
	}

	return true;
}
#endif

#ifdef _MODEL_DAE
#include "model/Model_dae.h"
#include "model/Model_dae.cpp"

// RB: added COLLADA support
bool idRenderModelStatic::LoadDAE( const char* fileName )
{
	bool loaded = false;

#if defined(USE_EXCEPTIONS)
	try
#endif
	{
		idTimer timer;
		timer.Start();

		ID_TIME_T sourceTimeStamp;
		ColladaParser parser( fileName, &sourceTimeStamp );

		loaded = ConvertDAEToModelSurfaces( &parser );

		timer.Stop();

		common->Printf( "...loaded '%s' in %5.2f seconds\n", fileName, timer.Milliseconds() / 1000.0 );
	}
#if defined(USE_EXCEPTIONS)
	catch( idException& e )
	{
		common->Warning( "%s", e.GetError() );
	}
#endif

	return loaded;
}

bool idRenderModelStatic::ConvertDAEToModelSurfaces( const ColladaParser* dae )
{
	Collada::Node**		object;
	Collada::Mesh**		mesh;
	Collada::MeshInstance* meshInstance;
//	Collada::Material**	material;
	const idMaterial*	im1, *im2;
	srfTriangles_t*		tri;
	int					objectNum;
	int					i, j, k;
	int					index, v, tv;
	int* 				vRemap;
	int* 				tvRemap;
	matchVert_t* 		mvTable;	// all of the match verts
	matchVert_t** 		mvHash;		// points inside mvTable for each xyz index
	matchVert_t* 		lastmv;
	matchVert_t* 		mv;
	idVec3				normal;
	int* 				mergeTo;
	byte* 				color;
	static byte	identityColor[4] = { 255, 255, 255, 255 };
	modelSurface_t		surf, *modelSurf;


	if( dae->mNodeLibrary.Num() < 1 )
	{
		return false;
	}

	timeStamp = dae->mReader->getTimestamp();

	// the modeling programs can save out multiple surfaces with a common
	// material, but we would like to mege them together where possible
	// meaning that this->NumSurfaces() <= ase->objects.currentElements
	mergeTo = ( int* )_alloca( dae->mNodeLibrary.Num() * sizeof( *mergeTo ) );
	surf.geometry = NULL;

	if( dae->mMaterialLibrary.Num() == 0 )
	{
		// if we don't have any materials, dump everything into a single surface
		surf.shader = tr.defaultMaterial;
		surf.id = 0;
		this->AddSurface( surf );
		for( i = 0 ; i < dae->mNodeLibrary.Num() ; i++ )
		{
			mergeTo[i] = 0;
		}
	}
	else if( !r_mergeModelSurfaces.GetBool() )
	{
		// don't merge any
		for( i = 0; i < dae->mNodeLibrary.Num(); i++ )
		{
			mergeTo[i] = i;
			object = dae->mNodeLibrary.GetIndex( i ); //object = ase->objects[i];

			if( ( *object )->mMeshes.Num() <= 0 )
			{
				continue;
			}

			meshInstance = &( *object )->mMeshes[0]; //mesh = &object->mesh;

			idStr matName = "_default";
			if( meshInstance->mMaterials.Num() > 0 )
			{
				matName = meshInstance->mMaterials[ 0 ]; //material = ase->materials[object->materialRef];

				matName.Replace( "_", "/" );
				matName.StripTrailingOnce( "-material" );
			}

			surf.shader = declManager->FindMaterial( matName );
			surf.id = this->NumSurfaces();
			this->AddSurface( surf );
		}
	}
	else
	{
		// search for material matches
		for( i = 0 ; i < dae->mNodeLibrary.Num() ; i++ )
		{
			object = dae->mNodeLibrary.GetIndex( i );

			if( ( *object )->mMeshes.Num() <= 0 )
			{
				continue;
			}

			meshInstance = &( *object )->mMeshes[0]; //mesh = &object->mesh;

			idStr matName = "_default";
			if( meshInstance->mMaterials.Num() > 0 )
			{
				matName = meshInstance->mMaterials[ 0 ]; //material = ase->materials[object->materialRef];

				matName.Replace( "_", "/" );
				matName.StripTrailingOnce( "-material" );
			}

			im1 = declManager->FindMaterial( matName );
			if( im1->IsDiscrete() )
			{
				// flares, autosprites, etc
				j = this->NumSurfaces();
			}
			else
			{
				for( j = 0 ; j < this->NumSurfaces() ; j++ )
				{
					modelSurf = &this->surfaces[j];
					im2 = modelSurf->shader;
					if( im1 == im2 )
					{
						// merge this
						mergeTo[i] = j;
						break;
					}
				}
			}
			if( j == this->NumSurfaces() )
			{
				// didn't merge
				mergeTo[i] = j;
				surf.shader = im1;
				surf.id = this->NumSurfaces();
				this->AddSurface( surf );
			}
		}
	}

	idVectorSubset<idVec3, 3> vertexSubset;
	idVectorSubset<idVec2, 2> texCoordSubset;

	// build the surfaces
	for( objectNum = 0; objectNum < dae->mNodeLibrary.Num(); objectNum++ )
	{
		object = ( dae->mNodeLibrary.GetIndex( objectNum ) ); //object = ase->objects[objectNum];

		if( ( *object )->mMeshes.Num() <= 0 )
		{
			continue;
		}

		meshInstance = &( *object )->mMeshes[0]; //mesh = &object->mesh;

		idStr matName = "_default";
#if 0
		if( meshInstance->mMaterials.Num() )
		{
			const Collada::SemanticMappingTable* mappingTable = meshInstance->mMaterials.GetIndex( 0 );
			idStr materialId = mappingTable->mMatName;

			if( dae->mMaterialLibrary.Get( materialId, &material ) )
			{
				matName = ( *material )->mEffect;
			}
		}
#else
		if( meshInstance->mMaterials.Num() > 0 )
		{
			matName = meshInstance->mMaterials[ 0 ]; //material = ase->materials[object->materialRef];

			matName.Replace( "_", "/" );
			matName.StripTrailingOnce( "-material" );
		}

#endif
		im1 = declManager->FindMaterial( matName );

		// It seems like the tools our artists are using often generate
		// verts and texcoords slightly separated that should be merged
		// note that we really should combine the surfaces with common materials
		// before doing this operation, because we can miss a slop combination
		// if they are in different surfaces

		idStr meshId = ( meshInstance )->mMeshOrController;
		dae->mMeshLibrary.Get( meshId, &mesh );

		bool normalsParsed = ( * mesh )->mNormals.Num() > 0; //mesh->normalsParsed;

		// completely ignore any explict normals on surfaces with a renderbump command
		// which will guarantee the best contours and least vertexes.
		const char* rb = im1->GetRenderBump();
		if( rb && rb[0] )
		{
			normalsParsed = false;
		}


		// TODO calculate nodeTransforms recursively using the scene hierarchy

		// transform vertex positions and normals
		idMat4 nodeTransform = dae->CalculateResultTransform( ( *object )->mTransforms );

		// reset origin to zero for root node
		if( objectNum == 0 )
		{
			nodeTransform[0][3] = 0;
			nodeTransform[1][3] = 0;
			nodeTransform[2][3] = 0;
		}


		for( j = 0; j < ( *mesh )->mPositions.Num(); j++ )
		{
			const idVec3& p = ( *mesh )->mPositions[j];

			( *mesh )->mPositions[ j ] = nodeTransform * p;
		}

		for( j = 0; j < ( *mesh )->mNormals.Num(); j++ )
		{
			const idVec3& n = ( *mesh )->mNormals[j];

			idVec4 n2( n.x, n.y, n.z, 0 );
			n2 = nodeTransform * n2;
			n2.Normalize();

			( *mesh )->mNormals[ j ] = n2.ToVec3();
		}


		vRemap = ( int* )R_StaticAlloc( ( *mesh )->mPositions.Num() * sizeof( vRemap[0] ) );

#if 0
		for( j = 0; j < ( *mesh )->mPositions.Num(); j++ )
		{
			vRemap[j] = j;
		}
#else
		if( fastLoad )
		{
			// renderbump doesn't care about vertex count
			for( j = 0; j < ( *mesh )->mPositions.Num(); j++ )
			{
				vRemap[j] = j;
			}
		}
		else
		{
			float vertexEpsilon = r_slopVertex.GetFloat();
			float expand = 2 * 32 * vertexEpsilon;
			idVec3 mins, maxs;

			SIMDProcessor->MinMax( mins, maxs, &( *mesh )->mPositions[0], ( *mesh )->mPositions.Num() );
			mins -= idVec3( expand, expand, expand );
			maxs += idVec3( expand, expand, expand );
			vertexSubset.Init( mins, maxs, 32, 1024 );
			for( j = 0; j < ( *mesh )->mPositions.Num(); j++ )
			{
				vRemap[j] = vertexSubset.FindVector( &( *mesh )->mPositions[0], j, vertexEpsilon );
			}
		}
#endif

		tvRemap = ( int* )R_StaticAlloc( ( *mesh )->mTexCoords.Num() * sizeof( tvRemap[0] ) );

#if 0
		for( j = 0; j < ( *mesh )->mTexCoords.Num(); j++ )
		{
			tvRemap[j] = j;
		}
#else
		if( fastLoad )
		{
			// renderbump doesn't care about vertex count
			for( j = 0; j < ( *mesh )->mTexCoords.Num(); j++ )
			{
				tvRemap[j] = j;
			}
		}
		else
		{
			float texCoordEpsilon = r_slopTexCoord.GetFloat();
			float expand = 2 * 32 * texCoordEpsilon;
			idVec2 mins, maxs;

			SIMDProcessor->MinMax( mins, maxs, &( *mesh )->mTexCoords[0], ( *mesh )->mTexCoords.Num() );
			mins -= idVec2( expand, expand );
			maxs += idVec2( expand, expand );
			texCoordSubset.Init( mins, maxs, 32, 1024 );
			for( j = 0; j < ( *mesh )->mTexCoords.Num(); j++ )
			{
				tvRemap[j] = texCoordSubset.FindVector( &( *mesh )->mTexCoords[0], j, texCoordEpsilon );
			}
		}
#endif

		// we need to find out how many unique vertex / texcoord combinations
		// there are, because ASE tracks them separately but we need them unified

		// the maximum possible number of combined vertexes is the number of indexes
		mvTable = ( matchVert_t* )R_ClearedStaticAlloc( ( *mesh )->mFacePosIndices.Num() * sizeof( mvTable[0] ) );

		// we will have a hash chain based on the xyz values
		mvHash = ( matchVert_t** )R_ClearedStaticAlloc( ( *mesh )->mPositions.Num() * sizeof( mvHash[0] ) );

		// allocate triangle surface
		tri = R_AllocStaticTriSurf();
		tri->numVerts = 0;
		tri->numIndexes = 0;
		R_AllocStaticTriSurfIndexes( tri, ( *mesh )->mFacePosIndices.Num() ); //mesh->numFaces * 3 );
		tri->generateNormals = !normalsParsed;

		// init default normal, color and tex coord index
		normal.Zero();
		color = identityColor;
		tv = 0;

		// find all the unique combinations
		float normalEpsilon = 1.0f - r_slopNormal.GetFloat();

		for( j = 0; j < ( ( *mesh )->mFacePosIndices.Num() / 3 ); j++ )
			//for( j = 0; j < ( *mesh )->mFacePosIndices.Num(); j++ )
		{
			// construct triangles in reverse order
			for( k = 2; k >= 0; k-- )
				//for( k = 0; k < 3; k++ )
			{
				//v = mesh->faces[j].vertexNum[k];

				//v = ( *mesh )->mFacePosIndices[j];
				index = j * 3 + k;

				if( index < 0 || index >= ( *mesh )->mPositions.Num() )
				{
					common->Error( "ConvertDAEToModelSurfaces: bad vertex index in DAE file %s", name.c_str() );
				}

				// collapse the position if it was slightly offset
				v = vRemap[ index ];

#if 1
				// we may or may not have texcoords to compare
				//if( mesh->numTVFaces == mesh->numFaces && mesh->numTVertexes != 0 )
				if( ( *mesh )->mTexCoords.Num() )
				{
					tv = index; //tv = mesh->faces[j].tVertexNum[k];

					if( tv < 0 || tv >= ( *mesh )->mTexCoords.Num() )
					{
						common->Error( "ConvertDAEToModelSurfaces: bad tex coord index in DAE file %s", name.c_str() );
					}

					// collapse the tex coord if it was slightly offset
					tv = tvRemap[tv];
				}


				// we may or may not have normals to compare
				if( normalsParsed )
				{
					normal = ( *mesh )->mNormals[ index ]; //mesh->faces[j].vertexNormals[k];
				}

				// we may or may not have colors to compare
				if( ( *mesh )->mColors.Num() > 0 )
				{
					color = ( byte* ) & ( *mesh )->mColors[ index ]; //mesh->faces[j].vertexColors[k];
				}
#endif
				// find a matching vert
				for( lastmv = NULL, mv = mvHash[v]; mv != NULL; lastmv = mv, mv = mv->next )
				{
#if 1
					if( mv->tv != tv )
					{
						continue;
					}

					if( *( unsigned* )mv->color != *( unsigned* )color )
					{
						continue;
					}

					if( !normalsParsed )
					{
						// if we are going to create the normals, just
						// matching texcoords is enough
						break;
					}

					if( mv->normal * normal > normalEpsilon )
					{
						break;		// we already have this one
					}
#endif
				}

				if( !mv )
				{
					// allocate a new match vert and link to hash chain
					mv = &mvTable[ tri->numVerts ];
					mv->v = v;
					mv->tv = tv;
					mv->normal = normal;
					*( unsigned* )mv->color = *( unsigned* )color;
					mv->next = NULL;

					if( lastmv )
					{
						lastmv->next = mv;
					}
					else
					{
						mvHash[v] = mv;
					}
					tri->numVerts++;
				}

				tri->indexes[tri->numIndexes] = mv - mvTable;
				tri->numIndexes++;
			}
		}

		// allocate space for the indexes and copy them
		if( tri->numIndexes > ( *mesh )->mFacePosIndices.Num() )// mesh->numFaces * 3 )
		{
			common->FatalError( "ConvertDAEToModelSurfaces: index miscount in DAE file %s", name.c_str() );
		}
		if( tri->numVerts > ( *mesh )->mFacePosIndices.Num() ) //mesh->numFaces * 3 )
		{
			common->FatalError( "ConvertDAEToModelSurfaces: vertex miscount in DAE file %s", name.c_str() );
		}

		// now allocate and generate the combined vertexes
		R_AllocStaticTriSurfVerts( tri, tri->numVerts );
		for( j = 0; j < tri->numVerts; j++ )
		{
			mv = &mvTable[j];
			tri->verts[ j ].Clear();
			tri->verts[ j ].xyz = ( *mesh )->mPositions[ mv->v ];
			tri->verts[ j ].normal = ( mv->normal );
			*( unsigned* )tri->verts[j].color = *( unsigned* )mv->color;

			if( ( *mesh )->mTexCoords.Num() > 0 )
			{
				const idVec2& tv = ( *mesh )->mTexCoords[ mv->tv ];
				float u = tv.x;
				float v = tv.y;
				tri->verts[ j ].st = idVec2( u, v );
			}
		}

		R_StaticFree( mvTable );
		R_StaticFree( mvHash );
		R_StaticFree( tvRemap );
		R_StaticFree( vRemap );

		// see if we need to merge with a previous surface of the same material
		modelSurf = &this->surfaces[mergeTo[ objectNum ]];
		srfTriangles_t*	mergeTri = modelSurf->geometry;
		if( !mergeTri )
		{
			modelSurf->geometry = tri;
		}
		else
		{
			modelSurf->geometry = R_MergeTriangles( mergeTri, tri );
			R_FreeStaticTriSurf( tri );
			R_FreeStaticTriSurf( mergeTri );
		}
	}

	return true;
}

#endif