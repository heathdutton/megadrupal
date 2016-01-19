--------------------------------------------------------------------------------
Profile Fields Force Filling
--------------------------------------------------------------------------------

Maintainer:  xlyz, xlyz@tiscali.it

Force users to fill any required profile fields at login.

Poject homepage: http://drupal.org/project/pfff

Issues: http://drupal.org/project/issues/pfff


Installation
------------

 * Copy the whole delivery_commerce directory to your modules directory
   (e.g. DRUPAL_ROOT/sites/all/modules) and activate it in the modules page


Documentation
-------------

When enabled, every time a user login it checks if any required fields in his
profile still need to be filled. User is not allowed to login unless he fills
the required fields.

Useful when: 

 * registration through third party services (facebook, linkedin, you name it)
   and required data can not be imported
 * a new required field has been added to the users profile and we need to
   force existing users to fill it 


Wish list
---------
 * force user to review any non required fields at login
 * allow admin to create new profile leaving required fields empty

Credits
-------

Thanks to the legal module devs from which I borrowed a nice chunk of the code.
