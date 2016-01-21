
-- SUMMARY --

The UBB integration module provides automatic creation/update of UBB
accounts whenever a user registers/edits a Drupal account. It also includes
simple import/export tools for synchronizing users between UBB and Drupal, as
well as a single sign-on feature. Accounts are matched by username; accounts
that share the same name in UBB and Drupal are assumed to be the same person.

For a full description of the module, visit the project page:
  http://drupal.org/project/ubb

To submit bug reports or feature suggestions, or to track changes:
  http://drupal.org/project/issues/ubb


-- REQUIREMENTS --

* UBB.threads 7.5.4.x. Other versions may work, but have not been tested.

* Your Drupal instance must be able to connect to UBB's database.

* The single sign-on feature only works when Drupal and UBB are installed under
  the same domain (either as the root or in a subdirectory). Cross-domain or
  subdomain single sign-on is not supported, due to UBB's cookie handling code.


-- INSTALLATION --

1. In your Drupal site's settings.php file, locate the "Database settings"
  section and carefully read the explanation of multiple database connections in
  the comments. MAKE SURE YOU UNDERSTAND WHAT YOU ARE DOING BEFORE PROCEEDING.
  If you make a mistake while configuring the $databases variable (step 2, below),
  your Drupal site may become unavailable.

2. Add a connection in the $databases variable for your UBB database.
  Use the key 'ubb' for the new connection, as shown here:

    $databases['ubb']['default'] = array(
      'driver' => 'mysql',
      'database' => 'ubb_database_name',
      'username' => 'ubb_database_user',
      'password' => 'ubb_database_password',
      'host' => 'localhost',
      'prefix' => 'ubbt_',
    );

  Check your UBB includes/config.inc.php file for the correct values to set.

3. Install the ubb module as usual (see http://drupal.org/documentation/install/modules-themes/modules-7).

4. Go to the module's config page (http://yourdrupalsite.com/admin/config/ubb).
  If the database connection was configured correctly, the page will display
  the UBB version number.

5. Fill in the cookie prefix and path fields to match the COOKIE_PREFIX and
  COOKIE_PATH values in your UBB includes/config.inc.php file.

6. Check the "Enable UBB integration" box. This enables the following features:
    - When a user registers in Drupal, a matching account will be created in UBB.
    - When a Drupal user changes their username, email address or password, the
      corresponding UBB account will also be updated.
    - Blocking a user in Drupal will also ban that user in UBB.

7. Click the "Save configuration" button.

*** PLEASE NOTE: Once you have enabled the integrations, it is highly
recommended that you also disable UBB's registration screens in order to
prevent users from registering through UBB directly rather than through Drupal.
If you are using the single sign-on feature, you may also wish to hide UBB's
login form. You may need to modify UBB's code in order to do this. The
maintainer of this Drupal module cannot provide support for modifications to UBB.
Contact the UBB developers for assistance instead.


-- SINGLE SIGN-ON --

The single sign-on feature allows your users to be automatically logged into
UBB when they login to your Drupal site. This feature works by creating a
session in the UBB database and setting a cookie that allows UBB to recognize
the user as having already logged in. Unfortunately, due to UBB's cookie
handling code, single sign-on will only work when UBB and Drupal are running
under the same domain name; subdomains or separate domains are not supported.

To activate single sign-on for some or all of your users, assign them the
"Automatically login to UBB" permission in your Drupal site config. Typically,
this permission would be assigned to the authenticated users role to allow all
users access to UBB.


-- IMPORT AND EXPORT --

If you already have users in UBB, you can go to the module config page and use
the Import tab to create Drupal accounts for them. Similarly, if your UBB site
has fallen out of sync with your Drupal site, you can use the Export tab to
copy your Drupal users to UBB. Note that these are both very simplistic tools;
if you have a large number of users to import/export, or if you need precise
control over which accounts get copied, consider using the Migrate module
instead (http://drupal.org/project/migrate).


-- CREDITS --

Author/maintainer:
* Dan Lobelle (muriqui) - http://muriqui.net/

The Drupal 7 version of this project was sponsored by:
* Christians in Recovery - http://christians-in-recovery.org/
  Your Internet community for recovery.  Always available, always caring.

THIS PROJECT IS NOT AFFILIATED WITH UBB OR ANY OF ITS DEVELOPERS OR STAFF.
USE OF THIS MODULE REQUIRES A PROPERLY LICENSED INSTALLATION OF UBB.
