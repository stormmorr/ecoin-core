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

DROP TABLE IF EXISTS `block`;

CREATE TABLE `block` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `mark` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `jobid` int(11) DEFAULT NULL,
  `eslid` int(11) DEFAULT NULL,
  `targetid` int(11) DEFAULT NULL,
  `shareid` int(11) DEFAULT NULL,
  `eslcode` varchar(32) DEFAULT NULL,
  `difficulty` float(7,3) DEFAULT NULL,
  `score` int(11) DEFAULT NULL,
  `maxvote` int(11) DEFAULT NULL,
  `blockledger` varchar(64) DEFAULT NULL,
  `jobledger` varchar(64) DEFAULT NULL,
  `owner` varchar(64) DEFAULT NULL,
  `shares` int(11) DEFAULT NULL,
  `bckred` float(9,7) DEFAULT NULL,
  `bckgreen` float(9,7) DEFAULT NULL,
  `bckblue` float(9,7) DEFAULT NULL,
  `reward` float(12,8) DEFAULT NULL,
  `pledge` float(12,8) DEFAULT NULL,
  `fee` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE block ADD CONSTRAINT block_unique UNIQUE (blockledger);

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;