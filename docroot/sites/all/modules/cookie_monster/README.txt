===============
Cookie Monster
===============

Cookie monster provides a simple api and rules integration for managing the creation and use of secure cookies.
NOTE: This module does nothing on its own, but provides tools for developers and site builders to use
when they need to set cookies.

Setting a cookie is a useful and common method for storing state on a user's browser. Also, it can be
preferable to using a 'session variable' (adding to $_SESSION) because sessions can kill advanced
reverse-proxy caching methods such as Varnish.

Installation and Configuration:

1) Install in the usual way.
2) Navigate to /admin/config/system/cookie-monster and add a 15-20 alpha-numerical string to the encryption-
   key field. This will be used in encrypting and decrypting the cookie data.

Use in Code:

Use the functions cookie_monster_set_cookie($data, $session), cookie_monster_cookie_exists($id), and
cookie_monster_expire_cookie($id).

  - cookie_monster_set_cookie($data, $session) sets a cookie using the data in array $data. The only required
    key/value in $data is the 'id' key which can be any string. If $session is TRUE then a session variable
    will also be set (defaults to FALSE).
  - cookie_monster_cookie_exists($id) returns TRUE if the cookie with id == $id is set. See above about the
    cookie 'id'.
  - cookie_monster_expire_cookie($id) expires (removes) a cookie and session variable (if exists).

Rules:

Cookie Monster provides the following rule components:

  - Events:
    cookie_monster_rules_cookie_set - triggered when a cookie is set.
    cookie_monster_rules_cookie_expired - fires when a cookie has expired (via callback function above only).
  - Conditions:
    cookie_monster_rules_cookie_exists - TRUE if cookie is set.
  - Actions:
    cookie_monster_rules_load_cookie_data - loads the cookie data into a serialized string that can then
                                            be used by other rules.
    cookie_monster_rules_set_cookie - sets a cookie.
    cookie_monster_rules_expire_cookie - expires a cookie.
