/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _Entities_EnemyRunInto_INCLUDED
#define _Entities_EnemyRunInto_INCLUDED 1
#include <Entities/EnemyBase.h>
extern "C" DECL_DLL CDLLEntityClass CEnemyRunInto_DLLClass;
class DECL_DLL  CEnemyRunInto : public CEnemyBase {
public:
   virtual void SetDefaultProperties(void);
  CEntityPointer m_penLastTouched;
  FLOAT m_fLastTouchedTime;
  BOOL m_bWhileLoop;
  FLOAT m_fMassKicked;
  ANGLE m_fAttackRotateRunInto;
  
#line 41 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
virtual void AdjustDifficulty(void);
   
#line 51 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
void IncreaseKickedMass(CEntity * pen);
  
#line 65 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
virtual void LiveEntityTouched(ETouch etouch);
  
#line 67 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
virtual BOOL HigherMass(void);
#define  STATE_CEnemyRunInto_Fire 0x01380000
  BOOL 
#line 76 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
Fire(const CEntityEvent &__eeInput);
#define  STATE_CEnemyRunInto_RotateToEnemy 0x01380001
  BOOL 
#line 84 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
RotateToEnemy(const CEntityEvent &__eeInput);
  BOOL H0x01380002_RotateToEnemy_01(const CEntityEvent &__eeInput);
  BOOL H0x01380003_RotateToEnemy_02(const CEntityEvent &__eeInput);
  BOOL H0x01380004_RotateToEnemy_03(const CEntityEvent &__eeInput);
  BOOL H0x01380005_RotateToEnemy_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemyRunInto_RunIntoEnemy 0x01380006
  BOOL 
#line 186 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
RunIntoEnemy(const CEntityEvent &__eeInput);
  BOOL H0x01380007_RunIntoEnemy_01(const CEntityEvent &__eeInput);
  BOOL H0x01380008_RunIntoEnemy_02(const CEntityEvent &__eeInput);
  BOOL H0x01380009_RunIntoEnemy_03(const CEntityEvent &__eeInput);
  BOOL H0x0138000a_RunIntoEnemy_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemyRunInto_RunAwayFromEnemy 0x0138000b
  BOOL 
#line 252 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
RunAwayFromEnemy(const CEntityEvent &__eeInput);
  BOOL H0x0138000c_RunAwayFromEnemy_01(const CEntityEvent &__eeInput);
  BOOL H0x0138000d_RunAwayFromEnemy_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyRunInto_MainLoop 0x0138000e
  BOOL 
#line 290 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
MainLoop(const CEntityEvent &__eeInput);
#define  STATE_CEnemyRunInto_Main 1
  BOOL 
#line 295 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/EnemyRunInto.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _Entities_EnemyRunInto_INCLUDED
