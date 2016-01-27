
CONTENTS OF THIS FILE
---------------------

 * Summary
 * Installation
 * Limitations
 * Troubleshooting
 

SUMMARY
-------

The stager module makes it possible to create one or more staging versions 
of your website where you can make any changes without affecting the 
live site. When you've done your changes in staging mode, you can sync all 
those changes back to the live site.

There are limitations in the 7--1 branch. Read them before you submit features.
Only bug fixes will go into that branch. New features should go into a separate
branch, but only if there is a co-maintainer willing to maintain that.


INSTALLATION
------------

1. Download the module and place it in sites/all/modules. If you place it
   somewhere else, remember that path, you'll need it later on.

2. Open settings.php and look for the $databases array and 
   add following line before that variable:

   require_once(DRUPAL_ROOT . '/sites/all/modules/stager/stager.module');

   If the module is in another directory, change the path to the module.

3. Still in settings php, look for the 'prefix' key in your database and
   change that to the following:

   'prefix' => (function_exists('stager_pre_bootstrap')) ? stager_pre_bootstrap() : '',

   If you are already using a prefix, put that prefix between the single quotes.

4. Go to admin/modules and enable the module, listed in the 'Other' fieldset.
   Give permissions to users at 'admin/people/permissions'.

5. Go to 'admin/config/system/stager' and create a stager site.
   If all went well, a grey bar should appear at the bottom of your screen with
   following text:

   "You are working on the staged version of your site called 'title here'. All changes 
   are not visible on the live version. Click here to put your changes live."

   You can now make any change without affecting the live version of your site.
   When you're ready, click on the link and sync all changes back and/or remove 
   the staging version of your website.
   
6. 'stager_ignore_tables' variable.
   Add a variable to settings.php to ignore data from to tables not to be synced back.
   
   $conf['stager_ignore_tables'] = array(
     'webform_submissions',
     'webform_submitted_data',
   );

LIMITATIONS
-----------

- The prefix of the original site must be empty. You can not start with a prefixed site.
- External stream wrappers will never work and be supported.
- It only works with public files.
- You can only create 1 stager site.
  
TROUBLESHOOTING
---------------

In case something goes wrong when syncing a site through the interface, you'll see a
database error. Look at your cookies and remove the stager cookie so you're back on the
original site.
