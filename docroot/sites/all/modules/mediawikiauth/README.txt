Mediawiki - Drupal authentication integration
=============================================

This module provides single sign-in and user database integration for
MediaWiki as slave of Drupal. It is set up so that users sign in to the
Drupal site, and as a result they automatically become logged in to the
wiki. User entries are still created in the wiki's user table and are
kept up to date on each login with email and real name.

The login integration works as follows: when the user signs in to
Drupal, an extra cookie is created containing their identity. When the
user visits the wiki, the wiki extension sees the cookie, extracts the
username, and logs the user in. When the user logs out of the Drupal
site, both the special cookie and any of the wiki's session cookies are
removed so the user is also signed out of the wiki.

Credits
-------

This implementation started with the code written by TazzyTazzy (Mitch
Schwenk) available on the DCCwiki. The code has changed a fair bit from
the original, modifications by Maarten van Dantzich. Support for
separate databases is based on Auth_phpBB. There is also some code from
Shibboleth_Authentication and
Ryan Lane.

The module was ported to Drupal 6 and maintained on Drupal.org by
Antoine Beaupré from Koumbit.org.

