<?php

/**
 * @file
 * Plugin for Adminer. Provides Adminer usage without login.
 */

class AdminerDrupal {
  protected $db;
  protected $access;

  /**
   * Plugin constructor.
   */
  public function __construct() {
    if ($this->access = user_access('use adminer without login')) {
      $this->db = Database::getConnectionInfo();
    }
  }

  /**
   * Returns site name.
   */
  public function name() {
    return variable_get('site_name', 'Drupal');
  }

  /**
   * Returns DB credentials.
   */
  public function credentials() {
    if ($this->access) {
      return array(
        $this->db['default']['host'],
        $this->db['default']['username'],
        $this->db['default']['password'],
      );
    }
    // SERVER - constant defined in Adminer.
    return array(SERVER, $_GET['username'], get_session('pwds'));
  }

  /**
   * Returns DB link.
   */
  public function database() {
    if ($this->access) {
      return $this->db['database'];
    }
    // DB - constant defined in Adminer.
    return DB;
  }

  /**
   * Include CSS.
   */
  public function head() {
    print '<link rel="stylesheet" type="text/css" href="' . file_create_url(adminer_get_css()) . '"/>' . "\n";
    return FALSE;
  }
}
