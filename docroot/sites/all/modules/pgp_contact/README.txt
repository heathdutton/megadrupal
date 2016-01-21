Description
-----------
This module provides a means for users to upload their PGP public keys and connect them to their profiles.

These keys can then be used to encrypt contact form submissions.

Requirements
------------

* Drupal 7.x
* OpenPGP.js 0.7.2 or lower

Installation
------------
1. Copy the pgp_contact directory into the Drupal sites/all/modules directory.

2. Download the openpgp.js library from https://github.com/openpgpjs/openpgpjs/releases/tag/v0.7.2, and put them in the directory `sites/all/libraries/openpgpjs`.

Usage
-----
1. Login.

2. Go to your user page (/user), click on the PGP public key tab, and paste your key into the textarea there.

3. Click on "verify public key". Check that the right email address appears. The key id and hash that appear are internal values for this module. (We're working on fixing that.)

4. Click on submit.

Configuration
-------------
The site can be configured via Admin -> Config -> System -> PGP settings (admin/structure/contact/pgp). You can specify a different location for your javascript libraries, as well as specify a default encryption policy for your site and allow anonymous users to use user contact forms.

This page also allows you to force the usage of SSL on certain pages. However, you must add the directive `$conf['https'] = TRUE;` to your site's settings.php file.

A pgp key for encrypting the site wide contact form can be uploaded via Admin -> Structure -> Contact form -> PGP public key (admin/structure/contact/pgp-key).

Support
-------
Please use the module issue queue to report bugs and request support:
http://drupal.org/project/issues/pgp_contact
