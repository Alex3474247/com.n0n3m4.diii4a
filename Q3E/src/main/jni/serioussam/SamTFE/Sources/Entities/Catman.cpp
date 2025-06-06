/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 17 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"

#include "Entities/StdH/StdH.h"
#include "Models/Enemies/Catman/CatMan.h"

#include <Entities/Catman.h>
#include <Entities/Catman_tables.h>
#line 31 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"

// info structure
static EntityInfo eiCatman = {
  EIBT_FLESH, 140.0f,
  0.0f, 2.0f, 0.0f,
  0.0f, 1.5f, 0.0f,
};

void CCatman::SetDefaultProperties(void) {
  m_cmtType = CMT_SOLDIER ;
  CEnemyBase::SetDefaultProperties();
}
  
#line 63 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void * CCatman::GetEntityInfo(void) {
#line 64 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
return & eiCatman ;
#line 65 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 68 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 69 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection) 
#line 70 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
{
#line 72 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
if(! IsOfClass  (penInflictor  , "Catman")){
#line 73 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
CEnemyBase  :: ReceiveDamage  (penInflictor  , dmtType  , fDamageAmmount  , vHitPoint  , vDirection );
#line 74 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
#line 75 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 79 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
INDEX CCatman::AnimForDamage(FLOAT fDamage) {
#line 80 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
INDEX iAnim ;
#line 81 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
switch(IRnd  () % 3){
#line 82 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
case 0: iAnim  = CATMAN_ANIM_WOUND01 ;break ;
#line 83 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
case 1: iAnim  = CATMAN_ANIM_WOUND02 ;break ;
#line 84 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
case 2: iAnim  = CATMAN_ANIM_WOUND03 ;break ;
#line 85 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
default  : iAnim  = CATMAN_ANIM_WOUND01 ;
#line 86 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
#line 87 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (iAnim  , 0);
#line 88 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
return iAnim ;
#line 89 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 92 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
INDEX CCatman::AnimForDeath(void) {
#line 93 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
INDEX iAnim ;
#line 94 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
switch(IRnd  () % 2){
#line 95 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
case 0: iAnim  = CATMAN_ANIM_DEATH01 ;break ;
#line 96 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
case 1: iAnim  = CATMAN_ANIM_DEATH02 ;break ;
#line 97 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
default  : iAnim  = CATMAN_ANIM_DEATH01 ;
#line 98 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
#line 99 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (iAnim  , 0);
#line 100 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
return iAnim ;
#line 101 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 103 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::DeathNotify(void) {
#line 104 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
ChangeCollisionBoxIndexWhenPossible  (CATMAN_COLLISION_BOX_DEATH );
#line 105 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
en_fDensity  = 500.0f;
#line 106 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 109 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::StandingAnim(void) {
#line 110 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (CATMAN_ANIM_STAND  , AOF_LOOPING  | AOF_NORESTART );
#line 111 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 112 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::WalkingAnim(void) {
#line 113 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (CATMAN_ANIM_WALK  , AOF_LOOPING  | AOF_NORESTART );
#line 114 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 115 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::RunningAnim(void) {
#line 116 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (CATMAN_ANIM_RUN  , AOF_LOOPING  | AOF_NORESTART );
#line 117 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 118 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::RotatingAnim(void) {
#line 119 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (CATMAN_ANIM_WALK  , AOF_LOOPING  | AOF_NORESTART );
#line 120 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 123 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::IdleSound(void) {
#line 124 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
PlaySound  (m_soSound  , SOUND_IDLE  , SOF_3D );
#line 125 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 126 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::SightSound(void) {
#line 127 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
PlaySound  (m_soSound  , SOUND_SIGHT  , SOF_3D );
#line 128 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 129 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::WoundSound(void) {
#line 130 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
PlaySound  (m_soSound  , SOUND_WOUND  , SOF_3D );
#line 131 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
  
#line 132 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
void CCatman::DeathSound(void) {
#line 133 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
PlaySound  (m_soSound  , SOUND_DEATH  , SOF_3D );
#line 134 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
}
BOOL CCatman::
#line 141 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
Fire(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CCatman_Fire
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CCatman::Fire expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 143 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StandingAnim  ();
#line 144 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetTimerAfter(0.2f + FRnd  () / 4);
Jump(STATE_CURRENT, 0x012d0001, FALSE, EBegin());return TRUE;}BOOL CCatman::H0x012d0001_Fire_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x012d0001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x012d0002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CCatman::H0x012d0002_Fire_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x012d0002
;
#line 147 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StartModelAnim  (CATMAN_ANIM_ATTACK02  , 0);
#line 148 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
ShootProjectile  (PRT_CATMAN_FIRE  , FLOAT3D (0.0f , 1.5f , 0.5f) , ANGLE3D (0 , 0 , 0));
#line 149 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
PlaySound  (m_soSound  , SOUND_FIRE  , SOF_3D );
#line 150 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetTimerAfter(FRnd  () / 3 + 0.6f);
Jump(STATE_CURRENT, 0x012d0003, FALSE, EBegin());return TRUE;}BOOL CCatman::H0x012d0003_Fire_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x012d0003
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x012d0004, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CCatman::H0x012d0004_Fire_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x012d0004
;
#line 152 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
Return(STATE_CURRENT,EReturn  ());
#line 152 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CCatman::
#line 160 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CCatman_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CCatman::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 162 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
InitAsModel  ();
#line 163 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetPhysicsFlags  (EPF_MODEL_WALKING  | EPF_HASLUNGS );
#line 164 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetCollisionFlags  (ECF_MODEL );
#line 165 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetFlags  (GetFlags  () | ENF_ALIVE );
#line 166 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetHealth  (15.0f);
#line 167 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fMaxHealth  = 15.0f;
#line 168 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
en_tmMaxHoldBreath  = 5.0f;
#line 169 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
en_fDensity  = 2000.0f;
#line 172 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetModel  (MODEL_CATMAN );
#line 173 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
SetModelMainTexture  (TEXTURE_SOLDIER );
#line 174 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
StandingAnim  ();
#line 176 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fWalkSpeed  = FRnd  () + 1.5f;
#line 177 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_aWalkRotateSpeed  = AngleDeg  (FRnd  () * 10.0f + 25.0f);
#line 178 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fAttackRunSpeed  = FRnd  () * 2.0f + 4.0f;
#line 179 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_aAttackRotateSpeed  = AngleDeg  (FRnd  () * 50 + 245.0f);
#line 180 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fCloseRunSpeed  = FRnd  () * 2.0f + 4.0f;
#line 181 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_aCloseRotateSpeed  = AngleDeg  (FRnd  () * 50 + 245.0f);
#line 183 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fAttackDistance  = 40.0f;
#line 184 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fCloseDistance  = 0.0f;
#line 185 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fStopDistance  = 10.0f;
#line 186 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fAttackFireTime  = 3.0f;
#line 187 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fCloseFireTime  = 1.0f;
#line 188 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fIgnoreRange  = 200.0f;
#line 190 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fBlowUpAmount  = 35.0f;
#line 191 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fBodyParts  = 4;
#line 192 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
m_fDamageWounded  = 0.0f;
#line 195 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Catman.es"
Jump(STATE_CURRENT, STATE_CEnemyBase_MainLoop, FALSE, EVoid());return TRUE; ASSERT(FALSE); return TRUE;};