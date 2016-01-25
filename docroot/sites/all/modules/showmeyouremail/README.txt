A really simple Drupal module that provides a permission that shows users
email address on their profile pages.

***Important***
Giving this permission to users should be thought through. The recommended
use of this module is on a site where you have control over the user accounts
that can view email addresses. In other words you probably shouldn't give the
authenticated user role this permission unless only site administrators can
create user accounts.

*******************************************************************************
INSTALLATION:

1. Place the showmeyouremail directory into your Drupal sites/all/modules
   directory.


2. Enable the showmeyouremail module by navigating to:

     Administer > Site building > Modules

3. If you want anyone besides the administrative user to be able
   to view user email addresses (usually a bad idea), they must be given
   the "View user email addresses" access permission in addition to the "access
   user profiles" permission:

     Administer > User management > Permissions

   When the module is enabled and the user has the "View user email addresses"
   access permission in addition to the "access user profiles" permission they
   should be able to navigate to a user profile page and view that user's
   email address.

*******************************************************************************