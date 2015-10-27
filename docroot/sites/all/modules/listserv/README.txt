Listserv ---------------------------------------------

DESCRIPTION
-----------
This module defines utilizes email to subscribe and unsubscribe users from a
listserv that is defined by an administrator. It creates two blocks(subscribe
and unsubscribe) that gives users the abiltity to enter their email address to
subscribe and unsubscribe from a listserv.

The module also allows other modules to use the listserv_subscription function
to subscribe and unsubscribe a user from their modules.

A custom drush command is also defined. Use 'drush lsvs [email]' to subscribe a
user to the listserv. Type 'drush lsvs --help' for more options.

This module has been tested with Listserv 16 only.

CONFIGURATION
-------------
After you install the module, visit the configuration
page(admin/config/services/listserv) where you will define two variables:

  * Listserv Email- What is the email address of the Listserv system. The
    module will send subscription emails to this address. An example would be
    "listserv@lists.mycompany.com".

  * Default Listserv Name- This will be the machine name of the list you
    would like users to be subscribing to or unsubcribing from. An example would
    be "L-WEBSITE-NEWSLETTER".

After you have defined your listserv information in the configuration page, you
can utilize the "listserv_subscribe" and "listserv_subscribe" blocks. You can
find these in "admin/structure/block".

MAINTAINERS
-----------
  * Michael Potter (heymp) - https://www.drupal.org/user/466258
