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
  `vert1x` float(12,8) DEFAULT NULL,
  `vert1y` float(12,8) DEFAULT NULL,
  `vert1z` float(12,8) DEFAULT NULL,
  `vert2x` float(12,8) DEFAULT NULL,
  `vert2y` float(12,8) DEFAULT NULL,
  `vert2z` float(12,8) DEFAULT NULL,
  `vert3x` float(12,8) DEFAULT NULL,
  `vert3y` float(12,8) DEFAULT NULL,
  `vert3z` float(12,8) DEFAULT NULL,
  `vert4x` float(12,8) DEFAULT NULL,
  `vert4y` float(12,8) DEFAULT NULL,
  `vert4z` float(12,8) DEFAULT NULL,
  `vert5x` float(12,8) DEFAULT NULL,
  `vert5y` float(12,8) DEFAULT NULL,
  `vert5z` float(12,8) DEFAULT NULL,
  `vert6x` float(12,8) DEFAULT NULL,
  `vert6y` float(12,8) DEFAULT NULL,
  `vert6z` float(12,8) DEFAULT NULL,
  `vert7x` float(12,8) DEFAULT NULL,
  `vert7y` float(12,8) DEFAULT NULL,
  `vert7z` float(12,8) DEFAULT NULL,
  `vert8x` float(12,8) DEFAULT NULL,
  `vert8y` float(12,8) DEFAULT NULL,
  `vert8z` float(12,8) DEFAULT NULL,
  `vert1r` float(12,8) DEFAULT NULL,
  `vert1g` float(12,8) DEFAULT NULL,
  `vert1b` float(12,8) DEFAULT NULL,
  `vert2r` float(12,8) DEFAULT NULL,
  `vert2g` float(12,8) DEFAULT NULL,
  `vert2b` float(12,8) DEFAULT NULL,
  `vert3r` float(12,8) DEFAULT NULL,
  `vert3g` float(12,8) DEFAULT NULL,
  `vert3b` float(12,8) DEFAULT NULL,
  `vert4r` float(12,8) DEFAULT NULL,
  `vert4g` float(12,8) DEFAULT NULL,
  `vert4b` float(12,8) DEFAULT NULL,
  `vert5r` float(12,8) DEFAULT NULL,
  `vert5g` float(12,8) DEFAULT NULL,
  `vert5b` float(12,8) DEFAULT NULL,
  `vert6r` float(12,8) DEFAULT NULL,
  `vert6g` float(12,8) DEFAULT NULL,
  `vert6b` float(12,8) DEFAULT NULL,
  `vert7r` float(12,8) DEFAULT NULL,
  `vert7g` float(12,8) DEFAULT NULL,
  `vert7b` float(12,8) DEFAULT NULL,
  `vert8r` float(12,8) DEFAULT NULL,
  `vert8g` float(12,8) DEFAULT NULL,
  `vert8b` float(12,8) DEFAULT NULL,
  `blockid` int(11) DEFAULT NULL,
  `shareid`  int(11) DEFAULT NULL,
  `jobid` int(11) DEFAULT NULL,
  `eslid` int(11) DEFAULT NULL,
  `targetid` int(11) DEFAULT NULL,
  `eslcode` varchar(32) DEFAULT NULL,
  `difficulty` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `hesh`;

CREATE TABLE `hesh` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
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

DROP TABLE IF EXISTS `adindex`;

CREATE TABLE `adindex` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `adindex` int(11) DEFAULT NULL,
  `cubeindex` int(11) DEFAULT NULL,
  `heshid` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `address`;

CREATE TABLE `address` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `owner` varchar(64) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
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