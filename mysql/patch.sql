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


# Dump of table exstance
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `exstance`;

CREATE TABLE `exstance` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) NOT NULL,
  `hash` varchar(800) DEFAULT NULL,
  `hashid` int(11) DEFAULT NULL,
  `startnonce` int(11) DEFAULT NULL,
  `finishnonce` int(11) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `modex` int(11) DEFAULT NULL,
  `hs` int(11) DEFAULT NULL,
  `userid` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `sharetype` int(11) DEFAULT NULL,
  `coinvalue` float(12,11) DEFAULT NULL,
  `remote` varchar(220) DEFAULT NULL,
  `forwarding` varchar(512) DEFAULT NULL,
  `solution` varchar(1024) DEFAULT NULL,
  `nonce` int(10) unsigned DEFAULT NULL,
  `target` varchar(512) DEFAULT NULL,
  `difficulty` float(7,3) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `getwork`;

CREATE TABLE `getwork` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `network` varchar(220) DEFAULT NULL,
  `workhash` varchar(800) DEFAULT NULL,
  `worktarget` varchar(800) DEFAULT NULL,
  `workmid` varchar(800) DEFAULT NULL,
  `workhash1` varchar(800) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `sharetype` int(11) DEFAULT NULL,
  `modex` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `solution` varchar(800) DEFAULT NULL,
  `nonce` int(10) unsigned DEFAULT NULL,
  `blinds` int(11) DEFAULT NULL,
  `source` int(11) DEFAULT NULL,
  `info` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;