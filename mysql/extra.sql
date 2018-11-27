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


# Dump of table extra exchange systems
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `buy`;

CREATE TABLE `buy` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `buypaypal` varchar(512) DEFAULT NULL,
  `coinname` varchar(512) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `tradeid` int(11) DEFAULT NULL,
  `groupname` varchar(512) DEFAULT NULL,
  `username` varchar(512) DEFAULT NULL,
  `price` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `tokenid` int(11) DEFAULT NULL,
  `tokenref` varchar(512) DEFAULT NULL,
  `tokenhash` varchar(512) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `sell`;

CREATE TABLE `sell` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `paypal` varchar(1024) DEFAULT NULL,
  `coinname` varchar(512) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `tradeid` int(11) DEFAULT NULL,
  `groupname` varchar(512) DEFAULT NULL,
  `username` varchar(512) DEFAULT NULL,
  `price` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `token`;

CREATE TABLE `token` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `coinname` varchar(512) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `buyid` int(11) DEFAULT NULL,
  `sellid` int(11) DEFAULT NULL,
  `fullfilled` int(11) DEFAULT NULL,
  `price` float(12,8) DEFAULT NULL,
  `amt` float(12,8) DEFAULT NULL,
  `receiveadr` varchar(512) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;