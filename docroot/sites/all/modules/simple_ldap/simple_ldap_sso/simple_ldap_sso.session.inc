<?php

/**
 * @file
 * Session include for loading session ids from LDAP.
 */

require_once DRUPAL_ROOT . '/includes/session.inc';
require_once 'simple_ldap_sso.inc';

simple_ldap_sso_detect_sid();
