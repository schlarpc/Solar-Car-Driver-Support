/*************************************************************
 * 
 * evc402.h - EVC402 Motor Controller Serial Comms Library
 * 
 * Defines all serial registers available to be read from
 *  or written to on the New Generation Motors EVC402 
 *  motor controller, based on the 330-000012 Rev. B
 *  Operating Manual (2003-11-12).
 * 
 * Revision History:
 * 06/08/08 - mas - Original version
 *
 ************************************************************/

#ifndef EVC402_H
#define EVC402_H

/*************************************************************
 *
 * NGM commands
 *
 ************************************************************/

#define CMD_CLRCLUTCH			0x000
#define CMD_SETCLUTCH			0x001
#define CMD_CLEARCHGR			0x002
#define CMD_SETCHGR				0x003
#define CMD_SETFWD				0x005
#define CMD_SETREV				0x007
#define CMD_CLRTHRDISABLE		0x008
#define CMD_SETTHRDISABLE		0x009
#define CMD_CLRDISABLE			0x00A
#define CMD_SETDISABLE			0x00B
#define CMD_CLRSPDCTRL			0x00C
#define CMD_SETSPDCTRL			0x00D
#define CMD_SERIALTORQUE		0x080
#define CMD_SERIALSPEED			0x082
#define CMD_DISCTORQUE			0x084
#define CMD_DISCSPEED			0x086
#define CMD_COAST				0x0F0
#define CMD_WRDISABLE			0x0F2
#define CMD_WRENABLE			0x0F3
#define CMD_SAVEALL				0x0F4
#define CMD_RESTOREALL			0x0F5
#define CMD_RESET				0x0FA


/*************************************************************
 *
 * Page 0 - Control
 * Access: Read/Write
 *
 ************************************************************/

#define SI_desiredphaseI		0x000
#define SI_desiredspd			0x001
#define SI_phIramp				0x002
#define SI_spdramp				0x003
#define SI_thrphaseIlimit		0x004
#define SI_rgnphaseIlimit		0x005
#define SI_dischargeIlimit		0x006
#define SI_chargeIlimit			0x007
#define SI_writeenable			0x008


/*************************************************************
 *
 * Page 1 - Instrumentation
 * Access: Read Only
 *
 ************************************************************/

#define AM_velocity				0x100
#define AM_supplyV				0x101
#define AM_supplyI				0x102
#define AM_baseplateT			0x103
#define AM_ambientT				0x104
#define AM_motorT				0x105
#define AM_SOC					0x106
#define AM_thr					0x107
#define AM_rgn					0x108
#define SV_desiredphaseI		0x109
#define SV_desiredspd			0x10A
#define SV_targetphaseI			0x10B
#define SV_drivestate			0x10C
#define SV_fault1latch			0x10D
#define SV_fault1				0x10E
#define SV_fault2				0x10F
#define SV_fault3				0x110
#define SV_thrlimit				0x111
#define SV_rgnlimit				0x112


/*************************************************************
 *
 * Page 2 - Development
 * Access: Read Only
 *
 ************************************************************/

#define DV_motorTest			0x200
#define DV_baseplateTest		0x201
#define IN_rgnphaseIlimit		0x202
#define IN_status				0x203
#define DV_DIstatus				0x204
#define DV_SIstatus				0x205
#define DV_thermallimitmtr		0x206
#define DV_baseplateTderating	0x207
#define DV_maxphaseIthr			0x208
#define DV_maxphaseIrgn			0x209
#define DV_batmaxphIthr			0x20A
#define DV_batmaxphIrgn			0x20B
//reserved						0x20C
//reserved						0x20D


/*************************************************************
 *
 * Page 3 - Vehicle Configuration
 * Access: Read/Write when selected
 *
 ************************************************************/

#define VC_SCsupplyI			0x300
#define VC_OFsupplyI			0x301
#define VC_discrete				0x302
#define VC_invert				0x303
#define VC_thringain			0x304
#define VC_rgningain			0x305
#define VC_thrdeadband			0x306
#define VC_rgndeadband			0x307
#define VC_thrfilter			0x308
#define VC_rgnfilter			0x309
#define VC_Xt					0x30A
#define VC_enginedamping0		0x30B
#define VC_enginedamping1		0x30C
#define VC_Yt0					0x30D
#define VC_Yt1					0x30E
#define VC_spd0					0x30F
#define VC_spd1					0x310
#define VC_lowspdxfrslope		0x311
#define VC_phaseIposramp		0x312
//reserved						0x313
#define VC_K_soc				0x314
#define VC_spdthreshold			0x315
#define VC_softstartT			0x316
#define VC_limphomeSOC			0x317
#define VC_limphomesupplyI		0x318
#define VC_limphomephaseI		0x319
#define VC_rgnphaseIlimit		0x31A
#define VC_rgnslope				0x31B
#define VC_spdSpts0				0x31C
#define VC_spdSpts1				0x31D
#define VC_spdSpts2				0x31E
#define VC_spdSpts3				0x31F
#define VC_spdIpts0				0x320
#define VC_spdIpts1				0x321
#define VC_spdIpts2				0x322
#define VC_spdIpts3				0x323
#define VC_dischargeVpts0		0x324
#define VC_dischargeVpts1		0x325
#define VC_dischargeVpts2		0x326
#define VC_dischargeIpts0		0x327
#define VC_dischargeIpts1		0x328
#define VC_dischargeIpts2		0x329
#define VC_chargeVpts0			0x32A
#define VC_chargeVpts1			0x32B
#define VC_chargeVpts2			0x32C
#define VC_chargeIpts0			0x32D
#define VC_chargeIpts1			0x32E
#define VC_chargeIpts2			0x32F
#define VC_FIbatlim				0x330
#define VC_maxspderror			0x331
#define VC_Kp					0x332
#define VC_Ki					0x333
//reserved						0x334
//reserved						0x335
#define VC_hsfantemp			0x336
//reserved						0x337
//reserved						0x338
#define VC_SCI					0x339
//reserved						0x33A
#define VC_spddeadband			0x33B
#define VC_revSpts0				0x33C
#define VC_revSpts1				0x33D
#define VC_revIpts0				0x33E
#define VC_revIpts1				0x33F


/*************************************************************
 *
 * Page 4 - Battery Configuration
 * Access: Read/Write when selected
 *
 ************************************************************/

#define BC_initbatcapacity		0x400
#define BC_fullchargeV			0x401
#define BC_fullchargeI			0x402
#define BC_fullchargeT			0x403
#define BC_ahrIpts0				0x404
#define BC_ahrIpts1				0x405
#define BC_ahrIpts2				0x406
#define BC_ahrIpts3				0x407
#define BC_ahrIpts4				0x408
#define BC_ahrIpts5				0x409
#define BC_ahrIpts6				0x40A
#define BC_ahrIpts7				0x40B
#define BC_ahrKpts0				0x40C
#define BC_ahrKpts1				0x40D
#define BC_ahrKpts2				0x40E
#define BC_ahrKpts3				0x40F
#define BC_ahrKpts4				0x410
#define BC_ahrKpts5				0x411
#define BC_ahrKpts6				0x412
#define BC_ahrKpts7				0x413


/*************************************************************
 *
 * Page 5 - Reserved
 * Access: Read Only
 *
 ************************************************************/


/*************************************************************
 *
 * Page 6 - Motor Configuration
 * Access: Read/Write when selected
 *
 ************************************************************/

#define MC_FI_motorT			0x600
#define MC_user_mtr0			0x601
#define MC_user_mtr1			0x602


/*************************************************************
 *
 * Page 7 - Motor Calibration
 * Access: Read/Write when selected
 *
 ************************************************************/

#define MA_Kimpts0				0x700
#define MA_Kimpts1				0x701
#define MA_Kpmpts0				0x702
#define MA_Kpmpts1				0x703
#define MA_spdpts0				0x704
#define MA_spdpts1				0x705
#define MA_SCmotorT				0x706
#define MA_OFmotorT				0x707
#define MA_motorITcoeff			0x708
#define MA_motortimeC			0x709
//reserved						0x70A
//reserved						0x70B
#define MA_OFutateangle			0x70C
#define MA_maxerror				0x70D


/*************************************************************
 *
 * Page 8 - Motor Factory Settings
 * Access: Read/Write when selected
 *
 ************************************************************/

#define MF_maxmotorI			0x800
#define MF_defaultmtrT			0x801
#define MF_tempTpts0			0x802
#define MF_tempTpts1			0x803
#define MF_tempTpts2			0x804
#define MF_tempTpts3			0x805
#define MF_tempIpts0			0x806
#define MF_tempIpts1			0x807
#define MF_tempIpts2			0x808
#define MF_tempIpts3			0x809
#define MF_polepairs			0x80A
#define MF_mtrsensors			0x80B
//reserved						0x80C
#define MF_pwmfreq				0x80D
#define MF_pherrfilter			0x80E


/*************************************************************
 *
 * Page 9 - Controller Configuration
 * Access: Read/Write when selected
 *
 ************************************************************/

#define CC_FI_supplyV			0x900
#define CC_FI_supplyI			0x901
#define CC_FI_baseplateT		0x902
#define CC_FI_ambientT			0x903
#define CC_FI_motorT2			0x904
#define CC_user0				0x905
#define CC_user1				0x906


/*************************************************************
 *
 * Page A - Reserved
 * Access: Read Only
 *
 ************************************************************/


/*************************************************************
 *
 * Page B - Controller Factory Settings
 * Access: Read Only
 *
 ************************************************************/

#define CF_thrVpts0				0xB00
#define CF_thrVpts1				0xB01
#define CF_thrVpts2				0xB02
#define CF_thrVpts3				0xBO3
#define CF_thrVpts4				0xB04
#define CF_thrIpts0				0xB05
#define CF_thrIpts1				0xB06
#define CF_thrIpts2				0xB07
#define CF_thrIpts3				0xB08
#define CF_thrIpts4				0xB09
#define CF_rgnVpts0				0xB0A
#define CF_rgnVpts1				0xB0B
#define CF_rgnVpts2				0xB0C
#define CF_rgnVpts3				0xB0D
#define CF_rgnVpts4				0xB0E
#define CF_rgnIpts0				0xB0F
#define CF_rgnIpts1				0xB10
#define CF_rgnIpts2				0xB11
#define CF_rgnIpts3				0xB12
#define CF_rgnIpts4				0xB13
#define CF_default_hsinkT		0xB14
#define CF_tempTpts0			0xB15
#define CF_tempTpts1			0xB16
#define CF_tempTpts2			0xB17
#define CF_tempTpts3			0xB18
#define CF_tempIpts0			0xB19
#define CF_tempIpts1			0xB1A
#define CF_tempIpts2			0xB1B
#define CF_tempIpts3			0xB1C
#define CF_spdrate				0xB1D
//reserved						0xB1E
//reserved						0xB1F
#define CF_SN1					0xB20
#define CF_SN2					0xB21
#define CF_swbuild				0xB22
#define CF_swbuilddate			0xB23


/*************************************************************
 *
 * Page C - Integrations
 * Access: Read Only
 *
 ************************************************************/

#define NV_hours0				0xC00
#define NV_hours1				0xC01
#define NV_revs0				0xC02
#define NV_revs1				0xC03
#define NV_activehours0			0xC04
#define NV_activehours1			0xC05
#define NV_amphours0			0xC06
#define NV_amphours1			0xC07
#define NV_chargehours0			0xC08
#define NV_chargehours1			0xC09
#define NV_batcapacity			0xC0A
#define NV_checksum				0xC0B

#endif