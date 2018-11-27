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

DROP TABLE IF EXISTS `txin`;

CREATE TABLE `txin` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `transactionid` int(11) DEFAULT NULL,
  `vout` varchar(64) DEFAULT NULL,
  `pubkey` varchar(64) DEFAULT NULL,
  `vsign` varchar(142) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `txout`;

CREATE TABLE `txout` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `transactionid` int(11) DEFAULT NULL,
  `adrid` int(11) DEFAULT NULL,
  `owner` varchar(64) DEFAULT NULL,
  `pubkey` varchar(64) DEFAULT NULL,
  `txoutamt` float(12,8) DEFAULT NULL,
  `owneramt` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `transactions`;

CREATE TABLE `transactions` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `status` int(11) DEFAULT NULL,
  `locktime` int(11) DEFAULT NULL,
  `confirmation` int(11) DEFAULT NULL,
  `hdid` varchar(50) DEFAULT NULL,
  `owner` varchar(64) DEFAULT NULL,
  `shareid` int(11) DEFAULT NULL,
  `gnshareid` int(11) DEFAULT NULL,
  `jobid` int(11) DEFAULT NULL,
  `hash` varchar(64) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;