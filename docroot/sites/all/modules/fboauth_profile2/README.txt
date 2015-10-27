Description
-----------
This module provides integration between the Facebook OAuth and Profile2
modules. It allows to map Facebook properties to profile fields during user
registration.

For more information see:
http://drupal.org/project/fboauth
http://drupal.org/project/profile2


Dependencies
------------
Profile2 7.x-1.0 or higher.
Facebook OAuth 7.x-1.0 or higher.


Usage
-----
1. Enable this module and the dependent modules on the Modules page.
2. Create a new profile at "Structure" -> "Profile Types" -> "Add Profile Type"
   (admin/structure/profiles/add). Make sure to enable the option "Show during
   user account registration".
3. Add some fields to the profile, e.g. "First name", "Last name", "Gender" and
   "Facebook profile link".
4. Map the Facebook properties to the profile fields at "Configuration" ->
   "People" -> "Facebook OAuth settings" (admin/config/people/fboauth).
