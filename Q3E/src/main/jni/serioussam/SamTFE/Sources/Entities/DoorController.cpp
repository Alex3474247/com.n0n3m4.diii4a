/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 17 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"

#include "Entities/StdH/StdH.h"
#include <Engine/CurrentVersion.h>

#include <Entities/DoorController.h>
#include <Entities/DoorController_tables.h>
void CDoorController::SetDefaultProperties(void) {
  m_strName = "DoorController";
  m_strDescription = "";
  m_penTarget1 = NULL;
  m_penTarget2 = NULL;
  m_fWidth = 2.0f;
  m_fHeight = 3.0f;
  m_bPlayersOnly = TRUE ;
  m_dtType = DT_AUTO ;
  m_strLockedMessage = "";
  m_penLockedTarget = NULL;
  m_kitKey = KIT_ANKHWOOD ;
  m_bTriggerOnAnything = FALSE ;
  m_bActive = TRUE ;
  m_bLocked = FALSE ;
  m_penCaused = NULL;
  CRationalEntity::SetDefaultProperties();
}
  
#line 60 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
CEntity * CDoorController::GetTarget(void)const {return m_penTarget1 ;}
  
#line 62 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
const CTString & CDoorController::GetDescription(void)const {
#line 63 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penTarget1  != NULL  && m_penTarget2  != NULL ){
#line 64 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
((CTString &) m_strDescription ) . PrintF  ("->%s,%s" , 
#line 65 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
(const char  *) m_penTarget1  -> GetName  () , (const char  *) m_penTarget2  -> GetName  ());
#line 66 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}else if(m_penTarget1  != NULL ){
#line 67 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
((CTString &) m_strDescription ) . PrintF  ("->%s" , 
#line 68 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
(const char  *) m_penTarget1  -> GetName  ());
#line 69 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}else {
#line 70 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
((CTString &) m_strDescription ) . PrintF  ("-><none>");
#line 71 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 72 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return m_strDescription ;
#line 73 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
  
#line 76 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
BOOL CDoorController::CanReactOnEntity(CEntity * pen) 
#line 77 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
{
#line 78 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(pen  == NULL ){
#line 79 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return FALSE ;
#line 80 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 82 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(! (pen  -> GetFlags  () & ENF_ALIVE )){
#line 83 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return FALSE ;
#line 84 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 86 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_bPlayersOnly  && ! IsDerivedFromClass  (pen  , "Player")){
#line 87 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return FALSE ;
#line 88 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 90 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE ;
#line 91 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
  
#line 94 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
BOOL CDoorController::CanTriggerOnEntity(CEntity * pen) 
#line 95 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
{
#line 96 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return m_bTriggerOnAnything  || CanReactOnEntity  (pen );
#line 97 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
  
#line 99 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
void CDoorController::TriggerDoor(void) 
#line 100 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
{
#line 101 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penTarget1  != NULL ){
#line 102 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SendToTarget  (m_penTarget1  , EET_TRIGGER  , m_penCaused );
#line 103 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 104 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penTarget2  != NULL ){
#line 105 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SendToTarget  (m_penTarget2  , EET_TRIGGER  , m_penCaused );
#line 106 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 107 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
  
#line 110 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
void CDoorController::MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX) 
#line 111 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
{
#line 113 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_fWidth  *= fStretch ;
#line 114 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_fHeight  *= fStretch ;
#line 115 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
BOOL CDoorController::
#line 120 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorAuto(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorAuto
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorAuto expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 123 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_bActive ){
#line 124 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAutoActive, TRUE, EVoid());return TRUE;
#line 125 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}else {
#line 126 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAutoInactive, TRUE, EVoid());return TRUE;
#line 127 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
} ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 131 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorAutoActive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorAutoActive
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorAutoActive expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 133 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
ASSERT  (m_bActive );
#line 134 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT,0x00dd0006, FALSE, EInternal());return TRUE;}BOOL CDoorController::H0x00dd0006_DoorAutoActive_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0006
if(!(TRUE )){ Jump(STATE_CURRENT,0x00dd0007, FALSE, EInternal());return TRUE;}
#line 136 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd0002, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0002_DoorAutoActive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0002
switch(__eeInput.ee_slEvent){case(EVENTCODE_EPass):{const EPass&ePass= (EPass&)__eeInput;

#line 140 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(CanReactOnEntity  (ePass  . penOther )){
#line 142 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_penCaused  = ePass  . penOther ;
#line 143 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
TriggerDoor  ();
#line 147 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(_SE_DEMO  && GetSP  () -> sp_bCooperative  && ! GetSP  () -> sp_bSinglePlayer ){
#line 148 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_strName  == "Appear gold amon"){
#line 149 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
CPlayer  * penPlayer  = (CPlayer  *) & * ePass  . penOther ;
#line 150 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
penPlayer  -> SetGameEnd  ();
#line 151 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 152 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 154 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 155 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 156 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 157 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 161 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_bActive  = FALSE ;
#line 162 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAutoInactive, TRUE, EVoid());return TRUE;
#line 163 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 165 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 166 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 167 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd0003_DoorAutoActive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0003
;
#line 170 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd0004, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0004_DoorAutoActive_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0004
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd0005, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd0005_DoorAutoActive_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0005
;Jump(STATE_CURRENT,0x00dd0006, FALSE, EInternal());return TRUE;
#line 171 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}BOOL CDoorController::H0x00dd0007_DoorAutoActive_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0007
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 175 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorAutoInactive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorAutoInactive
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorAutoInactive expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 177 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
ASSERT  (! m_bActive );
#line 178 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT,0x00dd000d, FALSE, EInternal());return TRUE;}BOOL CDoorController::H0x00dd000d_DoorAutoInactive_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd000d
if(!(TRUE )){ Jump(STATE_CURRENT,0x00dd000e, FALSE, EInternal());return TRUE;}
#line 180 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd0009, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0009_DoorAutoInactive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0009
switch(__eeInput.ee_slEvent){case(EVENTCODE_EActivate):{const EActivate&e= (EActivate&)__eeInput;

#line 184 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_bActive  = TRUE ;
#line 185 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAutoActive, TRUE, EVoid());return TRUE;
#line 186 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 188 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 189 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 190 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd000a_DoorAutoInactive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd000a
;
#line 193 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd000b, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd000b_DoorAutoInactive_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd000b
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd000c, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd000c_DoorAutoInactive_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd000c
;Jump(STATE_CURRENT,0x00dd000d, FALSE, EInternal());return TRUE;
#line 194 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}BOOL CDoorController::H0x00dd000e_DoorAutoInactive_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd000e
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 198 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorDummy(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorDummy
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorDummy expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 200 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd0010, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0010_DoorDummy_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0010
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 202 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 203 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 205 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 206 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 207 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd0011_DoorDummy_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0011
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 211 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorTriggered(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorTriggered
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorTriggered expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 213 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT,0x00dd0017, FALSE, EInternal());return TRUE;}BOOL CDoorController::H0x00dd0017_DoorTriggered_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0017
if(!(TRUE )){ Jump(STATE_CURRENT,0x00dd0018, FALSE, EInternal());return TRUE;}
#line 215 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd0013, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0013_DoorTriggered_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0013
switch(__eeInput.ee_slEvent){case(EVENTCODE_EPass):{const EPass&ePass= (EPass&)__eeInput;

#line 217 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(CanReactOnEntity  (ePass  . penOther )){
#line 218 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_strLockedMessage  != ""){
#line 219 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
PrintCenterMessage  (this  , ePass  . penOther  , TranslateConst  (m_strLockedMessage ) , 3.0f , MSS_INFO );
#line 220 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 221 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penLockedTarget  != NULL ){
#line 222 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SendToTarget  (m_penLockedTarget  , EET_TRIGGER  , ePass  . penOther );
#line 223 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 224 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 225 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 226 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 228 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
m_penCaused  = eTrigger  . penCaused ;
#line 229 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
TriggerDoor  ();
#line 230 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorDummy, TRUE, EVoid());return TRUE;
#line 231 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 233 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 234 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 235 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd0014_DoorTriggered_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0014
;
#line 238 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd0015, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0015_DoorTriggered_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0015
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd0016, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd0016_DoorTriggered_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0016
;Jump(STATE_CURRENT,0x00dd0017, FALSE, EInternal());return TRUE;
#line 239 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}BOOL CDoorController::H0x00dd0018_DoorTriggered_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0018
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 243 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorLocked(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorLocked
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorLocked expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 245 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT,0x00dd001e, FALSE, EInternal());return TRUE;}BOOL CDoorController::H0x00dd001e_DoorLocked_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001e
if(!(TRUE )){ Jump(STATE_CURRENT,0x00dd001f, FALSE, EInternal());return TRUE;}
#line 247 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd001a, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd001a_DoorLocked_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001a
switch(__eeInput.ee_slEvent){case(EVENTCODE_EPass):{const EPass&ePass= (EPass&)__eeInput;

#line 249 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(IsDerivedFromClass  (ePass  . penOther  , "Player")){
#line 250 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
CPlayer  * penPlayer  = (CPlayer  *) & * ePass  . penOther ;
#line 252 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
ULONG  ulKey  = (1 << INDEX (m_kitKey ));
#line 253 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(penPlayer  -> m_ulKeys  & ulKey ){
#line 255 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
penPlayer  -> m_ulKeys  &= ~ ulKey ;
#line 257 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
TriggerDoor  ();
#line 266 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAuto, TRUE, EVoid());return TRUE;
#line 268 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}else {
#line 269 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penLockedTarget  != NULL ){
#line 270 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SendToTarget  (m_penLockedTarget  , EET_TRIGGER  , ePass  . penOther );
#line 271 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 272 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 273 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 274 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 275 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 277 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 278 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 279 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd001b_DoorLocked_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001b
;
#line 282 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd001c, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd001c_DoorLocked_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001c
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd001d, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd001d_DoorLocked_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001d
;Jump(STATE_CURRENT,0x00dd001e, FALSE, EInternal());return TRUE;
#line 283 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}BOOL CDoorController::H0x00dd001f_DoorLocked_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd001f
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 287 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
DoorTriggeredAuto(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_DoorTriggeredAuto
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::DoorTriggeredAuto expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 289 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT,0x00dd0025, FALSE, EInternal());return TRUE;}BOOL CDoorController::H0x00dd0025_DoorTriggeredAuto_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0025
if(!(TRUE )){ Jump(STATE_CURRENT,0x00dd0026, FALSE, EInternal());return TRUE;}
#line 291 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00dd0021, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0021_DoorTriggeredAuto_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0021
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 294 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAuto, TRUE, EVoid());return TRUE;
#line 295 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;case(EVENTCODE_EPass):{const EPass&ePass= (EPass&)__eeInput;

#line 297 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(CanReactOnEntity  (ePass  . penOther )){
#line 298 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_strLockedMessage  != ""){
#line 299 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
PrintCenterMessage  (this  , ePass  . penOther  , TranslateConst  (m_strLockedMessage ) , 3.0f , MSS_INFO );
#line 300 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 301 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
if(m_penLockedTarget  != NULL ){
#line 302 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SendToTarget  (m_penLockedTarget  , EET_TRIGGER  , ePass  . penOther );
#line 303 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 304 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}
#line 305 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 306 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;default:{
#line 308 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
return TRUE;
#line 309 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}ASSERT(FALSE);break;
#line 310 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}return TRUE;}BOOL CDoorController::H0x00dd0022_DoorTriggeredAuto_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0022
;
#line 313 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd0023, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0023_DoorTriggeredAuto_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0023
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd0024, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd0024_DoorTriggeredAuto_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0024
;Jump(STATE_CURRENT,0x00dd0025, FALSE, EInternal());return TRUE;
#line 314 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}BOOL CDoorController::H0x00dd0026_DoorTriggeredAuto_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0026
 ASSERT(FALSE); return TRUE;};BOOL CDoorController::
#line 317 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDoorController_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDoorController::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 319 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
InitAsEditorModel  ();
#line 320 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 321 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetCollisionFlags  (ECF_TOUCHMODEL );
#line 324 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
GetModelObject  () -> StretchModel  (FLOAT3D (m_fWidth  , m_fHeight  , m_fWidth ));
#line 325 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetModel  (MODEL_DOORCONTROLLER );
#line 326 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetModelMainTexture  (TEXTURE_DOORCONTROLLER );
#line 327 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
ModelChangeNotify  ();
#line 330 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00dd0027, FALSE, EBegin());return TRUE;}BOOL CDoorController::H0x00dd0027_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0027
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00dd0028, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDoorController::H0x00dd0028_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00dd0028
;
#line 333 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
switch(m_dtType ){
#line 334 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
case DT_AUTO : {
#line 335 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorAuto, TRUE, EVoid());return TRUE;
#line 336 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}break ;
#line 337 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
case DT_TRIGGERED : {
#line 338 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorTriggered, TRUE, EVoid());return TRUE;
#line 339 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}break ;
#line 340 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
case DT_TRIGGEREDAUTO : {
#line 341 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorTriggeredAuto, TRUE, EVoid());return TRUE;
#line 342 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}break ;
#line 343 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
case DT_LOCKED : {
#line 344 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
Jump(STATE_CURRENT, STATE_CDoorController_DoorLocked, TRUE, EVoid());return TRUE;
#line 345 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
}break ;
#line 346 "/data/data/com.termux/files/home/doom3/SeriousSamClassic-1.10.7/SamTFE/Sources/Entities/DoorController.es"
} ASSERT(FALSE); return TRUE;};