<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 User Data Connector module - External script LIST client class
 * -----------------------------------------------------------------------------
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.drupaluser
 * @license GPL2
 */

require_once dirname(__FILE__) . '/DrupalUserBase.class.inc';

/**
 * Client class for retrieving a list of users, selectable if only active users
 * or all allowed users. Optionally, the user roles can be requested as well.
 */
class DrupalUserList extends DrupalUserBase {

  /**
   * Stores if the request yielded that the user login information matched
   * (user/password correct).
   * @var array
   */
  public $list = array();

  /**
   * Constructor
   *
   * @return DrupalUserList
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Send the request to the server script and update the instance variables
   * of this object as return values.
   * @param bool $activeUsersOnly = TRUE
   * @param bool $withRoles = FALSE
   * @throws Exception
   */
  public function request($activeUsersOnly = TRUE, $withRoles = FALSE) {
    $this->list = array();
    $this->list = $this->sendRequest('list', array(
      'active' => (bool) $activeUsersOnly,
      'withroles' => $withRoles
    ));
  }
}
