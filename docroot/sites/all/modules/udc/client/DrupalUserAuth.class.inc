<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 User Data Connector module - External script AUTH client class
 * -----------------------------------------------------------------------------
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.module.udc
 * @license GPL2
 */

require_once dirname(__FILE__) . '/DrupalUserBase.class.inc';

/**
 * Client class for user authentifications. Invoke the ::request() method to
 * send a request to the Drupal database. Note that user authentification using
 * SHA256 hashes is time consuming. Hence, the object should bs saved in the
 * session variables if multiple requests are needed.
 */
class DrupalUserAuth extends DrupalUserBase {

  /**
   * Stores if the request yielded that the user login information matched
   * (user/password correct).
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
   * @return DrupalUserAuth
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Send the request to the server script and update the instance variables
   * of this object as return values. Fields can be FALSE for 'none', TRUE
   * for 'all' or an array for selecting special fields, e.g.
   * array('profile_address', 'field_user_anything'). Note if you use profile
   * fields only, the script will be faster then if using common fields, because
   * no full bootsteap is required.
   *
   * @param string $name
   * @param string $pass
   * @param string $mail = ''
   * @param bool $active = TRUE
   * @param mixed $fields = FALSE
   * @throws Exception
   */
  public function request($name, $pass, $mail = '', $active = TRUE, $fields = FALSE) {
    $this->name = $this->mail = '';
    $this->active = $this->valid = FALSE;
    if (empty($name) && empty($mail)) {
      throw new Exception('Cannot check user: Neither user name nor email specified');
    }
    elseif (empty($pass)) {
      throw new Exception('Cannot check user: Password not specified');
    }
    else {
      $data = $this->sendRequest('auth', array(
        'name' => trim($name),
        'pass' => strval($pass),
        'mail' => trim($mail),
        'active' => (bool) $active,
        'fields' => $fields
      ));
      if (isset($data['name'])) $this->name = $data['name'];
      if (isset($data['mail'])) $this->mail = $data['mail'];
      if (isset($data['active'])) $this->active = $data['active'];
      if (isset($data['roles'])) $this->roles = $data['roles'];
      if (isset($data['fields'])) $this->fields = $data['fields'];
      $this->valid = !empty($this->name) && !empty($this->mail);
    }
  }
}
