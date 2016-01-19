***********
* README: *
***********

DESCRIPTION:
------------
This module extends the email field. It adds the option to sent out a 
mail with a verification link and the process the link when clicked.
Additionally it offers behavior, if a link is not confirmed within a 
given time.


INSTALLATION:
-------------
1. Place the entire email_field_verification directory into your Drupal 
   sites/all/modules/ directory.

2. Enable the email field verification module by navigating to:

     administer > modules
     
3. If using this module with entities other than 'node' and 'user', you 
   will need the "Entity API (entity)" module.


DEPENDENCIES:
-------------
  * Email (email / core)
  * (optional) Entity API (entity)


FEATURES:
---------
  * sending verification link
  * handling link
  * handling timeout
  * offering resending link
      

AUTHOR:
-------
Christian Adamski
ChristianAdamski@drupal.org
