CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers
 * API notes

INTRODUCTION
------------
Mapit (http://drupal.org/project/mapit) is a Drupal 7 module that
implements part of the MapIt API to auto-tag administrative boundaries 
based on a latitude and longitude. The module provides a Mapit Autofill widget
that automatically adds an area tag to an entity after cron is run, when a user
gives the entity geofield coordinates.

For more information on Mapit visit http://drupal.org/node/_______

REQUIREMENTS
------------
This module requires the following modules:
 *Chaos tools (https://drupal.org/project/ctools)
 *Geofield (https://drupal.org/project/geofield)
 *geoPHP (https://drupal.org/project/geophp)
 *Job Scheduler (https://drupal.org/project/job_scheduler)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
*Create a vocabulary that will hold the location tags fetched by the module
in Administration » Structure » Taxonomy.
*Add the following fields to the vocabulary in Administration » Structure
 » Taxonomy » <your vocabulary> :
  1. A textfield that will hold the OSM Id of the area .
  2. A textfield that will hold the OSM Relation Id of the area.
  3. A textfield that will hold the OSM type of the area.
  4. A geofield that will hold the full geometry of the area. The widget for
     this field should be a Well Known Text widget.
  5. A link field that will hold the wikipedia link of the area.
  6. A textfield that will hold the long name of the area.
  7. A textarea that will hold the svg data of the area's flag image.
*Configure the module in Administration » Configuration:
  1. Select your vocabulary from the Vocabularies select list.
  2. Select the corresponding field for each of the mapit settings.
  3. Save the  configuration.
*Add the following extra fields to any entity that you wish to be tagged
by the mapit module in Administration » Structure » <your chosen entity type> :
  1. A geofield that will hold the latitude and longitude of the area.
  2. A term reference field that uses the Mapit Autofill widget with <your vocabulary> 
     as the vocabulary that supplies options for the field and your previously 
     created geofield as the geofield that will be attched to this field.
*A user can then add geofield coordinates to the entity and it shall be
given an area tag automatically by the module when cron is run.

TROUBLESHOOTING
---------------
*If the entity is not tagged after cron is run, check that the users 
internet connection is good and rerun cron.

MAINTAINERS
-----------
Current maintainers:
 * Reinier Battenberg (batje) - https://drupal.org/user/2696
 * Dan Colin Mulindwa (dcmul) - https://drupal.org/user/2505784
 * Joanna Kisaakye (Joanna_Kisaakye) - https://drupal.org/user/2834589
 