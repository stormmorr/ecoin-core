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


# Dump of table custom
# ------------------------------------------------------------

USE globalscapedb;

DROP TABLE IF EXISTS `backgrnd`;

CREATE TABLE `backgrnd` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `alternativeid` int(11) DEFAULT NULL,
  `src` varchar(2048) DEFAULT NULL,
  `width` int(11) DEFAULT NULL,
  `height` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `theme`;

CREATE TABLE `theme` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `nick` varchar(220) DEFAULT NULL,
  `uses` int(11) DEFAULT NULL,
  `vale` float(12,11) DEFAULT NULL,
  `maintitle` varchar(1024) DEFAULT NULL,
  `subtext` varchar(1024) DEFAULT NULL,
  `mainbackid` int(11) DEFAULT NULL,
  `subtitle` varchar(1024) DEFAULT NULL,
  `subtexted` varchar(1024) DEFAULT NULL,
  `subbackid` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `lanzar`;

CREATE TABLE `lanzar` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `targid` int(11) DEFAULT NULL,
  `edit` int(11) DEFAULT NULL,
  `edittxt` varchar(220) DEFAULT NULL,
  `addedit` int(11) DEFAULT NULL,
  `muledit` int(11) DEFAULT NULL,
  `ranedit` int(11) DEFAULT NULL,
  `rangemin` int(11) DEFAULT NULL,
  `rangemax` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;