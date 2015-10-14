#!/bin/sh
# Test XC Drupal Toolkit harvesting schedule
#
# Before run edit the settings section below to match your machine

# SETTINGS SECTION ---------------------------------------------o

# MySQL database name ------------------------------------------o
MYSQL_DB=drupal

# MySQL command line parameters, like -u Username -pPassword ---o
MYSQL_OPT='-u Username -pS3cret'

# Solr site URL ------------------------------------------------o
SOLR=http://localhost:8983/solr

# Drupal site URL ----------------------------------------------o
DRUPAL_SITE=http://localhost/drupal

# Drupal path on file system -----------------------------------o
DRUPAL_PATH=/var/www/drupal

# Schedule ID to run -------------------------------------------o
SCHEDULE_ID=3

# Cron key -----------------------------------------------------o
# Administration > Reports > Status report (admin/reports/status)
CRON_KEY=_bJa9cluohX2iyU7ys9-pmu262AZG80Kvvc8FSV5pM4
#---------------------------------------------------------------o

echo =================================
date
echo =================================

# clear Drupal cache
mysql $MYSQL_OPT -e "DELETE FROM cache WHERE cid = 'variables';" $MYSQL_DB
mysql $MYSQL_OPT -e "DELETE FROM variable WHERE name = 'oaiharvester_cron_last';" $MYSQL_DB
mysql $MYSQL_OPT -e "DELETE FROM variable WHERE name = 'cron_semaphore';" $MYSQL_DB
mysql $MYSQL_OPT -e "DELETE FROM variable WHERE name = 'oaiharvester_processing_cron';" $MYSQL_DB
mysql $MYSQL_OPT -e "UPDATE variable SET value = 's:1:\"0\";' WHERE name = 'search_cron_limit';" $MYSQL_DB
mysql $MYSQL_OPT -e "UPDATE oaiharvester_harvest_schedule_steps SET last_ran = NULL WHERE schedule_id = $SCHEDULE_ID;" $MYSQL_DB
mysql $MYSQL_OPT -e "UPDATE oaiharvester_harvester_schedules SET status = 'passive' WHERE harvest_schedule_id = $SCHEDULE_ID;" $MYSQL_DB

# clear Drupal log
mysql $MYSQL_OPT -e "TRUNCATE TABLE watchdog;" $MYSQL_DB

# delete XC records -- run this if you really need it, e.g. you want to test the whole harvest process
# mysql $MYSQL_OPT $MYSQL_DB < $DRUPAL_PATH/sites/all/modules/xc/xc_util/resources/drop_data.sql

# delete Solr index
curl -s "$SOLR/select?q=*:*&rows=0" 2> /dev/null | xmllint --format - | grep numFound
curl -s $SOLR/update?stream.body=%3Cdelete%3E%3Cquery%3E*:*%3C/query%3E%3C/delete%3E > /dev/null
curl -s $SOLR/update?stream.body=%3Ccommit/%3E > /dev/null
curl -s $SOLR/update?stream.body=%3Coptimize/%3E > /dev/null

# count data
curl -s "$SOLR/select?q=*:*&rows=0" 2> /dev/null | xmllint --format - | grep numFound
mysql $MYSQL_OPT $MYSQL_DB < $DRUPAL_PATH/sites/all/modules/xc/xc_util/resources/count_data.sql

# RUN cron
curl $DRUPAL_SITE/cron.php?cron_key=$CRON_KEY

# count data
curl -s "$SOLR/select?q=*:*&rows=0" 2> /dev/null | xmllint --format - | grep numFound
mysql $MYSQL_OPT $MYSQL_DB < $DRUPAL_PATH/sites/all/modules/xc/xc_util/resources/count_data.sql

# Check memory consumption
mysql $MYSQL_OPT -e "SELECT type, message FROM watchdog WHERE type = 'memorycons';" $MYSQL_DB

echo =================================
date
echo =================================
