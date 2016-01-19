Alt Login Page
--------------
Provides an alternative page for users to log into the site. Designed as an
additional entry point for admins when a single sign-on (SSO) system is in use
for the primary site login.


Features
------------------------------------------------------------------------------
The primary features include:

* Full control over the path for the new login page.

* Full control over a custom path for an alternative logout destination path.

* Full control over which user roles have permission to use the login page.

* Leverages Drupal core's existing user login codebase for the login form,
  improving security over custom written code.


Configuration
------------------------------------------------------------------------------
 1. On the People Permissions administration page ("Administer >> People
    >> Permissions") you need to assign:

    - The "Use alternative login page" permission to the roles that are allowed
      to use the new login page.

 2. The main settings page controls the path to the new login page:

      admin/config/people/set-front-page


Credits / Contact
------------------------------------------------------------------------------
Currently maintained by Damien McKenna [1].

Development is sponsored by Mediacurrent [2].

The best way to contact the author is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/alt_login_page


References
------------------------------------------------------------------------------
1: https://www.drupal.org/u/damienmckenna
2: http://www.mediacurrent.com/
