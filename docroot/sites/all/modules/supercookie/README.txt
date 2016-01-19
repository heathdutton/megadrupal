Drupal supercookie module:
================================================================================
Maintainers:
  Nate Mow (https://drupal.org/user/1564666)
Requires - Drupal 7
License - GPL (see LICENSE)

Overview:
================================================================================
From some marketeer out in the ether:

  "Recently, online properties like Hulu, MSN and Flixster have been caught
  using a tougher version of the common cookie. These “supercookies” (aka
  "Flash cookies" and “zombie cookies”) serve the same purpose as regular
  cookies by tracking user preferences and browsing histories. Unlike their
  popular cousins, however, this breed is difficult to detect and subsequently
  remove. These cookies secretly collect user data beyond the limitations of
  common industry practice, and thus raise serious privacy concerns."

By default, this module only stores a hash of the client and server-side
variables that it gathers. The cookie itself however, is difficult to remove or
hack. Tracking unique anonymous visitors can be extremely useful for submodules
-- and the standard Drupal pattern of using core's ip_address() function for
uniqueness is often insufficient.

Inspired by the work at panopticlick.eff.org.
