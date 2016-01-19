<?php

/**
 * Creates filter formats in code.
 *
 * @code
 * $my_format = CINC::init('FilterFormat')->machine_name('full_html');
 * @endcode
 */
class CINC_FilterFormat extends CINC implements CINC_CRUD {


  public $config_machine_name = 'format';
  public $cinc_yaml_prefix = 'filter.format';

  function __construct() {

    $this->config_filters['format'] = 'set_format';

    $this->config = array(
      'format' => '',
      'name' => '',
      'cache' => 1,
      'status' => 1,
      'weight' => 0,
      'filters' => array(),
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
   * Saves filter format to the database.
   */
  public function create() {
    $format = cinc_object_from_array($this->config, 'StdClass');
    filter_format_save($format);
    return $this;
  }

  /**
   * Tests if filter format exists.
   */
  public function exists() {
    return filter_format_exists($this->get('format'));
  }

  /**
   * Loads filter format from the database.
   */
  public function read() {
    $this->config = (array) filter_format_load($this->get('format'));
    $filters = filter_list_format($this->get('format'));
    $this->config['filters'] = array();
    foreach ($filters as $filter) {
      $this->config['filters'][$filter->name] = (array) $filter;
    }
    return $this;
  }

  /**
   * Adds a filter.
   */
  function add_filter($filter_name) {

    $filter_info = filter_get_filters();

    if (isset($filter_info[$filter_name])) {

      if (!isset($this->config['filters'][$filter_name])) {
        $this->config['filters'][$filter_name] = array();
      }

      $this->config['filters'][$filter_name]['format'] = $this->get('format');
      $this->config['filters'][$filter_name]['module'] = $filter_info[$filter_name]['module'];
      $this->config['filters'][$filter_name]['name'] = $filter_name;
      $this->config['filters'][$filter_name]['status'] = 1;
      $this->config['filters'][$filter_name]['weight'] = $filter_info[$filter_name]['weight'];
      if (!isset($this->config['filters'][$filter_name]['settings'])) {
        $this->config['filters'][$filter_name]['settings'] = $filter_info[$filter_name]['default settings'];
      }

    }

    return $this;

  }

  /**
   * Removes a filter.
   */
  function remove_filter($filter_name) {
    unset($this->config['filters'][$filter_name]);
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();
    $formats = filter_formats();

    foreach ($formats as $machine_name => $format) {
      $list[$machine_name] = array(
        'display' => $format->name,
      );
    }

    return $list;

  }

  /**
   * Saves filter format to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes filter format from the database.
   */
  public function delete() {
    db_delete('filter_format')
      ->condition('format', $this->get('format'))
      ->execute();
    filter_formats_reset();
    return $this;
  }


}