<?php

/**
 * Creates WYSIWYG profiles in code.
 *
 * @code
 * $my_profile = CINC::init('WysiwygProfile')->machine_name('full_html');
 * @endcode
 */
class CINC_WysiwygProfile extends CINC implements CINC_CRUD {


  public $config_machine_name = 'format';
  public $cinc_yaml_prefix = 'wysiwyg.profile';

  function __construct() {

    $this->config_filters['format'] = 'set_format';

    $this->config = array(
      'format' => '',
      'editor' => '',
      'settings' => array(),
    );

  }

  /**
   * Cleans format name.
   */
  public function set_format(&$format) {
    $format = CINC::clean_machine_name($format);
    return TRUE;
  }

  /**
   * Saves WYSIWYG profile to the database.
   */
  public function create() {

    db_merge('wysiwyg')
    ->key(array('format' => $this->get('format')))
    ->fields(array(
      'editor' => $this->get('editor'),
      'settings' => serialize($this->get('settings')),
    ))
    ->execute();

    return $this;
  }

  /**
   * Tests if WYSIWYG profile exists.
   */
  public function exists() {
    $profile = wysiwyg_profile_load($this->get('format'));
    if ($profile) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads WYSIWYG profile from the database.
   */
  public function read() {
    $this->config = (array) wysiwyg_profile_load($this->get('format'));
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();
    $profiles = wysiwyg_profile_load_all();

    foreach ($profiles as $machine_name => $profile) {
      $list[$machine_name] = array(
        'display' => $machine_name,
      );
    }

    return $list;

  }

  /**
   * Saves WYSIWYG profile to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes WYSIWYG profile from the database.
   */
  public function delete() {
    wysiwyg_profile_delete($this->get('format'));
    return $this;
  }


}