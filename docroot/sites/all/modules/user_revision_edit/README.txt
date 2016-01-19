CONTENTS OF THIS FILE
---------------------
 * Summary
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers


SUMMARY
-------

This module adds the ability to edit recent revisions for the User Revision
module without modifying the user itself.

Example use case:
 * If you are running a site with revisioned user profiles and you use the
   revisions as a public archive. Sometimes you have to apply corrections to the
   public archive pages (revisions) without hacking the database or redoing the
   whole revision history.


REQUIREMENTS
------------

This module requires the following modules:
 * User Revision: http://drupal.org/project/user_revision


INSTALLATION
------------

 1. Place the entirety of this directory in:
      /sites/all/modules/user_revision_edit
 2. Navigate to Administration » Modules.
 3. Enable "User Revision Edit" and "User Revision".
 4. Navigate to a user profile and click the "Revision" tab.
 5. Find the "edit" link for each user revision available.


CONFIGURATION
-------------

The module overrides the "Revision" tab of the User Revision module. This module
is using the settings of the User Revision module.
 * Check out http://drupal.org/project/user_revision for more information.

This module creates two additional permissions in Administration » People »
Permissions:
 * Edit revisions: Allows the user with this permission to modify any existing
   user revision of the site. Warning: Give to trusted roles only, this
   permission has security implications.
 * Edit own revisions: Allows the user with this permission to modify only his
   own revisions. This permission has no known implications.

For a more detailed documentation and an issue tracker, check out:
 * https://www.drupal.org/sandbox/donschoe/2270791


TROUBLESHOOTING
---------------

 * The User Revision Edit module completely overrides the "Revision" tab of the
   User Revision module on the user's profile page. If you experience any issues
   related to that tab or you are missing features of the revision module,
   please get back to me as soon as possible.
 * If you notice any issues, you can report them here:
   https://drupal.org/project/issues/2270791


MAINTAINERS
-----------

Current maintainers:
 * Alexander Schoedon (donSchoe) - https://drupal.org/u/donSchoe

This project has been sponsored by:
 * Parlamentwatch e.V. (NGO)
   Parliament Watch enables citizens to question their members of parliament in
   a public environment, to find out about the voting record of their members
   of parliament, to follow up on promises made (all questions and answers are
   saved forever) and learn all about the extra earnings of members of a
   parliament on state, federal and European level.
   https://www.abgeordnetenwatch.de/ueber-uns/mehr/international
