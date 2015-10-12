<?php

/**
 * Creates IMCE profiles in code.
 *
 * @code
 * $my_profile = CINC::init('IMCEProfile')->machine_name('profile name')->read();
 * @endcode
 */
class CINC_IMCEProfile extends CINC implements CINC_CRUD {

  public static $dependencies = array('imce');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'imce.profile';

  function __construct() {

    $this->config = array(
      'name' => '',
      'usertab' => 1,
      'filesize' => 10,
      'quota' => 0,
      'tuquota' => 0,
      'extensions' => 'gif png jpg jpeg',
      'dimensions' => 0,
      'filenum' => 1,
      'directories' => array(
        array(
          'name' => 'imce',
          'subnav' => 1,
          'browse' => 1,
          'upload' => 1,
          'thumb' => 1,
          'delete' => 0,
          'resize' => 0,
        ),
      ),
      'thumbnails' => array(
        array(
          'name' => 'Thumb',
          'dimensions' => '90x90',
          'prefix' => 'thumb_',
          'suffix' => '',
        ),
      ),
    );

  }

  /**
   * Saves profile to the database.
   */
  public function create() {
    module_load_include('inc', 'imce', 'inc/imce.admin');
    imce_update_profiles('', $this->config);
  }

  /**
   * Tests if profile exists.
   */
  public function exists() {
    if ($this->get_pid()) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads profile from the database.
   */
  public function read() {
    $profiles = variable_get('imce_profiles', array());
    $pid = $this->get_pid();
    if ($pid) {
      $this->config = $profiles[$pid];
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves profile to the database.
   */
  public function update() {
    module_load_include('inc', 'imce', 'inc/imce.admin');
    imce_update_profiles($this->get_pid(), $this->config);
    return $this;
  }

  /**
   * Removes profile from the database.
   */
  public function delete() {
    module_load_include('inc', 'imce', 'inc/imce.admin');
    imce_update_profiles($this->get_pid(), NULL);
    return $this;
  }

  /**
   * Gets PID from name.
   */
  public function get_pid() {
    $profiles = variable_get('imce_profiles', array());
    $name = $this->get('name');
    foreach ($profiles as $pid => $profile) {
      if ($profile['name'] == $name) {
        return $pid;
      }
    }
    return FALSE;
  }

}
