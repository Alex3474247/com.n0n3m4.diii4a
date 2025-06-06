/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _Entities_Damager_INCLUDED
#define _Entities_Damager_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CDamager_DLLClass;
class CDamager : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  CTString m_strDescription;
  enum DamageType m_dmtType;
  FLOAT m_fAmmount;
  CEntityPointer m_penToDamage;
  BOOL m_bDamageFromTriggerer;
   
#line 39 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Damager.es"
const CTString & GetDescription(void)const;
#define  STATE_CDamager_Main 1
  BOOL 
#line 44 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/Damager.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00e50000_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00e50001_Main_02(const CEntityEvent &__eeInput);
  BOOL H0x00e50002_Main_03(const CEntityEvent &__eeInput);
  BOOL H0x00e50003_Main_04(const CEntityEvent &__eeInput);
  BOOL H0x00e50004_Main_05(const CEntityEvent &__eeInput);
  BOOL H0x00e50005_Main_06(const CEntityEvent &__eeInput);
};
#endif // _Entities_Damager_INCLUDED
