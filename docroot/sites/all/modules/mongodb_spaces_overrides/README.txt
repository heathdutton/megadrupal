
Configuration
=============

If you have already configured the mongodb module to connect to your mongodb 
instance, this module requires no further configuration.

New Installation
=============

Simply enable the module and your spaces_overrides data will be stored in
mongodb.  If you have an existing site with existing spaces_overrides data, see
the section on Migration.

Migration
=============

If you have an existing site which currently uses MySQL to store the
spaces_overrides table, you can either take your site offline, enable the
module and migrate the spaces_overrides data manually or attempt to use the
automatic migration process.

Automatic migration:
----------------------------
CAUTION: this process does NOT check whether an override already exists in
         mongodb before migrating/inserting it.

1. Set the site offline
  drush vset site_offline 1 -y

2. Set variable mongodb_spaces_overrides_auto_migrate = TRUE
  drush vset mongodb_spaces_overrides_auto_migrate 1

3. Enable the mongodb_spaces_overrides module
  drush pm-enable mongodb_spaces_overrides -y

4. Set the site online
  drush vdel site_offline -y

Manual migration:
-------------------------
NOTE: drupal is the default database for mongodb - if you are using a 
      non-default database, be sure to change this value

CAUTION: this command will drop the spaces_overrides collection if it already
         exists in the drupal database.

1. Set the site offline
  drush vset site_offline 1 -y

2. Enable the mongodb_spaces_overrides module
  drush pm-enable mongodb_spaces_overrides -y

3. Export existing MySQL data:

  SELECT type, id, object_type, object_id, value 
    INTO OUTFILE '/tmp/spaces_overrides.csv'
    FIELDS TERMINATED BY ',' 
    OPTIONALLY ENCLOSED BY '"'
    LINES TERMINATED BY '\n' 
    FROM spaces_overrides; 

4. Import data into MongoDB: 
  mongoimport -d drupal -c spaces_overrides -type csv -f type,id,object_type,object_id,value --drop /tmp/spaces_overrides.csv

5. Set the site online
  drush vdel site_offline -y