Add Dutch postal code(postcode) check and address lookup to Address Field.
This module uses the webservice of postcode.nl and only works for Dutch
(Nederlandse) postcodes.

Requirements
============

Ctools - ctools - http://drupal.org/project/ctools
Address Field (Dûh) - addressfield - http://drupal.org/project/addressfield

Usage
=====

- Install and enable module 'addressfield_nl_postcode' as usual.
- Get an account from https://api.postcode.nl/ (free use for webshops) and
  obtain a key and secret.
- Copy your Postcode.nl Key and Postcode.nl Secret to the settings form
  (admin/config/services/addressfield-nl-postcode).
- Create an addressfield on a entity or edit an existing one.
- Enable setting 'Postcode check and addresslookup (NL add-on)' in the
  addresfield widget settings.
- Test your entity and enjoy!

Bugs
====

Please report bugs and issues on drupal.org in the issue queue:
http://drupal.org/project/issues/addressfield_nl_postcode

Remember to first search the issue queue and help others where you can!

Credits
=======

The Drupal module Address Field NL Postcode is built and maintained by Drupal
internetbureau .VDMi/ in Rotterdam, The Netherlands.