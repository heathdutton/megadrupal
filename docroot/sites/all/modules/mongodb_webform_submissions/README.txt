
Configuration
=============

If you have already configured the mongodb module to connect to your mongodb 
instance, this module requires no further configuration.

New Installation
=============

Simply enable the module and your webform submissions will be stored in
mongodb.  If you have an existing site with existing webform submissions, see
the section on Migration.

Migration
=============

If you have an existing site which currently uses SQL to store the
webform submissions, you can either take your site offline, enable the
module and migrate the webform submissions manually or attempt to use the
automatic migration process.

Automatic migration:
----------------------------
CAUTION: this process does NOT check whether a submission already exists in
         mongodb before migrating/inserting it.

1. Set the site offline
  drush vset site_offline 1 -y

2. Set variable mongodb_webform_submissions_auto_migrate = TRUE
  drush vset mongodb_webform_submissions_auto_migrate 1

3. Enable the mongodb_webform_submissions module
  drush pm-enable mongodb_webform_submissions -y

4. Set the site online
  drush vdel site_offline -y

Manual migration:
-------------------------
NOTE: drupal is the default database for mongodb - if you are using a 
      non-default database, be sure to change this value

CAUTION: this command will drop the webform_submissions collection if it already
         exists in the drupal database.

1. Set the site offline
  drush vset site_offline 1 -y

2. Enable the mongodb_webform_submissions module
  drush pm-enable mongodb_webform_submissions -y

3. Export existing SQL data:

  SELECT ...
    INTO OUTFILE '/tmp/webform_submissions.csv'
    FIELDS TERMINATED BY ',' 
    OPTIONALLY ENCLOSED BY '"'
    LINES TERMINATED BY '\n' 
    FROM ...; 

4. Import data into MongoDB: 
  mongoimport -d drupal -c webform_submissions -type csv -f ... --drop /tmp/webform_submissions.csv

5. Set the site online
  drush vdel site_offline -y