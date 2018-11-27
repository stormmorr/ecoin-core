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

DROP TABLE IF EXISTS `addressscratch`;

CREATE TABLE `addressscratch` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) NOT NULL,
  `tag` varchar(3) DEFAULT NULL,
  `nickname` varchar(3) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `addr` varchar(64) DEFAULT NULL,
  `algo` varchar(220) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `hs` int(11) DEFAULT NULL,
  `sharecount` int(11) DEFAULT NULL,
  `blocks` int(11) DEFAULT NULL,
  `amt` float(12,11) DEFAULT NULL,
  `amtUSD` float(12,11) DEFAULT NULL,
  `oneBTC` float(12,11) DEFAULT NULL,
  `oneUSD` float(12,11) DEFAULT NULL,
  `volume` float(12,11) DEFAULT NULL,
  `volumeUSD` float(12,11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE addressscratch ADD CONSTRAINT addresss_unique UNIQUE (addr);

DROP TABLE IF EXISTS `address`;

CREATE TABLE `address` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) NOT NULL,
  `tag` varchar(3) DEFAULT NULL,
  `nickname` varchar(3) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `addr` varchar(64) DEFAULT NULL,
  `algo` varchar(220) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `hs` int(11) DEFAULT NULL,
  `sharecount` int(11) DEFAULT NULL,
  `blocks` int(11) DEFAULT NULL,
  `amt` float(12,11) DEFAULT NULL,
  `amtUSD` float(12,11) DEFAULT NULL,
  `oneBTC` float(12,11) DEFAULT NULL,
  `oneUSD` float(12,11) DEFAULT NULL,
  `volume` float(12,11) DEFAULT NULL,
  `volumeUSD` float(12,11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE address ADD CONSTRAINT address_unique UNIQUE (addr);

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;