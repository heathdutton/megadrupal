<?php

/**
 * @file
 * The entity controllers for the mailadmin entities.
 */

/**
 * Entity controller for the mailbox entity
 */
class MailadminMailboxController extends EntityAPIController {
  /**
   * Create a default mailbox.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A mailbox object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);
    return (object) ($values + array(
      'mailbox_id' => '',
      'local_part' => '',
      'domain_name' => '',
      'password' => '',
      'real_name' => '',
    ));
  }
}



/**
 * Entity controller for the mailbox entity
 */
class MailadminForwardController extends EntityAPIController {
  /**
   * Create a default mail forward.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A mail forward object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);
    return (object) ($values + array(
      'forward_id' => '',
      'local_part' => '',
      'domain_name' => '',
      'destination' => '',
    ));
  }
}

/**
 * Entity controller for the alias entity.
 */
class MailadminAliasController extends EntityAPIController {
  /**
   * Create a default mail forward.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A mail forward object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);
    return (object) ($values + array(
      'alias_id' => '',
      'source_domain_name' => '',
      'target_domain_name' => '',
    ));
  }
}
