The Simple LDAP project is a set of modules to provide Drupal integration with
an LDAPv3 server. It is an alternative to the Lightweight Directory Access
Protocol (LDAP) module, with a much narrower set of features. The goal of the
project is to provide very basic LDAP functionality which should cover most
common use cases. Any edge case functionality or site-specific requirements
should be implemented using a helper module.

The current implementation was developed against OpenLDAP, with some testing
against Active Directory. Most functionality should work with any LDAPv3
compliant server, but this is largely untested.

The project consists of one main module, and three submodules.

Simple LDAP
===========

This is the main module, on which all of the other modules are based. It
provides an interface to the configured LDAP directory with basic low-level
LDAP functions and no bells or whistles. It does not provide anything to
Drupal on its own.

Simple LDAP User
================

This module allows authentication to the LDAP directory configured in the
Simple LDAP module. It also provides synchronization services both to and from
LDAP and Drupal. It supports mapping LDAP attributes to Drupal user object
fields (both native, and using Field API).

Simple LDAP Role
================

This module allows Drupal roles to be derived from LDAP groups, and
vice-versa. It is dependent on the Simple LDAP User module.

Simple LDAP Test
================

This is a hidden module, which provides the glue to allow SimpleTest
integration to the other modules. In order to perform the other modules'
simpletests, there is a configuration file at
simple_ldap_test/simple_ldap_test.config.inc which needs to be edited.

===============================================================================

DEVELOPERS:

Enable debugging using devel module by adding the following setting to
settings.php

$conf['simple_ldap_devel'] = TRUE;
