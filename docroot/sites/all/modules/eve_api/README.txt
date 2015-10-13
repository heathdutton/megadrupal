WHAT DOES IT DO?
----------------

The purpose of Omni EVE API is to incorporate and make use of the EVE Online
API for user registrations, role management, and site administration.

The module will pull the EVE API from an alliance executors API Key to grab
the current standings and to pull the corporations in the alliance. This
information is what is used as the standard to measure the EVE API data being
validated at registration.

When a user registers on the site, they are required to enter an API Key and
Verification code that is linked to their EVE Online Gaming character. The
site then processes checks to verify if the character meets certain criteria,
such as standings. If the character passes the criteria the user is allowed
to join and is added to the appropriate roles at registration.

TeamSpeak 3 and Jabber have been fully integrated. Users are able to register
on the site to gain roles and access in TeamSpeak 3 and Jabber without any
admin interaction. Roles are mirrored in TeamSpeak 3 and Jabber. Any role a
user has in Drupal will be mirrored in TeamSpeak 3 and Jabber.

This module was inspired by the https://code.google.com/p/temars-eve-api/
Temars EVE API for SMF.

DEPENDENCIES
------------

 - Ctools (Only "Chaos tools" is required)
 https://drupal.org/project/ctools

 - Libraries
 https://drupal.org/project/libraries

 - TeamSpeak 3 (Optional)
 http://www.teamspeak.com/?page=downloads

 - Openfire (Jabber) (Optional)
 http://www.igniterealtime.org/downloads/index.jsp

 - cURL (libcurl For PHP) (Optional)
 http://www.php.net/manual/en/curl.installation.php
