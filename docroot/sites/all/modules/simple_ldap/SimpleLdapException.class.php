<?php
/**
 * @file
 * Class to handle Simple LDAP exceptions.
 */

class SimpleLdapException extends Exception {

  /**
   * Inherited __construct().
   */
  public function __construct($resource) {
    if (is_resource($resource)) {
      // Handle LDAP operation errors.
      $error = ldap_errno($resource);
      $message = ldap_err2str($error);
      parent::__construct($message, $error);
    }
    else {
      // Handle exceptions that are not related to an LDAP resource link.
      parent::__construct($resource);
    }

    // Log the exception.
    watchdog_exception('simple_ldap', $this);
  }
}
