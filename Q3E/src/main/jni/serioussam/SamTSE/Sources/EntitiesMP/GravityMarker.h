/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_GravityMarker_INCLUDED
#define _EntitiesMP_GravityMarker_INCLUDED 1
#include <EntitiesMP/Marker.h>
#include <EntitiesMP/GravityRouter.h>
extern DECL_DLL CEntityPropertyEnumType GravityType_enum;
enum GravityType {
  LT_PARALLEL = 0,
  LT_CENTRAL = 1,
  LT_CYLINDRICAL = 2,
  LT_TORUS = 3,
};
DECL_DLL inline void ClearToDefault(GravityType &e) { e = (GravityType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CGravityMarker_DLLClass;
class CGravityMarker : public CMarker {
public:
virtual BOOL IsImportant(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  enum GravityType m_gtType;
  FLOAT m_fStrength;
  RANGE m_rFallOff;
  RANGE m_rHotSpot;
  RANGE m_rTorusR;
  FLOAT m_fAcc;
  FLOAT m_fSign;
  FLOAT m_fStep;
  ANGLE3D m_aForceDir;
  FLOAT m_fForceA;
  FLOAT m_fForceV;
  FLOAT3D m_vForceDir;
   
#line 58 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
inline FLOAT StrengthAtDistance(FLOAT fDistance);
   
#line 65 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
const CTString & GetForceName(INDEX i);
   
#line 70 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
void GetForce(INDEX i,const FLOAT3D & vPoint,
#line 71 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
CForceStrength & fsGravity,CForceStrength & fsField);
   
#line 139 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
BOOL HandleEvent(const CEntityEvent & ee);
#define  STATE_CGravityMarker_Main 1
  BOOL 
#line 152 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTSE/Sources/EntitiesMP/GravityMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_GravityMarker_INCLUDED
