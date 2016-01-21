<?php

/**
 * Creates views in code.
 *
 * @code
 * $my_block = CINC::init('View')->machine_name('a_view')->read();
 * @endcode
 */
class CINC_View extends CINC implements CINC_CRUD {

  public static $dependencies = array('views');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'view';

  function __construct() {

    $this->config = array(
      'db_table' => 'views_view',
      'table' => 'views_view',
      'type' => 'Normal',
      'export_type' => 1,
      'base_table' => 'node',
      'base_field' => 'nid',
      'name' => '',
      'vid' => 'new',
      'description' => '',
      'tag' => 'default',
      'human_name' => '',
      'core' => 7,
      'display' => array(),
    );

    $this->add_default_display();

  }

  /**
   * Saves view to the database.
   */
  public function create() {

    $view = $this->get_view_object();
    $view->save();

    return $this;
  }

  /**
   * Tests if view exists.
   */
  public function exists() {

    $view = views_get_view($this->get('name'));
    if ($view) {
      return TRUE;
    }
    return FALSE;

  }

  /**
   * Loads view from the database.
   */
  public function read() {
    $view = views_get_view($this->get('name'));
    $this->set_from_view_object($view);
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
   * Saves view to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes view from the database.
   */
  public function delete() {
    $view = $this->get_view_object();
    views_delete_view($view);
    $this->config = array();
    return $this;
  }

  /**
   * Sets row style.
   */
  public function set_row_style($type, $display_id = 'default') {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->set_row_style($type, $one_display_id);
      }
      return $this;
    }

    switch ($type) {

      case 'node':
        $this
          ->set('display.' . $display_id . '.display_options.row_plugin', 'node')
          ->set('display.' . $display_id . '.display_options.row_options.build_mode', 'full');
        break;

    }

    return $this;
  }

  /**
   * Sets view mode.
   */
  public function set_view_mode($view_mode, $display_id = 'default') {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->set_view_mode($view_mode, $one_display_id);
      }
      return $this;
    }

    $this->set('display.' . $display_id . '.display_options.row_options.build_mode', $view_mode);
    return $this;
  }

  /**
   * Adds default display.
   */
  public function add_default_display() {
    if (class_exists('views_display')) {
      $display = new views_display;
      $display->options('default', 'default', 'Master');
      $this->config['display']['default'] = (array) $display;
    }
    return $this;
  }

  /**
   * Adds a block display.
   */
  public function add_block_display($id = 'block', $title = 'Block') {
    if (class_exists('views_display')) {
      $display = new views_display;
      $display->options('block', $id, $title);
      $this->config['display'][$id] = (array) $display;
    }
    return $this;
  }

  /**
   * Adds a page display.
   */
  public function add_page_display($id = 'page', $title = 'Page') {
    if (class_exists('views_display')) {
      $display = new views_display;
      $display->options('page', $id, $title);
      $this->config['display'][$id] = (array) $display;
    }
    return $this;
  }

  /**
   * Adds a footer.
   */
  public function add_footer($type = 'area', $display_id = 'default', $details = array()) {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->add_footer($type, $one_display_id, $details);
      }
      return $this;
    }

    $display = cinc_object_from_array($this->config['display'][$display_id], 'views_display');
    if (!isset($display->display_options['footer'])) {
      $display->display_options['footer'] = array();
    }

    switch ($type) {

      case 'area':
        $display->display_options['footer']['area'] = $details + array(
          'id' => 'area',
          'table' => 'views',
          'field' => 'area',
          'label' => 'Footer',
          'content' => '',
          'format' => 'full_html',
        );
        break;

    }

    $this->config['display'][$display_id] = (array) $display;

    return $this;
  }

  /**
   * Adds a sort.
   */
  public function add_sort($type, $display_id = 'default', $details = array()) {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->add_sort($type, $one_display_id, $details);
      }
      return $this;
    }

    $display = cinc_object_from_array($this->config['display'][$display_id], 'views_display');
    if (!isset($display->display_options['sorts'])) {
      $display->display_options['sorts'] = array();
    }

    switch ($type) {

      case 'random':
        $display->display_options['sorts']['random'] = $details + array(
          'id' => 'random',
          'table' => 'views',
          'field' => 'random',
          'relationship' => 'none',
          'group_type' => 'group',
          'ui_name' => '',
          'order' => 'ASC',
          'exposed' => FALSE,
        );
        break;

      case 'created':
        $display->display_options['sorts']['created'] = $details + array(
          'id' => 'created',
          'table' => 'node',
          'field' => 'created',
          'order' => 'DESC',
        );
        break;

    }

    if ($display_id !== 'default') {
      if (!isset($display->display_options['defaults'])) {
        $display->display_options['defaults'] = array();
      }
      $display->display_options['defaults']['sorts'] = FALSE;
    }

    $this->config['display'][$display_id] = (array) $display;

    return $this;
  }

  /**
   * Adds a filter.
   */
  public function add_filter($type, $display_id = 'default', $details = array()) {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->add_filter($type, $one_display_id, $details);
      }
      return $this;
    }

    $display = cinc_object_from_array($this->config['display'][$display_id], 'views_display');
    if (!isset($display->display_options['filters'])) {
      $display->display_options['filters'] = array();
    }

    switch ($type) {

      case 'published':
        $display->display_options['filters']['status'] = $details + array(
          'value' => 1,
          'table' => 'node',
          'field' => 'status',
          'id' => 'status',
          'group' => 1,
        );
        break;
      case 'node_type':
        $display->display_options['filters']['type'] = $details + array(
          'id' => 'type',
          'table' => 'node',
          'field' => 'type',
        );
        break;
      case 'taxonomy_term':
        $display->display_options['filters']['tid'] = $details + array(
          'id' => 'tid',
          'table' => 'taxonomy_index',
          'field' => 'tid',
        );
        break;

    }

    if ($display_id !== 'default') {
      if (!isset($display->display_options['defaults'])) {
        $display->display_options['defaults'] = array();
      }
      $display->display_options['defaults']['filters'] = FALSE;
      $display->display_options['defaults']['filter_groups'] = FALSE;
    }

    $this->config['display'][$display_id] = (array) $display;

    return $this;
  }

  /**
   * Adds a node type filter.
   */
  public function add_node_type_filter($types, $display_id = 'default') {

    if (!is_array($types)) {
      $types = array($types);
    }

    $value = array();
    foreach ($types as $type) {
      $value[$type] = $type;
    }

    return $this->add_filter('node_type', $display_id, array(
      'operator' => 'in',
      'value' => $value,
    ));

  }

  /**
   * Sets access restriction.
   */
  public function set_access($type, $display_id = 'default', $details = array()) {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->set_access($type, $one_display_id, $details);
      }
      return $this;
    }

    $display = cinc_object_from_array($this->config['display'][$display_id], 'views_display');
    if (!isset($display->display_options['access'])) {
      $display->display_options['access'] = array();
    }

    switch ($type) {

      case 'perm':
        $display->display_options['access'] = $details + array(
          'type' => 'perm',
          'perm' => 'access content',
        );
        break;

    }

    $this->config['display'][$display_id] = (array) $display;

    return $this;
  }

  /**
   * Sets cache.
   */
  public function set_cache($type = 'time', $display_id = 'default', $details = array()) {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->set_cache($type, $one_display_id, $details);
      }
      return $this;
    }

    $display = cinc_object_from_array($this->config['display'][$display_id], 'views_display');
    if (!isset($display->display_options['cache'])) {
      $display->display_options['cache'] = array();
    }

    switch ($type) {

      case 'time':
        $display->display_options['cache'] = $details + array(
          'type' => 'time',
          'results_lifespan' => '300',
          'results_lifespan_custom' => '0',
          'output_lifespan' => '300',
          'output_lifespan_custom' => '0',
        );
        break;

    }

    $this->config['display'][$display_id] = (array) $display;

    return $this;
  }

  /**
   * Sets a hard limit (no pager) on returned items.
   */
  public function limit_items($count, $display_id = 'default') {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->limit_items($count, $one_display_id);
      }
      return $this;
    }

    return $this->set('display.' . $display_id . '.display_options.pager', array(
      'type' => 'some',
      'options' => array(
        'items_per_page' => $count,
        'offset' => 0,
      ),
    ));

  }

  /**
   * Sets a paged limit (full pager) on returned items.
   */
  public function page_items($count, $display_id = 'default') {

    if (is_array($display_id)) {
      foreach ($display_id as $one_display_id) {
        $this->page_items($count, $one_display_id);
      }
      return $this;
    }

    return $this->set('display.' . $display_id . '.display_options.pager', array(
      'type' => 'full',
      'options' => array(
        'items_per_page' => $count,
      ),
    ));

  }

  /**
   * Gets view object based on config.
   */
  public function get_view_object() {
    $view = cinc_object_from_array($this->config, 'view');

    foreach ($view->display as $key => $display_array) {
      $view->display[$key] = cinc_object_from_array($display_array, 'views_display');
    }

    return $view;
  }

  /**
   * Sets config based on view object.
   */
  public function set_from_view_object($view) {
    $this->config = (array) $view;
    foreach ($this->config['display'] as $key => $display) {
      $this->config['display'][$key] = (array) $display;
    }
  }

}
