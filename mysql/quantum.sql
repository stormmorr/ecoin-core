# ************************************************************
# Sequel Pro SQL dump
# Version 4096
#
# http://www.sequelpro.com/
# http://code.google.com/p/sequel-pro/
#
# Host: 127.0.0.1 (MySQL 5.5.9)
# Database: yourdatabasename
# Generation Time: 2013-06-07 17:30:50 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


# Dump of table cube systems
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `cube`;

CREATE TABLE `cube` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Name` varchar(256) DEFAULT NULL,
  `AvatarREF` int(11) DEFAULT NULL,
  `ShadowREF` int(11) DEFAULT NULL,
  `ArchiveID` int(11) DEFAULT NULL,
  `Class` int(11) DEFAULT NULL,
  `Level` int(11) DEFAULT NULL,
  `AvatarSCALE` float(7,3) DEFAULT NULL,
  `UnusedXP` float(7,3) DEFAULT NULL,
  `NomITEMS` int(11) DEFAULT NULL,
  `CurrITEM` int(11) DEFAULT NULL,
  `NomSpells` int(11) DEFAULT NULL,
  `CurrSpell` int(11) DEFAULT NULL,
  `CurrSPELLSELL` int(11) DEFAULT NULL,
  `CurrWEAPON` int(11) DEFAULT NULL,
  `CurrHEADARMOUR` int(11) DEFAULT NULL,
  `CurrUPPERARMOUR` int(11) DEFAULT NULL,
  `CurrLOWERARMOUR` int(11) DEFAULT NULL,
  `CurrVEHICLE` int(11) DEFAULT NULL,
  `CurrMECH` int(11) DEFAULT NULL,
  `CurrZCAPTURE` int(11) DEFAULT NULL,
  `CurrZSENTRY` int(11) DEFAULT NULL,
  `CurrZMINE` int(11) DEFAULT NULL,
  `CurrZANOMALY` int(11) DEFAULT NULL,
  `CDataID` int(11) DEFAULT NULL,
  `BDataID` int(11) DEFAULT NULL,
  `Height` float(7,3) DEFAULT NULL,
  `Weight` float(7,3) DEFAULT NULL,
  `PhysicalStr` float(7,3) DEFAULT NULL,
  `Quickness` float(7,3) DEFAULT NULL,
  `Dexterity` float(7,3) DEFAULT NULL,
  `Constitution` float(7,3) DEFAULT NULL,
  `AggPower` float(7,3) DEFAULT NULL,
  `DefPower` float(7,3) DEFAULT NULL,
  `STOREAggPower` float(7,3) DEFAULT NULL,
  `STOREDefPower` float(7,3) DEFAULT NULL,
  `POWERBANK` float(7,3) DEFAULT NULL,
  `PBTIMER` float(7,3) DEFAULT NULL,
  `ATTACKPOWER` float(7,3) DEFAULT NULL,
  `JINKATT` int(11) DEFAULT NULL,
  `Intelligence` float(7,3) DEFAULT NULL,
  `EnergyEff` float(7,3) DEFAULT NULL,
  `RELQuickness` float(7,3) DEFAULT NULL,
  `RELMoveSpeed` float(7,3) DEFAULT NULL,
  `RELRawPower` float(7,3) DEFAULT NULL,
  `RELEnergyEff` float(7,3) DEFAULT NULL,
  `JINKINCmovespeed` float(7,3) DEFAULT NULL,
  `JINKINCpower` float(7,3) DEFAULT NULL,
  `JINKINC3intensity` float(7,3) DEFAULT NULL,
  `LeftArmSTRENGTH` float(7,3) DEFAULT NULL,
  `RightArmSTRENGTH` float(7,3) DEFAULT NULL,
  `LeftLegSTRENGTH` float(7,3) DEFAULT NULL,
  `RightLegSTRENGTH` float(7,3) DEFAULT NULL,
  `LeftArmSNAP` float(7,3) DEFAULT NULL,
  `RightArmSNAP` float(7,3) DEFAULT NULL,
  `LeftLegSNAP` float(7,3) DEFAULT NULL,
  `RightLegSNAP` float(7,3) DEFAULT NULL,

  `Accuracy` float(7,3) DEFAULT NULL,
  `DrawSPD` float(7,3) DEFAULT NULL,
  `ReloadSPD` float(7,3) DEFAULT NULL,
  `RecoilHEIGHTrecover` float(7,3) DEFAULT NULL,
	
	struct Q_AttackSKILL		AttackSKILL[30];
	struct Q_ITEMDATA			ItemDATA[151];
	struct Q_SpellSLOT			SpellSLOT[10];
	struct Q_AngelEFF			AngelEFFECTS;
	struct Q_AngelEDIT			AngelCOLOURS;
	struct Q_SPELLS				Spells[51];
	struct Q_CLSClericSKILL		CLSClericSKILL[12];
  `targetid` int(11) DEFAULT NULL,
  `eslcode` varchar(32) DEFAULT NULL,
  `difficulty` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `attackskill`;

CREATE TABLE `attackskill` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` int(11) DEFAULT NULL,
  `ArrayID` int(11) DEFAULT NULL,
  `Power` float(7,3) DEFAULT NULL,
  `Damage` float(7,3) DEFAULT NULL,
  `MoveSpeed` float(7,3) DEFAULT NULL,
  `NumRotate` float(7,3) DEFAULT NULL,
  `ExpRadius` float(7,3) DEFAULT NULL,
  `Accuracy` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `itemdata`;

CREATE TABLE `itemdata` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` int(11) DEFAULT NULL,
  `ArrayID` int(11) DEFAULT NULL,
  `Name` varchar(256) DEFAULT NULL,
  `ItemActorREF` varchar(256) DEFAULT NULL,
  `ItemScaleFACT` float(7,3) DEFAULT NULL,
  `Type` int(11) DEFAULT NULL,
  `ItemREF` int(11) DEFAULT NULL,
  `QualLVL` float(7,3) DEFAULT NULL,
  `Damage` float(7,3) DEFAULT NULL,
  `Power` float(7,3) DEFAULT NULL,
  `Ammo` float(7,3) DEFAULT NULL,
  `RecoilHeight` float(7,3) DEFAULT NULL,
  `RecoilDistance` float(7,3) DEFAULT NULL,
  `FireRate` float(7,3) DEFAULT NULL,
  `Accuracy` float(7,3) DEFAULT NULL,
  `BulletCNT` int(11) DEFAULT NULL,
  `ScopeTYPE` int(11) DEFAULT NULL,
  `ScopeLOCX` float(7,3) DEFAULT NULL,
  `ScopeLOCY` float(7,3) DEFAULT NULL,
  `ScopeLOCZ` float(7,3) DEFAULT NULL,
  `LaserTYPE` int(11) DEFAULT NULL,
  `LaserLOCX` float(7,3) DEFAULT NULL,
  `LaserLOCY` float(7,3) DEFAULT NULL,
  `LaserLOCZ` float(7,3) DEFAULT NULL,
  `QAS_Fire` int(11) DEFAULT NULL,
  `QAS_ReloadBoltOut` int(11) DEFAULT NULL,
  `QAS_ReloadBoltIn` int(11) DEFAULT NULL,
  `QAS_ReloadClipOut` int(11) DEFAULT NULL,
  `QAS_ReloadClipIn` int(11) DEFAULT NULL,
  `ArmourFACTOR` float(7,3) DEFAULT NULL,
  `ArmourAMOUNT` float(7,3) DEFAULT NULL,
  `ArmourREGEN` float(7,3) DEFAULT NULL,
  `ExpandSTRENGTH` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_NoWEP` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_TimeScale` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Gravity` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_LifeINC` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_EnergyINC` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_IntBoost` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_EnergyEff` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Height` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Weight` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Strength` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Quickness` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Dexterity` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_Constitution` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_AggPower` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_DefPower` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_RArmStr` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_LArmStr` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_RLegStr` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_LLegStr` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_RArmSnap` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_LArmSnap` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_RLegSnap` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_LLegSnap` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_WEPAccuracy` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_WEPDrawSPD` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_WEPReloadSPD` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_WEPRecoilHR` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_JINKmovespeed` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_JINKpower` float(7,3) DEFAULT NULL,
  `QA_ITEMINC_JINK3intensity` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `effectobject`;

CREATE TABLE `effectobject` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` int(11) DEFAULT NULL,
  `ArrayID` int(11) DEFAULT NULL,
  `TYPE` float(7,3) DEFAULT NULL,
  `INT` float(7,3) DEFAULT NULL,
  `ON` int(11) DEFAULT NULL,
  `Red` float(7,3) DEFAULT NULL,
  `Green` float(7,3) DEFAULT NULL,
  `Blue` float(7,3) DEFAULT NULL,
  `Alpha` float(7,3) DEFAULT NULL,
  `Scale` float(7,3) DEFAULT NULL,
  `TIMER` float(7,3) DEFAULT NULL,
  `CNT` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `hesh`;

CREATE TABLE `hesh` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `adindex1` int(11) DEFAULT NULL,
  `adindex2` int(11) DEFAULT NULL,
  `adindex3` int(11) DEFAULT NULL,
  `adindex4` int(11) DEFAULT NULL,
  `adindex5` int(11) DEFAULT NULL,
  `adindex6` int(11) DEFAULT NULL,
  `adindex7` int(11) DEFAULT NULL,
  `adindex8` int(11) DEFAULT NULL,
  `adindex9` int(11) DEFAULT NULL,
  `adindex10` int(11) DEFAULT NULL,
  `adindex11` int(11) DEFAULT NULL,
  `adindex12` int(11) DEFAULT NULL,
  `adindex13` int(11) DEFAULT NULL,
  `adindex14` int(11) DEFAULT NULL,
  `adindex15` int(11) DEFAULT NULL,
  `adindex16` int(11) DEFAULT NULL,
  `adindex17` int(11) DEFAULT NULL,
  `adindex18` int(11) DEFAULT NULL,
  `adindex19` int(11) DEFAULT NULL,
  `adindex20` int(11) DEFAULT NULL,
  `adindex21` int(11) DEFAULT NULL,
  `adindex22` int(11) DEFAULT NULL,
  `adindex23` int(11) DEFAULT NULL,
  `adindex24` int(11) DEFAULT NULL,
  `adindex25` int(11) DEFAULT NULL,
  `adindex26` int(11) DEFAULT NULL,
  `mark` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `blockid`  int(11) DEFAULT NULL,
  `shareid`  int(11) DEFAULT NULL,
  `jobid` int(11) DEFAULT NULL,
  `eslid` int(11) DEFAULT NULL,
  `targetid` int(11) DEFAULT NULL,
  `eslcode` varchar(32) DEFAULT NULL,
  `difficulty` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE hesh ADD CONSTRAINT hesh_unique UNIQUE (blockid);

DROP TABLE IF EXISTS `address`;

CREATE TABLE `address` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `address` varchar(512) DEFAULT NULL,
  `amt` float(12,11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `eslstore`;

CREATE TABLE `eslstore` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `eslstoreid` int(11) DEFAULT NULL,
  `contractid` int(11) DEFAULT NULL,
  `content` varchar(65535) DEFAULT NULL,
  `hashcode` varchar(32) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `targetstore`;

CREATE TABLE `targetstore` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `targetstoreid` int(11) DEFAULT NULL,
  `content` varchar(65535) DEFAULT NULL,
  `hashcode` varchar(32) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;