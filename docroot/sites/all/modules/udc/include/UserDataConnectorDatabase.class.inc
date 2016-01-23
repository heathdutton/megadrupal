<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 - User Data Connector module - server script database access
 * -----------------------------------------------------------------------------
 *
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.udc
 * @license GPL2
 */

require_once( dirname(__FILE__) . '/UserDataConnectorConfig.class.inc');

/**
 * Provider class for Drupal database lookups. Includes filtering and data
 * conditioning.
 */
class UserDataConnectorDatabase {

  /**
   * Stores all available roles as key/value pair (role id => role name).
   * @var array
   */
  public $availableRoles = array();

  /**
   * Stores the available user profile fields as key value pair: fid => name.
   * @var array
   */
  public $availableProfileFields = array();

  /**
   * Stores the available user fields as key value pair: id => field_name.
   * @var array
   */
  public $availableUserFields = array();

  /**
   * Contains the allowed RIDs for IN_SET queries
   * @var array
   */
  public $allowedRoleIds = array();


  /**
   * Contains the lowercase unlisted users
   * @var array
   */
  public $unlistedUsers = array();


  /**
   * Constructor
   *
   * @throws Exception
   */
  public function __construct() {
    // All roles

    $roles = db_select('role', 'r')
          ->fields('r', array('rid', 'name'))
          ->execute()
          ->fetchAll();

    foreach ($roles as $v) {
      $this->availableRoles[$v->rid] = $v->name;
    }

    // Listed role ids
    $listed_roles = UserDataConnectorConfig::instance()->getListedRoles();
    if (!empty($listed_roles) && $listed_roles !== TRUE) {
      foreach ($listed_roles as $k => $v) {
        $listed_roles[$k] = strtolower($v);
      }
      foreach ($this->availableRoles as $k => $v) {
        if (in_array(strtolower($v), $listed_roles)) {
          $this->allowedRoleIds[$k] = $k;
        }
      }
    }

    // Unlisted users
    $this->unlistedUsers = UserDataConnectorConfig::instance()->getUnlistedUsers();
    foreach ($this->unlistedUsers as $k => $v) {
      $this->unlistedUsers[$k] = trim(strtolower($v));
    }
  }

  /**
   * If filter is an array, the method removes all profile and user fields which
   * are not in the $filter list. If TRUE, no fields are removed. if FALSE, string
   * or object, all fields are removed.
   *
   * @param array $filter
   * @return void
   * @throws Exception
   */
  public function loadAvailableFields($filter) {
    if (is_array($filter) && empty($filter) || !is_array($filter) && !$filter) {
      $this->availableUserFields = array();
      $this->availableProfileFields = array();
      return;
    }
    else {
      $listed_pfields = UserDataConnectorConfig::instance()->getListedProfileFields();

      // Initialize user fields
      $this->availableUserFields = array();

      $query = db_select('field_config_instance', 'f')
              ->fields('f', array('id', 'field_name'))
              ->condition('entity_type', 'user', '=')
              ->execute()
              ->fetchAll();

      foreach ($query as $v) {
        if (($listed_pfields === TRUE || in_array($v->field_name, $listed_pfields))) {
          $this->availableUserFields[$v->id] = $v;
        }
      }

      // Initialize available profile fields for the case profile is used
      if (db_table_exists('profile_field') && db_table_exists('profile_value')) {

        $query = db_select('profile_field', 'p')
                ->fields('p', array('fid', 'name', 'type'))
                ->execute()
                ->fetchAll();

        foreach ($query as $v) {
          if (($listed_pfields === TRUE || in_array($v->name, $listed_pfields))) {
            $this->availableProfileFields[$v->fid] = $v;
          }
        }
      }

      // Filter
      if (is_array($filter)) {
        foreach ($filter as $k => $v) $filter[$k] = trim(strtolower($v));
        $newList = array();
        // User fields
        foreach ($this->availableUserFields as $k => $v) {
          if (in_array(strtolower($v->field_name), $filter)) {
            $newList[$k] = $v;
          }
        }
        $this->availableUserFields = $newList;

        // Profile fields
        $newList = array();
        foreach ($this->availableProfileFields as $k => $v) {
          if (in_array(strtolower($v->name), $filter)) {
            $newList[$k] = $v;
          }
        }
        $this->availableProfileFields = $newList;
      }
    }
  }

  /**
   * Returns the roles of the user
   *
   * @param int $uid
   * @return array
   * @throws Exception
   */
  public function getUserRoles($uid) {
    $roles = $this->availableRoles;
    $uid = intval($uid);
    if ($uid > 0) {

      $query = db_select('users_roles', 'u')
              ->fields('u', array('rid'))
              ->condition('uid', $uid, '=')
              ->execute()
              ->fetchCol();

      $return_roles = array();
      foreach ($query as $rid) {
        $return_roles[] = $roles[$rid];
      }
    }
    return $return_roles;
  }

  /**
   * Returns the user profile fields. If the $fields parameter is TRUE, all
   * allowed fields are listed, otherwise an array with the machine names
   * of the fields has to be specified. If $fields == FALSE, the method returns
   * an empty array. If a field cannot be fetched rapidly, a full drupal
   * bootstrap is performed (with corresponding performance losses).
   *
   * @param type $uid
   * @return array
   * @throws Exception
   */
  public function getUserFields($uid) {
    if (empty($this->availableUserFields)) {
      return $this->getUserProfileFields($uid);
    }
    else {
      return $this->getUserFieldsUserLoad($uid);
    }
  }

  /**
   * Returns the user profile fields. If the $fields parameter is TRUE, all
   * allowed fields are listed, otherwise an array with the machine names
   * of the fields has to be specified. If $fields == FALSE, the method returns
   * an empty array. Note that only fields can be exported here which provide
   * the table field "<field_name>_value".
   *
   * @param type $uid
   * @return array
   * @throws Exception
   */
  public function getUserProfileFields($uid) {
    $pfields = &$this->availableProfileFields;
    $return = array();
    if (!empty($pfields)) {

      $query = db_select('profile_value', 'p')
              ->fields('p', array('fid', 'value'))
              ->condition('uid', $uid, '=')
              ->condition('fid', array_keys($pfields), 'IN')
              ->execute()
              ->fetchAll();

      foreach ($query as $record) {
        // Known types, list to be extended ...
        switch ($pfields[$record->fid]->type) {
          case 'date':
            $record->value = unserialize($record->value);
            break;
        }
        $return[$pfields[$record->fid]->name] = $record->value;
      }
      return $return;
    }
  }

  /**
   * Returns user profile and common fields using Drupal user_load().
   *
   * @param type $uid
   * @return array
   */
  public function getUserFieldsUserLoad($uid) {
    if (!function_exists('user_load')) {
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      header('Content-Type: text/plain');
      set_error_handler('checkuser_error_handler', E_ALL | E_STRICT);
      set_exception_handler('checkuser_exception_handler');
    }

    $user = user_load($uid);
    $return = array();
    foreach ($this->availableProfileFields as $v) {
      $v = $v->name;
      if (isset($user->$v)) $return[$v] = $user->$v;
    }
    foreach ($this->availableUserFields as $v) {
      $v = $v->field_name;
      if (isset($user->$v)) $return[$v] = $user->$v;
    }
    return $return;
  }
}
