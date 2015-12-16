Simple LDAP Role
================

This module allows Drupal roles to be derived from LDAP groups, and
vice-versa. It is dependent on the Simple LDAP User module.

Testing
=======

The simpletests provided with this module automatically configure themeselves
to use the active configuration in order to perform a real-world test against
your real LDAP server.

THIS MEANS THAT DATA WILL BE ADDED/DELETED ON YOUR REAL LDAP SERVER!

The simpletests only operate against entries it creates, but in the event of a
failure, the test cannot clean up after itself. If you are testing a specific
configuration, it is recommended to run the test against a development or
staging directory first.
