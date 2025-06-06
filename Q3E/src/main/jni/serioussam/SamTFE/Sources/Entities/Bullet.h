/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _Entities_Bullet_INCLUDED
#define _Entities_Bullet_INCLUDED 1
#include <Entities/BasicEffects.h>
#include <Engine/Classes/MovableEntity.h>
#define EVENTCODE_EBulletInit 0x01f60000
class DECL_DLL EBulletInit : public CEntityEvent {
public:
EBulletInit();
CEntityEvent *MakeCopy(void);
CEntityPointer penOwner;
FLOAT fDamage;
};
DECL_DLL inline void ClearToDefault(EBulletInit &e) { e = EBulletInit(); } ;
extern DECL_DLL CEntityPropertyEnumType BulletHitType_enum;
enum BulletHitType {
  BHT_NONE = 0,
  BHT_FLESH = 1,
  BHT_BRUSH_STONE = 2,
  BHT_BRUSH_SAND = 3,
  BHT_BRUSH_WATER = 4,
  BHT_BRUSH_UNDER_WATER = 5,
  BHT_ACID = 6,
  BHT_BRUSH_RED_SAND = 7,
};
DECL_DLL inline void ClearToDefault(BulletHitType &e) { e = (BulletHitType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CBullet_DLLClass;
class DECL_DLL  CBullet : public CEntity {
public:
   virtual void SetDefaultProperties(void);
  CEntityPointer m_penOwner;
  FLOAT m_fDamage;
  FLOAT3D m_vTarget;
  FLOAT3D m_vTargetCopy;
  FLOAT3D m_vHitPoint;
  INDEX m_iBullet;
  enum DamageType m_EdtDamage;
  FLOAT m_fBulletSize;
   
#line 85 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void SetDamage(FLOAT fDamage);
   
#line 90 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void CalcTarget(FLOAT fRange);
   
#line 98 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void CalcTarget(CEntity * pen,FLOAT fRange);
   
#line 113 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void CalcJitterTarget(FLOAT fR);
   
#line 138 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void CalcJitterTargetFixed(FLOAT fX,FLOAT fY,FLOAT fJitter);
   
#line 150 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void LaunchBullet(BOOL bSound,BOOL bTrail,BOOL bHitFX);
   
#line 287 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void DestroyBullet(void);
   
#line 297 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void SpawnHitTypeEffect(enum BulletHitType bhtType,BOOL bSound,FLOAT3D vHitNormal,FLOAT3D vHitPoint,
#line 298 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
FLOAT3D vDirection,FLOAT3D vDistance);
   
#line 378 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void SpawnTrail(void);
   
#line 415 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
void SpawnEffect(const FLOAT3D & vHit,const ESpawnEffect & eSpawnEffect);
#define  STATE_CBullet_Main 1
  BOOL 
#line 426 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Bullet.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _Entities_Bullet_INCLUDED
