-- MySQL dump 10.13  Distrib 5.1.41, for debian-linux-gnu (i486)
--
-- Host: localhost    Database: drupal79
-- ------------------------------------------------------
-- Server version	5.1.41-3ubuntu12.10-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `marc_structure_types`
--

LOCK TABLES `opac_marc_structure_types` WRITE;
/*!40000 ALTER TABLE `opac_marc_structure_types` DISABLE KEYS */;
INSERT INTO `opac_marc_structure_types` VALUES ('marc21','Marc 21'),('test','Test'),('unimarc','Unimarc');
/*!40000 ALTER TABLE `opac_marc_structure_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `marc_subfield_structure`
--

LOCK TABLES `opac_marc_subfield_structure` WRITE;
/*!40000 ALTER TABLE `opac_marc_subfield_structure` DISABLE KEYS */;
INSERT INTO `opac_marc_subfield_structure` VALUES ('010','@','unimarc','International Standard Book Number',0),('010','a','unimarc','Number (ISBN)',0),('010','b','unimarc','Qualification',0),('010','d','unimarc','Terms of Availability and/or Price',0),('010','z','unimarc','Erroneous ISBN',0),('011','a','unimarc','Number (ISSN)',0),('011','b','unimarc','Qualification',0),('100','a','unimarc','General Processing Data',0),('101','a','unimarc','Language of Text, Soundtrack etc.',0),('101','b','unimarc','Language of Intermediate Text when Item is Not Translated from Original',0),('101','c','unimarc','Language of Original Work',0),('101','d','unimarc','Language of Summary',0),('101','e','unimarc','Language of Contents Page',0),('101','f','unimarc','Language of Title Page if Different from Text',0),('101','g','unimarc','Language of Title Proper if Not First Language of Text, Soundtrack, etc.',0),('101','h','unimarc','Language of Libretto, etc.',0),('101','i','unimarc','Language of Accompanying Material (Other than Summaries, Abstracts or Librettos)',0),('101','j','unimarc','Language of Subtitles',0),('102','a','unimarc','Country of publication',0),('102','b','unimarc','Locality of publication',0),('105','a','unimarc','Monograph Coded Data',0),('200','5','unimarc','Institution to Which Field Applies',0),('200','a','unimarc','Title Proper',0),('200','b','unimarc','General Material Designation',0),('200','c','unimarc','Title Proper by Another Author',0),('200','d','unimarc','Parallel Title Proper',0),('200','f','unimarc','First Statement of Responsibility',0),('200','g','unimarc','Subsequent Statement of Responsibility',0),('200','h','unimarc','Number of a Part',0),('200','i','unimarc','Name of a Part',0),('200','v','unimarc','Volume Designation',0),('200','z','unimarc','Language of Parallel Title Proper',0),('205','a','unimarc','Edition Statement',0),('205','b','unimarc','Issue Statement',0),('210','a','unimarc','Place of Publication, Distribution, etc.',0),('210','b','unimarc','Address of Publisher, Distributor, etc.',0),('210','c','unimarc','Name of Publisher, Distributor, etc.',0),('210','d','unimarc','Date of Publication, Distribution, etc.',0),('210','e','unimarc','Place of Manufacture',0),('210','f','unimarc','Address of Manufacturer',0),('210','g','unimarc','Name of Manufacturer',0),('210','h','unimarc','Date of Manufacture',0),('215','a','unimarc','Specific Material Designation and Extent of Item',0),('215','c','unimarc','Other Physical Details',0),('215','d','unimarc','Dimensions',0),('215','e','unimarc','Accompanying Material',0),('225','a','unimarc','Series Title',0),('225','d','unimarc','Parallel Series Title',0),('225','e','unimarc','Other Title Information',0),('225','f','unimarc','Statement of Responsibility',0),('225','h','unimarc','Number of a Part',0),('225','i','unimarc','Name of a Part',0),('225','v','unimarc','Volume Designation',0),('225','x','unimarc','ISSN of Series',0),('225','z','unimarc','Language of Parallel Title',0),('320','a','unimarc','Text of Note',0),('606','2','unimarc','System Code',0),('606','3','unimarc','Authority Record Number',0),('606','a','unimarc','Entry Element',0),('606','j','unimarc','Form Subdivision',0),('606','x','unimarc','Topical Subdivision',0),('606','y','unimarc','Geographical Subdivision',0),('606','z','unimarc','Chronological Subdivision',0);
/*!40000 ALTER TABLE `opac_marc_subfield_structure` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-01-06 12:30:40
