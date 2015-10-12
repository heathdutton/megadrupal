MailChimp List Reference Formatter
**********************************
This module provides a formatter for the entity reference[1] field that displays
MailChimp sign-up forms. You can use it to allow website admins an easy method
to choose which sign-up forms to display on individual entities (eg. nodes,
taxonomy terms, etc.).

Although it supports multiple sign-up forms, each one is displayed individually.
See #1232744[2] for progress on providing a single unified form.

Dependencies
************
1. Entity reference[1]
2. MailChimp Lists (a submodule of the MailChimp project[3])

Installation & Usage
********************
1. Install and enable as usual[4].
2. Set up an entity reference field that's configured to refer to entities of
   type 'MailChimp List'. It can be single or multi-value.
3. Under 'Manage Display' configure the field to use 'MailChimp sign-up form'.
   There's no further configuration.

Links
*****
[1] http://drupal.org/project/entityreference
[2] http://drupal.org/node/1232744
[3] http://drupal.org/project/mailchimp
[4] http://drupal.org/documentation/install/modules-themes/modules-7

author: AndyF
http://drupal.org/user/220112
