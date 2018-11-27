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

DROP TABLE IF EXISTS `miner`;

CREATE TABLE `miner` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `nickname` varchar(220) DEFAULT NULL,
  `network` varchar(220) DEFAULT NULL,
  `titlemsg` varchar(220) DEFAULT NULL,
  `networkmsg` varchar(2048) DEFAULT NULL,
  `tag` varchar(1024) DEFAULT NULL,
  `miner` varchar(220) DEFAULT NULL,
  `collapsed` int(11) DEFAULT NULL,
  `safemode` int(11) DEFAULT NULL,
  `display` int(11) DEFAULT NULL,
  `target` varchar(64) DEFAULT NULL,
  `shares` int(11) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `amt` float(12,11) DEFAULT NULL,
  `assetid` int(11) DEFAULT NULL,
  `assetofficeid` int(11) DEFAULT NULL,
  `nonceid` int(11) DEFAULT NULL,
  `blinds` int(11) DEFAULT NULL,
  `jobtime` int(11) DEFAULT NULL,
  `source` int(11) DEFAULT NULL,
  `info` varchar(64) DEFAULT NULL,
  `urlalpa` varchar(1024) DEFAULT NULL,
  `urlbeta` varchar(1024) DEFAULT NULL,
  `urlgama` varchar(1024) DEFAULT NULL,
  `resourceurlalpa` varchar(220) DEFAULT NULL,
  `resourceurlbeta` varchar(220) DEFAULT NULL,
  `resourceurlgama` varchar(220) DEFAULT NULL,
  `resourcetxtalpa` varchar(2048) DEFAULT NULL,
  `resourcetxtbeta` varchar(2048) DEFAULT NULL,
  `resourcetxtgama` varchar(2048) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `nonce`;

CREATE TABLE `nonce` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `tag` varchar(1024) DEFAULT NULL,
  `username` varchar(220) DEFAULT NULL,
  `nickname` varchar(220) DEFAULT NULL,
  `network` varchar(220) DEFAULT NULL,
  `hashes` int(11) DEFAULT NULL,
  `startrand` int(11) DEFAULT NULL,
  `blinds` int(11) DEFAULT NULL,
  `jobtime` int(11) DEFAULT NULL,
  `mergeassetid` int(11) DEFAULT NULL,
  `mergeassetofficeid` int(11) DEFAULT NULL,
  `mergeminerid` int(11) DEFAULT NULL,
  `results` int(11) DEFAULT NULL,
  `resultd` float(12,11) DEFAULT NULL,
  `progression` varchar(220) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `collection`;

CREATE TABLE `collection` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `tag` varchar(1024) DEFAULT NULL,
  `username` varchar(220) DEFAULT NULL,
  `nickname` varchar(220) DEFAULT NULL,
  `network` varchar(220) DEFAULT NULL,
  `title` varchar(1024) DEFAULT NULL,
  `sized` int(11) DEFAULT NULL,
  `typed` int(11) DEFAULT NULL,
  `results` int(11) DEFAULT NULL,
  `resultd` float(12,11) DEFAULT NULL,
  `progression` varchar(512) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `gridseed`;

CREATE TABLE `gridseed` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(220) DEFAULT NULL,
  `minerid` int(11) DEFAULT NULL,
  `collectionid` int(11) DEFAULT NULL,
  `gridx` int(11) DEFAULT NULL,
  `gridy` int(11) DEFAULT NULL,
  `dated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;