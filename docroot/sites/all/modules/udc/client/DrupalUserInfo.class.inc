<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 User Data Connector module - External script INFO client class
 * -----------------------------------------------------------------------------
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.module.udc
 * @license GPL2
 */

require_once dirname(__FILE__) . '/DrupalUserBase.class.inc';

/**
 * Client class for retrieving information about a user. The class is very similar
 * to the DrupalUserAuth class, with the exception that the authentification is
 * skipped, and the user is marked as valid (property valid=TRUE) if the request
 * succeeded and the user was found (and the server script is allowed to send
 * information about this user).
 */
class DrupalUserInfo extends DrupalUserBase {

  /**
   * Contains if the requested user exists
   * @var bool
   */
  public $valid = FALSE;

  /**
   * The drupal registered user name
   * @var string
   */
  public $name = '';

  /**
   * The drupal registered email address
   * @var string
   */
  public $mail = '';

  /**
   * Return value if the drupal user is active (TRUE) or blocked (FALSE)
   * @var bool
   */
  public $active = FALSE;

  /**
   * User roles obtained from the request
   * @var array
   */
  public $roles = array();

  /**
   * User (profile) fields
   * @var array
   */
  public $fields = array();

  /**
   * Constructor
   *
   * @return DrupalUserInfo
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Send the request to the server script and update the instance variables
   * of this object as return values.
   * @param string $name
   * @param string $mail = ''
   * @param bool $active = TRUE
   * @param mixed $fields = TRUE
   * @throws Exception
   */
  public function request($name, $mail = '', $active = TRUE, $fields = TRUE) {
    $this->name = $this->mail = '';
    $this->active = $this->valid = FALSE;
    if (empty($name) && empty($mail)) {
      throw new Exception('Cannot get user info: Neither user name nor email specified');
    }
    else {
      $data = $this->sendRequest('info', array(
        'name' => trim($name),
        'mail' => trim($mail),
        'active' => (int) $active,
        'fields' => $fields
      ));
      if (isset($data['name'])) $this->name = $data['name'];
      if (isset($data['mail'])) $this->mail = $data['mail'];
      if (isset($data['active'])) $this->active = $data['active'];
      if (isset($data['roles'])) $this->roles = $data['roles'];
      if (isset($data['fields'])) $this->fields = $data['fields'];
      $this->valid = !empty($this->name);
    }
  }
}
