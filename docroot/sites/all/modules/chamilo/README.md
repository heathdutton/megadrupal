Chamilo-Drupal module
====================
This module provides connectivity between a Drupal website and a Chamilo
LMS. Although stable, this module still lacks a good pack of usability
features, so please try it on a test server before you put it online.

Its current features are
- Single Sign On from Drupal to Chamilo (currently disabled)
- View list of Chamilo courses in a Drupal block
- View list of own Chamilo courses in a Drupal block
- View list of own Chamilo events in a Drupal block

To enable the Single Sign On, you should:
- always use HTTPS (because otherwise your call to Chamilo is not
  secure)
- login on your Chamilo portal and configure the SSO module from the
  Security tab in your admin section
- at this point, and before doing anything else, it is essential that
  you define a domain for the sso_authentication_domain parameter,
  because otherwise loading the main page of your portal will result in
  an ugly error
- it is possible that you would also need to insert the following into the
  settings_current table (this has to be verified):
    INSERT INTO settings_current (variable, subkey, type, category, selected_value, title, comment, scope, subkeytext) VALUES ('sso_authentication_subclass',NULL,'textfield','Security','Drupal','SSOAuthSubClassTitle','SSOAuthSubClassComment',NULL,NULL);

This should work (it requires accounts on Drupal to be created with the
same username as in Chamilo)
