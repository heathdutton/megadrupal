Clientcert (SSL/TLS) for login and mail encryption

This module is for managing SSL/TLS client certificates to use for login or
part of a multi-factor login strategy based on Rules - and Gate-module to
improve login security. If the complete public key is stored an encryption on
mail sending should be possible.

# THIS MODULE is in early state of development.

There is another module certificatelogin. This  only provides a simple direct
login functionality for SSL/TLS client certificates and a user generation
based on certificate information.
The clientcert module will be completely integrated in rules. So if you really
want to create users with it you can create a rule. But there will be more
possibilities like multi-faktor login or mail encryption like the OpenPGP
module.
And because everything will be based on rules you can create individual
security strategies like users with admin role have to use a certificate and
other users can decide.

# Dependencies
https://drupal.org/project/entity
https://drupal.org/project/rules
https://drupal.org/project/gate

# Current features
Managing SSL/TLS client certificates with additional information.
Login with SSL/TLS client certificate.

# Future features
Rules for encrypting Text especially to encrypt mails.
Default Rules/Rule components to use or clone.

# concept, development and maintenance
The module is developed and maintained by Carsten Logemann (paratio.com e.K.)
