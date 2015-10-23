CP2P2 - Content Profile to User Fields / Profile2
-------------------------------------------------
Converts Content Profile-managed content types on a Drupal 7 site, which has
already been upgraded from Drupal 6, to fields on the User entity or Profile2
profile types.


Prerequisites
-------------
In order for this to work the following is necessary:
* The site must be fully upgraded from Drupal 6 and all fields used on the
  profiles must have been upgraded already.
* If Profile2 is not enabled then all Content Profile content types will be
  merged together and copied to fields on the main User entity.
* If Profile2 is installed then Content Profile content types can be converted
  to separate Profile2 profile types.
