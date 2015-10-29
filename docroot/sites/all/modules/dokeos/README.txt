Dokeos-Drupal module
====================
This module provides connectivity between a Drupal website and
a Dokeos LMS. Although stable, this module still lacks a good pack of usability
features, so please try it on a test server before you put it online.

Its current features are
- Single Sign On from Drupal to Dokeos
- View list of Dokeos courses in a Drupal block
- View list of own Dokeos courses in a Drupal block
- View list of own Dokeos events in a Drupal block

To enable the Single Sign On, you should:
- always use HTTPS (because otherwise your call to Dokeos is not secure)
- insert new variables in the Dokeos main.settings_current table:
INSERT INTO settings_current 
(variable, subkey, type, category, selected_value, title, comment, scope, subkeytext)
VALUES
('sso_authentication',NULL,'radio','Security','false','EnableSSOTitle','EnableSSOComment',NULL,NULL),
('sso_authentication_domain',NULL, 'textfield','Security','','SSOServerDomainTitle','SSOServerDomainComment',NULL,NULL),
('sso_authentication_auth_uri',NULL,'texfield','Security','/?q=user','SSOServerAuthURITitle','SSOServerAuthURIComment',NULL,NULL),
('sso_authentication_unauth_uri',NULL,'textfield','Security','/?q=user/logout','SSOServerUnAuthURITitle','SSOServerUnAuthURIComment',NULL,NULL),
('sso_authentication_protocol',NULL,'radio','Security','http://','SSOServerProtocolTitle','SSOServerProtocolComment',NULL,NULL);

- insert new variables in the Dokeos main.settings_options table:
INSERT INTO settings_options 
(variable, value, display_text)
VALUES
('sso_authentication', 'true', 'Yes'),
('sso_authentication', 'false', 'No'),
('sso_authentication_protocol', 'http://', 'http://'),
('sso_authentication_protocol', 'https://', 'https://');

- login on your Dokeos portal and configure the SSO module from the Security tab
  in your admin section

- at this point, and before doing anything else, it is essential that you define a domain for the sso_authentication_domain parameter, because otherwise loading the main page of your portal will result in an ugly error (because of a bug in 1.8.6 which has been fixed in development versions after Dokeos 1.8.6 stable)

This should work (it requires accounts on Drupal to be created with the same
username as in Dokeos)
