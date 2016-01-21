******************************************************************** 
D R U P A L  M O D U L E 
******************************************************************** 
Name: LimeSurvey Sync
http://drupal.org/project/limesurvey_sync 
Author: Julien Duteil <julienledut at gmail dot com>
Version : 7.x-1.7, 2014/05/10
******************************************************************** 

DESCRIPTION :

In a nutshell, this module is a new and powerfull survey module on Drupal 
integrating in a user-friendly way the best open source survey software : 
LimeSurvey.

Compared to the well known webform module : this module does everything 
that webform does and much more.
Specifically :
- manage answers as nodes (allowing to manage access to answers),
- store revisions (older versions) of answers,
- make graph results availables,
- manage huge surveys.

******************************************************************** 
INSTALL INSTRUCTIONS :

1) Download the LimeSurvey software from http://www.limesurvey.org
2) Install it on a server (instructions on http://docs.limesurvey.org).
3) copy the 'limesurvey_sync' folder into the module folder 
of your Drupal site (into the '/sites/all/modules/' folder).
4) Enable all 3 modules : 'LimeSurvey Sync', 'LimeSurvey Sync Survey' and
'LimeSurvey Sync Responses'.
5) Go to the synchronization setting page on 'admin/config/media/ls_sync' 
and make sure all 5 tests have passed. For troubleshootings, read the 
help provided on the same page.
6) Got to the permission settings page (/admin/user/permissions) to set the
appropriate rights for creating, editing, deleting surveys and answers.

Note : It is not recommanded to install the LimeSurvey folder into the 
same root folder than your Drupal site. Anyway, If you install it there, 
you may need to edit the htaccess file in order to make the LimeSurvey 
site reachable.

******************************************************************** 
START USING THIS MODULE :

1) Go to your LimeSurvey admin interface and create a new survey. 
Activate it (Help on http://docs.limesurvey.org).
2) Import it on your Drupal site, creating a new survey content (Go to 
node/add/survey).
3) Start completing the survey : Go to 
node/add/the_type_you_have_defined_in_step2 or follow the link in the 
footer aera of the new survey node page.
4) Go to your LimeSurvey admin interface to manage results.
5) Depending on your need, you can stop here or go on importings answers 
into drupal. Go to the node survey synchronization page in order to 
import answers into Drupal.

Note that this module does not override the LimeSurvey settings : in 
order to access to specifics advanced features of the LimeSurvey 
synchronization module, you may need to adapt settings on your 
limeSurvey admin interface.
The node survey view page print the survey settings from the LimeSurvey 
site reflecting the ones that have consequences on availables features.
For a specific setting, check the help section on the node survey edit 
page or the 'node/add/survey' page. It is called 'Help on LimeSurvey 
settings for specific features on this site' and the section below : 
'For custom use'.

******************************************************************** 
LIMITATIONS :

- from LimeSurvey : once a survey has been completed, some changes are 
not possibles, for instance the ability to delete questions. Anyway some 
workarounds can solve those situations. This module should work on old 
LimeSurvey version too (> 1.81).
- from drupal : Drupal only support MySQLl, PostgreSQL and SQLite 
databases. If you already host a LimeSurvey site on a different database
system, you will have to migrate to a drupal compatible one.
