Purpose
------------

Allows Drupal to share user sessions with Primo.


Installation
------------

1. Place the primo_sso module into your modules directory (sites/all/modules).

2. Enable the module in admin >> site configuration >> modules.

3. Set up access permissions in admin >> user >> permissions for both
   'administer primo' and 'access primo discovery'.



Configuration
-------------

1. Go to the configuration page for primo
   (admin >> site configuration >> primo_sso ) and enter the base url,
   institution, and ticket secret for your primo server.


Usage
-----

1. When you click on a link to Primo, it will first ask Drupal if you are
   authenticated. If the user is currently logged in as has the
   'access primo discovery' permission then Drupal will report back to primo
   with the successful logon attempt and the user will be taken to Primo
   Discovery. If the user is not logged in, Drupal will prompt them to login.
   When successful, Drupal will report back to Primo with the successful logon
   attempt and the user will be taken to Primo Discovery.


Author/Maintainer
-----------------

Jason Sherman jsn.sherman@gmail.com
