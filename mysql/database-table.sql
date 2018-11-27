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


# Dump of table worker
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `worker`;

CREATE TABLE `worker` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) NOT NULL,
  `email` varchar(220) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `hs` int(11) DEFAULT NULL,
  `nomcon` int(11) DEFAULT NULL,
  `auto` int(11) DEFAULT NULL,
  `autoweb` int(11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `asset`;

CREATE TABLE `asset` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(220) DEFAULT NULL,
  `algo` varchar(220) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `hs` int(11) DEFAULT NULL,
  `nomcon` int(11) DEFAULT NULL,
  `USD` float(9,8) DEFAULT NULL,
  `volume` float(9,8) DEFAULT NULL,
  `volumeUSD` float(9,8) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `shares` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `ip` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
