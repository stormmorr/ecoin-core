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


# Dump of table trx
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `trx`;

CREATE TABLE `trx` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `network` varchar(512) DEFAULT NULL,
  `fromnettxt` varchar(220) DEFAULT NULL,
  `fromassetid` int(11) DEFAULT NULL,
  `fromassetofficeid` int(11) DEFAULT NULL,
  `frombodid` int(11) DEFAULT NULL,
  `fromminertxt` varchar(1024) DEFAULT NULL,
  `frombuyid` int(11) DEFAULT NULL,
  `fromaddr` int(11) DEFAULT NULL,
  `toaddr` varchar(2048) DEFAULT NULL,
  `toassetid` int(11) DEFAULT NULL,
  `toassetofficeid` int(11) DEFAULT NULL,
  `tohiyieldid` int(11) DEFAULT NULL,
  `toponziid` int(11) DEFAULT NULL,
  `tobitbotid` int(11) DEFAULT NULL,
  `sharecount` int(11) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `amtusd` float(12,8) DEFAULT NULL,
  `rate` float(12,8) DEFAULT NULL,
  `known` int(11) DEFAULT NULL,
  `confirmed` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `hiyield`;

CREATE TABLE `hiyield` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `title` varchar(220) DEFAULT NULL,
  `company` varchar(220) DEFAULT NULL,
  `website` varchar(1024) DEFAULT NULL,
  `policy` varchar(4096) DEFAULT NULL,
  `trade` varchar(4096) DEFAULT NULL,
  `activated` int(11) DEFAULT NULL,
  `confirmed` int(11) DEFAULT NULL,
  `verified` int(11) DEFAULT NULL,
  `results` int(11) DEFAULT NULL,
  `rate` float(12,8) DEFAULT NULL,
  `rateamt` float(12,8) DEFAULT NULL,
  `rateusd` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `amtusd` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `ponzi`;

CREATE TABLE `ponzi` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `title` varchar(220) DEFAULT NULL,
  `company` varchar(220) DEFAULT NULL,
  `website` varchar(1024) DEFAULT NULL,
  `policy` varchar(4096) DEFAULT NULL,
  `trade` varchar(4096) DEFAULT NULL,
  `activated` int(11) DEFAULT NULL,
  `confirmed` int(11) DEFAULT NULL,
  `verified` int(11) DEFAULT NULL,
  `results` int(11) DEFAULT NULL,
  `rateperc` float(12,8) DEFAULT NULL,
  `rateamt` float(12,8) DEFAULT NULL,
  `rateusd` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `amtusd` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `bitbot`;

CREATE TABLE `bitbot` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `title` varchar(220) DEFAULT NULL,
  `company` varchar(220) DEFAULT NULL,
  `website` varchar(1024) DEFAULT NULL,
  `policy` varchar(4096) DEFAULT NULL,
  `trade` varchar(4096) DEFAULT NULL,
  `activated` int(11) DEFAULT NULL,
  `confirmed` int(11) DEFAULT NULL,
  `verified` int(11) DEFAULT NULL,
  `results` int(11) DEFAULT NULL,
  `rateperc` float(12,8) DEFAULT NULL,
  `rateamt` float(12,8) DEFAULT NULL,
  `rateusd` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `amtusd` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;