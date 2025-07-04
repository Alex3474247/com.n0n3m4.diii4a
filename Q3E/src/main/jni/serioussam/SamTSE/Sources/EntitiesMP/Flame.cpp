/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 17 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"

#include "EntitiesMP/StdH/StdH.h"
#define TM_APPLY_DAMAGE_QUANTUM 0.25f
#define TM_APPLY_WHOLE_DAMAGE 7.5f
#define DAMAGE_AMMOUNT 30.0f
#define MIN_DAMAGE_QUANTUM (DAMAGE_AMMOUNT/TM_APPLY_WHOLE_DAMAGE*TM_APPLY_DAMAGE_QUANTUM)
#define MAX_DAMAGE_QUANTUM (MIN_DAMAGE_QUANTUM*10.0f)
#define DEATH_BURN_TIME 4.0f

#include "EntitiesMP/MovingBrush.h"

#include <EntitiesMP/Flame.h>
#include <EntitiesMP/Flame_tables.h>
CEntityEvent *EFlame::MakeCopy(void) { CEntityEvent *peeCopy = new EFlame(*this); return peeCopy;}
EFlame::EFlame() : CEntityEvent(EVENTCODE_EFlame) {;
 ClearToDefault(penOwner);
 ClearToDefault(penAttach);
};
CEntityEvent *EStopFlaming::MakeCopy(void) { CEntityEvent *peeCopy = new EStopFlaming(*this); return peeCopy;}
EStopFlaming::EStopFlaming() : CEntityEvent(EVENTCODE_EStopFlaming) {;
 ClearToDefault(m_bNow);
};
#line 42 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"

void CFlame_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) 
{
  pdec->PrecacheModel(MODEL_FLAME);
  pdec->PrecacheTexture(TEXTURE_FLAME);
  pdec->PrecacheSound(SOUND_FLAME);
}

void CFlame::SetDefaultProperties(void) {
  m_penOwner = NULL;
  m_penAttach = NULL;
  m_bLoop = FALSE ;
  m_vHitPoint = FLOAT3D(0.0f , 0.0f , 0.0f);
  m_soEffect.SetOwner(this);
m_soEffect.Stop_internal();
  m_tmStart = 0.0f;
  m_fDamageToApply = 0.0f;
  m_fDamageStep = 0.0f;
  m_fAppliedDamage = 0.0f;
  m_tmFirstStart = 0.0f;
  m_ctFlames = 0;
  m_vPos01 = FLOAT3D(0 , 0 , 0);
  m_vPos02 = FLOAT3D(0 , 0 , 0);
  m_vPos03 = FLOAT3D(0 , 0 , 0);
  m_vPos04 = FLOAT3D(0 , 0 , 0);
  m_vPos05 = FLOAT3D(0 , 0 , 0);
  m_vPos06 = FLOAT3D(0 , 0 , 0);
  m_vPos07 = FLOAT3D(0 , 0 , 0);
  m_vPos08 = FLOAT3D(0 , 0 , 0);
  m_vPos09 = FLOAT3D(0 , 0 , 0);
  m_vPos10 = FLOAT3D(0 , 0 , 0);
  m_vPlaneNormal = FLOAT3D(0 , 0 , 0);
  m_bBurningBrush = FALSE ;
  m_tmDeathParticlesStart = 1e6;
  m_penPrediction = NULL;
  CMovableModelEntity::SetDefaultProperties();
}
  
#line 101 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
void CFlame::AddDependentsToPrediction(void) 
#line 102 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 103 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_penOwner  -> AddToPrediction  ();
#line 104 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 106 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
void CFlame::PostMoving(void) {
#line 107 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CMovableModelEntity  :: PostMoving  ();
#line 110 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CContentType  & ctDn  = GetWorld  () -> wo_actContentTypes  [ en_iDnContent  ];
#line 112 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(! (ctDn  . ct_ulFlags  & CTF_BREATHABLE_LUNGS )){
#line 113 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
EStopFlaming  esf ;
#line 114 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
esf  . m_bNow  = TRUE ;
#line 115 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SendEvent  (esf );
#line 116 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 119 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
en_ulFlags  &= ~ ENF_INRENDERING ;
#line 121 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
en_plLastPlacement  = en_plPlacement ;
#line 122 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 125 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
void CFlame::Read_t(CTStream * istr) 
#line 126 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 127 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CMovableModelEntity  :: Read_t  (istr );
#line 128 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetupLightSource  ();
#line 129 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 131 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
BOOL CFlame::IsPointInsidePolygon(const FLOAT3D & vPos,CBrushPolygon * pbpo) 
#line 132 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 133 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOATplane3D & plPlane  = pbpo  -> bpo_pbplPlane  -> bpl_plAbsolute ;
#line 135 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
INDEX iMajorAxis1  , iMajorAxis2 ;
#line 136 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
GetMajorAxesForPlane  (plPlane  , iMajorAxis1  , iMajorAxis2 );
#line 139 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CIntersector  isIntersector  (vPos  (iMajorAxis1 ) , vPos  (iMajorAxis2 ));
#line 141 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FOREACHINSTATICARRAY  (pbpo  -> bpo_abpePolygonEdges  , CBrushPolygonEdge  , itbpePolygonEdge ){
#line 143 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
const FLOAT3D & vVertex0  = itbpePolygonEdge  -> bpe_pbedEdge  -> bed_pbvxVertex0  -> bvx_vAbsolute ;
#line 144 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
const FLOAT3D & vVertex1  = itbpePolygonEdge  -> bpe_pbedEdge  -> bed_pbvxVertex1  -> bvx_vAbsolute ;
#line 146 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
isIntersector  . AddEdge  (
#line 147 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vVertex0  (iMajorAxis1 ) , vVertex0  (iMajorAxis2 ) , 
#line 148 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vVertex1  (iMajorAxis1 ) , vVertex1  (iMajorAxis2 ));
#line 149 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 151 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return isIntersector  . IsIntersecting  ();
#line 152 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 155 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CLightSource * CFlame::GetLightSource(void) 
#line 156 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 157 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(! IsPredictor  ()){
#line 158 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return & m_lsLightSource ;
#line 159 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}else {
#line 160 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return NULL ;
#line 161 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 162 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 165 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
void CFlame::RenderParticles(void) 
#line 166 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 167 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fTimeFactor  = CalculateRatio  (_pTimer  -> CurrentTick  () , m_tmFirstStart  , m_tmStart  + TM_APPLY_WHOLE_DAMAGE  , 0.05f , 0.2f);
#line 168 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fDeathFactor  = 1.0f;
#line 169 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(_pTimer  -> CurrentTick  () > m_tmDeathParticlesStart )
#line 170 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 171 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
fDeathFactor  = 1.0f - Clamp  ((_pTimer  -> CurrentTick  () - m_tmDeathParticlesStart ) / DEATH_BURN_TIME  , 0.0f , 1.0f);
#line 172 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 173 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CEntity  * penParent  = GetParent  ();
#line 174 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fPower  = ClampUp  (m_fDamageStep  - MIN_DAMAGE_QUANTUM  , MAX_DAMAGE_QUANTUM ) / MAX_DAMAGE_QUANTUM ;
#line 175 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(penParent  != NULL )
#line 176 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 177 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if((penParent  -> en_RenderType  == CEntity  :: RT_MODEL  || penParent  -> en_RenderType  == CEntity  :: RT_EDITORMODEL  || 
#line 178 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
penParent  -> en_RenderType  == CEntity  :: RT_SKAMODEL  || penParent  -> en_RenderType  == CEntity  :: RT_SKAEDITORMODEL ) && 
#line 179 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
(Particle_GetViewer  () != penParent ))
#line 180 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 181 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Particles_Burning  (penParent  , fPower  , fTimeFactor  * fDeathFactor );
#line 182 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 183 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
else 
#line 184 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 185 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Particles_BrushBurning  (this  , & m_vPos01  , m_ctFlames  , m_vPlaneNormal  , fPower  , fTimeFactor  * fDeathFactor );
#line 186 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 187 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 188 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
  
#line 191 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
void CFlame::SetupLightSource(void) 
#line 192 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 194 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CLightSource  lsNew ;
#line 195 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_ulFlags  = LSF_NONPERSISTENT  | LSF_DYNAMIC ;
#line 196 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(m_bBurningBrush )
#line 197 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 198 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UBYTE  ubRndH  = UBYTE  (25 + (FLOAT (rand  ()) / (float ) (RAND_MAX ) - 0.5f) * 28);
#line 199 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UBYTE  ubRndS  = 166;
#line 200 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UBYTE  ubRndV  = 48;
#line 201 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_colColor  = HSVToColor  (ubRndH  , ubRndS  , ubRndV );
#line 203 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_rFallOff  = 4.0f;
#line 204 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_rHotSpot  = 0.2f;
#line 205 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 206 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
else 
#line 207 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 208 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_colColor  = 0x8F8F5000;
#line 209 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_rFallOff  = 6.0f;
#line 210 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_rHotSpot  = 0.50f;
#line 211 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 212 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_plftLensFlare  = NULL ;
#line 213 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_ubPolygonalMask  = 0;
#line 214 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
lsNew  . ls_paoLightAnimation  = NULL ;
#line 216 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_lsLightSource  . ls_penEntity  = this ;
#line 217 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_lsLightSource  . SetLightSource  (lsNew );
#line 218 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
BOOL CFlame::
#line 225 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CFlame_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EFlame, "CFlame::Main expects 'EFlame' as input!");  const EFlame &ef = (const EFlame &)__eeInput;
#line 227 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
ASSERT  (ef  . penOwner  != NULL );
#line 228 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
ASSERT  (ef  . penAttach  != NULL );
#line 229 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_penOwner  = ef  . penOwner ;
#line 230 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_penAttach  = ef  . penAttach ;
#line 233 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_fDamageToApply  = DAMAGE_AMMOUNT ;
#line 234 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_fDamageStep  = m_fDamageToApply  / (TM_APPLY_WHOLE_DAMAGE  / TM_APPLY_DAMAGE_QUANTUM );
#line 237 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_tmStart  = _pTimer  -> CurrentTick  ();
#line 238 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_tmFirstStart  = m_tmStart ;
#line 239 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetParent  (ef  . penAttach );
#line 241 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
InitAsEditorModel  ();
#line 242 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetPhysicsFlags  (EPF_MODEL_FLYING );
#line 243 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetCollisionFlags  (ECF_FLAME );
#line 244 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetFlags  (GetFlags  () | ENF_SEETHROUGH );
#line 246 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetModel  (MODEL_FLAME );
#line 247 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetModelMainTexture  (TEXTURE_FLAME );
#line 248 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
ModelChangeNotify  ();
#line 251 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_soEffect  . Set3DParameters  (10.0f , 1.0f , 1.0f , 1.0f);
#line 252 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
PlaySound  (m_soEffect  , SOUND_FLAME  , SOF_3D  | SOF_LOOP );
#line 255 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
AddToMovers  ();
#line 257 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bBurningBrush  = FALSE ;
#line 258 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
BOOL bAllowFlame  = TRUE ;
#line 259 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(! (ef  . penAttach  -> en_RenderType  == CEntity  :: RT_MODEL  || ef  . penAttach  -> en_RenderType  == CEntity  :: RT_EDITORMODEL  || 
#line 260 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
ef  . penAttach  -> en_RenderType  == CEntity  :: RT_SKAMODEL  || ef  . penAttach  -> en_RenderType  == CEntity  :: RT_SKAEDITORMODEL ))
#line 261 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 262 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bBurningBrush  = TRUE ;
#line 263 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vPos  = GetPlacement  () . pl_PositionVector ;
#line 264 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOATplane3D plPlane ;
#line 265 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fDistanceToEdge ;
#line 266 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FindSectorsAroundEntity  ();
#line 267 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
CBrushPolygon  * pbpo  = NULL ;
#line 268 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
pbpo  = GetNearestPolygon  (vPos  , plPlane  , fDistanceToEdge );
#line 269 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vBrushPos  = ef  . penAttach  -> GetPlacement  () . pl_PositionVector ;
#line 270 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOATmatrix3D mBrushRotInv  = ! ef  . penAttach  -> GetRotationMatrix  ();
#line 271 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(pbpo  != NULL  && pbpo  -> bpo_pbscSector  -> bsc_pbmBrushMip  -> bm_pbrBrush  -> br_penEntity  == ef  . penAttach )
#line 272 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 273 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
plPlane  = pbpo  -> bpo_pbplPlane  -> bpl_plAbsolute ;
#line 274 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_vPlaneNormal  = (FLOAT3D &) plPlane ;
#line 275 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_vPlaneNormal  . Normalize  ();
#line 277 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vU  , vV ;
#line 279 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(plPlane  (2) < - 0.1f)
#line 280 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 281 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
bAllowFlame  = FALSE ;
#line 282 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 285 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(Abs  (plPlane  (2)) > 0.5f){
#line 287 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vU  = FLOAT3D (1.0f , 0.0f , 0.0f) * m_vPlaneNormal ;
#line 289 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}else {
#line 291 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vU  = FLOAT3D (0.0f , 1.0f , 0.0f) * m_vPlaneNormal ;
#line 292 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 294 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vU  . Normalize  ();
#line 296 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vV  = vU  * m_vPlaneNormal ;
#line 297 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vV  . Normalize  ();
#line 300 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_ctFlames  = 0;
#line 301 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
for(INDEX iTest  = 0;iTest  < 20;iTest  ++)
#line 302 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 303 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fA  = FRnd  () * 360.0f;
#line 304 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fR  = FRnd  () * 2.0f;
#line 305 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vRndV  = vV  * fR  * SinFast  (fA );
#line 306 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vRndU  = vU  * fR  * CosFast  (fA );
#line 307 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vRndPos  = vPos ;
#line 308 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(iTest  != 0)
#line 309 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 310 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
vRndPos  += vRndV  + vRndU ;
#line 311 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 313 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT3D vProjectedRndPos  = plPlane  . ProjectPoint  (vRndPos );
#line 314 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(IsPointInsidePolygon  (vProjectedRndPos  , pbpo ))
#line 315 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 316 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
(& m_vPos01 ) [ m_ctFlames  ] = (vProjectedRndPos  - vBrushPos ) * mBrushRotInv ;
#line 317 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_ctFlames  ++;
#line 318 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(m_ctFlames  == 6){break ;};
#line 319 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 320 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 321 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 322 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
else 
#line 323 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 324 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
bAllowFlame  = FALSE ;
#line 325 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 326 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 328 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(bAllowFlame )
#line 329 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 330 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetupLightSource  ();
#line 331 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 333 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bLoop  = bAllowFlame ;
#line 334 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Jump(STATE_CURRENT,0x01f80004, FALSE, EInternal());return TRUE;}BOOL CFlame::H0x01f80004_Main_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80004
if(!(m_bLoop )){ Jump(STATE_CURRENT,0x01f80005, FALSE, EInternal());return TRUE;}
#line 335 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
SetTimerAfter(TM_APPLY_DAMAGE_QUANTUM );
Jump(STATE_CURRENT, 0x01f80002, FALSE, EBegin());return TRUE;}BOOL CFlame::H0x01f80002_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80002
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 339 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(m_penAttach  == NULL  || (m_penAttach  -> GetFlags  () & ENF_DELETED )){
#line 341 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 342 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 343 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 345 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
const FLOAT fDamageMul  = GetSeriousDamageMultiplier  (m_penOwner );
#line 346 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fDamageToApply  = fDamageMul  * (m_fDamageToApply  / TM_APPLY_WHOLE_DAMAGE  * TM_APPLY_DAMAGE_QUANTUM ) * m_fDamageStep ;
#line 348 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
InflictDirectDamage  (m_penAttach  , m_penOwner  , DMT_BURNING  , fDamageToApply  , 
#line 349 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
GetPlacement  () . pl_PositionVector  , - en_vGravityDir );
#line 350 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_fAppliedDamage  += fDamageToApply ;
#line 352 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return TRUE;
#line 353 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EFlame):{const EFlame&ef= (EFlame&)__eeInput;

#line 355 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_penOwner  = ef  . penOwner ;
#line 356 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fTimeLeft  = m_tmStart  + TM_APPLY_WHOLE_DAMAGE  - _pTimer  -> CurrentTick  ();
#line 357 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
FLOAT fDamageLeft  = (fTimeLeft  / TM_APPLY_DAMAGE_QUANTUM ) * m_fDamageStep ;
#line 358 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_fDamageToApply  = ClampUp  (fDamageLeft  + DAMAGE_AMMOUNT  , 80.0f);
#line 359 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_tmStart  = _pTimer  -> CurrentTick  ();
#line 360 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_fDamageStep  = m_fDamageToApply  / (TM_APPLY_WHOLE_DAMAGE  / TM_APPLY_DAMAGE_QUANTUM );
#line 361 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return TRUE;
#line 362 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStopFlaming):{const EStopFlaming&esf= (EStopFlaming&)__eeInput;

#line 364 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(! esf  . m_bNow )
#line 365 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 366 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_tmDeathParticlesStart  = _pTimer  -> CurrentTick  ();
#line 367 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return TRUE;
#line 368 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 369 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
else 
#line 370 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 371 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 372 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 373 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}
#line 374 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EBrushDestroyed):{const EBrushDestroyed&e= (EBrushDestroyed&)__eeInput;

#line 376 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 377 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 378 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 380 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}return TRUE;}BOOL CFlame::H0x01f80003_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80003

#line 381 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
if(_pTimer  -> CurrentTick  () > m_tmStart  + TM_APPLY_WHOLE_DAMAGE )
#line 382 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
{
#line 383 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 384 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}Jump(STATE_CURRENT,0x01f80004, FALSE, EInternal());return TRUE;
#line 385 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
}BOOL CFlame::H0x01f80005_Main_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80005

#line 388 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Destroy  ();
#line 389 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
Return(STATE_CURRENT,EVoid());
#line 389 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/Flame.es"
return TRUE; ASSERT(FALSE); return TRUE;};