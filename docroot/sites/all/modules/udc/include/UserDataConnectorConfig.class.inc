<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 - User Data Connector module - config handling
 * -----------------------------------------------------------------------------
 *
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.udc
 * @license GPL2
 */


/**
 * Provider class for configuration in/out handling stored in the DP database.
 */
class UserDataConnectorConfig {

  /**
   * The main instance of this config class
   * @var UserDataConnectorConfig
   */
  private static $instance = NULL;

  /**
   * Returns main the instance of this config class
   *
   * @return UserDataConnectorConfig
   */
  public static function instance() {
    if (self::$instance === NULL) {
      self::$instance = new self();
    }
    return self::$instance;
  }

  /**
   * Retrieves a variable from the Drupal database
   *
   * @param string $name
   * @param mixed $default
   * @return mixed
   */
  private function getDbVariable($name, $default) {
    return variable_get('udc_' . $name, $default);
  }

  /**
   * Deletes the database variables for the uninstall process. You must set
   * the argument to 'yes', otherwise the method throws an exception.
   *
   * @throws Exception
   * @return void
   */
  public function deleteDbVariables() {
    if ($this->getModuleEnabled()) {
      throw new Exception('Cannot delete User Data Connector config variables: Module is still enabled');
    }
    else {
      db_query("DELETE FROM {variable} WHERE LOCATE('udc_', name) = 1");
    }
  }

  /**
   * Returns if the module is enabled
   *
   * @throws Exception
   * @return bool
   */
  public function getModuleEnabled() {
    static $enabled = NULL;
    if ($enabled === NULL) {
      $enabled = db_select('system', 's')->fields('s', array('status'))->condition('name', 'udc', '=')->execute()->fetchCol();
      $enabled = (bool) reset($enabled);
    }
    return $enabled;
  }

  /**
   * Returns the server/client identification token
   *
   * @return string
   */
  public function getToken() {
    return $this->getDbVariable('token', '');
  }

  /**
   * Returns if the server script can add debug information to the response
   *
   * @return bool
   */
  public function getDebugEnabled() {
    return $this->getDbVariable('enable_debug', FALSE);
  }

  /**
   * Returns if the server script only accepts HTTPS/SSL requests
   *
   * @return bool
   */
  public function getRequireHttps() {
    return $this->getDbVariable('require_https', FALSE);
  }

  /**
   * Returns the login names of unlisted users as array
   *
   * @return array
   */
  public function getUnlistedUsers() {
    return $this->getDbVariable('unlisted_users', array('admin'));
  }

  /**
   * Returns the profile fields that the server is allowed to return. If
   * all fields shall be be returned, the method returns TRUE.
   *
   * @return array
   */
  public function getListedProfileFields() {
    return $this->getDbVariable('listed_profile_fields', TRUE);
  }

  /**
   * Returns the roles which can be listed. If empty, all roles are accepted
   *
   * @return array
   */
  public function getListedRoles() {
    return $this->getDbVariable('listed_roles', array());
  }
}
