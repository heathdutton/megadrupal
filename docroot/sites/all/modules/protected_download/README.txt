Protected Download
==================

This module provides a way to grant access to specific files for a limited
period of time to anonymous users. The generated links are protected by a HMAC,
i.e. a long string which is not likely to be guessed by an attacker.

Unlike private files delivered by Drupal core, downloads provided by this module
are cacheable and also support HTTP cache revalidation. This makes it possible
to efficiently serve assets to mobile applications while still maintaining some
access restrictions.

The module provides text replacement tokens. This is especially useful when
sending links to private files via email.



Use cases
=========


Deliver cacheable non-public images
-----------------------------------

For some sites it is necessary to restrict access to media files (e.g., images)
to authenticated users. Traditionally this is done by using the built-in private
file system as the upload destination for an image field. However, files
delivered with that method are not cacheable - neither by the browser nor by an
intermediate cache.

In order to solve this problem, this module provides a protected file system.
The URLs generated with this method never point directly to the file. Instead an
expiry date is embedded into the URL which is used to govern the cache lifetime.
After the expiry date passed it is not possible anymore to access the file using
that URL.

Configuration:
1. Install and enable the "Protected Download" module.
2. Navigate to "Administration » Configuration » Media » File system".
3. Enter a path into "File system path for HMAC protected files". Note that it
   is recommended to enter a path outside the document root.
4. Optionally customize the cache characteristics inside the "Protected file
   system" fieldset.
5. Click Save configuration.
6. Select the option "Protected files" as "Upload destination" for image or file
   fields.

Note that protected files are accessible by any user - including anonymous. It
is therefore important to choose the "Minimum/Maximum TTL" values wisely. The
"Minimum TTL" for assets must be higher than any expiry date of the generated
markup. Especially it must be higher than the "Expiration of cached pages"
setting on the "Administration » Configuration » Development » Performance"
screen. The "Maximum TTL" setting should be set as high as possible. The bigger
the difference between "Minimum TTL" and "Maximum TTL", the better the cache hit
ratio, but also the longer potentially leaked URLs are accessible to anyone.


Send expirable links to private files via email
-----------------------------------------------

Sometimes it is desirable to send links to private files via email. However, if
people are not logged into the site when opening those links they likely get
confused by the access denied screen popping up when clicking on the link. This
module provides a way to generate links with an expiry date accessible to anyone
regardless of whether they are logged into the site or not.

The following tokens are provided by this module:
* site:protected-download-expire The date when a protected download generated
  now will expire.
* file:protected-download-url The web-accessible URL for the file protected with
  an authentication code.

A great place to use this tokens is in a "Send mail" rules action. For example
when selling virtual goods via Drupal Commerce, the token can be used in a mail
sent to the customer as a reaction to the "When an order is first paid in full"
event.
