-- MySQL dump 10.13  Distrib 5.6.17, for osx10.9 (x86_64)
--
-- Host: localhost    Database: devops_survey
-- ------------------------------------------------------
-- Server version	5.6.17

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
-- Table structure for table `answers`
--

DROP TABLE IF EXISTS `answers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `answers` (
  `qid` int(11) NOT NULL DEFAULT '0',
  `code` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `answer` text COLLATE utf8_unicode_ci NOT NULL,
  `sortorder` int(11) NOT NULL,
  `assessment_value` int(11) NOT NULL DEFAULT '0',
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `scale_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`qid`,`code`,`language`,`scale_id`),
  KEY `answers_idx2` (`sortorder`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `answers`
--

LOCK TABLES `answers` WRITE;
/*!40000 ALTER TABLE `answers` DISABLE KEYS */;
INSERT INTO `answers` VALUES (3,'A5','There are many layers of management in between, by the time they get our message it has been severely scrambled',5,1,'en',0),(3,'A4','They are on the other side of the world in a different timezone,  not that much',4,1,'en',0),(3,'A2','Weekly',2,1,'en',0),(3,'A3','They sit next to me... we chat  all the time',3,1,'en',0),(3,'A1','I am the other roles',1,0,'en',0),(8,'A2','Hosting Partner',2,1,'en',0),(8,'A3','Webmaster ',3,1,'en',0),(7,'A3','IT (technical people getting content from others)',3,1,'en',0),(7,'A2','Dedicated Webmaster ',2,1,'en',0),(7,'A1','Marketing  (non technical) ',1,0,'en',0),(30,'5','Monthly ',5,1,'en',0),(30,'4','Weekly',4,1,'en',0),(30,'3','A couple of times per week',3,1,'en',0),(5,'A1','1-5',1,0,'en',0),(5,'A2','6-20',2,1,'en',0),(5,'A3','21-100',3,1,'en',0),(5,'A4','101-1000',4,1,'en',0),(5,'A5','1000+',5,1,'en',0),(4,'A5','Love to .. but  I`m not allowed ',4,1,'en',0),(4,'A4','Love to .. but don\'t know where to start ',3,1,'en',0),(4,'A3','I got pulled into a devops mentality',2,1,'en',0),(4,'A2','I started devops internally',1,1,'en',0),(32,'A2','Yes, sometimes',2,1,'en',0),(32,'A1','Yes, always',1,0,'en',0),(10,'A1','Yes',1,0,'en',0),(10,'A2','Core Only',2,1,'en',0),(10,'A3','Some trivial modules',3,1,'en',0),(10,'A4','Not at all',4,1,'en',0),(12,'A1','Yes, for everything',1,0,'en',0),(13,'A3','svn',2,1,'en',0),(13,'A4','mercurial',3,1,'en',0),(15,'A1','On a development platform',1,0,'en',0),(15,'A2','On your personal workstation',2,1,'en',0),(15,'A3','Live in production',3,1,'en',0),(15,'A4','On a staging platform',4,1,'en',0),(16,'A3','Staging, then production ',3,1,'en',0),(16,'A2','Live in production ',2,1,'en',0),(16,'A1','Development first, then testing, then production ',1,0,'en',0),(17,'A1','From the webgui',1,0,'en',0),(17,'A2','Configuration is in our code',2,1,'en',0),(17,'A3','We use features',3,1,'en',0),(18,'A1','No',1,0,'en',0),(18,'A2','Jenkins / Hudson',2,1,'en',0),(18,'A3','CruiseControl',3,1,'en',0),(18,'A4','Bamboo',4,1,'en',0),(18,'A5','Other',5,1,'en',0),(19,'A3','Master Master replication ',3,1,'en',0),(19,'A2','Master with multiple read only slaves',2,1,'en',0),(19,'A1','Single Instance',1,0,'en',0),(30,'1','Multiple Times per day',1,1,'en',0),(22,'A1','None',4,0,'en',0),(22,'A2','Solr',2,1,'en',0),(22,'A3','Elastic Search ',3,1,'en',0),(22,'A4','Basic Drupal Search',1,1,'en',0),(23,'A4','Heavy use of features ',5,1,'en',0),(12,'A2','Yes, but only for our own modules',2,1,'en',0),(23,'A1','Launch browser and connect to site to make config changes (fully manual) ',1,0,'en',0),(23,'A9','Partly automated (with drush)  but lots of manual work',2,1,'en',0),(23,'A3','A drupal Profile',4,1,'en',0),(23,'A2','Database Import ',3,1,'en',0),(30,'2','Daily',2,1,'en',0),(13,'A2','cvs',1,1,'en',0),(13,'A5','git',4,1,'en',0),(13,'A1','None',5,1,'en',0),(35,'2','Staging first, then it gets promoted to production',2,1,'en',0),(35,'1','Dedicated Content Platform',1,1,'en',0),(8,'A1','Internal IT / System Administrator',1,0,'en',0),(3,'A6','We are not allowed to talk to them',6,1,'en',0),(8,'A4','Nobody',4,1,'en',0),(35,'3','In production ',3,1,'en',0),(12,'A3','No',3,1,'en',0),(16,'A4','Critical Fixes ?',4,1,'en',0),(19,'A4','MySQL Proxy ',4,1,'en',0),(32,'A3','No',3,1,'en',0),(105,'A5','There are many layers of management in between, by the time they get our message it has been severely scrambled',5,1,'en',0),(105,'A4','They are on the other side of the world in a different timezone,  not that much',4,1,'en',0),(105,'A2','Weekly',2,1,'en',0),(105,'A3','They sit next to me... we chat  all the time',3,1,'en',0),(105,'A1','I am the other roles',1,0,'en',0),(110,'A2','Hosting Partner',2,1,'en',0),(110,'A3','Webmaster ',3,1,'en',0),(109,'A3','IT (technical people getting content from others)',3,1,'en',0),(109,'A2','Dedicated Webmaster ',2,1,'en',0),(109,'A1','Marketing  (non technical) ',1,0,'en',0),(132,'5','Monthly ',5,1,'en',0),(132,'4','Weekly',4,1,'en',0),(132,'3','A couple of times per week',3,1,'en',0),(107,'A1','1-5',1,0,'en',0),(107,'A2','6-20',2,1,'en',0),(107,'A3','21-100',3,1,'en',0),(107,'A4','101-1000',4,1,'en',0),(107,'A5','1000+',5,1,'en',0),(106,'A1','I was at devopsdays',1,0,'en',0),(106,'A2','I started devops internally',2,1,'en',0),(106,'A3','I got pulled into a devops mentality',3,1,'en',0),(106,'A4','Love to .. but don\'t know where to start ',4,1,'en',0),(106,'A5','Love to .. but  I`m not allowed ',5,1,'en',0),(106,'A6','No, why should I ?',6,1,'en',0),(134,'A2','Yes, sometimes',2,1,'en',0),(134,'A1','Yes, always',1,0,'en',0),(112,'A1','Yes',1,0,'en',0),(112,'A2','Core Only',2,1,'en',0),(112,'A3','Some trivial modules',3,1,'en',0),(112,'A4','Not at all',4,1,'en',0),(114,'A1','Yes, for everything',1,0,'en',0),(115,'A3','svn',2,1,'en',0),(115,'A4','mercurial',3,1,'en',0),(117,'A1','On a development platform',1,0,'en',0),(117,'A2','On your personal workstation',2,1,'en',0),(117,'A3','Live in production',3,1,'en',0),(117,'A4','On a staging platform',4,1,'en',0),(118,'A3','Staging, then production ',3,1,'en',0),(118,'A2','Live in production ',2,1,'en',0),(118,'A1','Development first, then testing, then production ',1,0,'en',0),(119,'A1','From the webgui',1,0,'en',0),(119,'A2','Configuration is in our code',2,1,'en',0),(119,'A3','We use features',3,1,'en',0),(120,'A1','No',1,0,'en',0),(120,'A2','Jenkins / Hudson',2,1,'en',0),(120,'A3','CruiseControl',3,1,'en',0),(120,'A4','Bamboo',4,1,'en',0),(120,'A5','Other',5,1,'en',0),(121,'A3','Master Master replication ',3,1,'en',0),(121,'A2','Master with multiple read only slaves',2,1,'en',0),(121,'A1','Single Instance',1,0,'en',0),(132,'1','Multiple Times per day',1,1,'en',0),(124,'A1','None',4,0,'en',0),(124,'A2','Solr',2,1,'en',0),(124,'A3','Elastic Search ',3,1,'en',0),(124,'A4','Basic Drupal Search',1,1,'en',0),(125,'A4','Heavy use of features ',5,1,'en',0),(114,'A2','Yes, but only for our own modules',2,1,'en',0),(125,'A1','Launch browser and connect to site to make config changes (fully manual) ',1,0,'en',0),(125,'A9','Partly automated (with drush)  but lots of manual work',2,1,'en',0),(125,'A3','A drupal Profile',4,1,'en',0),(125,'A2','Database Import ',3,1,'en',0),(132,'2','Daily',2,1,'en',0),(115,'A2','cvs',1,1,'en',0),(115,'A5','git',4,1,'en',0),(115,'A1','None',5,1,'en',0),(137,'2','Staging first, then it gets promoted to production',2,1,'en',0),(137,'1','Dedicated Content Platform',1,1,'en',0),(110,'A1','Internal IT / System Administrator',1,0,'en',0),(105,'A6','We are not allowed to talk to them',6,1,'en',0),(110,'A4','Nobody',4,1,'en',0),(137,'3','In production ',3,1,'en',0),(114,'A3','No',3,1,'en',0),(118,'A4','Critical Fixes ?',4,1,'en',0),(121,'A4','MySQL Proxy ',4,1,'en',0),(134,'A3','No',3,1,'en',0),(200,'A1','IT Operations',1,0,'en',0),(200,'A2','Development/Engineering',2,1,'en',0),(200,'A3','QA/Other',3,1,'en',0),(204,'A1','Administrator or Engineer',1,0,'en',0),(204,'A2','Manager or C/Level',2,1,'en',0),(204,'A3','Consultant',3,1,'en',0),(209,'A1','Minutes',1,0,'en',0),(209,'A2','Hours',2,1,'en',0),(209,'A3','Days',3,1,'en',0),(209,'A4','Longer',4,1,'en',0),(4,'A6','No, why should I ?',5,1,'en',0),(216,'A6','Whatever Drupal Core does.',6,1,'en',0),(216,'A5','75-100%',5,1,'en',0),(216,'A4','50-75%',4,1,'en',0),(216,'A3','25-50%',3,1,'en',0),(216,'A2','1-25%',2,1,'en',0),(216,'A7','None',1,1,'en',0);
/*!40000 ALTER TABLE `answers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `assessments`
--

DROP TABLE IF EXISTS `assessments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `assessments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) NOT NULL DEFAULT '0',
  `scope` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `gid` int(11) NOT NULL DEFAULT '0',
  `name` text COLLATE utf8_unicode_ci NOT NULL,
  `minimum` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `maximum` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `message` text COLLATE utf8_unicode_ci NOT NULL,
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  PRIMARY KEY (`id`,`language`),
  KEY `assessments_idx2` (`sid`),
  KEY `assessments_idx3` (`gid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `assessments`
--

LOCK TABLES `assessments` WRITE;
/*!40000 ALTER TABLE `assessments` DISABLE KEYS */;
/*!40000 ALTER TABLE `assessments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `conditions`
--

DROP TABLE IF EXISTS `conditions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `conditions` (
  `cid` int(11) NOT NULL AUTO_INCREMENT,
  `qid` int(11) NOT NULL DEFAULT '0',
  `cqid` int(11) NOT NULL DEFAULT '0',
  `cfieldname` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `method` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `value` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `scenario` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`cid`),
  KEY `conditions_idx2` (`qid`),
  KEY `conditions_idx3` (`cqid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `conditions`
--

LOCK TABLES `conditions` WRITE;
/*!40000 ALTER TABLE `conditions` DISABLE KEYS */;
/*!40000 ALTER TABLE `conditions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `defaultvalues`
--

DROP TABLE IF EXISTS `defaultvalues`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `defaultvalues` (
  `qid` int(11) NOT NULL DEFAULT '0',
  `scale_id` int(11) NOT NULL DEFAULT '0',
  `sqid` int(11) NOT NULL DEFAULT '0',
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `specialtype` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `defaultvalue` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`qid`,`specialtype`,`language`,`scale_id`,`sqid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `defaultvalues`
--

LOCK TABLES `defaultvalues` WRITE;
/*!40000 ALTER TABLE `defaultvalues` DISABLE KEYS */;
/*!40000 ALTER TABLE `defaultvalues` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `expression_errors`
--

DROP TABLE IF EXISTS `expression_errors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `expression_errors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `errortime` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `sid` int(11) DEFAULT NULL,
  `gid` int(11) DEFAULT NULL,
  `qid` int(11) DEFAULT NULL,
  `gseq` int(11) DEFAULT NULL,
  `qseq` int(11) DEFAULT NULL,
  `type` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `eqn` text COLLATE utf8_unicode_ci,
  `prettyprint` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `expression_errors`
--

LOCK TABLES `expression_errors` WRITE;
/*!40000 ALTER TABLE `expression_errors` DISABLE KEYS */;
/*!40000 ALTER TABLE `expression_errors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `failed_login_attempts`
--

DROP TABLE IF EXISTS `failed_login_attempts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `failed_login_attempts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `last_attempt` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `number_attempts` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `failed_login_attempts`
--

LOCK TABLES `failed_login_attempts` WRITE;
/*!40000 ALTER TABLE `failed_login_attempts` DISABLE KEYS */;
/*!40000 ALTER TABLE `failed_login_attempts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groups` (
  `gid` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) NOT NULL DEFAULT '0',
  `group_name` varchar(100) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `group_order` int(11) NOT NULL DEFAULT '0',
  `description` text COLLATE utf8_unicode_ci,
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `randomization_group` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `grelevance` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`gid`,`language`),
  KEY `groups_idx2` (`sid`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groups`
--

LOCK TABLES `groups` WRITE;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
INSERT INTO `groups` VALUES (1,81259,'About You',0,'We\'d like to know a bit about you, your role in the organisation and so on .. \n','en','',NULL),(2,81259,'About your organisation',1,'','en','',NULL),(3,81259,'About your site',2,'','en','',NULL),(4,81259,'About your site development ',3,'','en','',NULL),(5,81259,'About Drupal and your backends  and frontends ',4,'','en','',NULL),(6,81259,'About your Deployment',5,'How do you deploy a Drupal site ? ','en','',''),(7,81259,'Finishing',6,'','en','',NULL),(8,689125,'About You',0,'We\'d like to know a bit about you, your role in the organisation and so on .. \n','en','',NULL),(9,689125,'About your organisation',1,'','en','',NULL),(10,689125,'About your site',2,'','en','',NULL),(11,689125,'About your site development ',3,'','en','',NULL),(12,689125,'About Drupal and your backends  and frontends ',4,'','en','',NULL),(13,689125,'About your Deployment',5,'How do you deploy a Drupal site ? ','en','',NULL),(14,689125,'Finishing',6,'','en','',NULL);
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `labels`
--

DROP TABLE IF EXISTS `labels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `labels` (
  `lid` int(11) NOT NULL DEFAULT '0',
  `code` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `title` text COLLATE utf8_unicode_ci,
  `sortorder` int(11) NOT NULL,
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `assessment_value` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`lid`,`sortorder`,`language`),
  KEY `labels_code_idx` (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `labels`
--

LOCK TABLES `labels` WRITE;
/*!40000 ALTER TABLE `labels` DISABLE KEYS */;
/*!40000 ALTER TABLE `labels` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `labelsets`
--

DROP TABLE IF EXISTS `labelsets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `labelsets` (
  `lid` int(11) NOT NULL AUTO_INCREMENT,
  `label_name` varchar(100) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `languages` varchar(200) COLLATE utf8_unicode_ci DEFAULT 'en',
  PRIMARY KEY (`lid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `labelsets`
--

LOCK TABLES `labelsets` WRITE;
/*!40000 ALTER TABLE `labelsets` DISABLE KEYS */;
/*!40000 ALTER TABLE `labelsets` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participant_attribute`
--

DROP TABLE IF EXISTS `participant_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participant_attribute` (
  `participant_id` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `attribute_id` int(11) NOT NULL,
  `value` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`participant_id`,`attribute_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participant_attribute`
--

LOCK TABLES `participant_attribute` WRITE;
/*!40000 ALTER TABLE `participant_attribute` DISABLE KEYS */;
/*!40000 ALTER TABLE `participant_attribute` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participant_attribute_names`
--

DROP TABLE IF EXISTS `participant_attribute_names`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participant_attribute_names` (
  `attribute_id` int(11) NOT NULL AUTO_INCREMENT,
  `attribute_type` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `defaultname` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `visible` varchar(5) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`attribute_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participant_attribute_names`
--

LOCK TABLES `participant_attribute_names` WRITE;
/*!40000 ALTER TABLE `participant_attribute_names` DISABLE KEYS */;
/*!40000 ALTER TABLE `participant_attribute_names` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participant_attribute_names_lang`
--

DROP TABLE IF EXISTS `participant_attribute_names_lang`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participant_attribute_names_lang` (
  `attribute_id` int(11) NOT NULL,
  `attribute_name` varchar(30) COLLATE utf8_unicode_ci NOT NULL,
  `lang` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`attribute_id`,`lang`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participant_attribute_names_lang`
--

LOCK TABLES `participant_attribute_names_lang` WRITE;
/*!40000 ALTER TABLE `participant_attribute_names_lang` DISABLE KEYS */;
/*!40000 ALTER TABLE `participant_attribute_names_lang` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participant_attribute_values`
--

DROP TABLE IF EXISTS `participant_attribute_values`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participant_attribute_values` (
  `value_id` int(11) NOT NULL AUTO_INCREMENT,
  `attribute_id` int(11) NOT NULL,
  `value` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`value_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participant_attribute_values`
--

LOCK TABLES `participant_attribute_values` WRITE;
/*!40000 ALTER TABLE `participant_attribute_values` DISABLE KEYS */;
/*!40000 ALTER TABLE `participant_attribute_values` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participant_shares`
--

DROP TABLE IF EXISTS `participant_shares`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participant_shares` (
  `participant_id` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `share_uid` int(11) NOT NULL,
  `date_added` datetime NOT NULL,
  `can_edit` varchar(5) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`participant_id`,`share_uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participant_shares`
--

LOCK TABLES `participant_shares` WRITE;
/*!40000 ALTER TABLE `participant_shares` DISABLE KEYS */;
/*!40000 ALTER TABLE `participant_shares` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `participants`
--

DROP TABLE IF EXISTS `participants`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `participants` (
  `participant_id` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `firstname` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `lastname` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(254) COLLATE utf8_unicode_ci DEFAULT NULL,
  `language` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `blacklisted` varchar(1) COLLATE utf8_unicode_ci NOT NULL,
  `owner_uid` int(11) NOT NULL,
  `created_by` int(11) NOT NULL,
  `created` datetime DEFAULT NULL,
  `modified` datetime DEFAULT NULL,
  PRIMARY KEY (`participant_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `participants`
--

LOCK TABLES `participants` WRITE;
/*!40000 ALTER TABLE `participants` DISABLE KEYS */;
/*!40000 ALTER TABLE `participants` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `permissions`
--

DROP TABLE IF EXISTS `permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `entity` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `entity_id` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `permission` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
  `create_p` int(11) NOT NULL DEFAULT '0',
  `read_p` int(11) NOT NULL DEFAULT '0',
  `update_p` int(11) NOT NULL DEFAULT '0',
  `delete_p` int(11) NOT NULL DEFAULT '0',
  `import_p` int(11) NOT NULL DEFAULT '0',
  `export_p` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idxPermissions` (`entity_id`,`entity`,`permission`,`uid`)
) ENGINE=MyISAM AUTO_INCREMENT=56 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `permissions`
--

LOCK TABLES `permissions` WRITE;
/*!40000 ALTER TABLE `permissions` DISABLE KEYS */;
INSERT INTO `permissions` VALUES (1,'global',0,1,'superadmin',0,1,0,0,0,0),(2,'survey',81259,1,'assessments',1,1,1,1,0,0),(3,'survey',81259,1,'translations',0,1,1,0,0,0),(4,'survey',81259,1,'quotas',1,1,1,1,0,0),(5,'survey',81259,1,'responses',1,1,1,1,1,1),(6,'survey',81259,1,'statistics',0,1,0,0,0,0),(7,'survey',81259,1,'surveyactivation',0,0,1,0,0,0),(8,'survey',81259,1,'surveycontent',1,1,1,1,1,1),(9,'survey',81259,1,'survey',0,1,0,1,0,0),(10,'survey',81259,1,'surveylocale',0,1,1,0,0,0),(11,'survey',81259,1,'surveysecurity',1,1,1,1,0,0),(12,'survey',81259,1,'surveysettings',0,1,1,0,0,0),(13,'survey',81259,1,'tokens',1,1,1,1,1,1),(15,'global',0,2,'labelsets',1,1,1,1,1,1),(16,'global',0,2,'participantpanel',1,1,1,1,0,1),(17,'global',0,2,'settings',0,1,1,0,1,0),(18,'global',0,2,'surveys',1,1,1,1,0,1),(19,'global',0,2,'templates',1,1,1,1,1,1),(20,'global',0,2,'usergroups',1,1,1,1,0,0),(21,'global',0,2,'users',1,1,1,1,0,0),(23,'global',0,3,'labelsets',1,1,1,1,1,1),(24,'global',0,3,'participantpanel',1,1,1,1,0,1),(25,'global',0,3,'settings',0,1,1,0,1,0),(26,'global',0,3,'surveys',1,1,1,1,0,1),(27,'global',0,3,'templates',1,1,1,1,1,1),(28,'survey',689125,1,'assessments',1,1,1,1,0,0),(29,'survey',689125,1,'translations',0,1,1,0,0,0),(30,'survey',689125,1,'quotas',1,1,1,1,0,0),(31,'survey',689125,1,'responses',1,1,1,1,1,1),(32,'survey',689125,1,'statistics',0,1,0,0,0,0),(33,'survey',689125,1,'surveyactivation',0,0,1,0,0,0),(34,'survey',689125,1,'surveycontent',1,1,1,1,1,1),(35,'survey',689125,1,'survey',0,1,0,1,0,0),(36,'survey',689125,1,'surveylocale',0,1,1,0,0,0),(37,'survey',689125,1,'surveysecurity',1,1,1,1,0,0),(38,'survey',689125,1,'surveysettings',0,1,1,0,0,0),(39,'survey',689125,1,'tokens',1,1,1,1,1,1),(41,'survey',81259,3,'assessments',1,1,1,1,0,0),(42,'survey',81259,3,'quotas',1,1,1,1,0,0),(43,'survey',81259,3,'responses',1,1,1,1,1,1),(44,'survey',81259,3,'statistics',0,1,0,0,0,0),(45,'survey',81259,3,'surveycontent',1,1,1,1,1,1),(46,'survey',81259,3,'survey',0,1,0,0,0,0),(48,'survey',81259,2,'assessments',1,1,1,1,0,0),(49,'survey',81259,2,'quotas',1,1,1,1,0,0),(50,'survey',81259,2,'responses',1,1,1,1,1,1),(51,'survey',81259,2,'statistics',0,1,0,0,0,0),(52,'survey',81259,2,'surveycontent',1,1,1,1,1,1),(53,'survey',81259,2,'survey',0,1,0,0,0,0),(54,'survey',81259,2,'surveylocale',0,1,1,0,0,0),(55,'survey',81259,2,'surveysecurity',1,1,1,1,0,0);
/*!40000 ALTER TABLE `permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plugin_settings`
--

DROP TABLE IF EXISTS `plugin_settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plugin_settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `plugin_id` int(11) NOT NULL,
  `model` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `model_id` int(11) DEFAULT NULL,
  `key` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `value` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plugin_settings`
--

LOCK TABLES `plugin_settings` WRITE;
/*!40000 ALTER TABLE `plugin_settings` DISABLE KEYS */;
/*!40000 ALTER TABLE `plugin_settings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plugins`
--

DROP TABLE IF EXISTS `plugins`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plugins` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `active` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plugins`
--

LOCK TABLES `plugins` WRITE;
/*!40000 ALTER TABLE `plugins` DISABLE KEYS */;
INSERT INTO `plugins` VALUES (1,'Authdb',1);
/*!40000 ALTER TABLE `plugins` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `question_attributes`
--

DROP TABLE IF EXISTS `question_attributes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `question_attributes` (
  `qaid` int(11) NOT NULL AUTO_INCREMENT,
  `qid` int(11) NOT NULL DEFAULT '0',
  `attribute` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `value` text COLLATE utf8_unicode_ci,
  `language` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`qaid`),
  KEY `question_attributes_idx2` (`qid`),
  KEY `question_attributes_idx3` (`attribute`)
) ENGINE=MyISAM AUTO_INCREMENT=1481 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `question_attributes`
--

LOCK TABLES `question_attributes` WRITE;
/*!40000 ALTER TABLE `question_attributes` DISABLE KEYS */;
INSERT INTO `question_attributes` VALUES (1,1,'array_filter','',NULL),(2,1,'array_filter_exclude','',NULL),(3,1,'assessment_value','1',NULL),(4,1,'display_columns','1',NULL),(5,1,'exclude_all_others','',NULL),(6,1,'exclude_all_others_auto','0',NULL),(7,1,'hidden','0',NULL),(8,1,'hide_tip','0',NULL),(9,1,'max_answers','',NULL),(10,1,'min_answers','',NULL),(11,1,'other_numbers_only','0',NULL),(12,1,'other_replace_text','','en'),(13,1,'page_break','0',NULL),(14,1,'public_statistics','0',NULL),(15,1,'random_group','',NULL),(16,1,'random_order','0',NULL),(17,1,'scale_export','0',NULL),(18,2,'hidden','0',NULL),(19,2,'hide_tip','0',NULL),(20,2,'max_num_value_n','',NULL),(21,2,'maximum_chars','',NULL),(22,2,'min_num_value_n','',NULL),(23,2,'num_value_int_only','0',NULL),(24,2,'page_break','0',NULL),(25,2,'prefix','','en'),(26,2,'public_statistics','0',NULL),(27,2,'random_group','',NULL),(28,2,'suffix','','en'),(29,2,'text_input_width','',NULL),(30,3,'alphasort','0',NULL),(31,3,'array_filter','',NULL),(32,3,'array_filter_exclude','',NULL),(33,3,'display_columns','1',NULL),(34,3,'hidden','0',NULL),(35,3,'hide_tip','0',NULL),(36,3,'other_comment_mandatory','0',NULL),(37,3,'other_numbers_only','0',NULL),(38,3,'other_replace_text','','en'),(39,3,'page_break','0',NULL),(40,3,'public_statistics','0',NULL),(41,3,'random_group','',NULL),(42,3,'random_order','0',NULL),(43,3,'scale_export','0',NULL),(44,4,'alphasort','0',NULL),(45,4,'array_filter','',NULL),(46,4,'array_filter_exclude','',NULL),(47,4,'display_columns','1',NULL),(48,4,'hidden','0',NULL),(49,4,'hide_tip','0',NULL),(50,4,'other_comment_mandatory','0',NULL),(51,4,'other_numbers_only','0',NULL),(52,4,'other_replace_text','','en'),(53,4,'page_break','0',NULL),(54,4,'public_statistics','0',NULL),(55,4,'random_group','',NULL),(56,4,'random_order','0',NULL),(57,4,'scale_export','0',NULL),(58,5,'alphasort','0',NULL),(59,5,'array_filter','',NULL),(60,5,'array_filter_exclude','',NULL),(61,5,'display_columns','1',NULL),(62,5,'hidden','0',NULL),(63,5,'hide_tip','0',NULL),(64,5,'other_comment_mandatory','0',NULL),(65,5,'other_numbers_only','0',NULL),(66,5,'other_replace_text','','en'),(67,5,'page_break','0',NULL),(68,5,'public_statistics','0',NULL),(69,5,'random_group','',NULL),(70,5,'random_order','0',NULL),(71,5,'scale_export','0',NULL),(72,6,'array_filter','',NULL),(73,6,'array_filter_exclude','',NULL),(74,6,'assessment_value','1',NULL),(75,6,'display_columns','1',NULL),(76,6,'exclude_all_others','',NULL),(77,6,'exclude_all_others_auto','0',NULL),(78,6,'hidden','0',NULL),(79,6,'hide_tip','0',NULL),(80,6,'max_answers','',NULL),(81,6,'min_answers','',NULL),(82,6,'other_numbers_only','0',NULL),(83,6,'other_replace_text','','en'),(84,6,'page_break','0',NULL),(85,6,'public_statistics','0',NULL),(86,6,'random_group','',NULL),(87,6,'random_order','0',NULL),(88,6,'scale_export','0',NULL),(89,7,'alphasort','0',NULL),(90,7,'array_filter','',NULL),(91,7,'array_filter_exclude','',NULL),(92,7,'display_columns','1',NULL),(93,7,'hidden','0',NULL),(94,7,'hide_tip','0',NULL),(95,7,'other_comment_mandatory','0',NULL),(96,7,'other_numbers_only','0',NULL),(97,7,'other_replace_text','','en'),(98,7,'page_break','0',NULL),(99,7,'public_statistics','0',NULL),(100,7,'random_group','',NULL),(101,7,'random_order','0',NULL),(102,7,'scale_export','0',NULL),(103,8,'alphasort','0',NULL),(104,8,'array_filter','',NULL),(105,8,'array_filter_exclude','',NULL),(106,8,'display_columns','1',NULL),(107,8,'hidden','0',NULL),(108,8,'hide_tip','0',NULL),(109,8,'other_comment_mandatory','0',NULL),(110,8,'other_numbers_only','0',NULL),(111,8,'other_replace_text','','en'),(112,8,'page_break','0',NULL),(113,8,'public_statistics','0',NULL),(114,8,'random_group','',NULL),(115,8,'random_order','0',NULL),(116,8,'scale_export','0',NULL),(117,9,'array_filter','',NULL),(118,9,'array_filter_exclude','',NULL),(119,9,'assessment_value','1',NULL),(120,9,'display_columns','1',NULL),(121,9,'exclude_all_others','',NULL),(122,9,'exclude_all_others_auto','0',NULL),(123,9,'hidden','0',NULL),(124,9,'hide_tip','0',NULL),(125,9,'max_answers','',NULL),(126,9,'min_answers','',NULL),(127,9,'other_numbers_only','0',NULL),(128,9,'other_replace_text','','en'),(129,9,'page_break','0',NULL),(130,9,'public_statistics','0',NULL),(131,9,'random_group','',NULL),(132,9,'random_order','0',NULL),(133,9,'scale_export','0',NULL),(134,10,'alphasort','0',NULL),(135,10,'array_filter','',NULL),(136,10,'array_filter_exclude','',NULL),(137,10,'display_columns','1',NULL),(138,10,'hidden','0',NULL),(139,10,'hide_tip','0',NULL),(140,10,'other_comment_mandatory','0',NULL),(141,10,'other_numbers_only','0',NULL),(142,10,'other_replace_text','','en'),(143,10,'page_break','0',NULL),(144,10,'public_statistics','0',NULL),(145,10,'random_group','',NULL),(146,10,'random_order','0',NULL),(147,10,'scale_export','0',NULL),(148,11,'hidden','0',NULL),(149,11,'page_break','0',NULL),(150,11,'public_statistics','0',NULL),(151,11,'random_group','',NULL),(152,11,'scale_export','0',NULL),(153,12,'alphasort','0',NULL),(154,12,'array_filter','',NULL),(155,12,'array_filter_exclude','',NULL),(156,12,'display_columns','1',NULL),(157,12,'hidden','0',NULL),(158,12,'hide_tip','0',NULL),(159,12,'other_comment_mandatory','0',NULL),(160,12,'other_numbers_only','0',NULL),(161,12,'other_replace_text','','en'),(162,12,'page_break','0',NULL),(163,12,'public_statistics','0',NULL),(164,12,'random_group','',NULL),(165,12,'random_order','0',NULL),(166,12,'scale_export','0',NULL),(167,13,'alphasort','0',NULL),(168,13,'array_filter','',NULL),(169,13,'array_filter_exclude','',NULL),(170,13,'display_columns','1',NULL),(171,13,'hidden','0',NULL),(172,13,'hide_tip','0',NULL),(173,13,'other_comment_mandatory','0',NULL),(174,13,'other_numbers_only','0',NULL),(175,13,'other_replace_text','','en'),(176,13,'page_break','0',NULL),(177,13,'public_statistics','0',NULL),(178,13,'random_group','',NULL),(179,13,'random_order','0',NULL),(180,13,'scale_export','0',NULL),(181,14,'array_filter','',NULL),(182,14,'array_filter_exclude','',NULL),(183,14,'assessment_value','1',NULL),(184,14,'display_columns','1',NULL),(185,14,'exclude_all_others','',NULL),(186,14,'exclude_all_others_auto','0',NULL),(187,14,'hidden','0',NULL),(188,14,'hide_tip','0',NULL),(189,14,'max_answers','',NULL),(190,14,'min_answers','',NULL),(191,14,'other_numbers_only','0',NULL),(192,14,'other_replace_text','','en'),(193,14,'page_break','0',NULL),(194,14,'public_statistics','0',NULL),(195,14,'random_group','',NULL),(196,14,'random_order','0',NULL),(197,14,'scale_export','0',NULL),(198,15,'alphasort','0',NULL),(199,15,'array_filter','',NULL),(200,15,'array_filter_exclude','',NULL),(201,15,'display_columns','1',NULL),(202,1,'array_filter_exclude','',NULL),(203,1,'assessment_value','1',NULL),(204,1,'display_columns','1',NULL),(205,1,'exclude_all_others','',NULL),(206,1,'exclude_all_others_auto','0',NULL),(207,1,'hidden','0',NULL),(208,1,'hide_tip','0',NULL),(209,1,'max_answers','',NULL),(210,1,'min_answers','',NULL),(211,1,'other_numbers_only','0',NULL),(212,1,'other_replace_text','','en'),(213,1,'page_break','0',NULL),(214,1,'public_statistics','0',NULL),(215,1,'random_group','',NULL),(216,1,'random_order','0',NULL),(217,1,'scale_export','0',NULL),(218,2,'hidden','0',NULL),(219,2,'hide_tip','0',NULL),(220,2,'max_num_value_n','',NULL),(221,2,'maximum_chars','',NULL),(222,2,'min_num_value_n','',NULL),(223,2,'num_value_int_only','0',NULL),(224,2,'page_break','0',NULL),(225,2,'prefix','','en'),(226,2,'public_statistics','0',NULL),(227,2,'random_group','',NULL),(228,2,'suffix','','en'),(229,2,'text_input_width','',NULL),(230,3,'alphasort','0',NULL),(231,3,'array_filter','',NULL),(232,3,'array_filter_exclude','',NULL),(233,3,'display_columns','1',NULL),(234,3,'hidden','0',NULL),(235,3,'hide_tip','0',NULL),(236,3,'other_comment_mandatory','0',NULL),(237,3,'other_numbers_only','0',NULL),(238,3,'other_replace_text','','en'),(239,3,'page_break','0',NULL),(240,3,'public_statistics','0',NULL),(241,3,'random_group','',NULL),(242,3,'random_order','0',NULL),(243,3,'scale_export','0',NULL),(244,4,'alphasort','0',NULL),(245,4,'array_filter','',NULL),(246,4,'array_filter_exclude','',NULL),(247,4,'display_columns','1',NULL),(248,4,'hidden','0',NULL),(249,4,'hide_tip','0',NULL),(250,4,'other_comment_mandatory','0',NULL),(251,4,'other_numbers_only','0',NULL),(252,4,'other_replace_text','','en'),(253,4,'page_break','0',NULL),(254,4,'public_statistics','0',NULL),(255,4,'random_group','',NULL),(256,4,'random_order','0',NULL),(257,4,'scale_export','0',NULL),(258,5,'alphasort','0',NULL),(259,5,'array_filter','',NULL),(260,5,'array_filter_exclude','',NULL),(261,5,'display_columns','1',NULL),(262,5,'hidden','0',NULL),(263,5,'hide_tip','0',NULL),(264,5,'other_comment_mandatory','0',NULL),(265,5,'other_numbers_only','0',NULL),(266,5,'other_replace_text','','en'),(267,5,'page_break','0',NULL),(268,5,'public_statistics','0',NULL),(269,5,'random_group','',NULL),(270,5,'random_order','0',NULL),(271,5,'scale_export','0',NULL),(272,6,'array_filter','',NULL),(273,6,'array_filter_exclude','',NULL),(274,6,'assessment_value','1',NULL),(275,6,'display_columns','1',NULL),(276,6,'exclude_all_others','',NULL),(277,6,'exclude_all_others_auto','0',NULL),(278,6,'hidden','0',NULL),(279,6,'hide_tip','0',NULL),(280,6,'max_answers','',NULL),(281,6,'min_answers','',NULL),(282,6,'other_numbers_only','0',NULL),(283,6,'other_replace_text','','en'),(284,6,'page_break','0',NULL),(285,6,'public_statistics','0',NULL),(286,6,'random_group','',NULL),(287,6,'random_order','0',NULL),(288,6,'scale_export','0',NULL),(289,7,'alphasort','0',NULL),(290,7,'array_filter','',NULL),(291,7,'array_filter_exclude','',NULL),(292,7,'display_columns','1',NULL),(293,7,'hidden','0',NULL),(294,7,'hide_tip','0',NULL),(295,7,'other_comment_mandatory','0',NULL),(296,7,'other_numbers_only','0',NULL),(297,7,'other_replace_text','','en'),(298,7,'page_break','0',NULL),(299,7,'public_statistics','0',NULL),(300,7,'random_group','',NULL),(301,7,'random_order','0',NULL),(302,7,'scale_export','0',NULL),(303,8,'alphasort','0',NULL),(304,8,'array_filter','',NULL),(305,8,'array_filter_exclude','',NULL),(306,8,'display_columns','1',NULL),(307,8,'hidden','0',NULL),(308,8,'hide_tip','0',NULL),(309,8,'other_comment_mandatory','0',NULL),(310,8,'other_numbers_only','0',NULL),(311,8,'other_replace_text','','en'),(312,8,'page_break','0',NULL),(313,8,'public_statistics','0',NULL),(314,8,'random_group','',NULL),(315,8,'random_order','0',NULL),(316,8,'scale_export','0',NULL),(317,9,'array_filter','',NULL),(318,9,'array_filter_exclude','',NULL),(319,9,'assessment_value','1',NULL),(320,9,'display_columns','1',NULL),(321,9,'exclude_all_others','',NULL),(322,9,'exclude_all_others_auto','0',NULL),(323,9,'hidden','0',NULL),(324,9,'hide_tip','0',NULL),(325,9,'max_answers','',NULL),(326,9,'min_answers','',NULL),(327,9,'other_numbers_only','0',NULL),(328,9,'other_replace_text','','en'),(329,9,'page_break','0',NULL),(330,9,'public_statistics','0',NULL),(331,9,'random_group','',NULL),(332,9,'random_order','0',NULL),(333,9,'scale_export','0',NULL),(334,10,'alphasort','0',NULL),(335,10,'array_filter','',NULL),(336,10,'array_filter_exclude','',NULL),(337,10,'display_columns','1',NULL),(338,10,'hidden','0',NULL),(339,10,'hide_tip','0',NULL),(340,10,'other_comment_mandatory','0',NULL),(341,10,'other_numbers_only','0',NULL),(342,10,'other_replace_text','','en'),(343,10,'page_break','0',NULL),(344,10,'public_statistics','0',NULL),(345,10,'random_group','',NULL),(346,10,'random_order','0',NULL),(347,10,'scale_export','0',NULL),(348,11,'hidden','0',NULL),(349,11,'page_break','0',NULL),(350,11,'public_statistics','0',NULL),(351,11,'random_group','',NULL),(352,11,'scale_export','0',NULL),(353,12,'alphasort','0',NULL),(354,12,'array_filter','',NULL),(355,12,'array_filter_exclude','',NULL),(356,12,'display_columns','1',NULL),(357,12,'hidden','0',NULL),(358,12,'hide_tip','0',NULL),(359,12,'other_comment_mandatory','0',NULL),(360,12,'other_numbers_only','0',NULL),(361,12,'other_replace_text','','en'),(362,12,'page_break','0',NULL),(363,12,'public_statistics','0',NULL),(364,12,'random_group','',NULL),(365,12,'random_order','0',NULL),(366,12,'scale_export','0',NULL),(367,13,'alphasort','0',NULL),(368,13,'array_filter','',NULL),(369,13,'array_filter_exclude','',NULL),(370,13,'display_columns','1',NULL),(371,13,'hidden','0',NULL),(372,13,'hide_tip','0',NULL),(373,13,'other_comment_mandatory','0',NULL),(374,13,'other_numbers_only','0',NULL),(375,13,'other_replace_text','','en'),(376,13,'page_break','0',NULL),(377,13,'public_statistics','0',NULL),(378,13,'random_group','',NULL),(379,13,'random_order','0',NULL),(380,13,'scale_export','0',NULL),(381,14,'array_filter','',NULL),(382,14,'array_filter_exclude','',NULL),(383,14,'assessment_value','1',NULL),(384,14,'display_columns','1',NULL),(385,14,'exclude_all_others','',NULL),(386,14,'exclude_all_others_auto','0',NULL),(387,14,'hidden','0',NULL),(388,14,'hide_tip','0',NULL),(389,14,'max_answers','',NULL),(390,14,'min_answers','',NULL),(391,14,'other_numbers_only','0',NULL),(392,14,'other_replace_text','','en'),(393,14,'page_break','0',NULL),(394,14,'public_statistics','0',NULL),(395,14,'random_group','',NULL),(396,14,'random_order','0',NULL),(397,14,'scale_export','0',NULL),(398,15,'alphasort','0',NULL),(399,15,'array_filter','',NULL),(400,15,'array_filter_exclude','',NULL),(401,15,'display_columns','1',NULL),(402,15,'hidden','0',NULL),(403,15,'hide_tip','0',NULL),(404,15,'other_comment_mandatory','0',NULL),(405,15,'other_numbers_only','0',NULL),(406,15,'other_replace_text','','en'),(407,15,'page_break','0',NULL),(408,15,'public_statistics','0',NULL),(409,15,'random_group','',NULL),(410,15,'random_order','0',NULL),(411,15,'scale_export','0',NULL),(412,16,'alphasort','0',NULL),(413,16,'array_filter','',NULL),(414,16,'array_filter_exclude','',NULL),(415,16,'display_columns','1',NULL),(416,16,'hidden','0',NULL),(417,16,'hide_tip','0',NULL),(418,16,'other_comment_mandatory','0',NULL),(419,16,'other_numbers_only','0',NULL),(420,16,'other_replace_text','','en'),(421,16,'page_break','0',NULL),(422,16,'public_statistics','0',NULL),(423,16,'random_group','',NULL),(424,16,'random_order','0',NULL),(425,16,'scale_export','0',NULL),(426,17,'alphasort','0',NULL),(427,17,'hidden','0',NULL),(428,17,'hide_tip','0',NULL),(429,17,'page_break','0',NULL),(430,17,'public_statistics','0',NULL),(431,17,'random_group','',NULL),(432,17,'random_order','0',NULL),(433,17,'scale_export','0',NULL),(434,18,'alphasort','0',NULL),(435,18,'hidden','0',NULL),(436,18,'hide_tip','0',NULL),(437,18,'page_break','0',NULL),(438,18,'public_statistics','0',NULL),(439,18,'random_group','',NULL),(440,18,'random_order','0',NULL),(441,18,'scale_export','0',NULL),(442,19,'alphasort','0',NULL),(443,19,'array_filter','',NULL),(444,19,'array_filter_exclude','',NULL),(445,19,'display_columns','1',NULL),(446,19,'hidden','0',NULL),(447,19,'hide_tip','0',NULL),(448,19,'other_comment_mandatory','0',NULL),(449,19,'other_numbers_only','0',NULL),(450,19,'other_replace_text','','en'),(451,19,'page_break','0',NULL),(452,19,'public_statistics','0',NULL),(453,19,'random_group','',NULL),(454,19,'random_order','0',NULL),(455,19,'scale_export','0',NULL),(456,20,'hidden','0',NULL),(457,20,'page_break','0',NULL),(458,20,'public_statistics','0',NULL),(459,20,'random_group','',NULL),(460,20,'scale_export','0',NULL),(461,21,'array_filter','',NULL),(462,21,'array_filter_exclude','',NULL),(463,21,'assessment_value','1',NULL),(464,21,'display_columns','1',NULL),(465,21,'exclude_all_others','',NULL),(466,21,'exclude_all_others_auto','0',NULL),(467,21,'hidden','0',NULL),(468,21,'hide_tip','0',NULL),(469,21,'max_answers','',NULL),(470,21,'min_answers','',NULL),(471,21,'other_numbers_only','0',NULL),(472,21,'other_replace_text','','en'),(473,21,'page_break','0',NULL),(474,21,'public_statistics','0',NULL),(475,21,'random_group','',NULL),(476,21,'random_order','0',NULL),(477,21,'scale_export','0',NULL),(478,22,'alphasort','0',NULL),(479,22,'array_filter','',NULL),(480,22,'array_filter_exclude','',NULL),(481,22,'display_columns','1',NULL),(482,22,'hidden','0',NULL),(483,22,'hide_tip','0',NULL),(484,22,'other_comment_mandatory','0',NULL),(485,22,'other_numbers_only','0',NULL),(486,22,'other_replace_text','','en'),(487,22,'page_break','0',NULL),(488,22,'public_statistics','0',NULL),(489,22,'random_group','',NULL),(490,22,'random_order','0',NULL),(491,22,'scale_export','0',NULL),(492,23,'alphasort','0',NULL),(493,23,'array_filter','',NULL),(494,23,'array_filter_exclude','',NULL),(495,23,'display_columns','1',NULL),(496,23,'hidden','0',NULL),(497,23,'hide_tip','0',NULL),(498,23,'other_comment_mandatory','0',NULL),(499,23,'other_numbers_only','0',NULL),(500,23,'other_replace_text','','en'),(502,23,'public_statistics','0',NULL),(503,23,'random_group','',NULL),(504,23,'random_order','0',NULL),(505,23,'scale_export','0',NULL),(506,24,'hidden','0',NULL),(507,24,'page_break','0',NULL),(508,24,'public_statistics','0',NULL),(509,24,'random_group','',NULL),(510,24,'scale_export','0',NULL),(511,25,'hidden','0',NULL),(512,25,'page_break','0',NULL),(513,25,'public_statistics','0',NULL),(514,25,'random_group','',NULL),(515,25,'scale_export','0',NULL),(516,26,'display_rows','',NULL),(517,26,'hidden','0',NULL),(518,26,'hide_tip','0',NULL),(519,26,'location_city','0',NULL),(520,26,'location_country','0',NULL),(521,26,'location_defaultcoordinates','',NULL),(522,26,'location_mapheight','300',NULL),(523,26,'location_mapservice','0',NULL),(524,26,'location_mapwidth','500',NULL),(525,26,'location_mapzoom','11',NULL),(526,26,'location_nodefaultfromip','0',NULL),(527,26,'location_postal','0',NULL),(528,26,'location_state','0',NULL),(529,26,'maximum_chars','',NULL),(530,26,'numbers_only','0',NULL),(531,26,'page_break','0',NULL),(532,26,'prefix','','en'),(533,26,'random_group','',NULL),(534,26,'suffix','','en'),(535,26,'text_input_width','',NULL),(536,26,'time_limit','',NULL),(537,26,'time_limit_action','1',NULL),(538,26,'time_limit_countdown_message','','en'),(539,26,'time_limit_disable_next','0',NULL),(540,26,'time_limit_disable_prev','0',NULL),(541,26,'time_limit_message','','en'),(542,26,'time_limit_message_delay','',NULL),(543,26,'time_limit_message_style','',NULL),(544,26,'time_limit_timer_style','',NULL),(545,26,'time_limit_warning','',NULL),(546,26,'time_limit_warning_2','',NULL),(547,26,'time_limit_warning_2_display_time','',NULL),(548,26,'time_limit_warning_2_message','','en'),(549,26,'time_limit_warning_2_style','',NULL),(550,26,'time_limit_warning_display_time','',NULL),(551,26,'time_limit_warning_message','','en'),(552,26,'time_limit_warning_style','',NULL),(553,27,'hidden','0',NULL),(554,27,'page_break','0',NULL),(555,27,'public_statistics','0',NULL),(556,27,'random_group','',NULL),(557,27,'scale_export','0',NULL),(558,28,'display_rows','',NULL),(559,28,'hidden','0',NULL),(560,28,'maximum_chars','',NULL),(561,28,'page_break','0',NULL),(562,28,'random_group','',NULL),(563,28,'text_input_width','',NULL),(564,28,'time_limit','',NULL),(565,28,'time_limit_action','1',NULL),(566,28,'time_limit_countdown_message','','en'),(567,28,'time_limit_disable_next','0',NULL),(568,28,'time_limit_disable_prev','0',NULL),(569,28,'time_limit_message','','en'),(570,28,'time_limit_message_delay','',NULL),(571,28,'time_limit_message_style','',NULL),(572,28,'time_limit_timer_style','',NULL),(573,28,'time_limit_warning','',NULL),(574,28,'time_limit_warning_2','',NULL),(575,28,'time_limit_warning_2_display_time','',NULL),(576,28,'time_limit_warning_2_message','','en'),(577,28,'time_limit_warning_2_style','',NULL),(578,28,'time_limit_warning_display_time','',NULL),(579,28,'time_limit_warning_message','','en'),(580,28,'time_limit_warning_style','',NULL),(581,29,'display_rows','',NULL),(582,29,'hidden','0',NULL),(583,29,'maximum_chars','',NULL),(584,29,'page_break','0',NULL),(585,29,'random_group','',NULL),(586,29,'text_input_width','',NULL),(587,29,'time_limit','',NULL),(588,29,'time_limit_action','1',NULL),(589,29,'time_limit_countdown_message','','en'),(590,29,'time_limit_disable_next','0',NULL),(591,29,'time_limit_disable_prev','0',NULL),(592,29,'time_limit_message','','en'),(593,29,'time_limit_message_delay','',NULL),(594,29,'time_limit_message_style','',NULL),(595,29,'time_limit_timer_style','',NULL),(596,29,'time_limit_warning','',NULL),(597,29,'time_limit_warning_2','',NULL),(598,29,'time_limit_warning_2_display_time','',NULL),(599,29,'time_limit_warning_2_message','','en'),(600,29,'time_limit_warning_2_style','',NULL),(601,29,'time_limit_warning_display_time','',NULL),(602,29,'time_limit_warning_message','','en'),(603,29,'time_limit_warning_style','',NULL),(604,30,'alphasort','0',NULL),(605,30,'array_filter','',NULL),(606,30,'array_filter_exclude','',NULL),(607,30,'display_columns','1',NULL),(608,30,'hidden','0',NULL),(609,30,'hide_tip','0',NULL),(610,30,'other_comment_mandatory','0',NULL),(611,30,'other_numbers_only','0',NULL),(612,30,'other_replace_text','','en'),(613,30,'page_break','0',NULL),(614,30,'public_statistics','0',NULL),(615,30,'random_group','',NULL),(616,30,'random_order','0',NULL),(617,30,'scale_export','0',NULL),(618,31,'hidden','0',NULL),(619,31,'page_break','0',NULL),(620,31,'public_statistics','0',NULL),(621,31,'random_group','',NULL),(622,31,'scale_export','0',NULL),(623,32,'alphasort','0',NULL),(624,32,'array_filter','',NULL),(625,32,'array_filter_exclude','',NULL),(626,32,'display_columns','1',NULL),(627,32,'hidden','0',NULL),(628,32,'hide_tip','0',NULL),(629,32,'other_comment_mandatory','0',NULL),(630,32,'other_numbers_only','0',NULL),(631,32,'other_replace_text','','en'),(632,32,'page_break','0',NULL),(633,32,'public_statistics','0',NULL),(634,32,'random_group','',NULL),(635,32,'random_order','0',NULL),(636,32,'scale_export','0',NULL),(637,33,'array_filter','',NULL),(638,33,'array_filter_exclude','',NULL),(639,33,'assessment_value','1',NULL),(640,33,'display_columns','1',NULL),(641,33,'exclude_all_others','',NULL),(642,33,'exclude_all_others_auto','0',NULL),(643,33,'hidden','0',NULL),(644,33,'hide_tip','0',NULL),(645,33,'max_answers','',NULL),(646,33,'min_answers','',NULL),(647,33,'other_numbers_only','0',NULL),(648,33,'other_replace_text','','en'),(649,33,'page_break','0',NULL),(650,33,'public_statistics','0',NULL),(651,33,'random_group','',NULL),(652,33,'random_order','0',NULL),(653,33,'scale_export','0',NULL),(654,34,'array_filter','',NULL),(655,34,'array_filter_exclude','',NULL),(656,34,'assessment_value','1',NULL),(657,34,'display_columns','1',NULL),(658,34,'exclude_all_others','',NULL),(659,34,'exclude_all_others_auto','0',NULL),(660,34,'hidden','0',NULL),(661,34,'hide_tip','0',NULL),(662,34,'max_answers','',NULL),(663,34,'min_answers','',NULL),(664,34,'other_numbers_only','0',NULL),(665,34,'other_replace_text','','en'),(666,34,'page_break','0',NULL),(667,34,'public_statistics','0',NULL),(668,34,'random_group','',NULL),(669,34,'random_order','0',NULL),(670,34,'scale_export','0',NULL),(671,35,'alphasort','0',NULL),(672,35,'array_filter','',NULL),(673,35,'array_filter_exclude','',NULL),(674,35,'display_columns','1',NULL),(675,35,'hidden','0',NULL),(676,35,'hide_tip','0',NULL),(677,35,'other_comment_mandatory','0',NULL),(678,35,'other_numbers_only','0',NULL),(679,35,'other_replace_text','','en'),(680,35,'page_break','0',NULL),(681,35,'public_statistics','0',NULL),(682,35,'random_group','',NULL),(683,35,'random_order','0',NULL),(684,35,'scale_export','0',NULL),(685,36,'hidden','0',NULL),(686,36,'page_break','0',NULL),(687,36,'public_statistics','0',NULL),(688,36,'random_group','',NULL),(689,36,'scale_export','0',NULL),(690,37,'array_filter','',NULL),(691,37,'array_filter_exclude','',NULL),(692,37,'assessment_value','1',NULL),(693,37,'display_columns','1',NULL),(694,37,'exclude_all_others','',NULL),(695,37,'exclude_all_others_auto','0',NULL),(696,37,'hidden','0',NULL),(697,37,'hide_tip','0',NULL),(698,37,'max_answers','',NULL),(699,37,'min_answers','',NULL),(700,37,'other_numbers_only','0',NULL),(701,37,'other_replace_text','','en'),(702,37,'page_break','0',NULL),(703,37,'public_statistics','0',NULL),(704,37,'random_group','',NULL),(705,37,'random_order','0',NULL),(706,37,'scale_export','0',NULL),(707,38,'array_filter','',NULL),(708,38,'array_filter_exclude','',NULL),(709,38,'assessment_value','1',NULL),(710,38,'display_columns','1',NULL),(711,38,'exclude_all_others','',NULL),(712,38,'exclude_all_others_auto','0',NULL),(713,38,'hidden','0',NULL),(714,38,'hide_tip','0',NULL),(715,38,'max_answers','',NULL),(716,38,'min_answers','',NULL),(717,38,'other_numbers_only','0',NULL),(718,38,'other_replace_text','','en'),(719,38,'page_break','0',NULL),(720,38,'public_statistics','0',NULL),(721,38,'random_group','',NULL),(722,38,'random_order','0',NULL),(723,38,'scale_export','0',NULL),(724,39,'array_filter','',NULL),(725,39,'array_filter_exclude','',NULL),(726,39,'assessment_value','1',NULL),(727,39,'display_columns','1',NULL),(728,39,'exclude_all_others','',NULL),(729,39,'exclude_all_others_auto','0',NULL),(730,39,'hidden','0',NULL),(731,39,'hide_tip','0',NULL),(732,39,'max_answers','',NULL),(733,39,'min_answers','',NULL),(734,39,'other_numbers_only','0',NULL),(735,39,'other_replace_text','','en'),(736,39,'page_break','0',NULL),(737,39,'public_statistics','0',NULL),(738,39,'random_group','',NULL),(739,39,'random_order','0',NULL),(740,39,'scale_export','0',NULL),(741,103,'array_filter','',NULL),(742,103,'array_filter_exclude','',NULL),(743,103,'assessment_value','1',NULL),(744,103,'display_columns','1',NULL),(745,103,'exclude_all_others','',NULL),(746,103,'exclude_all_others_auto','0',NULL),(747,103,'hidden','0',NULL),(748,103,'hide_tip','0',NULL),(749,103,'max_answers','',NULL),(750,103,'min_answers','',NULL),(751,103,'other_numbers_only','0',NULL),(752,103,'other_replace_text','','en'),(753,103,'page_break','0',NULL),(754,103,'public_statistics','0',NULL),(755,103,'random_group','',NULL),(756,103,'random_order','0',NULL),(757,103,'scale_export','0',NULL),(758,104,'hidden','0',NULL),(759,104,'hide_tip','0',NULL),(760,104,'max_num_value_n','',NULL),(761,104,'maximum_chars','',NULL),(762,104,'min_num_value_n','',NULL),(763,104,'num_value_int_only','0',NULL),(764,104,'page_break','0',NULL),(765,104,'prefix','','en'),(766,104,'public_statistics','0',NULL),(767,104,'random_group','',NULL),(768,104,'suffix','','en'),(769,104,'text_input_width','',NULL),(770,105,'alphasort','0',NULL),(771,105,'array_filter','',NULL),(772,105,'array_filter_exclude','',NULL),(773,105,'display_columns','1',NULL),(774,105,'hidden','0',NULL),(775,105,'hide_tip','0',NULL),(776,105,'other_comment_mandatory','0',NULL),(777,105,'other_numbers_only','0',NULL),(778,105,'other_replace_text','','en'),(779,105,'page_break','0',NULL),(780,105,'public_statistics','0',NULL),(781,105,'random_group','',NULL),(782,105,'random_order','0',NULL),(783,105,'scale_export','0',NULL),(784,106,'alphasort','0',NULL),(785,106,'array_filter','',NULL),(786,106,'array_filter_exclude','',NULL),(787,106,'display_columns','1',NULL),(788,106,'hidden','0',NULL),(789,106,'hide_tip','0',NULL),(790,106,'other_comment_mandatory','0',NULL),(791,106,'other_numbers_only','0',NULL),(792,106,'other_replace_text','','en'),(793,106,'page_break','0',NULL),(794,106,'public_statistics','0',NULL),(795,106,'random_group','',NULL),(796,106,'random_order','0',NULL),(797,106,'scale_export','0',NULL),(798,107,'alphasort','0',NULL),(799,107,'array_filter','',NULL),(800,107,'array_filter_exclude','',NULL),(801,107,'display_columns','1',NULL),(802,107,'hidden','0',NULL),(803,107,'hide_tip','0',NULL),(804,107,'other_comment_mandatory','0',NULL),(805,107,'other_numbers_only','0',NULL),(806,107,'other_replace_text','','en'),(807,107,'page_break','0',NULL),(808,107,'public_statistics','0',NULL),(809,107,'random_group','',NULL),(810,107,'random_order','0',NULL),(811,107,'scale_export','0',NULL),(812,108,'array_filter','',NULL),(813,108,'array_filter_exclude','',NULL),(814,108,'assessment_value','1',NULL),(815,108,'display_columns','1',NULL),(816,108,'exclude_all_others','',NULL),(817,108,'exclude_all_others_auto','0',NULL),(818,108,'hidden','0',NULL),(819,108,'hide_tip','0',NULL),(820,108,'max_answers','',NULL),(821,108,'min_answers','',NULL),(822,108,'other_numbers_only','0',NULL),(823,108,'other_replace_text','','en'),(824,108,'page_break','0',NULL),(825,108,'public_statistics','0',NULL),(826,108,'random_group','',NULL),(827,108,'random_order','0',NULL),(828,108,'scale_export','0',NULL),(829,109,'alphasort','0',NULL),(830,109,'array_filter','',NULL),(831,109,'array_filter_exclude','',NULL),(832,109,'display_columns','1',NULL),(833,109,'hidden','0',NULL),(834,109,'hide_tip','0',NULL),(835,109,'other_comment_mandatory','0',NULL),(836,109,'other_numbers_only','0',NULL),(837,109,'other_replace_text','','en'),(838,109,'page_break','0',NULL),(839,109,'public_statistics','0',NULL),(840,109,'random_group','',NULL),(841,109,'random_order','0',NULL),(842,109,'scale_export','0',NULL),(843,110,'alphasort','0',NULL),(844,110,'array_filter','',NULL),(845,110,'array_filter_exclude','',NULL),(846,110,'display_columns','1',NULL),(847,110,'hidden','0',NULL),(848,110,'hide_tip','0',NULL),(849,110,'other_comment_mandatory','0',NULL),(850,110,'other_numbers_only','0',NULL),(851,110,'other_replace_text','','en'),(852,110,'page_break','0',NULL),(853,110,'public_statistics','0',NULL),(854,110,'random_group','',NULL),(855,110,'random_order','0',NULL),(856,110,'scale_export','0',NULL),(857,111,'array_filter','',NULL),(858,111,'array_filter_exclude','',NULL),(859,111,'assessment_value','1',NULL),(860,111,'display_columns','1',NULL),(861,111,'exclude_all_others','',NULL),(862,111,'exclude_all_others_auto','0',NULL),(863,111,'hidden','0',NULL),(864,111,'hide_tip','0',NULL),(865,111,'max_answers','',NULL),(866,111,'min_answers','',NULL),(867,111,'other_numbers_only','0',NULL),(868,111,'other_replace_text','','en'),(869,111,'page_break','0',NULL),(870,111,'public_statistics','0',NULL),(871,111,'random_group','',NULL),(872,111,'random_order','0',NULL),(873,111,'scale_export','0',NULL),(874,112,'alphasort','0',NULL),(875,112,'array_filter','',NULL),(876,112,'array_filter_exclude','',NULL),(877,112,'display_columns','1',NULL),(878,112,'hidden','0',NULL),(879,112,'hide_tip','0',NULL),(880,112,'other_comment_mandatory','0',NULL),(881,112,'other_numbers_only','0',NULL),(882,112,'other_replace_text','','en'),(883,112,'page_break','0',NULL),(884,112,'public_statistics','0',NULL),(885,112,'random_group','',NULL),(886,112,'random_order','0',NULL),(887,112,'scale_export','0',NULL),(888,113,'hidden','0',NULL),(889,113,'page_break','0',NULL),(890,113,'public_statistics','0',NULL),(891,113,'random_group','',NULL),(892,113,'scale_export','0',NULL),(893,114,'alphasort','0',NULL),(894,114,'array_filter','',NULL),(895,114,'array_filter_exclude','',NULL),(896,114,'display_columns','1',NULL),(897,114,'hidden','0',NULL),(898,114,'hide_tip','0',NULL),(899,114,'other_comment_mandatory','0',NULL),(900,114,'other_numbers_only','0',NULL),(901,114,'other_replace_text','','en'),(902,114,'page_break','0',NULL),(903,114,'public_statistics','0',NULL),(904,114,'random_group','',NULL),(905,114,'random_order','0',NULL),(906,114,'scale_export','0',NULL),(907,115,'alphasort','0',NULL),(908,115,'array_filter','',NULL),(909,115,'array_filter_exclude','',NULL),(910,115,'display_columns','1',NULL),(911,115,'hidden','0',NULL),(912,115,'hide_tip','0',NULL),(913,115,'other_comment_mandatory','0',NULL),(914,115,'other_numbers_only','0',NULL),(915,115,'other_replace_text','','en'),(916,115,'page_break','0',NULL),(917,115,'public_statistics','0',NULL),(918,115,'random_group','',NULL),(919,115,'random_order','0',NULL),(920,115,'scale_export','0',NULL),(921,116,'array_filter','',NULL),(922,116,'array_filter_exclude','',NULL),(923,116,'assessment_value','1',NULL),(924,116,'display_columns','1',NULL),(925,116,'exclude_all_others','',NULL),(926,116,'exclude_all_others_auto','0',NULL),(927,116,'hidden','0',NULL),(928,116,'hide_tip','0',NULL),(929,116,'max_answers','',NULL),(930,116,'min_answers','',NULL),(931,116,'other_numbers_only','0',NULL),(932,116,'other_replace_text','','en'),(933,116,'page_break','0',NULL),(934,116,'public_statistics','0',NULL),(935,116,'random_group','',NULL),(936,116,'random_order','0',NULL),(937,116,'scale_export','0',NULL),(938,117,'alphasort','0',NULL),(939,117,'array_filter','',NULL),(940,117,'array_filter_exclude','',NULL),(941,117,'display_columns','1',NULL),(942,103,'array_filter_exclude','',NULL),(943,103,'assessment_value','1',NULL),(944,103,'display_columns','1',NULL),(945,103,'exclude_all_others','',NULL),(946,103,'exclude_all_others_auto','0',NULL),(947,103,'hidden','0',NULL),(948,103,'hide_tip','0',NULL),(949,103,'max_answers','',NULL),(950,103,'min_answers','',NULL),(951,103,'other_numbers_only','0',NULL),(952,103,'other_replace_text','','en'),(953,103,'page_break','0',NULL),(954,103,'public_statistics','0',NULL),(955,103,'random_group','',NULL),(956,103,'random_order','0',NULL),(957,103,'scale_export','0',NULL),(958,104,'hidden','0',NULL),(959,104,'hide_tip','0',NULL),(960,104,'max_num_value_n','',NULL),(961,104,'maximum_chars','',NULL),(962,104,'min_num_value_n','',NULL),(963,104,'num_value_int_only','0',NULL),(964,104,'page_break','0',NULL),(965,104,'prefix','','en'),(966,104,'public_statistics','0',NULL),(967,104,'random_group','',NULL),(968,104,'suffix','','en'),(969,104,'text_input_width','',NULL),(970,105,'alphasort','0',NULL),(971,105,'array_filter','',NULL),(972,105,'array_filter_exclude','',NULL),(973,105,'display_columns','1',NULL),(974,105,'hidden','0',NULL),(975,105,'hide_tip','0',NULL),(976,105,'other_comment_mandatory','0',NULL),(977,105,'other_numbers_only','0',NULL),(978,105,'other_replace_text','','en'),(979,105,'page_break','0',NULL),(980,105,'public_statistics','0',NULL),(981,105,'random_group','',NULL),(982,105,'random_order','0',NULL),(983,105,'scale_export','0',NULL),(984,106,'alphasort','0',NULL),(985,106,'array_filter','',NULL),(986,106,'array_filter_exclude','',NULL),(987,106,'display_columns','1',NULL),(988,106,'hidden','0',NULL),(989,106,'hide_tip','0',NULL),(990,106,'other_comment_mandatory','0',NULL),(991,106,'other_numbers_only','0',NULL),(992,106,'other_replace_text','','en'),(993,106,'page_break','0',NULL),(994,106,'public_statistics','0',NULL),(995,106,'random_group','',NULL),(996,106,'random_order','0',NULL),(997,106,'scale_export','0',NULL),(998,107,'alphasort','0',NULL),(999,107,'array_filter','',NULL),(1000,107,'array_filter_exclude','',NULL),(1001,107,'display_columns','1',NULL),(1002,107,'hidden','0',NULL),(1003,107,'hide_tip','0',NULL),(1004,107,'other_comment_mandatory','0',NULL),(1005,107,'other_numbers_only','0',NULL),(1006,107,'other_replace_text','','en'),(1007,107,'page_break','0',NULL),(1008,107,'public_statistics','0',NULL),(1009,107,'random_group','',NULL),(1010,107,'random_order','0',NULL),(1011,107,'scale_export','0',NULL),(1012,108,'array_filter','',NULL),(1013,108,'array_filter_exclude','',NULL),(1014,108,'assessment_value','1',NULL),(1015,108,'display_columns','1',NULL),(1016,108,'exclude_all_others','',NULL),(1017,108,'exclude_all_others_auto','0',NULL),(1018,108,'hidden','0',NULL),(1019,108,'hide_tip','0',NULL),(1020,108,'max_answers','',NULL),(1021,108,'min_answers','',NULL),(1022,108,'other_numbers_only','0',NULL),(1023,108,'other_replace_text','','en'),(1024,108,'page_break','0',NULL),(1025,108,'public_statistics','0',NULL),(1026,108,'random_group','',NULL),(1027,108,'random_order','0',NULL),(1028,108,'scale_export','0',NULL),(1029,109,'alphasort','0',NULL),(1030,109,'array_filter','',NULL),(1031,109,'array_filter_exclude','',NULL),(1032,109,'display_columns','1',NULL),(1033,109,'hidden','0',NULL),(1034,109,'hide_tip','0',NULL),(1035,109,'other_comment_mandatory','0',NULL),(1036,109,'other_numbers_only','0',NULL),(1037,109,'other_replace_text','','en'),(1038,109,'page_break','0',NULL),(1039,109,'public_statistics','0',NULL),(1040,109,'random_group','',NULL),(1041,109,'random_order','0',NULL),(1042,109,'scale_export','0',NULL),(1043,110,'alphasort','0',NULL),(1044,110,'array_filter','',NULL),(1045,110,'array_filter_exclude','',NULL),(1046,110,'display_columns','1',NULL),(1047,110,'hidden','0',NULL),(1048,110,'hide_tip','0',NULL),(1049,110,'other_comment_mandatory','0',NULL),(1050,110,'other_numbers_only','0',NULL),(1051,110,'other_replace_text','','en'),(1052,110,'page_break','0',NULL),(1053,110,'public_statistics','0',NULL),(1054,110,'random_group','',NULL),(1055,110,'random_order','0',NULL),(1056,110,'scale_export','0',NULL),(1057,111,'array_filter','',NULL),(1058,111,'array_filter_exclude','',NULL),(1059,111,'assessment_value','1',NULL),(1060,111,'display_columns','1',NULL),(1061,111,'exclude_all_others','',NULL),(1062,111,'exclude_all_others_auto','0',NULL),(1063,111,'hidden','0',NULL),(1064,111,'hide_tip','0',NULL),(1065,111,'max_answers','',NULL),(1066,111,'min_answers','',NULL),(1067,111,'other_numbers_only','0',NULL),(1068,111,'other_replace_text','','en'),(1069,111,'page_break','0',NULL),(1070,111,'public_statistics','0',NULL),(1071,111,'random_group','',NULL),(1072,111,'random_order','0',NULL),(1073,111,'scale_export','0',NULL),(1074,112,'alphasort','0',NULL),(1075,112,'array_filter','',NULL),(1076,112,'array_filter_exclude','',NULL),(1077,112,'display_columns','1',NULL),(1078,112,'hidden','0',NULL),(1079,112,'hide_tip','0',NULL),(1080,112,'other_comment_mandatory','0',NULL),(1081,112,'other_numbers_only','0',NULL),(1082,112,'other_replace_text','','en'),(1083,112,'page_break','0',NULL),(1084,112,'public_statistics','0',NULL),(1085,112,'random_group','',NULL),(1086,112,'random_order','0',NULL),(1087,112,'scale_export','0',NULL),(1088,113,'hidden','0',NULL),(1089,113,'page_break','0',NULL),(1090,113,'public_statistics','0',NULL),(1091,113,'random_group','',NULL),(1092,113,'scale_export','0',NULL),(1093,114,'alphasort','0',NULL),(1094,114,'array_filter','',NULL),(1095,114,'array_filter_exclude','',NULL),(1096,114,'display_columns','1',NULL),(1097,114,'hidden','0',NULL),(1098,114,'hide_tip','0',NULL),(1099,114,'other_comment_mandatory','0',NULL),(1100,114,'other_numbers_only','0',NULL),(1101,114,'other_replace_text','','en'),(1102,114,'page_break','0',NULL),(1103,114,'public_statistics','0',NULL),(1104,114,'random_group','',NULL),(1105,114,'random_order','0',NULL),(1106,114,'scale_export','0',NULL),(1107,115,'alphasort','0',NULL),(1108,115,'array_filter','',NULL),(1109,115,'array_filter_exclude','',NULL),(1110,115,'display_columns','1',NULL),(1111,115,'hidden','0',NULL),(1112,115,'hide_tip','0',NULL),(1113,115,'other_comment_mandatory','0',NULL),(1114,115,'other_numbers_only','0',NULL),(1115,115,'other_replace_text','','en'),(1116,115,'page_break','0',NULL),(1117,115,'public_statistics','0',NULL),(1118,115,'random_group','',NULL),(1119,115,'random_order','0',NULL),(1120,115,'scale_export','0',NULL),(1121,116,'array_filter','',NULL),(1122,116,'array_filter_exclude','',NULL),(1123,116,'assessment_value','1',NULL),(1124,116,'display_columns','1',NULL),(1125,116,'exclude_all_others','',NULL),(1126,116,'exclude_all_others_auto','0',NULL),(1127,116,'hidden','0',NULL),(1128,116,'hide_tip','0',NULL),(1129,116,'max_answers','',NULL),(1130,116,'min_answers','',NULL),(1131,116,'other_numbers_only','0',NULL),(1132,116,'other_replace_text','','en'),(1133,116,'page_break','0',NULL),(1134,116,'public_statistics','0',NULL),(1135,116,'random_group','',NULL),(1136,116,'random_order','0',NULL),(1137,116,'scale_export','0',NULL),(1138,117,'alphasort','0',NULL),(1139,117,'array_filter','',NULL),(1140,117,'array_filter_exclude','',NULL),(1141,117,'display_columns','1',NULL),(1142,117,'hidden','0',NULL),(1143,117,'hide_tip','0',NULL),(1144,117,'other_comment_mandatory','0',NULL),(1145,117,'other_numbers_only','0',NULL),(1146,117,'other_replace_text','','en'),(1147,117,'page_break','0',NULL),(1148,117,'public_statistics','0',NULL),(1149,117,'random_group','',NULL),(1150,117,'random_order','0',NULL),(1151,117,'scale_export','0',NULL),(1152,118,'alphasort','0',NULL),(1153,118,'array_filter','',NULL),(1154,118,'array_filter_exclude','',NULL),(1155,118,'display_columns','1',NULL),(1156,118,'hidden','0',NULL),(1157,118,'hide_tip','0',NULL),(1158,118,'other_comment_mandatory','0',NULL),(1159,118,'other_numbers_only','0',NULL),(1160,118,'other_replace_text','','en'),(1161,118,'page_break','0',NULL),(1162,118,'public_statistics','0',NULL),(1163,118,'random_group','',NULL),(1164,118,'random_order','0',NULL),(1165,118,'scale_export','0',NULL),(1166,119,'alphasort','0',NULL),(1167,119,'hidden','0',NULL),(1168,119,'hide_tip','0',NULL),(1169,119,'page_break','0',NULL),(1170,119,'public_statistics','0',NULL),(1171,119,'random_group','',NULL),(1172,119,'random_order','0',NULL),(1173,119,'scale_export','0',NULL),(1174,120,'alphasort','0',NULL),(1175,120,'hidden','0',NULL),(1176,120,'hide_tip','0',NULL),(1177,120,'page_break','0',NULL),(1178,120,'public_statistics','0',NULL),(1179,120,'random_group','',NULL),(1180,120,'random_order','0',NULL),(1181,120,'scale_export','0',NULL),(1182,121,'alphasort','0',NULL),(1183,121,'array_filter','',NULL),(1184,121,'array_filter_exclude','',NULL),(1185,121,'display_columns','1',NULL),(1186,121,'hidden','0',NULL),(1187,121,'hide_tip','0',NULL),(1188,121,'other_comment_mandatory','0',NULL),(1189,121,'other_numbers_only','0',NULL),(1190,121,'other_replace_text','','en'),(1191,121,'page_break','0',NULL),(1192,121,'public_statistics','0',NULL),(1193,121,'random_group','',NULL),(1194,121,'random_order','0',NULL),(1195,121,'scale_export','0',NULL),(1196,122,'hidden','0',NULL),(1197,122,'page_break','0',NULL),(1198,122,'public_statistics','0',NULL),(1199,122,'random_group','',NULL),(1200,122,'scale_export','0',NULL),(1201,123,'array_filter','',NULL),(1202,123,'array_filter_exclude','',NULL),(1203,123,'assessment_value','1',NULL),(1204,123,'display_columns','1',NULL),(1205,123,'exclude_all_others','',NULL),(1206,123,'exclude_all_others_auto','0',NULL),(1207,123,'hidden','0',NULL),(1208,123,'hide_tip','0',NULL),(1209,123,'max_answers','',NULL),(1210,123,'min_answers','',NULL),(1211,123,'other_numbers_only','0',NULL),(1212,123,'other_replace_text','','en'),(1213,123,'page_break','0',NULL),(1214,123,'public_statistics','0',NULL),(1215,123,'random_group','',NULL),(1216,123,'random_order','0',NULL),(1217,123,'scale_export','0',NULL),(1218,124,'alphasort','0',NULL),(1219,124,'array_filter','',NULL),(1220,124,'array_filter_exclude','',NULL),(1221,124,'display_columns','1',NULL),(1222,124,'hidden','0',NULL),(1223,124,'hide_tip','0',NULL),(1224,124,'other_comment_mandatory','0',NULL),(1225,124,'other_numbers_only','0',NULL),(1226,124,'other_replace_text','','en'),(1227,124,'page_break','0',NULL),(1228,124,'public_statistics','0',NULL),(1229,124,'random_group','',NULL),(1230,124,'random_order','0',NULL),(1231,124,'scale_export','0',NULL),(1232,125,'alphasort','0',NULL),(1233,125,'array_filter','',NULL),(1234,125,'array_filter_exclude','',NULL),(1235,125,'display_columns','1',NULL),(1236,125,'hidden','0',NULL),(1237,125,'hide_tip','0',NULL),(1238,125,'other_comment_mandatory','0',NULL),(1239,125,'other_numbers_only','0',NULL),(1240,125,'other_replace_text','','en'),(1241,125,'page_break','0',NULL),(1242,125,'public_statistics','0',NULL),(1243,125,'random_group','',NULL),(1244,125,'random_order','0',NULL),(1245,125,'scale_export','0',NULL),(1246,126,'hidden','0',NULL),(1247,126,'page_break','0',NULL),(1248,126,'public_statistics','0',NULL),(1249,126,'random_group','',NULL),(1250,126,'scale_export','0',NULL),(1251,127,'hidden','0',NULL),(1252,127,'page_break','0',NULL),(1253,127,'public_statistics','0',NULL),(1254,127,'random_group','',NULL),(1255,127,'scale_export','0',NULL),(1256,128,'display_rows','',NULL),(1257,128,'hidden','0',NULL),(1258,128,'hide_tip','0',NULL),(1259,128,'location_city','0',NULL),(1260,128,'location_country','0',NULL),(1261,128,'location_defaultcoordinates','',NULL),(1262,128,'location_mapheight','300',NULL),(1263,128,'location_mapservice','0',NULL),(1264,128,'location_mapwidth','500',NULL),(1265,128,'location_mapzoom','11',NULL),(1266,128,'location_nodefaultfromip','0',NULL),(1267,128,'location_postal','0',NULL),(1268,128,'location_state','0',NULL),(1269,128,'maximum_chars','',NULL),(1270,128,'numbers_only','0',NULL),(1271,128,'page_break','0',NULL),(1272,128,'prefix','','en'),(1273,128,'random_group','',NULL),(1274,128,'suffix','','en'),(1275,128,'text_input_width','',NULL),(1276,128,'time_limit','',NULL),(1277,128,'time_limit_action','1',NULL),(1278,128,'time_limit_countdown_message','','en'),(1279,128,'time_limit_disable_next','0',NULL),(1280,128,'time_limit_disable_prev','0',NULL),(1281,128,'time_limit_message','','en'),(1282,128,'time_limit_message_delay','',NULL),(1283,128,'time_limit_message_style','',NULL),(1284,128,'time_limit_timer_style','',NULL),(1285,128,'time_limit_warning','',NULL),(1286,128,'time_limit_warning_2','',NULL),(1287,128,'time_limit_warning_2_display_time','',NULL),(1288,128,'time_limit_warning_2_message','','en'),(1289,128,'time_limit_warning_2_style','',NULL),(1290,128,'time_limit_warning_display_time','',NULL),(1291,128,'time_limit_warning_message','','en'),(1292,128,'time_limit_warning_style','',NULL),(1293,129,'hidden','0',NULL),(1294,129,'page_break','0',NULL),(1295,129,'public_statistics','0',NULL),(1296,129,'random_group','',NULL),(1297,129,'scale_export','0',NULL),(1298,130,'display_rows','',NULL),(1299,130,'hidden','0',NULL),(1300,130,'maximum_chars','',NULL),(1301,130,'page_break','0',NULL),(1302,130,'random_group','',NULL),(1303,130,'text_input_width','',NULL),(1304,130,'time_limit','',NULL),(1305,130,'time_limit_action','1',NULL),(1306,130,'time_limit_countdown_message','','en'),(1307,130,'time_limit_disable_next','0',NULL),(1308,130,'time_limit_disable_prev','0',NULL),(1309,130,'time_limit_message','','en'),(1310,130,'time_limit_message_delay','',NULL),(1311,130,'time_limit_message_style','',NULL),(1312,130,'time_limit_timer_style','',NULL),(1313,130,'time_limit_warning','',NULL),(1314,130,'time_limit_warning_2','',NULL),(1315,130,'time_limit_warning_2_display_time','',NULL),(1316,130,'time_limit_warning_2_message','','en'),(1317,130,'time_limit_warning_2_style','',NULL),(1318,130,'time_limit_warning_display_time','',NULL),(1319,130,'time_limit_warning_message','','en'),(1320,130,'time_limit_warning_style','',NULL),(1321,131,'display_rows','',NULL),(1322,131,'hidden','0',NULL),(1323,131,'maximum_chars','',NULL),(1324,131,'page_break','0',NULL),(1325,131,'random_group','',NULL),(1326,131,'text_input_width','',NULL),(1327,131,'time_limit','',NULL),(1328,131,'time_limit_action','1',NULL),(1329,131,'time_limit_countdown_message','','en'),(1330,131,'time_limit_disable_next','0',NULL),(1331,131,'time_limit_disable_prev','0',NULL),(1332,131,'time_limit_message','','en'),(1333,131,'time_limit_message_delay','',NULL),(1334,131,'time_limit_message_style','',NULL),(1335,131,'time_limit_timer_style','',NULL),(1336,131,'time_limit_warning','',NULL),(1337,131,'time_limit_warning_2','',NULL),(1338,131,'time_limit_warning_2_display_time','',NULL),(1339,131,'time_limit_warning_2_message','','en'),(1340,131,'time_limit_warning_2_style','',NULL),(1341,131,'time_limit_warning_display_time','',NULL),(1342,131,'time_limit_warning_message','','en'),(1343,131,'time_limit_warning_style','',NULL),(1344,132,'alphasort','0',NULL),(1345,132,'array_filter','',NULL),(1346,132,'array_filter_exclude','',NULL),(1347,132,'display_columns','1',NULL),(1348,132,'hidden','0',NULL),(1349,132,'hide_tip','0',NULL),(1350,132,'other_comment_mandatory','0',NULL),(1351,132,'other_numbers_only','0',NULL),(1352,132,'other_replace_text','','en'),(1353,132,'page_break','0',NULL),(1354,132,'public_statistics','0',NULL),(1355,132,'random_group','',NULL),(1356,132,'random_order','0',NULL),(1357,132,'scale_export','0',NULL),(1358,133,'hidden','0',NULL),(1359,133,'page_break','0',NULL),(1360,133,'public_statistics','0',NULL),(1361,133,'random_group','',NULL),(1362,133,'scale_export','0',NULL),(1363,134,'alphasort','0',NULL),(1364,134,'array_filter','',NULL),(1365,134,'array_filter_exclude','',NULL),(1366,134,'display_columns','1',NULL),(1367,134,'hidden','0',NULL),(1368,134,'hide_tip','0',NULL),(1369,134,'other_comment_mandatory','0',NULL),(1370,134,'other_numbers_only','0',NULL),(1371,134,'other_replace_text','','en'),(1372,134,'page_break','0',NULL),(1373,134,'public_statistics','0',NULL),(1374,134,'random_group','',NULL),(1375,134,'random_order','0',NULL),(1376,134,'scale_export','0',NULL),(1377,135,'array_filter','',NULL),(1378,135,'array_filter_exclude','',NULL),(1379,135,'assessment_value','1',NULL),(1380,135,'display_columns','1',NULL),(1381,135,'exclude_all_others','',NULL),(1382,135,'exclude_all_others_auto','0',NULL),(1383,135,'hidden','0',NULL),(1384,135,'hide_tip','0',NULL),(1385,135,'max_answers','',NULL),(1386,135,'min_answers','',NULL),(1387,135,'other_numbers_only','0',NULL),(1388,135,'other_replace_text','','en'),(1389,135,'page_break','0',NULL),(1390,135,'public_statistics','0',NULL),(1391,135,'random_group','',NULL),(1392,135,'random_order','0',NULL),(1393,135,'scale_export','0',NULL),(1394,136,'array_filter','',NULL),(1395,136,'array_filter_exclude','',NULL),(1396,136,'assessment_value','1',NULL),(1397,136,'display_columns','1',NULL),(1398,136,'exclude_all_others','',NULL),(1399,136,'exclude_all_others_auto','0',NULL),(1400,136,'hidden','0',NULL),(1401,136,'hide_tip','0',NULL),(1402,136,'max_answers','',NULL),(1403,136,'min_answers','',NULL),(1404,136,'other_numbers_only','0',NULL),(1405,136,'other_replace_text','','en'),(1406,136,'page_break','0',NULL),(1407,136,'public_statistics','0',NULL),(1408,136,'random_group','',NULL),(1409,136,'random_order','0',NULL),(1410,136,'scale_export','0',NULL),(1411,137,'alphasort','0',NULL),(1412,137,'array_filter','',NULL),(1413,137,'array_filter_exclude','',NULL),(1414,137,'display_columns','1',NULL),(1415,137,'hidden','0',NULL),(1416,137,'hide_tip','0',NULL),(1417,137,'other_comment_mandatory','0',NULL),(1418,137,'other_numbers_only','0',NULL),(1419,137,'other_replace_text','','en'),(1420,137,'page_break','0',NULL),(1421,137,'public_statistics','0',NULL),(1422,137,'random_group','',NULL),(1423,137,'random_order','0',NULL),(1424,137,'scale_export','0',NULL),(1425,138,'hidden','0',NULL),(1426,138,'page_break','0',NULL),(1427,138,'public_statistics','0',NULL),(1428,138,'random_group','',NULL),(1429,138,'scale_export','0',NULL),(1430,139,'array_filter','',NULL),(1431,139,'array_filter_exclude','',NULL),(1432,139,'assessment_value','1',NULL),(1433,139,'display_columns','1',NULL),(1434,139,'exclude_all_others','',NULL),(1435,139,'exclude_all_others_auto','0',NULL),(1436,139,'hidden','0',NULL),(1437,139,'hide_tip','0',NULL),(1438,139,'max_answers','',NULL),(1439,139,'min_answers','',NULL),(1440,139,'other_numbers_only','0',NULL),(1441,139,'other_replace_text','','en'),(1442,139,'page_break','0',NULL),(1443,139,'public_statistics','0',NULL),(1444,139,'random_group','',NULL),(1445,139,'random_order','0',NULL),(1446,139,'scale_export','0',NULL),(1447,140,'array_filter','',NULL),(1448,140,'array_filter_exclude','',NULL),(1449,140,'assessment_value','1',NULL),(1450,140,'display_columns','1',NULL),(1451,140,'exclude_all_others','',NULL),(1452,140,'exclude_all_others_auto','0',NULL),(1453,140,'hidden','0',NULL),(1454,140,'hide_tip','0',NULL),(1455,140,'max_answers','',NULL),(1456,140,'min_answers','',NULL),(1457,140,'other_numbers_only','0',NULL),(1458,140,'other_replace_text','','en'),(1459,140,'page_break','0',NULL),(1460,140,'public_statistics','0',NULL),(1461,140,'random_group','',NULL),(1462,140,'random_order','0',NULL),(1463,140,'scale_export','0',NULL),(1464,141,'array_filter','',NULL),(1465,141,'array_filter_exclude','',NULL),(1466,141,'assessment_value','1',NULL),(1467,141,'display_columns','1',NULL),(1468,141,'exclude_all_others','',NULL),(1469,141,'exclude_all_others_auto','0',NULL),(1470,141,'hidden','0',NULL),(1471,141,'hide_tip','0',NULL),(1472,141,'max_answers','',NULL),(1473,141,'min_answers','',NULL),(1474,141,'other_numbers_only','0',NULL),(1475,141,'other_replace_text','','en'),(1476,141,'page_break','0',NULL),(1477,141,'public_statistics','0',NULL),(1478,141,'random_group','',NULL),(1479,141,'random_order','0',NULL),(1480,141,'scale_export','0',NULL);
/*!40000 ALTER TABLE `question_attributes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `questions`
--

DROP TABLE IF EXISTS `questions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `questions` (
  `qid` int(11) NOT NULL AUTO_INCREMENT,
  `parent_qid` int(11) NOT NULL DEFAULT '0',
  `sid` int(11) NOT NULL DEFAULT '0',
  `gid` int(11) NOT NULL DEFAULT '0',
  `type` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'T',
  `title` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `question` text COLLATE utf8_unicode_ci NOT NULL,
  `preg` text COLLATE utf8_unicode_ci,
  `help` text COLLATE utf8_unicode_ci,
  `other` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `mandatory` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `question_order` int(11) NOT NULL,
  `language` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `scale_id` int(11) NOT NULL DEFAULT '0',
  `same_default` int(11) NOT NULL DEFAULT '0' COMMENT 'Saves if user set to use the same default value across languages in default options dialog',
  `relevance` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`qid`,`language`),
  KEY `questions_idx2` (`sid`),
  KEY `questions_idx3` (`gid`),
  KEY `questions_idx4` (`type`),
  KEY `parent_qid_idx` (`parent_qid`)
) ENGINE=MyISAM AUTO_INCREMENT=221 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `questions`
--

LOCK TABLES `questions` WRITE;
/*!40000 ALTER TABLE `questions` DISABLE KEYS */;
INSERT INTO `questions` VALUES (1,0,81259,1,'M','q0001','<p>\n	What is your role in creating a site ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(2,0,81259,1,'N','q0002','<p>\n	How many years have you been doing this kind of role ?</p>\n','','','N','Y',1,'en',0,0,NULL),(3,0,81259,1,'L','q0003','<p>\n	How often do you talk to the people in the other roles ?</p>\n','','','N','Y',3,'en',0,0,NULL),(4,0,81259,1,'L','q0005','<p>\n	Do you practice devops ?</p>\n','','','Y','Y',8,'en',0,0,NULL),(5,0,81259,2,'L','r228q0','<p>\n	How big is your organisation ?</p>\n','','','N','N',1,'en',0,0,NULL),(6,0,81259,2,'M','r228q1','<p>\n	How would  you describe your organisation ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(7,0,81259,2,'L','r228q2','<p>\n	Who typically edits the content ?</p>\n','','','Y','Y',2,'en',0,0,NULL),(8,0,81259,2,'L','q0004','<p>\n	Who typically updates your code base / monitors security fixes and applies them when needed ?</p>\n','','','Y','Y',3,'en',0,0,NULL),(9,0,81259,3,'M','r228q3','<p>\n	What Drupal Version are you using ?</p>\n','','','N','Y',0,'en',0,0,NULL),(10,0,81259,3,'L','r228q4','<p>\n	Is your current production site up to date ?</p>\n<p>\n	Have all the required security patches been applied ?</p>\n<p>\n	 </p>\n','','','N','Y',1,'en',0,0,NULL),(11,0,81259,3,'Y','r228q5','<p>\n	Are you subscribed to Drupal Security News</p>\n','','','N','Y',3,'en',0,0,NULL),(12,0,81259,4,'L','r228q6','<p>\n	Do you use any form of version control ?</p>\n','','','N','Y',0,'en',0,0,NULL),(13,0,81259,4,'L','r228q7','<p>\n	What version control system do you mainly use ?</p>\n','','','Y','Y',3,'en',0,0,NULL),(14,0,81259,4,'M','r228q8','<p>\n	Which of the following platforms do you use in your code lifecycle ?</p>\n','','','N','N',5,'en',0,0,NULL),(15,0,81259,4,'L','r228q9','<p>\n	Where do you develop your site usually ?</p>\n','','','N','Y',18,'en',0,0,NULL),(16,0,81259,4,'L','r228q10','<p>\n	Where do you make critical fixes first  ?</p>\n','','','N','Y',19,'en',0,0,NULL),(17,0,81259,4,'O','q0006','<p>\n	How do you configure your modules</p>\n','','','N','Y',20,'en',0,0,NULL),(18,0,81259,4,'O','q0007','<p>\n	Do you use any type of CI ?</p>\n','','','N','Y',21,'en',0,0,NULL),(19,0,81259,5,'L','r228q11','<p>\n	What does your MySQL backend look like ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(20,0,81259,5,'Y','r228q12','<p>\n	Are you using Memcache ?</p>\n','','','N','Y',1,'en',0,0,NULL),(21,0,81259,5,'M','r228q13','<p>\n	Are you using any NoSQL Backends ?</p>\n','','','Y','Y',8,'en',0,0,NULL),(22,0,81259,5,'L','r228q14','<p>\n	What are you using as search backend ?</p>\n','','','Y','Y',12,'en',0,0,NULL),(23,0,81259,6,'L','alfapappa','How do you deploy your drupal site ?','','','Y','Y',0,'en',0,0,NULL),(24,0,81259,6,'Y','reproduce','Do you have a disaster recovery plan for your site ?\n\n','','','N','Y',2,'en',0,0,NULL),(25,0,81259,7,'Y','contact','Would you like to be contacted with the results of this survey ?','','','N','Y',0,'en',0,0,NULL),(26,0,81259,7,'S','email','On what email address should we notify you ?','/(w[-._w]*w@w[-._w]*w.w{2,3})/','','N','Y',1,'en',0,0,NULL),(27,0,81259,7,'Y','feedback','Are you interested in introducing devops ideas or continuous delivery for your organisation ?     Are you looking for help on that subject ?','','','N','Y',2,'en',0,0,NULL),(28,0,81259,1,'T','explaindevops','How would you define devops if someone asked you ? ','','','N','N',6,'en',0,0,NULL),(29,0,81259,3,'T','other','Do you use other Content Management Platforms ?','','','N','N',9,'en',0,0,NULL),(30,0,81259,6,'L','frequency','How frequently do you update / make changes to your site ? ','','','N','N',3,'en',0,0,NULL),(31,0,81259,6,'Y','cd','Have you heard about continuous delivery ?','','','N','Y',4,'en',0,0,NULL),(32,0,81259,6,'L','dploytest','Do you test your deployments ?','','','N','Y',1,'en',0,0,NULL),(33,0,81259,4,'M','doyoutest','What kind of testing do you do ?','','','Y','Y',22,'en',0,0,NULL),(34,0,81259,4,'M','testframeworks','Do you use any of the following Test Frameworks ?','','','Y','Y',23,'en',0,0,NULL),(35,0,81259,3,'L','contentediting','Where do you edit content ?','','','Y','Y',10,'en',0,0,NULL),(36,0,81259,3,'Y','ugc','Do you have user generated content ?\n','','','N','Y',11,'en',0,0,NULL),(37,0,81259,5,'M','deploy','Where are you deploying your site ?','','','Y','Y',18,'en',0,0,NULL),(38,0,81259,5,'M','httpd','On which webserver are you hosting ?','','','Y','Y',19,'en',0,0,NULL),(39,0,81259,5,'M','proxy','Are you using any kind of proxy / caching server ? ','','','Y','Y',20,'en',0,0,NULL),(40,1,81259,1,'T','1','Development/Engineering','','','N','N',3,'en',0,0,NULL),(41,1,81259,1,'T','2','QA/Other','','','N','N',4,'en',0,0,NULL),(42,1,81259,1,'T','3','Site Builder','','','N','N',1,'en',0,0,NULL),(43,1,81259,1,'T','4','IT Operations','','','N','N',2,'en',0,0,NULL),(44,14,81259,4,'T','SQ001','Development on personal workstation','','','N','N',1,'en',0,0,NULL),(45,14,81259,4,'T','SQ002','Development on a personal virtual machine on a personal workstation','','','N','N',8,'en',0,0,NULL),(46,14,81259,4,'T','SQ003','Develoment on a virtual machine that looks close to production','','','N','N',10,'en',0,0,NULL),(47,14,81259,4,'T','SQ004','A dedicated physical server for deveopment','','','N','N',12,'en',0,0,NULL),(48,14,81259,4,'T','SQ009','Test Environment','','','N','N',14,'en',0,0,NULL),(49,14,81259,4,'T','SQ005','User Acceptance Environment','','','N','N',15,'en',0,0,NULL),(50,14,81259,4,'T','SQ006','Staging Environment','','','N','N',16,'en',0,0,NULL),(51,14,81259,4,'T','SQ007','Production','','','N','N',17,'en',0,0,NULL),(52,9,81259,3,'T','1','Drupal 5','','','N','N',1,'en',0,0,NULL),(53,9,81259,3,'T','2','Drupal 6','','','N','N',2,'en',0,0,NULL),(54,9,81259,3,'T','3','Drupal 7 ','','','N','N',3,'en',0,0,NULL),(55,9,81259,3,'T','4','Drupal 8','','','N','N',4,'en',0,0,NULL),(56,9,81259,3,'T','5','Way Too Much of them ','','','N','N',5,'en',0,0,NULL),(57,6,81259,2,'T','1','We build our own site','','','N','N',1,'en',0,0,NULL),(58,6,81259,2,'T','2','We build sites for customers','','','N','N',2,'en',0,0,NULL),(59,6,81259,2,'T','3','We host sites','','','N','N',3,'en',0,0,NULL),(60,6,81259,2,'T','4','We manage, deploy and monitor sites for our customers','','','N','N',4,'en',0,0,NULL),(61,21,81259,5,'T','1','None','','','N','N',5,'en',0,0,NULL),(62,21,81259,5,'T','2','Mongo','','','N','N',10,'en',0,0,NULL),(63,21,81259,5,'T','3','Cassandra','','','N','N',14,'en',0,0,NULL),(64,21,81259,5,'T','4','Hadoop','','','N','N',16,'en',0,0,NULL),(65,21,81259,5,'T','5','Voldermort','','','N','N',17,'en',0,0,NULL),(66,33,81259,4,'T','1','Unit testing','','','N','N',2,'en',0,0,NULL),(67,33,81259,4,'T','2','Performance testing','','','N','N',3,'en',0,0,NULL),(68,33,81259,4,'T','3','Load testing','','','N','N',4,'en',0,0,NULL),(69,33,81259,4,'T','4','Exploratory testing','','','N','N',5,'en',0,0,NULL),(70,33,81259,4,'T','5','Usability testing','','','N','N',6,'en',0,0,NULL),(71,33,81259,4,'T','6','Gui tests','','','N','N',7,'en',0,0,NULL),(72,34,81259,4,'T','1','Selenium','','','N','N',2,'en',0,0,NULL),(73,34,81259,4,'T','2','Webrat','','','N','N',3,'en',0,0,NULL),(74,34,81259,4,'T','3','Cucumber','','','N','N',4,'en',0,0,NULL),(75,34,81259,4,'T','4','Jemeter','','','N','N',5,'en',0,0,NULL),(76,34,81259,4,'T','5','Sahi','','','N','N',6,'en',0,0,NULL),(77,34,81259,4,'T','6','Watir','','','N','N',7,'en',0,0,NULL),(78,34,81259,4,'T','7','Hpricot','','','N','N',8,'en',0,0,NULL),(79,34,81259,4,'T','8','Webinject','','','N','N',9,'en',0,0,NULL),(80,37,81259,5,'T','1','Your own  dedicated infrastructure','','','N','N',1,'en',0,0,NULL),(81,37,81259,5,'T','2','Shared server ','','','N','N',2,'en',0,0,NULL),(82,37,81259,5,'T','3','Private cloud','','','N','N',3,'en',0,0,NULL),(83,37,81259,5,'T','4','Amazon ','','','N','N',4,'en',0,0,NULL),(84,37,81259,5,'T','5','Another Cloud provider','','','N','N',5,'en',0,0,NULL),(85,37,81259,5,'T','6','Dedicated Drupal hoster','','','N','N',6,'en',0,0,NULL),(86,38,81259,5,'T','1','apache ','','','N','N',2,'en',0,0,NULL),(87,38,81259,5,'T','2','nginx','','','N','N',4,'en',0,0,NULL),(88,38,81259,5,'T','3','lighthttpd','','','N','N',6,'en',0,0,NULL),(89,39,81259,5,'T','1','apache ','','','N','N',1,'en',0,0,NULL),(90,39,81259,5,'T','2','nginx','','','N','N',2,'en',0,0,NULL),(91,39,81259,5,'T','3','squid','','','N','N',3,'en',0,0,NULL),(92,39,81259,5,'T','4','varnish','','','N','N',4,'en',0,0,NULL),(93,39,81259,5,'T','5','pound','','','N','N',5,'en',0,0,NULL),(94,39,81259,5,'T','6','lvs','','','N','N',6,'en',0,0,NULL),(95,33,81259,4,'T','9','None','','','N','N',1,'en',0,0,NULL),(96,34,81259,4,'T','9','None','','','N','N',1,'en',0,0,NULL),(97,39,81259,5,'T','7','None','','','N','N',7,'en',0,0,NULL),(205,0,81259,1,'S','yourcountry','What Country do you live in?','','','N','Y',10,'en',0,0,'1'),(204,0,81259,1,'L','bestdescriberole','Which of the following best describes you?','','','Y','Y',9,'en',0,0,'1'),(103,0,689125,8,'M','q0001','<p>\n	What is your role in creating a site ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(104,0,689125,8,'N','q0002','<p>\n	How many years have you been doing this kind of role ?</p>\n','','','N','Y',1,'en',0,0,NULL),(105,0,689125,8,'L','q0003','<p>\n	How often do you talk to the people in the other roles ?</p>\n','','','N','Y',3,'en',0,0,NULL),(106,0,689125,8,'L','q0005','<p>\n	Do you practice devops ?</p>\n','','','Y','Y',8,'en',0,0,NULL),(107,0,689125,9,'L','r213q0','<p>\n	How big is your organisation ?</p>\n','','','N','N',1,'en',0,0,NULL),(108,0,689125,9,'M','r213q1','<p>\n	How would  you describe your organisation ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(109,0,689125,9,'L','r213q2','<p>\n	Who typically edits the content ?</p>\n','','','Y','Y',2,'en',0,0,NULL),(110,0,689125,9,'L','q0004','<p>\n	Who typically updates your code base / monitors security fixes and applies them when needed ?</p>\n','','','Y','Y',3,'en',0,0,NULL),(111,0,689125,10,'M','r213q3','<p>\n	What Drupal Version are you using ?</p>\n','','','N','Y',0,'en',0,0,NULL),(112,0,689125,10,'L','r213q4','<p>\n	Is your current production site up to date ?</p>\n<p>\n	Have all the required security patches been applied ?</p>\n<p>\n	 </p>\n','','','N','Y',1,'en',0,0,NULL),(113,0,689125,10,'Y','r213q5','<p>\n	Are you subscribed to Drupal Security News</p>\n','','','N','Y',3,'en',0,0,NULL),(114,0,689125,11,'L','r213q6','<p>\n	Do you use any form of version control ?</p>\n','','','N','Y',0,'en',0,0,NULL),(115,0,689125,11,'L','r213q7','<p>\n	What version control system do you mainly use ?</p>\n','','','Y','Y',3,'en',0,0,NULL),(116,0,689125,11,'M','r213q8','<p>\n	Which of the following platforms do you use in your code lifecycle ?</p>\n','','','N','N',5,'en',0,0,NULL),(117,0,689125,11,'L','r213q9','<p>\n	Where do you develop your site usually ?</p>\n','','','N','Y',18,'en',0,0,NULL),(118,0,689125,11,'L','r213q10','<p>\n	Where do you make critical fixes first  ?</p>\n','','','N','Y',19,'en',0,0,NULL),(119,0,689125,11,'O','q0006','<p>\n	How do you configure your modules</p>\n','','','N','Y',20,'en',0,0,NULL),(120,0,689125,11,'O','q0007','<p>\n	Do you use any type of CI ?</p>\n','','','N','Y',21,'en',0,0,NULL),(121,0,689125,12,'L','r213q11','<p>\n	What does your MySQL backend look like ?</p>\n','','','Y','Y',0,'en',0,0,NULL),(122,0,689125,12,'Y','r213q12','<p>\n	Are you using Memcache ?</p>\n','','','N','Y',1,'en',0,0,NULL),(123,0,689125,12,'M','r213q13','<p>\n	Are you using any NoSQL Backends ?</p>\n','','','Y','Y',8,'en',0,0,NULL),(124,0,689125,12,'L','r213q14','<p>\n	What are you using as search backend ?</p>\n','','','Y','Y',12,'en',0,0,NULL),(125,0,689125,13,'L','alfapappa','How do you deploy your drupal site ?','','','Y','Y',0,'en',0,0,NULL),(126,0,689125,13,'Y','reproduce','Do you have a disaster recovery plan for your site ?\n\n','','','N','Y',2,'en',0,0,NULL),(127,0,689125,14,'Y','contact','Would you like to be contacted with the results of this survey ?','','','N','Y',0,'en',0,0,NULL),(128,0,689125,14,'S','email','On what email address should we notify you ?','/(w[-._w]*w@w[-._w]*w.w{2,3})/','','N','Y',1,'en',0,0,NULL),(129,0,689125,14,'Y','feedback','Are you interested in introducing devops ideas or continuous delivery for your organisation ?     Are you looking for help on that subject ?','','','N','Y',2,'en',0,0,NULL),(130,0,689125,8,'T','explaindevops','How would you define devops if someone asked you ? ','','','N','N',6,'en',0,0,NULL),(131,0,689125,10,'T','other','Do you use other Content Management Platforms ?','','','N','N',9,'en',0,0,NULL),(132,0,689125,13,'L','frequency','How frequently do you update / make changes to your site ? ','','','N','N',3,'en',0,0,NULL),(133,0,689125,13,'Y','cd','Have you heard about continuous delivery ?','','','N','Y',4,'en',0,0,NULL),(134,0,689125,13,'L','dploytest','Do you test your deployments ?','','','N','Y',1,'en',0,0,NULL),(135,0,689125,11,'M','doyoutest','What kind of testing do you do ?','','','Y','Y',22,'en',0,0,NULL),(136,0,689125,11,'M','testframeworks','Do you use any of the following Test Frameworks ?','','','Y','Y',23,'en',0,0,NULL),(137,0,689125,10,'L','contentediting','Where do you edit content ?','','','Y','Y',10,'en',0,0,NULL),(138,0,689125,10,'Y','ugc','Do you have user generated content ?\n','','','N','Y',11,'en',0,0,NULL),(139,0,689125,12,'M','deploy','Where are you deploying your site ?','','','Y','Y',18,'en',0,0,NULL),(140,0,689125,12,'M','httpd','On which webserver are you hosting ?','','','Y','Y',19,'en',0,0,NULL),(141,0,689125,12,'M','proxy','Are you using any kind of proxy / caching server ? ','','','Y','Y',20,'en',0,0,NULL),(142,103,689125,8,'T','1','Web Developer','','','N','N',2,'en',0,0,NULL),(143,103,689125,8,'T','2','Webmaster','','','N','N',4,'en',0,0,NULL),(144,103,689125,8,'T','3','Content Editor','','','N','N',5,'en',0,0,NULL),(145,103,689125,8,'T','4','System Administrator','','','N','N',7,'en',0,0,NULL),(146,116,689125,11,'T','SQ001','Development on personal workstation','','','N','N',1,'en',0,0,NULL),(147,116,689125,11,'T','SQ002','Development on a personal virtual machine on a personal workstation','','','N','N',8,'en',0,0,NULL),(148,116,689125,11,'T','SQ003','Develoment on a virtual machine that looks close to production','','','N','N',10,'en',0,0,NULL),(149,116,689125,11,'T','SQ004','A dedicated physical server for deveopment','','','N','N',12,'en',0,0,NULL),(150,116,689125,11,'T','SQ009','Test Environment','','','N','N',14,'en',0,0,NULL),(151,116,689125,11,'T','SQ005','User Acceptance Environment','','','N','N',15,'en',0,0,NULL),(152,116,689125,11,'T','SQ006','Staging Environment','','','N','N',16,'en',0,0,NULL),(153,116,689125,11,'T','SQ007','Production','','','N','N',17,'en',0,0,NULL),(154,111,689125,10,'T','1','Drupal 5','','','N','N',1,'en',0,0,NULL),(155,111,689125,10,'T','2','Drupal 6','','','N','N',2,'en',0,0,NULL),(156,111,689125,10,'T','3','Drupal 7 ','','','N','N',3,'en',0,0,NULL),(157,111,689125,10,'T','4','Drupal 8','','','N','N',4,'en',0,0,NULL),(158,111,689125,10,'T','5','Way Too Much of them ','','','N','N',5,'en',0,0,NULL),(159,108,689125,9,'T','1','We build our own site','','','N','N',1,'en',0,0,NULL),(160,108,689125,9,'T','2','We build sites for customers','','','N','N',2,'en',0,0,NULL),(161,108,689125,9,'T','3','We host sites','','','N','N',3,'en',0,0,NULL),(162,108,689125,9,'T','4','We manage, deploy and monitor sites for our customers','','','N','N',4,'en',0,0,NULL),(163,123,689125,12,'T','1','None','','','N','N',5,'en',0,0,NULL),(164,123,689125,12,'T','2','Mongo','','','N','N',10,'en',0,0,NULL),(165,123,689125,12,'T','3','Cassandra','','','N','N',14,'en',0,0,NULL),(166,123,689125,12,'T','4','Hadoop','','','N','N',16,'en',0,0,NULL),(167,123,689125,12,'T','5','Voldermort','','','N','N',17,'en',0,0,NULL),(168,135,689125,11,'T','1','Unit testing','','','N','N',2,'en',0,0,NULL),(169,135,689125,11,'T','2','Performance testing','','','N','N',3,'en',0,0,NULL),(170,135,689125,11,'T','3','Load testing','','','N','N',4,'en',0,0,NULL),(171,135,689125,11,'T','4','Exploratory testing','','','N','N',5,'en',0,0,NULL),(172,135,689125,11,'T','5','Usability testing','','','N','N',6,'en',0,0,NULL),(173,135,689125,11,'T','6','Gui tests','','','N','N',7,'en',0,0,NULL),(174,136,689125,11,'T','1','Selenium','','','N','N',2,'en',0,0,NULL),(175,136,689125,11,'T','2','Webrat','','','N','N',3,'en',0,0,NULL),(176,136,689125,11,'T','3','Cucumber','','','N','N',4,'en',0,0,NULL),(177,136,689125,11,'T','4','Jemeter','','','N','N',5,'en',0,0,NULL),(178,136,689125,11,'T','5','Sahi','','','N','N',6,'en',0,0,NULL),(179,136,689125,11,'T','6','Watir','','','N','N',7,'en',0,0,NULL),(180,136,689125,11,'T','7','Hpricot','','','N','N',8,'en',0,0,NULL),(181,136,689125,11,'T','8','Webinject','','','N','N',9,'en',0,0,NULL),(182,139,689125,12,'T','1','Your own  dedicated infrastructure','','','N','N',1,'en',0,0,NULL),(183,139,689125,12,'T','2','Shared server ','','','N','N',2,'en',0,0,NULL),(184,139,689125,12,'T','3','Private cloud','','','N','N',3,'en',0,0,NULL),(185,139,689125,12,'T','4','Amazon ','','','N','N',4,'en',0,0,NULL),(186,139,689125,12,'T','5','Another Cloud provider','','','N','N',5,'en',0,0,NULL),(187,139,689125,12,'T','6','Dedicated Drupal hoster','','','N','N',6,'en',0,0,NULL),(188,140,689125,12,'T','1','apache ','','','N','N',2,'en',0,0,NULL),(189,140,689125,12,'T','2','nginx','','','N','N',4,'en',0,0,NULL),(190,140,689125,12,'T','3','lighthttpd','','','N','N',6,'en',0,0,NULL),(191,141,689125,12,'T','1','apache ','','','N','N',1,'en',0,0,NULL),(192,141,689125,12,'T','2','nginx','','','N','N',2,'en',0,0,NULL),(193,141,689125,12,'T','3','squid','','','N','N',3,'en',0,0,NULL),(194,141,689125,12,'T','4','varnish','','','N','N',4,'en',0,0,NULL),(195,141,689125,12,'T','5','pound','','','N','N',5,'en',0,0,NULL),(196,141,689125,12,'T','6','lvs','','','N','N',6,'en',0,0,NULL),(197,135,689125,11,'T','9','None','','','N','N',1,'en',0,0,NULL),(198,136,689125,11,'T','9','None','','','N','N',1,'en',0,0,NULL),(199,141,689125,12,'T','7','None','','','N','N',7,'en',0,0,NULL),(200,0,81259,2,'L','orgdept','What department are you primarily engaged with?','','','Y','Y',5,'en',0,0,'1'),(209,0,81259,6,'L','recoverytime','If you were to loose a primary componet of your production site (files, database, etc.), how long do you think it would take you to recover?','','','N','Y',5,'en',0,0,'1'),(206,0,81259,1,'S','stateprovince','What Province or State do you live in?','','','N','N',11,'en',0,0,'1'),(210,0,81259,3,'M','featurelength','How long does it typically take to deliver a new feature to the site?','','','N','N',12,'en',0,0,'1'),(211,210,81259,3,'T','SQ001','Minutes',NULL,NULL,'N',NULL,1,'en',0,0,NULL),(212,210,81259,3,'T','SQ002','Hours',NULL,NULL,'N',NULL,2,'en',0,0,NULL),(213,210,81259,3,'T','SQ003','Days',NULL,NULL,'N',NULL,3,'en',0,0,NULL),(214,210,81259,3,'T','SQ004','Weeks',NULL,NULL,'N',NULL,4,'en',0,0,NULL),(215,210,81259,3,'T','SQ005','Months',NULL,NULL,'N',NULL,5,'en',0,0,NULL),(216,0,81259,4,'L','testingcoverage','What percentage of your code is covered by automated tests?','','','N','N',24,'en',0,0,'1'),(218,0,81259,1,'S','yourname','What is your name (so we know what to call you if you win the ipad)?','','','N','Y',12,'en',0,0,'1');
/*!40000 ALTER TABLE `questions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quota`
--

DROP TABLE IF EXISTS `quota`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quota` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) DEFAULT NULL,
  `name` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `qlimit` int(11) DEFAULT NULL,
  `action` int(11) DEFAULT NULL,
  `active` int(11) NOT NULL DEFAULT '1',
  `autoload_url` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `quota_idx2` (`sid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quota`
--

LOCK TABLES `quota` WRITE;
/*!40000 ALTER TABLE `quota` DISABLE KEYS */;
/*!40000 ALTER TABLE `quota` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quota_languagesettings`
--

DROP TABLE IF EXISTS `quota_languagesettings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quota_languagesettings` (
  `quotals_id` int(11) NOT NULL AUTO_INCREMENT,
  `quotals_quota_id` int(11) NOT NULL DEFAULT '0',
  `quotals_language` varchar(45) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `quotals_name` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `quotals_message` text COLLATE utf8_unicode_ci NOT NULL,
  `quotals_url` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `quotals_urldescrip` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`quotals_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quota_languagesettings`
--

LOCK TABLES `quota_languagesettings` WRITE;
/*!40000 ALTER TABLE `quota_languagesettings` DISABLE KEYS */;
/*!40000 ALTER TABLE `quota_languagesettings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quota_members`
--

DROP TABLE IF EXISTS `quota_members`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quota_members` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) DEFAULT NULL,
  `qid` int(11) DEFAULT NULL,
  `quota_id` int(11) DEFAULT NULL,
  `code` varchar(11) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `sid` (`sid`,`qid`,`quota_id`,`code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quota_members`
--

LOCK TABLES `quota_members` WRITE;
/*!40000 ALTER TABLE `quota_members` DISABLE KEYS */;
/*!40000 ALTER TABLE `quota_members` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `saved_control`
--

DROP TABLE IF EXISTS `saved_control`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `saved_control` (
  `scid` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) NOT NULL DEFAULT '0',
  `srid` int(11) NOT NULL DEFAULT '0',
  `identifier` text COLLATE utf8_unicode_ci NOT NULL,
  `access_code` text COLLATE utf8_unicode_ci NOT NULL,
  `email` varchar(254) COLLATE utf8_unicode_ci DEFAULT NULL,
  `ip` text COLLATE utf8_unicode_ci NOT NULL,
  `saved_thisstep` text COLLATE utf8_unicode_ci NOT NULL,
  `status` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `saved_date` datetime NOT NULL,
  `refurl` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`scid`),
  KEY `saved_control_idx2` (`sid`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `saved_control`
--

LOCK TABLES `saved_control` WRITE;
/*!40000 ALTER TABLE `saved_control` DISABLE KEYS */;
/*!40000 ALTER TABLE `saved_control` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `id` varchar(32) COLLATE utf8_unicode_ci NOT NULL,
  `expire` int(11) DEFAULT NULL,
  `data` longblob,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sessions`
--

LOCK TABLES `sessions` WRITE;
/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `settings_global`
--

DROP TABLE IF EXISTS `settings_global`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `settings_global` (
  `stg_name` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `stg_value` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`stg_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `settings_global`
--

LOCK TABLES `settings_global` WRITE;
/*!40000 ALTER TABLE `settings_global` DISABLE KEYS */;
INSERT INTO `settings_global` VALUES ('DBVersion','177');
/*!40000 ALTER TABLE `settings_global` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `survey_81259`
--

DROP TABLE IF EXISTS `survey_81259`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `survey_81259` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `token` varchar(36) COLLATE utf8_unicode_ci DEFAULT NULL,
  `submitdate` datetime DEFAULT NULL,
  `lastpage` int(11) DEFAULT NULL,
  `startlanguage` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `startdate` datetime NOT NULL,
  `datestamp` datetime NOT NULL,
  `ipaddr` text COLLATE utf8_unicode_ci,
  `refurl` text COLLATE utf8_unicode_ci,
  `81259X1X13` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X14` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X11` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X12` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X1other` text COLLATE utf8_unicode_ci,
  `81259X1X2` decimal(30,10) DEFAULT NULL,
  `81259X1X3` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X28` text COLLATE utf8_unicode_ci,
  `81259X1X4` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X4other` text COLLATE utf8_unicode_ci,
  `81259X1X204` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X1X204other` text COLLATE utf8_unicode_ci,
  `81259X1X205` text COLLATE utf8_unicode_ci,
  `81259X1X206` text COLLATE utf8_unicode_ci,
  `81259X1X218` text COLLATE utf8_unicode_ci,
  `81259X2X61` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X62` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X63` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X64` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X6other` text COLLATE utf8_unicode_ci,
  `81259X2X5` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X7` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X7other` text COLLATE utf8_unicode_ci,
  `81259X2X8` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X8other` text COLLATE utf8_unicode_ci,
  `81259X2X200` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X2X200other` text COLLATE utf8_unicode_ci,
  `81259X3X91` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X92` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X93` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X94` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X95` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X10` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X11` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X29` text COLLATE utf8_unicode_ci,
  `81259X3X35` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X35other` text COLLATE utf8_unicode_ci,
  `81259X3X36` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X210SQ001` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X210SQ002` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X210SQ003` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X210SQ004` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X3X210SQ005` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X12` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X13` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X13other` text COLLATE utf8_unicode_ci,
  `81259X4X14SQ001` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ002` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ003` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ004` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ009` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ005` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ006` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X14SQ007` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X15` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X16` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X17` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X17comment` text COLLATE utf8_unicode_ci,
  `81259X4X18` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X18comment` text COLLATE utf8_unicode_ci,
  `81259X4X339` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X331` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X332` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X333` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X334` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X335` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X336` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X33other` text COLLATE utf8_unicode_ci,
  `81259X4X349` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X341` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X342` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X343` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X344` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X345` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X346` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X347` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X348` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X4X34other` text COLLATE utf8_unicode_ci,
  `81259X4X216` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X19` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X19other` text COLLATE utf8_unicode_ci,
  `81259X5X20` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X211` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X212` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X213` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X214` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X215` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X21other` text COLLATE utf8_unicode_ci,
  `81259X5X22` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X22other` text COLLATE utf8_unicode_ci,
  `81259X5X371` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X372` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X373` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X374` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X375` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X376` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X37other` text COLLATE utf8_unicode_ci,
  `81259X5X381` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X382` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X383` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X38other` text COLLATE utf8_unicode_ci,
  `81259X5X391` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X392` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X393` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X394` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X395` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X396` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X397` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X5X39other` text COLLATE utf8_unicode_ci,
  `81259X6X23` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X6X23other` text COLLATE utf8_unicode_ci,
  `81259X6X32` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X6X24` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X6X30` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X6X31` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X6X209` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X7X25` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `81259X7X26` text COLLATE utf8_unicode_ci,
  `81259X7X27` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_survey_token_81259_24684` (`token`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `survey_81259`
--

LOCK TABLES `survey_81259` WRITE;
/*!40000 ALTER TABLE `survey_81259` DISABLE KEYS */;
INSERT INTO `survey_81259` VALUES (7,NULL,'2014-04-08 18:06:43',7,'en','2014-04-08 17:55:11','2014-04-08 18:06:43','198.91.149.40',NULL,'Y','Y','Y','Y','Project Manager',6.0000000000,'A3','Treating the entire stack as the service/product deliverable.','A2','','A1','','Canada','Quebec','','Y','Y','Y','Y','','A1','-oth-','Clients\' staff','A1','','A2','','','Y','Y','Y','','A1','Y','no','3','','N','','Y','','','','A2','A5','','','Y','Y','Y','Y','','','Y','A2','A3','A3','a combination of these, as well as Puppet','A2','','','','Y','','','','','BDD','','','','Y','','','','','','','A3','A1','','N','Y','','','','','','A2','','Y','','','Y','','','','Y','Y','','','','Y','','Y','','','','','-oth-','Aegir','A2','Y','3','Y','A1','Y','','N'),(6,NULL,'2014-04-08 17:50:28',7,'en','2014-04-08 17:40:59','2014-04-08 17:50:28','64.19.101.243',NULL,'Y','Y','Y','','',6.0000000000,'A1','Managing IT operations using proven software development principles','A2','','A2','','United States','Oklahoma','','','Y','Y','Y','','A1','A1','','A1','','A2','','','','Y','Y','','A1','Y','No.','3','','Y','Y','','','','','A1','A5','','','','Y','','Y','','Y','Y','A1','A1','A2','We use update hooks in site deployment modules to deploy configuration changes.','A2','','','Y','Y','Y','','','','','','','','','','','','','','PHPUnit, Loader.io','A5','A3','','Y','Y','','','','','','A3','','','','','','','Y','','','Y','','','','Y','','Y','','','','','-oth-','Site deployment module','A1','Y','3','Y','A1','Y','','N'),(3,NULL,NULL,NULL,'en','2014-04-08 09:40:40','2014-04-08 09:40:40','84.19.42.82','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(4,NULL,NULL,NULL,'en','2014-04-08 14:48:15','2014-04-08 14:48:15','174.16.204.139','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(5,NULL,'2014-04-08 15:58:10',7,'en','2014-04-08 15:53:07','2014-04-08 15:58:10','65.189.246.220',NULL,'Y','Y','Y','','',4.0000000000,'A3','Sysadmin + developer','A3','','A3','','USA','Ohio','','Y','Y','','Y','','A3','A1','','A3','','A2','','','Y','Y','','','A2','Y','No','3','','Y','Y','','','','','A1','A5','','Y','Y','Y','','Y','Y','Y','Y','A2','A1','A3','','A1','','','','','','','','Y','','Y','','','','','','','','','','A6','A1','','Y','Y','','','','','','A2','','','','','Y','Y','Y','','Y','','','','Y','','','Y','','','','','A9','','A1','N','3','N','A2','Y','','N'),(8,NULL,'2014-04-08 18:16:22',7,'en','2014-04-08 18:07:45','2014-04-08 18:16:22','65.211.39.138',NULL,'','Y','','','Responsible for providing Hosting and managing custom Drupal distribution',2.0000000000,'A2','Process involved in developing and maintaining server side hosting','A2','','A1','','US','New Jersey','','','','Y','','We ooutsource development of 50 brand sites and associated Internet properties e.g. FaceBook, Phone Apps','A5','A1','','-oth-','Developers at external agencies','-oth-','Corporate Communications (I know it sounds weird)','','Y','Y','','','A4','Y','Standardizing on Drupal - have some legacy platforms Symfony, HTML, straight PHP, WordPress, Typo3, SilverStripe','2','','Y','','','','Y','','A1','A5','','Y','Y','Y','','Y','Y','Y','Y','A1','A1','A1','We use all of these methods','A1','','','','Y','Y','','Y','','','Y','','','','','','','','','','A2','-oth-','Simple Master/Slave - should have been an option on list','Y','Y','','','','','','-oth-','I have mutiple sites - some Solr, some simple','','','','Y','','','','Y','','','','Y','','','Y','','','','','A9','','A1','Y','2','Y','A2','Y','','N'),(9,NULL,'2014-04-08 19:51:00',7,'en','2014-04-08 18:16:20','2014-04-08 19:51:00','199.84.183.2',NULL,'Y','','Y','','',5.0000000000,'A2','Development methodologies and tools applied to deployment and maintenance processes.','A2','','A1','','Canada','Quebec','','','Y','Y','Y','','A3','A2','','A1','','A2','','','Y','Y','','','A2','Y','','3','','N','','Y','Y','','','A2','A5','','Y','Y','Y','Y','Y','Y','Y','Y','A2','A1','A3','','A2','','','','','','','','','Syntax check and static analysis','','Y','','','','','','','','','A2','A1','','Y','Y','','','','','','A2','','Y','','','','Y','','','Y','Y','','','','','','Y','','','','HAProxy','A9','','A1','Y','5','Y','A2','N','','N'),(10,NULL,NULL,NULL,'en','2014-04-08 18:24:50','2014-04-08 18:24:50','175.103.25.98',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(11,NULL,'2014-04-08 18:36:45',7,'en','2014-04-08 18:27:41','2014-04-08 18:36:45','198.129.37.70',NULL,'Y','Y','Y','Y','',12.0000000000,'A5','Breaking down the barriers between the people that write the code and the people that maintain and tend to the machines. Only by learning and understanding what \'those other guys\' do and how they work can we be effective at delivering great service and products. ','A2','','A1','','United States','California','','Y','Y','Y','Y','','A5','A1','','A1','','A2','','','Y','Y','Y','','A1','Y','Sharepoint, Confluence, Wordpress','1','','Y','','','','Y','','A1','A5','','Y','Y','','Y','Y','','Y','Y','A2','A1','A3','Drush','A1','','','Y','Y','Y','','Y','','','Y','','','','','','','','','','A7','A2','','Y','','Y','','','','','-oth-','idol :(','Y','','','','','','','Y','','','','','','','','','','','Load Balancer','-oth-','Aegir','A1','Y','1','Y','A1','Y','','Y'),(12,NULL,'2014-04-08 18:34:32',7,'en','2014-04-08 18:29:39','2014-04-08 18:34:32','204.115.117.128',NULL,'Y','Y','Y','','',15.0000000000,'A2','','A3','','A1','','US','CO','','Y','Y','Y','Y','','A3','A2','','A1','','A1','','Y','Y','Y','','','A1','Y','yes!','2','','Y','','Y','','','','A1','A5','','','','','','','','','','A1','A3','A3','','A5','','','Y','Y','Y','','Y','','','','Y','','Y','Y','','','','Y','','A3','A2','','Y','','Y','','Y','','','A3','','','','Y','Y','Y','','','Y','Y','','','','Y','','Y','','','','','-oth-','CI','A2','Y','2','Y','A1','Y','','N'),(13,NULL,'2014-04-08 18:37:42',7,'en','2014-04-08 18:30:45','2014-04-08 18:37:42','84.112.182.213',NULL,'Y','Y','Y','Y','',8.0000000000,'A3','','A3','','A2','','Austria','','','','Y','','','','A1','A1','','A1','','A2','','','Y','Y','Y','','A1','Y','-','3','','Y','','','Y','','','A1','A5','','','Y','','','Y','','Y','Y','A2','A3','A3','','A2','','Y','','','','','','','','','','','','','','','','','behat','A6','A1','','Y','Y','','','','','','A2','','Y','','','','','Y','','Y','Y','','','','','','Y','','','','','A4','','A1','N','3','Y','A2','Y','','N'),(14,NULL,NULL,1,'en','2014-04-08 18:32:35','2014-04-08 18:34:21','82.25.20.15','http://cyberswat.limequery.com/index.php/81259/lang-en','','Y','','','',7.0000000000,'A3','http://en.wikipedia.org/wiki/DevOps ;-)','A3','','A2','','UK','Cardiff','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(15,NULL,'2014-04-08 18:43:56',7,'en','2014-04-08 18:33:09','2014-04-08 18:43:56','75.71.147.131','http://cyberswat.limequery.com/index.php/81259/lang-en','Y','Y','Y','','',14.0000000000,'A3','It\'s a mix of a few different things:\r\n* applying software development lifecycle to operations\r\n* automating everything and making it easy to know if things are done right/not (automated testing, automated backups, continuous integration/deployment)\r\n* encouraging closer collaboration between operations and other organizations (especially development)\r\n* increasing visibility of operations work','A2','','A1','','USA','Colorado','','Y','','','','','A2','A1','','A1','','A2','','','','Y','','','A1','Y','Yes.','3','','N','Y','Y','Y','Y','Y','A1','A5','','Y','Y','','','','','','Y','A2','A1','A3','We use a mix of features and hook_update_n functions to set variables and $conf in settings.php','A2','','','','','','','','','Behat','','','','','','','','','','Behat/Mink/DrupalExtension','A2','A2','','N','','','','','','Redis','A4','','','','','Y','','','','Y','','','','','','','','','','','CloudFront for static assets','-oth-','Features + hook_update_n and several drush commands in a jenkins script','A1','Y','1','Y','A2','Y','','Y'),(16,NULL,'2014-04-08 19:02:02',7,'en','2014-04-08 18:53:13','2014-04-08 19:02:02','192.0.35.0','http://cyberswat.limequery.com/index.php/81259/lang-en','','Y','Y','','',5.0000000000,'A3','\"Make it easy for everyone else involved in the site development (programming, theming, site building) to do their job.','A2','','A1','','US','CA','','Y','Y','Y','Y','','A1','A1','','A1','','A2','','','Y','Y','Y','','A1','Y','Wordpress','3','','Y','Y','Y','','','','A1','A5','','','','','','','','','Y','A2','A1','A3','A combination of all three','A2','','','','Y','','Y','','','Upgrade Path','','Y','','','','','','','','','A2','-oth-','RDS*','Y','','Y','','','','','A2','','','','','Y','','','','Y','Y','','','','Y','','Y','','','','','A9','','A2','Y','2','Y','A1','N','','Y'),(17,NULL,NULL,NULL,'en','2014-04-08 19:22:01','2014-04-08 19:22:01','209.251.144.150',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(18,NULL,'2014-04-08 19:46:14',7,'en','2014-04-08 19:23:47','2014-04-08 19:46:14','24.9.180.145',NULL,'Y','','','','',3.0000000000,'A1','Programmatically creating and configuring environments in which to deliver websites and applications','A3','','A3','','USA','CO','','','Y','','','','A1','A1','','-oth-','Me','A2','','','','Y','','','A1','Y','nope','-oth-','I don\'t do much content; clients edit content in production','N','Y','','','','','A1','A5','','','Y','','','','','Y','Y','A1','A1','A3','','A1','','Y','','','','','','','','Y','','','','','','','','','','A6','-oth-','No idea','N','Y','','','','','','A4','','','Y','','','','','','Y','','','','','','','','','','Y','','A2','','A3','N','5','N','A1','Y','','Y'),(19,NULL,'2014-04-08 20:16:08',7,'en','2014-04-08 20:04:47','2014-04-08 20:16:08','157.166.159.230',NULL,'Y','','Y','','Sys Admin',4.0000000000,'A3','An Agile-like perspective to software production, that empowers the promotion of functionality from idea to customer use in a continuous manner, utilizing a set of tools and processes that streamline the promotion while ensuring quality delivery. ','A3','','A1','','USA','Florida','','Y','Y','Y','Y','','A5','A1','','A1','','A2','','','','Y','','','A1','N','Zend, Symphony, Sharepoint, WordPress','3','','Y','','','Y','','','A1','A5','','','Y','','Y','','Y','Y','Y','A2','A1','A2','code, modules, features, deployment scripts','A2','looking at bamboo','','Y','','Y','','','','Pen test','','Y','','','','','','','','','A2','A2','','Y','Y','','','','','','A2','','Y','','','Y','','','Aquia','Y','Y','','','Y','Y','','Y','','','','','-oth-','features, drush and deploy scripts','A1','Y','4','Y','A2','Y','','Y'),(20,NULL,'2014-04-08 20:51:46',7,'en','2014-04-08 20:40:59','2014-04-08 20:51:46','107.203.168.41',NULL,'Y','Y','Y','','',5.0000000000,'A1','all of the system level work; web server setup and optimization, mysql optimization, any caching mechanisms that need to be applied, any unique libraries that have to be installed on the server, etc.','A3','','A3','','United States','Ohio','','','Y','Y','Y','','A1','-oth-','client','-oth-','me','-oth-','All','','','Y','','','A1','Y','WordPress','-oth-','client adds and edits on live site','Y','','Y','Y','Y','','A1','A5','','Y','Y','','Y','','','Y','Y','A2','A1','A3','and Context','A1','I want to but have not had the time to properly set it up yet','','Y','','Y','','Y','','','','','','','','','','','','New Relic analysis','A4','A1','','Y','Y','','','','','','A4','','Y','Y','Y','','Y','Y','','Y','Y','','','','','Y','Y','','','','','A4','','A1','Y','4','Y','A2','Y','','Y'),(21,NULL,NULL,NULL,'en','2014-04-08 21:03:28','2014-04-08 21:03:28','68.170.73.51',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(22,NULL,'2014-04-08 22:03:01',7,'en','2014-04-08 21:55:16','2014-04-08 22:03:01','186.77.197.249',NULL,'Y','','Y','','',4.0000000000,'A2','','A3','','A3','','Nicaragua','Leon','','','Y','','','','A1','A3','','A1','','A2','','','','Y','Y','','A3','Y','','3','','Y','','','Y','','','A1','A5','','Y','','','Y','','','Y','Y','A2','A1','A3','','A2','','','Y','Y','','','','','','','Y','','','','','','','','','A7','A1','','N','','','','','','Redis','A2','','','','','','Y','','','Y','Y','','','','','','Y','','','','','A4','','A1','N','','Y','A2','Y','','Y'),(23,NULL,NULL,NULL,'en','2014-04-09 01:44:23','2014-04-09 01:44:23','173.14.19.34',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(24,NULL,'2014-04-09 15:24:50',7,'en','2014-04-09 15:16:36','2014-04-09 15:24:50','182.72.78.214','http://cyberswat.limequery.com/index.php/81259/lang-en','Y','','','','',2.5000000000,'A2','communication, collaboration and integration','A2','','A1','','India','Maharashtra','','','Y','','Y','','A4','A1','','A1','','A2','','','Y','Y','','','A2','Y','No','2','','Y','','Y','','','','A1','A5','','Y','Y','Y','Y','Y','','Y','Y','A1','A1','A3','','A1','','','Y','Y','Y','Y','Y','Y','','','Y','','','','','','','','','A3','A4','','N','Y','','','','','','A4','','','Y','Y','','','Y','','Y','','','','Y','','','','','','','','A9','','A1','Y','5','Y','A2','Y','','N'),(25,NULL,'2014-04-09 16:15:12',7,'en','2014-04-09 16:06:08','2014-04-09 16:15:12','128.255.79.83',NULL,'Y','Y','Y','Y','',4.0000000000,'A2','Shared ownership of the quality and avaliability of code and platform','A3','','A1','','USA','Iowa','','','Y','','','','A2','A1','','A1','','A2','','','','Y','','','A1','Y','No','3','','N','','','Y','','','A1','A5','','Y','','','','Y','Y','','Y','A2','A1','A3','','A2','','Y','','','','','','','','Y','','','','','','','','','','A6','A2','','Y','Y','','','','','','-oth-','GSA','Y','','','','','','','Y','','','','Y','','','Y','','','','','A4','','A3','Y','5','Y','A2','Y','','Y'),(26,NULL,NULL,NULL,'en','2014-04-09 21:50:25','2014-04-09 21:50:25','69.165.140.72','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(27,NULL,NULL,NULL,'en','2014-04-10 02:31:16','2014-04-10 02:31:16','74.69.5.66','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(28,NULL,NULL,NULL,'en','2014-04-10 18:16:05','2014-04-10 18:16:05','38.88.176.122','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(29,NULL,NULL,NULL,'en','2014-04-10 19:05:45','2014-04-10 19:05:45','38.88.176.122',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(30,NULL,NULL,NULL,'en','2014-04-10 19:11:31','2014-04-10 19:11:31','129.97.108.31',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(31,NULL,NULL,NULL,'en','2014-04-11 00:01:38','2014-04-11 00:01:38','89.23.241.240','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(32,NULL,'2014-04-11 00:13:18',7,'en','2014-04-11 00:05:12','2014-04-11 00:13:18','50.201.154.94','http://cyberswat.limequery.com/index.php/81259/lang-en','Y','','Y','','Front-end',4.0000000000,'A3','Devops is the process of removing repetition and human error from deployments. It\'s about automating things that need to be done consistently on a project.','A3','','A1','','US','Colorado','','Y','Y','Y','','','A3','A1','','A3','','A3','','','Y','Y','','','A1','Y','','3','','Y','Y','Y','','','','A1','A5','','Y','','','','Y','','Y','Y','A2','A1','A1','There\'s been some push to move more toward features. But we\'re not using them 100%','A1','','','','Y','','Y','Y','Y','','Y','','','','','','','','','','A7','A1','','Y','Y','','','','','','A4','','','','','','','Y','','Y','','','','','','','Y','','','','','A9','','A1','Y','5','Y','A2','Y','','Y'),(33,NULL,'2014-04-11 00:35:08',7,'en','2014-04-11 00:08:06','2014-04-11 00:35:08','82.225.93.102','http://cyberswat.limequery.com/index.php/81259/','Y','','Y','','',5.0000000000,'A2','practices that invites all the people (technical / functional / testing, etc) taking part in the creation of an application to be part of it since the start, to share tools, methodology, vocabulary. The idea is also to have all this people sharing the same goal and allowing each part understanding the specifics tasks of each others works.','A2','','A1','','France','','','','Y','','','','A1','A1','','-oth-','us','A2','','','','Y','','','A1','Y','','3','','Y','Y','Y','Y','Y','','A1','A5','','Y','Y','','','Y','Y','','Y','A2','A1','A3','','A2','','','','','','','Y','Y','','Y','','','','','','','','','','','A1','','Y','Y','','','','','','A2','','Y','','','','','Y','','Y','Y','','','','Y','','Y','','','','','A4','','A1','N','2','Y','A2','N','','N'),(34,NULL,NULL,NULL,'en','2014-04-11 00:09:50','2014-04-11 00:09:50','75.144.242.34','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(35,NULL,'2014-04-11 00:17:34',7,'en','2014-04-11 00:10:00','2014-04-11 00:17:34','79.255.109.103','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','','',3.0000000000,'A2','Automation of environments. Configuration, monitoring. Reusable, reproducible, defined state of stuff.','A2','','A1','','Germany','','','','Y','Y','Y','','A3','A1','','A1','','A2','','','Y','Y','Y','','A1','Y','TYPO3, Magento, SharePoint','3','','Y','','','Y','Y','','A1','A5','','Y','','','','Y','','Y','Y','A2','A1','A3','','A2','','','','','','Y','','','','Y','','','','','','','','','','A2','A1','','N','','','','','','Redis','A2','','Y','','Y','','Y','','','Y','','','','','','','Y','','','','','A4','','A1','Y','4','Y','A2','Y','','N'),(36,NULL,'2014-04-11 00:16:43',7,'en','2014-04-11 00:10:31','2014-04-11 00:16:43','68.111.252.75','http://cyberswat.limequery.com/index.php/81259/','','','Y','','',8.0000000000,'A1','ops of dev','A3','','A3','','USA','CA','','','Y','','','','A1','A1','','A2','','-oth-','marketing','','','Y','','','A1','Y','Yes.','3','','Y','','','','Y','','A1','A5','','Y','','','','Y','','Y','','A2','A1','A3','Only so far as features can be used.','A2','For some projects.','','','','','','Y','Y','','Y','','','','','','','','','','A6','A2','','Y','Y','','','','','','A2','','','','','','','Y','','Y','Y','','','','Y','','Y','','','','','A4','','A2','Y','1','Y','A1','Y','','N'),(37,NULL,'2014-04-11 00:16:47',7,'en','2014-04-11 00:10:39','2014-04-11 00:16:47','216.9.100.5','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','Y','',9.0000000000,'A3','Developer who also has to manage some server layers','A3','','A1','','United States','California','','Y','Y','Y','Y','','A5','A1','','A1','','A2','','','Y','Y','','','A1','Y','ColdFusion\r\nSharepoint\r\nJava (custom framework)','3','','Y','','','','','Y','A1','A5','','Y','Y','Y','','Y','Y','','Y','A2','A1','A1','','A1','','','','Y','','Y','Y','Y','','','Y','','','Y','','','','','WAPT','A4','A1','','Y','Y','','','','','','A4','','Y','','','','','','','Y','','','','Y','','','','','','','Zend OPcache','A9','','A2','Y','5','N','A2','N','','Y'),(38,NULL,'2014-04-11 00:16:57',7,'en','2014-04-11 00:11:40','2014-04-11 00:16:57','173.12.5.73','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','Y','I run the business, and wear all the hats.',5.0000000000,'A1','The process of moving code / content  between the development environment to the production environment, in a way that minimizes confusion and overhead, streamlining the tasks to make your time BUILDING sites better.','A2','','-oth-','all of the above.','USA','PA','','','Y','Y','Y','','A1','A1','','A1','','-oth-','all of the above.','','Y','Y','Y','','A3','N','no','3','','Y','','','Y','','','A1','A5','','Y','Y','','','Y','','Y','Y','A2','A1','A3','','A1','','','','','','','Y','Y','','Y','','','','','','','','','','A7','A1','','N','Y','','','','','','A4','','','','','','Y','Y','','Y','Y','','','Y','','','Y','','','','','A4','','A2','Y','3','Y','A2','Y','','Y'),(39,NULL,NULL,NULL,'en','2014-04-11 00:12:02','2014-04-11 00:12:02','173.252.42.122','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(40,NULL,'2014-04-11 00:27:50',7,'en','2014-04-11 00:19:10','2014-04-11 00:27:50','128.138.79.85','http://cyberswat.limequery.com/index.php/81259/lang-en','Y','','Y','Y','',8.0000000000,'A2','Automate all the things!','A2','','A1','','USA','Colorado','','Y','Y','Y','Y','','A5','A1','','A1','','A2','','','Y','Y','','','A1','Y','A handful of WP sites.','3','','Y','','','','Y','','A1','A5','','Y','','','Y','Y','','Y','Y','A2','A1','A3','Features for when it works, otherwise other methods of getting configs in to code.','A2','','','','Y','Y','','','Y','','','Y','','','','','','','','Behat','A3','A3','','Y','','Y','','','','','-oth-','Google Search Appliance','Y','','','','','','','Y','','','','','','','Y','','','','','A4','','A2','Y','1','Y','A2','Y','','N'),(41,NULL,'2014-04-11 00:30:28',7,'en','2014-04-11 00:23:49','2014-04-11 00:30:28','194.213.192.9','http://cyberswat.limequery.com/index.php/81259/','','','Y','','',3.0000000000,'A1','knows the beast and understands its needs, manages to provide them, too.','A2','','A3','','czech republic','','','','Y','Y','Y','','A1','A1','','A1','','A2','','','','Y','Y','','A2','Y','','3','','Y','','Y','Y','','','A1','A5','','','Y','Y','','','','Y','Y','A2','A1','A3','','A1','','','','','','','Y','Y','','Y','','','','','','','','','','A7','A1','','Y','Y','','','','','','A2','','Y','','','','','','','Y','','','','','','','','','','Y','','A4','','A2','Y','1','Y','A2','Y','','Y'),(42,NULL,NULL,NULL,'en','2014-04-11 00:31:27','2014-04-11 00:31:27','75.164.147.6','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(43,NULL,NULL,NULL,'en','2014-04-11 01:28:05','2014-04-11 01:28:05','75.177.184.244',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(44,NULL,NULL,NULL,'en','2014-04-11 01:35:49','2014-04-11 01:35:49','188.220.122.37','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(45,NULL,NULL,NULL,'en','2014-04-11 02:12:36','2014-04-11 02:12:36','202.89.190.71','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(46,NULL,NULL,NULL,'en','2014-04-11 04:37:52','2014-04-11 04:37:52','80.101.46.215','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(47,NULL,NULL,NULL,'en','2014-04-11 04:38:10','2014-04-11 04:38:10','71.123.193.109','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(48,NULL,NULL,NULL,'en','2014-04-11 05:22:14','2014-04-11 05:22:14','97.89.138.247',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(49,NULL,'2014-04-11 06:08:12',7,'en','2014-04-11 05:46:37','2014-04-11 06:08:12','76.105.147.85',NULL,'Y','Y','Y','','',6.0000000000,'A3','coordination between dev and sys admin.  less throwing stuff over the wall, more team playing and holistic understanding of the app/code/stack','-oth-','I work within my client\'s needs and limits','A3','','US','Oregon','','','Y','','','','A1','-oth-','Client, usually non-technical','A1','','-oth-','Usually all depts who touch the site','','','Y','','','A1','Y','No, but I code in rails too','3','','Y','','','Y','Y','','A1','A5','','Y','Y','','Y','Y','','Y','Y','A1','A1','A2','I don\'t do a lot of site building and don\'t create features for clients who can\'t maintain them.  I use them when applicable','A2','Current primary client has bamboo.','','Y','Y','Y','','','','','Y','','','','','','','','','','A2','A2','','Y','','Y','','','','','A2','','Y','','','Y','Y','','','Y','Y','','','','','','Y','','','','','-oth-','','A1','Y','4','Y','A2','Y','','N'),(50,NULL,'2014-04-11 07:35:28',7,'en','2014-04-11 07:21:08','2014-04-11 07:35:28','87.72.204.63','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','Y','',10.0000000000,'A3','Fullstack-backend developer','-oth-','is required to do my job properly.','A1','','Denmark','','','Y','Y','Y','Y','','A3','-oth-','Frontend devs, managers or customers','-oth-','devops','A2','','','Y','Y','Y','','A3','Y','Yes, among others magento, umbraco, wordpress etc. basically the best tool for the job at hand.','3','','Y','Y','Y','Y','','','A1','A5','','','Y','Y','Y','Y','Y','','Y','A2','A1','A3','All configuration which are not meant for the customer to change are placed in features & custom install/update function and created so that it restores the site when doing a site install.','A2','Wish there where a more resource effecient system that could be self hosted.','','','Y','','Y','Y','Y','dedicated testing personal as well as developer testing','Y','','','','','','','','','','A6','A1','','Y','','Y','','','','','A2','','Y','','','','','','','Y','Y','','','','Y','','Y','','','','','-oth-','one click deploys using jenkins, features and update hooks.','A1','Y','3','Y','A2','Y','','Y'),(51,NULL,NULL,NULL,'en','2014-04-11 07:58:12','2014-04-11 07:58:12','94.191.188.207','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(52,NULL,'2014-04-11 08:20:33',7,'en','2014-04-11 08:08:31','2014-04-11 08:20:33','131.207.242.5','http://cyberswat.limequery.com/index.php/81259/','','','Y','','',6.0000000000,'A3','Devops is a combination of development and operations (hence the name). In practice devops handle all things regarding automations, deployments and such alongside their normal development work.','A3','','A1','','Finland','','','Y','Y','Y','Y','Support, end-to-end','A5','A1','','-oth-','Application Support team','A2','','Y','Y','Y','Y','Y','A1','Y','Not really. Looking into static site generators for microsites.','3','','Y','','','Y','Y','Y','A1','A5','','Y','Y','Y','','Y','Y','Y','Y','A2','A3','A2','','A4','','','Y','Y','Y','Y','Y','Y','','','Y','','Y','Y','','','','','','','A3','','Y','','Y','','','','','A2','','Y','','Y','','Y','Y','','Y','Y','','','','Y','','Y','','','','','A4','','A1','Y','4','Y','A1','Y','','N'),(53,NULL,'2014-04-11 08:33:57',7,'en','2014-04-11 08:25:35','2014-04-11 08:33:57','117.198.84.75','http://cyberswat.limequery.com/index.php/81259/','Y','','Y','','',4.0000000000,'A1','','A4','','A1','','India','Maharashtra','','','Y','','Y','','A1','A3','','A1','','A2','','','','Y','','','A1','Y','NO','3','','Y','','Y','Y','','','A2','A5','','Y','','','','','','Y','Y','A2','A3','A1','','A1','','','Y','Y','Y','','','','','Y','','','','','','','','','','A2','A1','','N','Y','','','','','','A2','','','','','','Y','','','Y','','','','','','','','','','Y','','A9','','A2','Y','5','N','A1','Y','','Y'),(54,NULL,NULL,NULL,'en','2014-04-11 08:32:18','2014-04-11 08:32:18','79.213.81.243',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(55,NULL,NULL,3,'en','2014-04-11 09:46:38','2014-04-11 09:54:35','193.88.64.250','http://cyberswat.limequery.com/index.php/81259/','','Y','','','',15.0000000000,'A3','','-oth-','yes','A1','','Denmark','Copenhagen','','','Y','Y','Y','','A3','-oth-','the client','-oth-','collaboration between developer/operations','A1','','','Y','Y','','','A4','Y','wordpress, magento, custom','3','','Y','Y','Y','Y','','','A1','A5','','Y','Y','Y','','Y','','','Y','A1','A1','A3','','A2','','Y','','','','','','','','Y','','','','','','','','','','A7',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(56,NULL,NULL,NULL,'en','2014-04-11 12:47:34','2014-04-11 12:47:34','101.169.42.144',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(57,NULL,NULL,NULL,'en','2014-04-11 13:06:45','2014-04-11 13:06:45','173.69.134.202',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(58,NULL,'2014-04-11 14:25:08',7,'en','2014-04-11 14:13:00','2014-04-11 14:25:08','184.57.3.183',NULL,'','Y','','','',1.0000000000,'A3','Mentors and facilitators, operationally responsible for maintaining the deployment process. ','A2','','A1','','USA ','Ohio','','Y','','','','','A4','A1','','A1','','A2','','','','Y','','','A2','Y','Nope','-oth-','Mostly in production, but we do also employ a content syndication site. ','N','','','Y','Y','','A1','A5','','','Y','Y','','','','Y','Y','A2','A1','A3','We actually are transitioning to config in code, so there are still instances of all three options. :-(','A1','','','','','','','Y','Y','','Y','','','','','','','','','','A7','A3','','Y','Y','','','','','','A4','','','','Y','','','','','Y','','','','','','','','','','Y','','A9','','A1','Y','4','Y','A2','Y','','Y'),(59,NULL,'2014-04-11 16:40:23',7,'en','2014-04-11 16:34:19','2014-04-11 16:40:23','208.108.110.254','http://cyberswat.limequery.com/index.php/81259/','Y','','','','',5.0000000000,'A1','','A4','','A1','','US','OH','','Y','Y','Y','Y','','A1','-oth-','Customers','-oth-','Me','-oth-','All','','Y','Y','','','A4','N','Primarily Drupal - some WordPress.','3','','Y','','','Y','Y','','A3','A1','','','','','','Y','','Y','Y','A2','A2','A3','','A1','','Y','','','','','','','','Y','','','','','','','','','','A7','A1','','Y','Y','','','','','','A4','','Y','','','','','','','Y','','','','','','','Y','','','','','A1','','A2','Y','2','N','A2','N','','Y'),(60,NULL,NULL,NULL,'en','2014-04-11 18:57:24','2014-04-11 18:57:24','188.189.67.106',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(61,NULL,NULL,NULL,'en','2014-04-11 21:53:34','2014-04-11 21:53:34','174.101.190.4','http://cyberswat.limequery.com/index.php/81259/',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(62,NULL,'2014-04-12 14:47:30',7,'en','2014-04-12 14:38:00','2014-04-12 14:47:30','86.21.10.248',NULL,'Y','Y','Y','','',10.0000000000,'A1','Infrastructure setup and maintenance ','A4','','A1','','United Kingdom ','Nottinghamshire ','','Y','Y','Y','','','A1','A2','','A3','','A2','','','Y','Y','Y','','A1','Y','','3','','N','','Y','','','','A1','A5','','Y','Y','Y','','','','Y','Y','A1','A1','A1','','A1','','','','','','Y','Y','Y','','Y','','','','','','','','','','A7','A1','','N','Y','','','','','','A4','','','','','','','','Linode VPS','Y','','','','','','','','','','Y','','A9','','A2','N','4','N','A2','N','','N'),(63,NULL,'2014-04-12 20:07:00',7,'en','2014-04-12 19:57:53','2014-04-12 20:07:00','66.201.52.99','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','','',10.0000000000,'A3','Standardization of development environments and deployment workflows to facilitate delivery on any schedule.','A3','','A2','','United States','Idaho','','Y','Y','','Y','','A4','A1','','-oth-','Developers','A2','','','Y','Y','Y','','A1','Y','Wordpress','-oth-','Depends on the site - usually in prod','Y','Y','Y','','','','A1','A5','','Y','','Y','Y','Y','Y','Y','Y','A2','A1','A3','','A5','It\'s a poor-mans CI system: A post receive hook in the repo that deploys code to the web root, runs drush updb && && drush fr-all && drush cc all','','','Y','Y','','','','','Y','','','','','','','','','But we\'re looking into it.','A6','-oth-','Depends on the site. Usually single, master-slave, or master-master. No mysql proxy right now.','Y','Y','','','','','','A2','','','Y','','','Y','Y','','Y','Y','','','','','','Y','','','','','A4','','A1','Y','1','Y','A1','Y','','N'),(64,NULL,'2014-04-13 03:08:13',7,'en','2014-04-13 03:00:38','2014-04-13 03:08:13','216.80.25.3','http://cyberswat.limequery.com/index.php/81259/','Y','Y','Y','Y','',5.0000000000,'A1','Modern best practices for putting websites on servers','A3','','A1','','US','Illinois','','Y','Y','','Y','','A1','A1','','A4','','A2','','','Y','Y','','','A1','N','WordPress, rarely','3','','Y','Y','','','','','A1','A5','','Y','','','','Y','','Y','Y','A2','A1','A3','','A1','','Y','','','','','','','','Y','','','','','','','','','','A6','A1','','Y','Y','','','','','','A4','','','Y','','','Y','Y','','Y','Y','','','Y','Y','','Y','','','','','-oth-','All of the above','A2','Y','5','N','A1','N','','Y'),(65,NULL,NULL,NULL,'en','2014-04-15 03:16:16','2014-04-15 03:16:16','66.168.20.35',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(66,NULL,'2014-04-15 04:50:47',7,'en','2014-04-15 04:40:50','2014-04-15 04:50:47','24.66.209.167',NULL,'','','Y','','',15.0000000000,'A3','Automation of repetitive tasks to ensure consistent and repeatable results time after time.','A3','','A3','','Canada','BC','','Y','','','','','A3','A1','','A1','','A2','','','','Y','','','A1','Y','No','3','','Y','Y','Y','Y','Y','Y','A1','A5','','','Y','','Y','','','Y','Y','A1','A1','A2','Update hooks','A2','','','','','','','','','Integration testing','Y','','','','','','','','','','A6','A2','','Y','','Y','','','','','-oth-','Google','Y','','','','','','','Y','Y','','','','','','Y','','','','','-oth-','Bash script that deploys code, runs update hooks and clears caches','A1','Y','3','Y','A4','Y','','N'),(67,NULL,'2014-04-15 22:13:22',7,'en','2014-04-15 08:05:07','2014-04-15 22:13:22','142.66.63.98','http://cyberswat.limequery.com/index.php/81259/lang-en','Y','Y','Y','','',7.0000000000,'A3','Development practices and workflow to ensure quality of a product through consistent configurations and testing.','A2','','A1','','Canada','Alberta','','Y','','','','','A4','A1','','A1','','A1','','','Y','Y','','','A4','Y','Primarily Drupal, but a few small sites use WordPress.','3','','Y','','','','Y','','A1','A5','','','Y','Y','','','','Y','Y','A1','A3','A3','','A1','','Y','','','','','','','','Y','','','','','','','','','','A7','A1','','Y','Y','','','','','','A4','','Y','','','','','','','Y','','','','','','Y','','','','','','A1','','A2','N','4','N','A2','Y','','N'),(68,NULL,'2014-04-15 23:12:11',7,'en','2014-04-15 23:01:49','2014-04-15 23:12:11','64.54.249.155',NULL,'Y','Y','Y','','',3.0000000000,'A3','No restrictions development that is more about collaboration and communication than code perfection. It\'s a movement of developers that understand the process and have experience in agile, scrum and waterfall techniques.','A3','','A1','','United States of America','California','','Y','Y','Y','Y','','A5','A2','','A2','','A2','','','Y','Y','','','A1','N','We have a wide variety of other CMS\'s used throughout our institution, our Drupal team is the largest.','3','','Y','Y','Y','Y','Y','Y','A1','A5','','Y','','','','','','Y','Y','A2','A1','A1','If it is a contributed module the configuration is done through the webgui. If it is a custom module our team developed we will make code and webgui changes.','A1','','','','Y','','','Y','Y','','','Y','','Y','','','','','','','A2','A2','','Y','Y','','','','','','A2','','','','','Y','','','','Y','Y','','','','','','Y','','','','','A9','','A3','Y','2','Y','A1','Y','','N'),(69,NULL,'2014-04-16 15:47:09',7,'en','2014-04-16 15:29:43','2014-04-16 15:47:09','24.173.111.218','http://cyberswat.limequery.com/index.php/81259/lang-en','','','Y','','',7.0000000000,'A3','DevOps is the collaboration between operations and development. It\'s also become a synonym for continuous integration, configuration management and operations in general. In other news Literally is now a synonym for figuratively.','A3','','A2','','United States','Texas','','','Y','','Y','','A3','A1','','-oth-','robots','A2','','','Y','Y','Y','','A1','Y','Wordpress, Jeckyll. ','1','','N','Y','','','','','A1','A5','','Y','Y','Y','','Y','Y','Y','Y','A1','A1','A2','','A2','','','','Y','','','Y','Y','','','Y','','','','','','','','behat','A3','A1','','Y','','Y','','','','','A2','','','Y','','Y','Y','Y','','Y','Y','','','','Y','','Y','Y','','','','A4','','A1','Y','3','Y','A1','Y','','Y'),(70,NULL,NULL,NULL,'en','2014-04-16 22:45:06','2014-04-16 22:45:06','136.152.142.16',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(71,NULL,NULL,NULL,'en','2014-04-17 14:29:24','2014-04-17 14:29:24','74.93.17.13',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(72,NULL,NULL,NULL,'en','2014-04-18 20:13:51','2014-04-18 20:13:51','128.117.88.148',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(73,NULL,NULL,NULL,'en','2014-04-19 00:21:09','2014-04-19 00:21:09','50.155.243.149',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(74,NULL,'2014-04-20 01:37:42',7,'en','2014-04-20 01:29:31','2014-04-20 01:37:42','71.178.195.59',NULL,'Y','Y','Y','','',4.0000000000,'A3','','A4','','A1','','USA','DC','','','Y','Y','Y','','A3','A1','','A1','','A2','','','Y','Y','','','A1','Y','no','3','','Y','','Y','','','','A1','A5','','Y','','','Y','','','','Y','A2','A1','A1','','A1','','','','Y','','','Y','','','Y','','','','','','','','','','A7','A1','','N','Y','','','','','','A2','','','Y','','','','Y','','Y','','','','','','','Y','','','','','A2','','A1','Y','3','Y','A1','Y','','Y'),(75,NULL,NULL,NULL,'en','2014-04-20 02:06:37','2014-04-20 02:06:37','146.115.40.129','http://cyberswat.limequery.com/index.php/81259/lang-en',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(76,NULL,NULL,NULL,'en','2014-04-24 14:54:44','2014-04-24 14:54:44','71.232.63.164',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(77,NULL,'2014-04-26 20:37:25',7,'en','2014-04-26 20:24:24','2014-04-26 20:37:24','24.103.203.242',NULL,'Y','Y','Y','Y','site administration',16.0000000000,'A3','','-oth-','Not really sure, but probably yes','A1','','United States','New York','','Y','','','','','','-oth-','Mix of IT, MarCom and users','-oth-','Roles are ill-defined','A1','','','Y','Y','','','A4','Y','yes: Mura, WordPress','3','','Y','','','','Y','Y','A3','A1','','','','','','Y','','','Y','A1','A1','A1','','A1','','','Y','','','','','','','Y','','','','','','','','','','A7','A1','','N','Y','','','','','','A4','','Y','','','','','','','Y','','','iis7','','','','','','','Y','','-oth-','mix of drush & database import','A1','N','1','Y','A2','Y','','N'),(78,NULL,'2014-04-29 15:15:15',7,'en','2014-04-29 15:01:56','2014-04-29 15:15:15','152.3.43.188',NULL,'Y','','','Y','Designer',12.0000000000,'A3','It is ambiguous and hard to define. DevOps is a cultural conversation supported by technology.','A6','','-oth-','IT Analyst','US','NC','','Y','Y','Y','Y','We train customers how to use their site.','A2','A1','','A1','','A1','','','','Y','','','A4','Y','Apostrophe, WordPress, Contribute','3','','N','Y','Y','Y','Y','','A1','A5','','Y','','','','','','Y','Y','A3','A3','A3','We are starting to move away from features (using better documentation) as it can be difficult to uncouple functionality from a feature.','A1','','','','Y','','','Y','Y','','Y','','','','','','','','','','A2','-oth-','not sure','N','','Y','','','','','A4','','Y','','','','','','','Y','','','','','','','','','','','apc cache','-oth-','A mix of all of the above, depending on the stage of the project','A2','Y','4','N','A2','Y','','N'),(79,NULL,NULL,NULL,'en','2014-04-30 16:03:17','2014-04-30 16:03:17','134.59.22.73','http://cyberswat.limequery.com/index.php/81259',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(80,NULL,'2014-04-30 16:55:09',7,'en','2014-04-30 16:45:55','2014-04-30 16:55:09','128.61.178.41',NULL,'Y','Y','Y','Y','',5.0000000000,'A1','Building sites and the systems that run them','A2','','-oth-','developer','usa','ga','','Y','','Y','Y','','A4','A1','','A3','','-oth-','communications','','Y','Y','','','A1','Y','no','3','','N','Y','Y','','','','A1','A5','','','','','Y','','','Y','Y','A1','A2','A1','','A1','','Y','','','','','','','','','','','','Y','','','','','','A7','A1','','N','Y','','','','','','A4','','Y','','','','','','','Y','','','','Y','','','','','','','','A3','','A2','N','3','N','A2','Y','','Y'),(81,NULL,'2014-05-01 13:20:10',7,'en','2014-05-01 13:02:06','2014-05-01 13:20:10','35.9.197.208',NULL,'Y','','Y','','',5.0000000000,'A3','DevOps is another buzzword focused on a stabilized set of processes for software and hardware infrastructure management.','A3','','-oth-','Developer','United State','MI','','Y','Y','Y','Y','','A3','A1','','A3','','A2','','','Y','Y','','','A1','Y','Yes, Magento','3','','Y','','Y','','','','A2','A5','','Y','Y','Y','Y','Y','Y','','Y','A1','A1','A3','Combination of features, drush, and webgui.','A1','','','','Y','','Y','Y','','','Y','','','','','','','','','','A7','A1','','Y','Y','','','','','','A4','','Y','','','','','','','Y','','','','','','','Y','','','','','A9','','A1','N','5','N','A1','Y','','N'),(82,NULL,'2014-05-01 23:29:21',7,'en','2014-05-01 23:22:52','2014-05-01 23:29:21','70.36.226.29',NULL,'Y','Y','Y','Y','',4.0000000000,'A3','i would direct them to speak to someone smarter than me','A2','','A2','','USA','California','','Y','Y','','','','A2','A3','','-oth-','Everybody','A2','','','Y','Y','Y','','A1','Y','WordPress\r\nNodeJS','3','','Y','Y','Y','Y','','','A1','A5','','Y','Y','Y','Y','Y','Y','Y','Y','A2','A1','A3','','A2','','','Y','Y','Y','','Y','','','','Y','','Y','','','','','','','A2','-oth-','Pantheon','N','Y','','','','','','A2','','','','','','','Y','','','Y','','','','','','Y','','','','','A3','','A2','Y','4','Y','A1','N','','Y'),(83,NULL,'2014-05-02 08:11:12',7,'en','2014-05-02 07:58:58','2014-05-02 08:11:12','91.176.155.54',NULL,'','Y','','','',10.0000000000,'A3','With a lot of frustration about the lack of understanding and the broken vendorisation we have after 5 years ','A2','','A3','','Belgium','','','','Y','','Y','','A3','A3','','A1','','A2','','','Y','Y','Y','Y','A1','Y','No','3','','Y','Y','Y','Y','','','A1','A5','','','','Y','','Y','Y','','Y','A1','A1','A2','','A2','','','Y','','','','','','','Y','','','','','','','','','','A5','A3','','N','Y','','','','','','A2','','Y','','','','','','','Y','','','','','','','','','','Y','','A3','','A2','Y','3','Y','A2','Y','','N'),(86,NULL,'2014-05-02 15:38:31',7,'en','2014-05-02 15:25:49','2014-05-02 15:38:31','98.114.91.80','http://cyberswat.limequery.com/index.php/81259','Y','','Y','','',15.0000000000,'A4','Creating infrastructure to support and optimize development workflows.','A2','','A3','','USA','PA','','','Y','','Y','','A3','A1','','A1','','A2','','','Y','Y','','','A1','Y','There are others?','3','','Y','Y','Y','Y','Y','Y','A1','A5','','','Y','Y','','','','','','A1','A1','A3','','A4','','','Y','','Y','','','Y','','Y','','','','','','','','','','','A2','','Y','Y','','','','','','A2','','','','','','','Y','','Y','','','','','','','Y','','','','','A4','','A2','N','','Y','A2','Y','','Y'),(85,NULL,'2014-05-02 14:57:38',7,'en','2014-05-02 14:41:19','2014-05-02 14:57:38','65.126.154.6',NULL,'Y','Y','Y','Y','',6.0000000000,'A3','As being a full stack developer, able to build the app and the production server it runs on.  Basically.','A3','','A1','','USA','New Jersey','','Y','','','','','A4','-oth-','Editorial staff','A1','','A2','','','','Y','','','A1','Y','No','3','','N','','','','Y','Y','A1','A5','','Y','Y','','','Y','','Y','Y','A2','A4','A3','This should be a checkbox question, not a radio button.  We do all.','A2','','Y','','','','','','','','Y','','','','','','','','','','A6','A2','','Y','Y','','','','','','A2','','','','','','','','Acquia Ent cloud','Y','','','','','Y','','Y','','','','','A4','','A1','Y','4','Y','A2','N','','N'),(87,NULL,NULL,4,'en','2014-05-02 16:22:45','2014-05-02 16:29:50','81.202.80.242',NULL,'Y','','Y','','',5.0000000000,'A3','','A2','','A3','','Spain','','','Y','','','','','A2','A1','','A1','','A2','','','','Y','','','A4','N','','3','','Y','','Y','','','','A2','A3','','Y','','','','','','Y','Y','A2','A2','A3','','A1','','','','','','','','Y','','Y','','','','','','','','','','A2',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL),(89,NULL,'2014-05-02 18:12:23',7,'en','2014-05-02 18:03:27','2014-05-02 18:12:23','62.83.68.67','http://cyberswat.limequery.com/index.php/81259','','Y','','','',5.0000000000,'A3','','A2','','A1','','Spain','Madrid','','Y','','Y','Y','','A1','A1','','A1','','A1','','','Y','Y','','','A4','Y','No','3','','Y','Y','Y','','','','A1','A5','','','Y','Y','Y','Y','Y','Y','Y','A2','A3','A3','','A1','','Y','','','','','','','','','Y','','','','','','','','','A6','A3','','Y','Y','','','','','','A2','','Y','','','Y','','','','Y','','','','','','','Y','','','','haproxy','A9','','A2','Y','2','Y','A1','Y','','Y'),(90,NULL,NULL,1,'en','2014-05-02 18:09:08','2014-05-02 18:12:25','85.87.116.231','http://cyberswat.limequery.com/index.php/81259','','','Y','','',2.0000000000,'A1','Operations to help developer to do more productivity ','A2','','-oth-','Developer and SysAdmin','Spain','Bizkaia','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'',NULL);
/*!40000 ALTER TABLE `survey_81259` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `survey_81259_timings`
--

DROP TABLE IF EXISTS `survey_81259_timings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `survey_81259_timings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `interviewtime` float DEFAULT NULL,
  `81259X1time` float DEFAULT NULL,
  `81259X1X1time` float DEFAULT NULL,
  `81259X1X2time` float DEFAULT NULL,
  `81259X1X3time` float DEFAULT NULL,
  `81259X1X28time` float DEFAULT NULL,
  `81259X1X4time` float DEFAULT NULL,
  `81259X1X204time` float DEFAULT NULL,
  `81259X1X205time` float DEFAULT NULL,
  `81259X1X206time` float DEFAULT NULL,
  `81259X1X218time` float DEFAULT NULL,
  `81259X2time` float DEFAULT NULL,
  `81259X2X6time` float DEFAULT NULL,
  `81259X2X5time` float DEFAULT NULL,
  `81259X2X7time` float DEFAULT NULL,
  `81259X2X8time` float DEFAULT NULL,
  `81259X2X200time` float DEFAULT NULL,
  `81259X3time` float DEFAULT NULL,
  `81259X3X9time` float DEFAULT NULL,
  `81259X3X10time` float DEFAULT NULL,
  `81259X3X11time` float DEFAULT NULL,
  `81259X3X29time` float DEFAULT NULL,
  `81259X3X35time` float DEFAULT NULL,
  `81259X3X36time` float DEFAULT NULL,
  `81259X3X210time` float DEFAULT NULL,
  `81259X4time` float DEFAULT NULL,
  `81259X4X12time` float DEFAULT NULL,
  `81259X4X13time` float DEFAULT NULL,
  `81259X4X14time` float DEFAULT NULL,
  `81259X4X15time` float DEFAULT NULL,
  `81259X4X16time` float DEFAULT NULL,
  `81259X4X17time` float DEFAULT NULL,
  `81259X4X18time` float DEFAULT NULL,
  `81259X4X33time` float DEFAULT NULL,
  `81259X4X34time` float DEFAULT NULL,
  `81259X4X216time` float DEFAULT NULL,
  `81259X5time` float DEFAULT NULL,
  `81259X5X19time` float DEFAULT NULL,
  `81259X5X20time` float DEFAULT NULL,
  `81259X5X21time` float DEFAULT NULL,
  `81259X5X22time` float DEFAULT NULL,
  `81259X5X37time` float DEFAULT NULL,
  `81259X5X38time` float DEFAULT NULL,
  `81259X5X39time` float DEFAULT NULL,
  `81259X6time` float DEFAULT NULL,
  `81259X6X23time` float DEFAULT NULL,
  `81259X6X32time` float DEFAULT NULL,
  `81259X6X24time` float DEFAULT NULL,
  `81259X6X30time` float DEFAULT NULL,
  `81259X6X31time` float DEFAULT NULL,
  `81259X6X209time` float DEFAULT NULL,
  `81259X7time` float DEFAULT NULL,
  `81259X7X25time` float DEFAULT NULL,
  `81259X7X26time` float DEFAULT NULL,
  `81259X7X27time` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `survey_81259_timings`
--

LOCK TABLES `survey_81259_timings` WRITE;
/*!40000 ALTER TABLE `survey_81259_timings` DISABLE KEYS */;
INSERT INTO `survey_81259_timings` VALUES (7,694.47,176.38,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,96.37,NULL,NULL,NULL,NULL,NULL,70.52,NULL,NULL,NULL,NULL,NULL,NULL,NULL,199.3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,54.5,NULL,NULL,NULL,NULL,NULL,NULL,NULL,63.09,NULL,NULL,NULL,NULL,NULL,NULL,34.31,NULL,NULL,NULL),(6,570.95,131.91,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,46.98,NULL,NULL,NULL,NULL,NULL,57.18,NULL,NULL,NULL,NULL,NULL,NULL,NULL,189.71,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,56.09,NULL,NULL,NULL,NULL,NULL,NULL,NULL,61.17,NULL,NULL,NULL,NULL,NULL,NULL,27.91,NULL,NULL,NULL),(3,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(4,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(5,304.27,70.37,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,35.81,NULL,NULL,NULL,NULL,NULL,36.05,NULL,NULL,NULL,NULL,NULL,NULL,NULL,67.3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,40.63,NULL,NULL,NULL,NULL,NULL,NULL,NULL,37.29,NULL,NULL,NULL,NULL,NULL,NULL,16.82,NULL,NULL,NULL),(8,521.12,139.46,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,91.67,NULL,NULL,NULL,NULL,NULL,81.41,NULL,NULL,NULL,NULL,NULL,NULL,NULL,73.26,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,68.8,NULL,NULL,NULL,NULL,NULL,NULL,NULL,40.62,NULL,NULL,NULL,NULL,NULL,NULL,25.9,NULL,NULL,NULL),(9,5681.07,5377.17,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,35.2,NULL,NULL,NULL,NULL,NULL,38.31,NULL,NULL,NULL,NULL,NULL,NULL,NULL,115.08,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,67.74,NULL,NULL,NULL,NULL,NULL,NULL,NULL,32.97,NULL,NULL,NULL,NULL,NULL,NULL,14.6,NULL,NULL,NULL),(10,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(11,546.87,280.64,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,41.17,NULL,NULL,NULL,NULL,NULL,39.62,NULL,NULL,NULL,NULL,NULL,NULL,NULL,83.89,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,52.46,NULL,NULL,NULL,NULL,NULL,NULL,NULL,29.84,NULL,NULL,NULL,NULL,NULL,NULL,19.25,NULL,NULL,NULL),(12,296.96,69.53,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,24.11,NULL,NULL,NULL,NULL,NULL,32.67,NULL,NULL,NULL,NULL,NULL,NULL,NULL,86.85,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,45.04,NULL,NULL,NULL,NULL,NULL,NULL,NULL,25.81,NULL,NULL,NULL,NULL,NULL,NULL,12.95,NULL,NULL,NULL),(13,417.8,72.76,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.29,NULL,NULL,NULL,NULL,NULL,45.88,NULL,NULL,NULL,NULL,NULL,NULL,NULL,109.97,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,59.21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,67.01,NULL,NULL,NULL,NULL,NULL,NULL,19.68,NULL,NULL,NULL),(14,106.18,106.18,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(15,649.52,221.13,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,30.75,NULL,NULL,NULL,NULL,NULL,164.78,NULL,NULL,NULL,NULL,NULL,NULL,NULL,103.76,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,46.31,NULL,NULL,NULL,NULL,NULL,NULL,NULL,56.64,NULL,NULL,NULL,NULL,NULL,NULL,26.15,NULL,NULL,NULL),(16,530.27,211.16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,48.08,NULL,NULL,NULL,NULL,NULL,38.68,NULL,NULL,NULL,NULL,NULL,NULL,NULL,123.23,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.19,NULL,NULL,NULL,NULL,NULL,NULL,NULL,50.41,NULL,NULL,NULL,NULL,NULL,NULL,16.52,NULL,NULL,NULL),(17,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(18,1348.9,164.06,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,41.8,NULL,NULL,NULL,NULL,NULL,139.12,NULL,NULL,NULL,NULL,NULL,NULL,NULL,883.57,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.01,NULL,NULL,NULL,NULL,NULL,NULL,NULL,53.41,NULL,NULL,NULL,NULL,NULL,NULL,23.93,NULL,NULL,NULL),(19,683.06,271.6,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,45.13,NULL,NULL,NULL,NULL,NULL,62.04,NULL,NULL,NULL,NULL,NULL,NULL,NULL,145.16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,68.07,NULL,NULL,NULL,NULL,NULL,NULL,NULL,69.83,NULL,NULL,NULL,NULL,NULL,NULL,21.23,NULL,NULL,NULL),(20,648.13,180.98,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,54.31,NULL,NULL,NULL,NULL,NULL,76.06,NULL,NULL,NULL,NULL,NULL,NULL,NULL,228.61,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,50,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.77,NULL,NULL,NULL,NULL,NULL,NULL,15.4,NULL,NULL,NULL),(21,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(22,468.25,87.12,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.27,NULL,NULL,NULL,NULL,NULL,67.36,NULL,NULL,NULL,NULL,NULL,NULL,NULL,82.16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,97.89,NULL,NULL,NULL,NULL,NULL,NULL,NULL,67.5,NULL,NULL,NULL,NULL,NULL,NULL,22.95,NULL,NULL,NULL),(23,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(24,497.41,133.73,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.63,NULL,NULL,NULL,NULL,NULL,44.57,NULL,NULL,NULL,NULL,NULL,NULL,NULL,79.85,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,128.8,NULL,NULL,NULL,NULL,NULL,NULL,NULL,40.28,NULL,NULL,NULL,NULL,NULL,NULL,26.55,NULL,NULL,NULL),(25,546.29,113.85,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,121.6,NULL,NULL,NULL,NULL,NULL,30.79,NULL,NULL,NULL,NULL,NULL,NULL,NULL,187.34,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.59,NULL,NULL,NULL,NULL,NULL,NULL,NULL,33.78,NULL,NULL,NULL,NULL,NULL,NULL,15.34,NULL,NULL,NULL),(26,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(27,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(28,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(29,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(30,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(31,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(32,488.61,157.86,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,50.31,NULL,NULL,NULL,NULL,NULL,50.75,NULL,NULL,NULL,NULL,NULL,NULL,NULL,123.46,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,39.65,NULL,NULL,NULL,NULL,NULL,NULL,NULL,46.91,NULL,NULL,NULL,NULL,NULL,NULL,19.67,NULL,NULL,NULL),(33,1626.21,725.02,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,70.55,NULL,NULL,NULL,NULL,NULL,179.68,NULL,NULL,NULL,NULL,NULL,NULL,NULL,389.48,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,73.45,NULL,NULL,NULL,NULL,NULL,NULL,NULL,154.19,NULL,NULL,NULL,NULL,NULL,NULL,33.84,NULL,NULL,NULL),(34,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(35,457.09,146.55,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.62,NULL,NULL,NULL,NULL,NULL,58.54,NULL,NULL,NULL,NULL,NULL,NULL,NULL,103.59,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,38.2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,47.02,NULL,NULL,NULL,NULL,NULL,NULL,20.57,NULL,NULL,NULL),(36,374.24,61.79,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,36.33,NULL,NULL,NULL,NULL,NULL,83.92,NULL,NULL,NULL,NULL,NULL,NULL,NULL,99.29,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,39.39,NULL,NULL,NULL,NULL,NULL,NULL,NULL,41.08,NULL,NULL,NULL,NULL,NULL,NULL,12.44,NULL,NULL,NULL),(37,369.83,66.21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,37.99,NULL,NULL,NULL,NULL,NULL,53.72,NULL,NULL,NULL,NULL,NULL,NULL,NULL,104.99,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,44.53,NULL,NULL,NULL,NULL,NULL,NULL,NULL,40.12,NULL,NULL,NULL,NULL,NULL,NULL,22.27,NULL,NULL,NULL),(38,319.48,143.14,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,27.23,NULL,NULL,NULL,NULL,NULL,33.48,NULL,NULL,NULL,NULL,NULL,NULL,NULL,47.27,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,27.98,NULL,NULL,NULL,NULL,NULL,NULL,NULL,28.45,NULL,NULL,NULL,NULL,NULL,NULL,11.93,NULL,NULL,NULL),(39,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(40,522.24,141.16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.45,NULL,NULL,NULL,NULL,NULL,65.34,NULL,NULL,NULL,NULL,NULL,NULL,NULL,157.95,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,48.75,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.58,NULL,NULL,NULL,NULL,NULL,NULL,23.01,NULL,NULL,NULL),(41,402.52,100.52,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,57.43,NULL,NULL,NULL,NULL,NULL,40.65,NULL,NULL,NULL,NULL,NULL,NULL,NULL,95.81,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,34.98,NULL,NULL,NULL,NULL,NULL,NULL,NULL,45.64,NULL,NULL,NULL,NULL,NULL,NULL,27.49,NULL,NULL,NULL),(42,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(43,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(44,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(45,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(46,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(47,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(48,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(49,1298.16,616.58,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,113.72,NULL,NULL,NULL,NULL,NULL,107.36,NULL,NULL,NULL,NULL,NULL,NULL,NULL,277.71,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,46.48,NULL,NULL,NULL,NULL,NULL,NULL,NULL,86.21,NULL,NULL,NULL,NULL,NULL,NULL,50.1,NULL,NULL,NULL),(50,863.23,165.85,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,90.68,NULL,NULL,NULL,NULL,NULL,125.9,NULL,NULL,NULL,NULL,NULL,NULL,NULL,248.76,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,116.95,NULL,NULL,NULL,NULL,NULL,NULL,NULL,89.61,NULL,NULL,NULL,NULL,NULL,NULL,25.48,NULL,NULL,NULL),(51,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(52,722.9,187.24,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,59.69,NULL,NULL,NULL,NULL,NULL,109.7,NULL,NULL,NULL,NULL,NULL,NULL,NULL,114.88,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,124.95,NULL,NULL,NULL,NULL,NULL,NULL,NULL,104.12,NULL,NULL,NULL,NULL,NULL,NULL,22.32,NULL,NULL,NULL),(53,504.53,84.69,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.41,NULL,NULL,NULL,NULL,NULL,52.47,NULL,NULL,NULL,NULL,NULL,NULL,NULL,178.89,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,59.2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,59.11,NULL,NULL,NULL,NULL,NULL,NULL,27.76,NULL,NULL,NULL),(54,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(55,478.97,88.5,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,80.41,NULL,NULL,NULL,NULL,NULL,106.38,NULL,NULL,NULL,NULL,NULL,NULL,NULL,203.68,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(56,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(57,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(58,731.29,151.76,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,72.55,NULL,NULL,NULL,NULL,NULL,120,NULL,NULL,NULL,NULL,NULL,NULL,NULL,208.54,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,57.9,NULL,NULL,NULL,NULL,NULL,NULL,NULL,85.81,NULL,NULL,NULL,NULL,NULL,NULL,34.73,NULL,NULL,NULL),(59,365.66,54.97,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.79,NULL,NULL,NULL,NULL,NULL,61.46,NULL,NULL,NULL,NULL,NULL,NULL,NULL,98.8,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,47.06,NULL,NULL,NULL,NULL,NULL,NULL,NULL,36.43,NULL,NULL,NULL,NULL,NULL,NULL,24.15,NULL,NULL,NULL),(60,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(61,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(62,571.16,107.96,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,57.44,NULL,NULL,NULL,NULL,NULL,56.79,NULL,NULL,NULL,NULL,NULL,NULL,NULL,193.32,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,76.77,NULL,NULL,NULL,NULL,NULL,NULL,NULL,41.87,NULL,NULL,NULL,NULL,NULL,NULL,37.01,NULL,NULL,NULL),(63,549.45,151.57,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,41.05,NULL,NULL,NULL,NULL,NULL,56.41,NULL,NULL,NULL,NULL,NULL,NULL,NULL,195.08,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,56.29,NULL,NULL,NULL,NULL,NULL,NULL,NULL,36.19,NULL,NULL,NULL,NULL,NULL,NULL,12.86,NULL,NULL,NULL),(64,456.2,100.2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,54.01,NULL,NULL,NULL,NULL,NULL,57.31,NULL,NULL,NULL,NULL,NULL,NULL,NULL,86.21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,56.02,NULL,NULL,NULL,NULL,NULL,NULL,NULL,64.93,NULL,NULL,NULL,NULL,NULL,NULL,37.52,NULL,NULL,NULL),(65,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(66,599.37,178.82,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,31.98,NULL,NULL,NULL,NULL,NULL,60.41,NULL,NULL,NULL,NULL,NULL,NULL,NULL,139.23,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,47.01,NULL,NULL,NULL,NULL,NULL,NULL,NULL,112.64,NULL,NULL,NULL,NULL,NULL,NULL,29.28,NULL,NULL,NULL),(67,598.27,347.66,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,28.16,NULL,NULL,NULL,NULL,NULL,51.86,NULL,NULL,NULL,NULL,NULL,NULL,NULL,84.24,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,24.45,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.71,NULL,NULL,NULL,NULL,NULL,NULL,19.19,NULL,NULL,NULL),(68,624.44,127.04,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,40.71,NULL,NULL,NULL,NULL,NULL,122.37,NULL,NULL,NULL,NULL,NULL,NULL,NULL,165.43,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,81.37,NULL,NULL,NULL,NULL,NULL,NULL,NULL,60.71,NULL,NULL,NULL,NULL,NULL,NULL,26.81,NULL,NULL,NULL),(69,1049.68,707.72,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,44.23,NULL,NULL,NULL,NULL,NULL,61.73,NULL,NULL,NULL,NULL,NULL,NULL,NULL,132.32,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,44.23,NULL,NULL,NULL,NULL,NULL,NULL,NULL,44.62,NULL,NULL,NULL,NULL,NULL,NULL,14.83,NULL,NULL,NULL),(70,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(71,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(72,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(73,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(74,494.58,98.75,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,57.68,NULL,NULL,NULL,NULL,NULL,67.98,NULL,NULL,NULL,NULL,NULL,NULL,NULL,119.77,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,58.62,NULL,NULL,NULL,NULL,NULL,NULL,NULL,71.34,NULL,NULL,NULL,NULL,NULL,NULL,20.44,NULL,NULL,NULL),(75,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(76,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(77,783.04,228.36,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,121.56,NULL,NULL,NULL,NULL,NULL,146.9,NULL,NULL,NULL,NULL,NULL,NULL,NULL,128.3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,65.46,NULL,NULL,NULL,NULL,NULL,NULL,NULL,65.47,NULL,NULL,NULL,NULL,NULL,NULL,26.99,NULL,NULL,NULL),(78,648.64,62.36,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,66.3,NULL,NULL,NULL,NULL,NULL,67.04,NULL,NULL,NULL,NULL,NULL,NULL,NULL,201.21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,133.32,NULL,NULL,NULL,NULL,NULL,NULL,NULL,91.94,NULL,NULL,NULL,NULL,NULL,NULL,26.47,NULL,NULL,NULL),(79,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(80,556.72,144.69,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,58.72,NULL,NULL,NULL,NULL,NULL,33.27,NULL,NULL,NULL,NULL,NULL,NULL,NULL,220.86,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,37.63,NULL,NULL,NULL,NULL,NULL,NULL,NULL,45.19,NULL,NULL,NULL,NULL,NULL,NULL,16.36,NULL,NULL,NULL),(81,1087.24,419.31,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,94.55,NULL,NULL,NULL,NULL,NULL,62.44,NULL,NULL,NULL,NULL,NULL,NULL,NULL,269.34,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,123.59,NULL,NULL,NULL,NULL,NULL,NULL,NULL,84.48,NULL,NULL,NULL,NULL,NULL,NULL,33.53,NULL,NULL,NULL),(82,392.55,76.67,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,53.19,NULL,NULL,NULL,NULL,NULL,65,NULL,NULL,NULL,NULL,NULL,NULL,NULL,99.41,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,32.58,NULL,NULL,NULL,NULL,NULL,NULL,NULL,48.8,NULL,NULL,NULL,NULL,NULL,NULL,16.9,NULL,NULL,NULL),(83,737.07,498.63,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,35.71,NULL,NULL,NULL,NULL,NULL,49.62,NULL,NULL,NULL,NULL,NULL,NULL,NULL,74.91,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,32.09,NULL,NULL,NULL,NULL,NULL,NULL,NULL,30.88,NULL,NULL,NULL,NULL,NULL,NULL,15.23,NULL,NULL,NULL),(84,722.53,310.38,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,43.16,NULL,NULL,NULL,NULL,NULL,51.21,NULL,NULL,NULL,NULL,NULL,NULL,NULL,139.15,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,101.26,NULL,NULL,NULL,NULL,NULL,NULL,NULL,77.37,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(85,981.52,83.84,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,34.05,NULL,NULL,NULL,NULL,NULL,49.14,NULL,NULL,NULL,NULL,NULL,NULL,NULL,549.71,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,179.02,NULL,NULL,NULL,NULL,NULL,NULL,NULL,66.55,NULL,NULL,NULL,NULL,NULL,NULL,19.21,NULL,NULL,NULL),(86,764.47,151.81,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,26.87,NULL,NULL,NULL,NULL,NULL,428.34,NULL,NULL,NULL,NULL,NULL,NULL,NULL,68.93,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,33.6,NULL,NULL,NULL,NULL,NULL,NULL,NULL,33.1,NULL,NULL,NULL,NULL,NULL,NULL,21.82,NULL,NULL,NULL),(87,426.96,233.48,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,53.44,NULL,NULL,NULL,NULL,NULL,41.41,NULL,NULL,NULL,NULL,NULL,NULL,NULL,98.63,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(88,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(89,538.28,127.98,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,42.66,NULL,NULL,NULL,NULL,NULL,62.62,NULL,NULL,NULL,NULL,NULL,NULL,NULL,152.17,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,63.2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,67.12,NULL,NULL,NULL,NULL,NULL,NULL,22.53,NULL,NULL,NULL),(90,197.4,197.4,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `survey_81259_timings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `survey_links`
--

DROP TABLE IF EXISTS `survey_links`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `survey_links` (
  `participant_id` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `token_id` int(11) NOT NULL,
  `survey_id` int(11) NOT NULL,
  `date_created` datetime DEFAULT NULL,
  `date_invited` datetime DEFAULT NULL,
  `date_completed` datetime DEFAULT NULL,
  PRIMARY KEY (`participant_id`,`token_id`,`survey_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `survey_links`
--

LOCK TABLES `survey_links` WRITE;
/*!40000 ALTER TABLE `survey_links` DISABLE KEYS */;
/*!40000 ALTER TABLE `survey_links` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `survey_url_parameters`
--

DROP TABLE IF EXISTS `survey_url_parameters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `survey_url_parameters` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) NOT NULL,
  `parameter` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `targetqid` int(11) DEFAULT NULL,
  `targetsqid` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `survey_url_parameters`
--

LOCK TABLES `survey_url_parameters` WRITE;
/*!40000 ALTER TABLE `survey_url_parameters` DISABLE KEYS */;
/*!40000 ALTER TABLE `survey_url_parameters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveys`
--

DROP TABLE IF EXISTS `surveys`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `surveys` (
  `sid` int(11) NOT NULL,
  `owner_id` int(11) NOT NULL,
  `admin` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `active` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `expires` datetime DEFAULT NULL,
  `startdate` datetime DEFAULT NULL,
  `adminemail` varchar(254) COLLATE utf8_unicode_ci DEFAULT NULL,
  `anonymized` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `faxto` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `format` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `savetimings` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `template` varchar(100) COLLATE utf8_unicode_ci DEFAULT 'default',
  `language` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `additional_languages` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `datestamp` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `usecookie` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `allowregister` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `allowsave` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'Y',
  `autonumber_start` int(11) NOT NULL DEFAULT '0',
  `autoredirect` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `allowprev` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `printanswers` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `ipaddr` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `refurl` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `datecreated` date DEFAULT NULL,
  `publicstatistics` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `publicgraphs` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `listpublic` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `htmlemail` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `sendconfirmation` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'Y',
  `tokenanswerspersistence` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `assessments` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `usecaptcha` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `usetokens` varchar(1) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'N',
  `bounce_email` varchar(254) COLLATE utf8_unicode_ci DEFAULT NULL,
  `attributedescriptions` text COLLATE utf8_unicode_ci,
  `emailresponseto` text COLLATE utf8_unicode_ci,
  `emailnotificationto` text COLLATE utf8_unicode_ci,
  `tokenlength` int(11) NOT NULL DEFAULT '15',
  `showxquestions` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'Y',
  `showgroupinfo` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'B',
  `shownoanswer` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'Y',
  `showqnumcode` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'X',
  `bouncetime` int(11) DEFAULT NULL,
  `bounceprocessing` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'N',
  `bounceaccounttype` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bounceaccounthost` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bounceaccountpass` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bounceaccountencryption` varchar(3) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bounceaccountuser` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `showwelcome` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'Y',
  `showprogress` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'Y',
  `questionindex` int(11) NOT NULL DEFAULT '0',
  `navigationdelay` int(11) NOT NULL DEFAULT '0',
  `nokeyboard` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'N',
  `alloweditaftercompletion` varchar(1) COLLATE utf8_unicode_ci DEFAULT 'N',
  `googleanalyticsstyle` varchar(1) COLLATE utf8_unicode_ci DEFAULT NULL,
  `googleanalyticsapikey` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`sid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `surveys`
--

LOCK TABLES `surveys` WRITE;
/*!40000 ALTER TABLE `surveys` DISABLE KEYS */;
INSERT INTO `surveys` VALUES (81259,1,'Kevin Bridges','Y','2014-05-03 00:00:00',NULL,'kevin@cyberswat.com','N','','G','Y','default','en','','Y','N','N','Y',0,'Y','N','N','Y','Y','2014-04-05','N','N','Y','Y','Y','N','N','D','N','kevin@newmediadenver.com','','kevin@cyberswat.com','kevin@cyberswat.com',15,'Y','B','Y','N',0,'N','','','','','','Y','Y',0,0,'N','N','0',''),(689125,1,'Kris Buytaert','N',NULL,NULL,'kris.buytaert@inuits.eu','N','','G','N','default','en','','N','N','N','Y',0,'Y','N','N','N','N','2014-04-05','N','N','Y','Y','Y','N','N','D','N','your-email@example.net','','','',15,'Y','B','Y','N',0,'N','','','','','','Y','Y',0,0,'N','N',NULL,NULL);
/*!40000 ALTER TABLE `surveys` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `surveys_languagesettings`
--

DROP TABLE IF EXISTS `surveys_languagesettings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `surveys_languagesettings` (
  `surveyls_survey_id` int(11) NOT NULL,
  `surveyls_language` varchar(45) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'en',
  `surveyls_title` varchar(200) COLLATE utf8_unicode_ci NOT NULL,
  `surveyls_description` text COLLATE utf8_unicode_ci,
  `surveyls_welcometext` text COLLATE utf8_unicode_ci,
  `surveyls_endtext` text COLLATE utf8_unicode_ci,
  `surveyls_url` text COLLATE utf8_unicode_ci,
  `surveyls_urldescription` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `surveyls_email_invite_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `surveyls_email_invite` text COLLATE utf8_unicode_ci,
  `surveyls_email_remind_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `surveyls_email_remind` text COLLATE utf8_unicode_ci,
  `surveyls_email_register_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `surveyls_email_register` text COLLATE utf8_unicode_ci,
  `surveyls_email_confirm_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `surveyls_email_confirm` text COLLATE utf8_unicode_ci,
  `surveyls_dateformat` int(11) NOT NULL DEFAULT '1',
  `surveyls_attributecaptions` text COLLATE utf8_unicode_ci,
  `email_admin_notification_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email_admin_notification` text COLLATE utf8_unicode_ci,
  `email_admin_responses_subj` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email_admin_responses` text COLLATE utf8_unicode_ci,
  `surveyls_numberformat` int(11) NOT NULL DEFAULT '0',
  `attachments` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`surveyls_survey_id`,`surveyls_language`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `surveys_languagesettings`
--

LOCK TABLES `surveys_languagesettings` WRITE;
/*!40000 ALTER TABLE `surveys_languagesettings` DISABLE KEYS */;
INSERT INTO `surveys_languagesettings` VALUES (81259,'en','2014 State of Drupal DevOps','How does DevOps impact your Drupal life ... do you like the way you work? ','<p>\r\n	Two years ago Kris Buytaert presented a survey to the Drupal community in order to gauge the state of DevOps in Drupal. You can see the slides from this presentation at <a href=\"http://www.slideshare.net/KrisBuytaert/drupal-and-devops-the-survey-results\">http://www.slideshare.net/KrisBuytaert/drupal-and-devops-the-survey-results</a>.&nbsp; We have resurrected the essence of his original questionaire and will be presenting this data at Drupalcon Austin in <a href=\"https://austin2014.drupal.org/session/state-drupal-devops\">https://austin2014.drupal.org/session/state-drupal-devops</a></p>\r\n<p>\r\n	&nbsp;</p>\r\n<p>\r\n	Much like the Agile movements, the definition of DevOps changes between people and organizations.&nbsp; The only way for us, as a community, to benefit from devops is to understand and share that knowledge.&nbsp;&nbsp; We are interested in hearing from you if you have ever worked to deliver a Drupal site from a technical, or non-technical perspective.</p>\r\n<p>\r\n	&nbsp;</p>\r\n<p>\r\n	In exhange for your time, I will be giving away a new 16g wifi iPad mini with Retina Display to a randomly chosen winner.&nbsp; In order to qualify for the iPad you must fill out 100% of the survey questions.&nbsp; The winner will be announced on stage at Drupalcon Austin during the presentation.</p>\r\n<p>\r\n	&nbsp;</p>\r\n<p>\r\n	Your contact information will only be used to send you the ipad if you win or to send you the survey results if you select that checkbox. If at any time, you would like your results completely removed from the survey, simply let me know.&nbsp; If you would like to discuss any of this before submitting, you can receive assistance at https://groups.drupal.org/devops</p>\r\n<p>\r\n	&nbsp;</p>\r\n<p>\r\n	<strong>This survey will expire on May.05.2014 00:00</strong></p>\r\n','Thanks for participating in our survey, we\'ll share the results soon on https://groups.drupal.org/devops','https://groups.drupal.org/devops','Drupal DevOps','Invitation to participate in a survey','Dear {FIRSTNAME},<br /><br />you have been invited to participate in a survey.<br /><br />The survey is titled:<br />\"{SURVEYNAME}\"<br /><br />\"{SURVEYDESCRIPTION}\"<br /><br />To participate, please click on the link below.<br /><br />Sincerely,<br /><br />{ADMINNAME} ({ADMINEMAIL})<br /><br />----------------------------------------------<br />Click here to do the survey:<br />{SURVEYURL}<br /><br />If you do not want to participate in this survey and don\'t want to receive any more invitations please click the following link:<br />{OPTOUTURL}','Reminder to participate in a survey','Dear {FIRSTNAME},<br /><br />Recently we invited you to participate in a survey.<br /><br />We note that you have not yet completed the survey, and wish to remind you that the survey is still available should you wish to take part.<br /><br />The survey is titled:<br />\"{SURVEYNAME}\"<br /><br />\"{SURVEYDESCRIPTION}\"<br /><br />To participate, please click on the link below.<br /><br />Sincerely,<br /><br />{ADMINNAME} ({ADMINEMAIL})<br /><br />----------------------------------------------<br />Click here to do the survey:<br />{SURVEYURL}<br /><br />If you do not want to participate in this survey and don\'t want to receive any more invitations please click the following link:<br />{OPTOUTURL}','Survey registration confirmation','Dear {FIRSTNAME},<br /><br />You, or someone using your email address, have registered to participate in an online survey titled {SURVEYNAME}.<br /><br />To complete this survey, click on the following URL:<br /><br />{SURVEYURL}<br /><br />If you have any questions about this survey, or if you did not register to participate and believe this email is in error, please contact {ADMINNAME} at {ADMINEMAIL}.','Confirmation of your participation in our survey','Dear {FIRSTNAME},<br /><br />this email is to confirm that you have completed the survey titled {SURVEYNAME} and your response has been saved. Thank you for participating.<br /><br />If you have any further questions about this email, please contact {ADMINNAME} on {ADMINEMAIL}.<br /><br />Sincerely,<br /><br />{ADMINNAME}',2,NULL,'Response submission for survey {SURVEYNAME}','Hello,<br /><br />A new response was submitted for your survey \'{SURVEYNAME}\'.<br /><br />Click the following link to reload the survey:<br />{RELOADURL}<br /><br />Click the following link to see the individual response:<br />{VIEWRESPONSEURL}<br /><br />Click the following link to edit the individual response:<br />{EDITRESPONSEURL}<br /><br />View statistics by clicking here:<br />{STATISTICSURL}','Response submission for survey {SURVEYNAME} with results','<style type=\"text/css\">\n                                                .printouttable {\n                                                  margin:1em auto;\n                                                }\n                                                .printouttable th {\n                                                  text-align: center;\n                                                }\n                                                .printouttable td {\n                                                  border-color: #ddf #ddf #ddf #ddf;\n                                                  border-style: solid;\n                                                  border-width: 1px;\n                                                  padding:0.1em 1em 0.1em 0.5em;\n                                                }\n\n                                                .printouttable td:first-child {\n                                                  font-weight: 700;\n                                                  text-align: right;\n                                                  padding-right: 5px;\n                                                  padding-left: 5px;\n\n                                                }\n                                                .printouttable .printanswersquestion td{\n                                                  background-color:#F7F8FF;\n                                                }\n\n                                                .printouttable .printanswersquestionhead td{\n                                                  text-align: left;\n                                                  background-color:#ddf;\n                                                }\n\n                                                .printouttable .printanswersgroup td{\n                                                  text-align: center;        \n                                                  font-weight:bold;\n                                                  padding-top:1em;\n                                                }\n                                                </style>Hello,<br /><br />A new response was submitted for your survey \'{SURVEYNAME}\'.<br /><br />Click the following link to reload the survey:<br />{RELOADURL}<br /><br />Click the following link to see the individual response:<br />{VIEWRESPONSEURL}<br /><br />Click the following link to edit the individual response:<br />{EDITRESPONSEURL}<br /><br />View statistics by clicking here:<br />{STATISTICSURL}<br /><br /><br />The following answers were given by the participant:<br />{ANSWERTABLE}',0,NULL),(689125,'en','Drupal Deployments, how do YOU do them ? ','How do you deploy and manage your Drupal setup,  do you like the way you work ? \n\n','Devops is gaining momentum, the idea that developers and operations should work much closer together , the idea that one should automate as much as possible in both their infrastructure and their release process  brings along a lot of questions, ideas and tools that need to be integrated in your daily way of working.\n\nWorking with drupal, build with drupal in mind .. how do you release your sites ..\nThat\'s what we are trying to figure out ... for everybody else to learn from','Thnx for participating in our survey ,   we\'ll blog about the results of our soon on www.inuits.eu','http://www.inuits.eu/content/thanks','Inuits.eu corporate site','Invitation to participate in a survey','Dear {FIRSTNAME},<br /><br />you have been invited to participate in a survey.<br /><br />The survey is titled:<br />\"{SURVEYNAME}\"<br /><br />\"{SURVEYDESCRIPTION}\"<br /><br />To participate, please click on the link below.<br /><br />Sincerely,<br /><br />{ADMINNAME} ({ADMINEMAIL})<br /><br />----------------------------------------------<br />Click here to do the survey:<br />{SURVEYURL}<br /><br />If you do not want to participate in this survey and don\'t want to receive any more invitations please click the following link:<br />{OPTOUTURL}','Reminder to participate in a survey','Dear {FIRSTNAME},<br /><br />Recently we invited you to participate in a survey.<br /><br />We note that you have not yet completed the survey, and wish to remind you that the survey is still available should you wish to take part.<br /><br />The survey is titled:<br />\"{SURVEYNAME}\"<br /><br />\"{SURVEYDESCRIPTION}\"<br /><br />To participate, please click on the link below.<br /><br />Sincerely,<br /><br />{ADMINNAME} ({ADMINEMAIL})<br /><br />----------------------------------------------<br />Click here to do the survey:<br />{SURVEYURL}<br /><br />If you do not want to participate in this survey and don\'t want to receive any more invitations please click the following link:<br />{OPTOUTURL}','Survey registration confirmation','Dear {FIRSTNAME},<br /><br />You, or someone using your email address, have registered to participate in an online survey titled {SURVEYNAME}.<br /><br />To complete this survey, click on the following URL:<br /><br />{SURVEYURL}<br /><br />If you have any questions about this survey, or if you did not register to participate and believe this email is in error, please contact {ADMINNAME} at {ADMINEMAIL}.','Confirmation of your participation in our survey','Dear {FIRSTNAME},<br /><br />this email is to confirm that you have completed the survey titled {SURVEYNAME} and your response has been saved. Thank you for participating.<br /><br />If you have any further questions about this email, please contact {ADMINNAME} on {ADMINEMAIL}.<br /><br />Sincerely,<br /><br />{ADMINNAME}',2,NULL,'Response submission for survey {SURVEYNAME}','Hello,<br /><br />A new response was submitted for your survey \'{SURVEYNAME}\'.<br /><br />Click the following link to reload the survey:<br />{RELOADURL}<br /><br />Click the following link to see the individual response:<br />{VIEWRESPONSEURL}<br /><br />Click the following link to edit the individual response:<br />{EDITRESPONSEURL}<br /><br />View statistics by clicking here:<br />{STATISTICSURL}','Response submission for survey {SURVEYNAME} with results','<style type=\"text/css\">\n                                                .printouttable {\n                                                  margin:1em auto;\n                                                }\n                                                .printouttable th {\n                                                  text-align: center;\n                                                }\n                                                .printouttable td {\n                                                  border-color: #ddf #ddf #ddf #ddf;\n                                                  border-style: solid;\n                                                  border-width: 1px;\n                                                  padding:0.1em 1em 0.1em 0.5em;\n                                                }\n\n                                                .printouttable td:first-child {\n                                                  font-weight: 700;\n                                                  text-align: right;\n                                                  padding-right: 5px;\n                                                  padding-left: 5px;\n\n                                                }\n                                                .printouttable .printanswersquestion td{\n                                                  background-color:#F7F8FF;\n                                                }\n\n                                                .printouttable .printanswersquestionhead td{\n                                                  text-align: left;\n                                                  background-color:#ddf;\n                                                }\n\n                                                .printouttable .printanswersgroup td{\n                                                  text-align: center;        \n                                                  font-weight:bold;\n                                                  padding-top:1em;\n                                                }\n                                                </style>Hello,<br /><br />A new response was submitted for your survey \'{SURVEYNAME}\'.<br /><br />Click the following link to reload the survey:<br />{RELOADURL}<br /><br />Click the following link to see the individual response:<br />{VIEWRESPONSEURL}<br /><br />Click the following link to edit the individual response:<br />{EDITRESPONSEURL}<br /><br />View statistics by clicking here:<br />{STATISTICSURL}<br /><br /><br />The following answers were given by the participant:<br />{ANSWERTABLE}',0,NULL);
/*!40000 ALTER TABLE `surveys_languagesettings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `templates`
--

DROP TABLE IF EXISTS `templates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `templates` (
  `folder` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `creator` int(11) NOT NULL,
  PRIMARY KEY (`folder`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `templates`
--

LOCK TABLES `templates` WRITE;
/*!40000 ALTER TABLE `templates` DISABLE KEYS */;
INSERT INTO `templates` VALUES ('basic',1),('bluengrey',1),('citronade',1),('clear_logo',1),('default',1),('eirenicon',1),('limespired',1),('mint_idea',1),('sherpa',1),('vallendar',1);
/*!40000 ALTER TABLE `templates` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `templates_rights`
--

DROP TABLE IF EXISTS `templates_rights`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `templates_rights` (
  `uid` int(11) NOT NULL,
  `folder` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `use` int(11) NOT NULL,
  PRIMARY KEY (`uid`,`folder`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `templates_rights`
--

LOCK TABLES `templates_rights` WRITE;
/*!40000 ALTER TABLE `templates_rights` DISABLE KEYS */;
/*!40000 ALTER TABLE `templates_rights` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_groups`
--

DROP TABLE IF EXISTS `user_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_groups` (
  `ugid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `description` text COLLATE utf8_unicode_ci NOT NULL,
  `owner_id` int(11) NOT NULL,
  PRIMARY KEY (`ugid`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_groups`
--

LOCK TABLES `user_groups` WRITE;
/*!40000 ALTER TABLE `user_groups` DISABLE KEYS */;
INSERT INTO `user_groups` VALUES (1,'Administrators','',1);
/*!40000 ALTER TABLE `user_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_in_groups`
--

DROP TABLE IF EXISTS `user_in_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_in_groups` (
  `ugid` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  PRIMARY KEY (`ugid`,`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_in_groups`
--

LOCK TABLES `user_in_groups` WRITE;
/*!40000 ALTER TABLE `user_in_groups` DISABLE KEYS */;
INSERT INTO `user_in_groups` VALUES (1,1);
/*!40000 ALTER TABLE `user_in_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `users_name` varchar(64) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `password` blob NOT NULL,
  `full_name` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `parent_id` int(11) NOT NULL,
  `lang` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(254) COLLATE utf8_unicode_ci DEFAULT NULL,
  `htmleditormode` varchar(7) COLLATE utf8_unicode_ci DEFAULT 'default',
  `templateeditormode` varchar(7) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'default',
  `questionselectormode` varchar(7) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'default',
  `one_time_pw` blob,
  `dateformat` int(11) NOT NULL DEFAULT '1',
  `created` datetime DEFAULT NULL,
  `modified` datetime DEFAULT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `users_name` (`users_name`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'cyberswat','9d55be28789a18847000ae65078cea6bfbb8cced5affecac9eb7b1103f34e0db','',0,'auto','kevin@cyberswat.com','default','default','default',NULL,1,NULL,NULL),(2,'rickmanellius','00b1d7afef3d8152febce822881a4a2a02c3637c2e619130618f80431e62ed5f','Rick Manellius',1,'auto','rick@newmediadenver.com','default','default','default',NULL,1,'2014-04-05 17:35:04',NULL),(3,'jpw1116','ce0fe44943edb30b0f8e067cb195e9d4945e2ad73e9358641ca654a20726d866','John P. Weiksnar',1,'auto','jpw@roadrunner.com','default','default','default',NULL,1,'2014-04-05 17:36:26','2014-04-08 18:30:08');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-06-02  4:37:26
