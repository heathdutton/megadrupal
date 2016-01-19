<?php

/**
 * Creates menus in code.
 *
 * @code
 * $menu = CINC::init('Menu')->machine_name('main-menu');
 * @endcode
 */
class CINC_Menu extends CINC implements CINC_CRUD {

  public static $dependencies = array('menu');
  public $config_machine_name = 'menu_name';
  public $cinc_yaml_prefix = 'system.menu';
  public $links = array();

  function __construct() {

    $this->config_filters['menu_name'] = 'set_menu_name';

  	$this->config = array(
      'menu_name' => '',
      'title' => '',
      'description' => '',
		);

  }

  /**
   * Auto-adds title when menu_name is set, and cleans menu_name.
   */
  public function set_menu_name(&$menu_name) {
    $menu_name = CINC::clean_machine_name($menu_name);
    if (empty($this->config['title'])) {
      $this->set('title', CINC::display_name_from_machine_name($menu_name));
    }
    return TRUE;
  }

  /**
   * Saves menu to the database.
   */
  public function create() {
    menu_save($this->config);
    return $this;
  }

  /**
   * Tests if menu exists.
   */
  public function exists() {
    $menu = menu_load($this->config['menu_name']);
    if ($menu) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads menu from the database.
   */
  public function read() {
    $menu = menu_load($this->config['menu_name']);
    if ($menu) {
      $this->config = $menu;
      $this->links = menu_load_links($this->config['menu_name']);
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $menus = menu_load_all();

    foreach ($menus as $machine_name => $menu) {
      $list[$machine_name] = array(
        'display' => $menu['title'],
      );
    }

    return $list;

  }

  public function get_links_as_tree() {
    return menu_tree_data($this->links);
  }

  public function get_link_by_property($property, $value) {

    foreach ($this->links as $link) {
      if ($link[$property] == $value) {
        return $link;
      }
    }

  }

  /**
   * Saves menu to the database and migrates previous data if machine name changed.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes menu from the database.
   */
  public function delete() {
    menu_delete($this->config);
    return $this;
  }

}

/**
 * Creates menu links in code.
 *
 * @code
 * $menu_link = CINC::init('MenuLink')->machine_name('path');
 * @endcode
 */
class CINC_MenuLink extends CINC implements CINC_CRUD {

  public static $dependencies = array('menu');
  public $config_machine_name = 'full_link_path';
  public $cinc_yaml_prefix = 'menu_link';
  public $links = array();

  function __construct() {

    $this->config_filters['link_path'] = 'set_link_path';
    $this->config_filters['full_link_path'] = 'set_full_link_path';

    $this->config = array(
      'mlid' => 0,
      'plid' => 0,
      'options' => array(),
      'module' => 'menu',
      'has_children' => 0,
      'expanded' => 1,
      'hidden' => 0,
      'link_path' => '',
      'link_title' => '',
      'menu_name' => 'main-menu',
      'weight' => 1,
    );

  }

  /**
   * Auto-adds title when menu_name is set, and cleans menu_name.
   */
  public function set_link_path(&$link_path) {
    $link_title = $this->get('link_title');
    if (empty($link_title)) {
      $this->set('link_title', CINC::display_name_from_machine_name($link_path));
    }
    $link_path = drupal_get_normal_path($link_path);
    return TRUE;
  }

  /**
   * Auto-adds title when menu_name is set, and cleans menu_name.
   */
  public function set_full_link_path(&$full_link_path) {
    list($menu_name, $full_path) = explode('|', $full_link_path);
    list($link_path, $query) = explode('?', $full_path);
    $this->set('link_path', $link_path);
    $this->set('menu_name', $menu_name);
    if (!empty($query)) {
      $this->set('options.query', parse_str($query));
    }
    return FALSE;
  }

  /**
   * Saves menu link to the database.
   */
  public function create() {
    menu_link_save($this->config);
    return $this;
  }

  /**
   * Tests if menu link exists.
   */
  public function exists() {
    $results = $this->read_query();
    while ($menu_item = $results->fetchAssoc()) {
      $menu_item['options'] = unserialize($menu_item['options']);
      if ($menu_item['options']['query'] == $this->get('options.query')) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Loads menu link from the database.
   */
  public function read() {
    $results = $this->read_query();
    while ($menu_item = $results->fetchAssoc()) {
      $menu_item['options'] = unserialize($menu_item['options']);
      if ($menu_item['options']['query'] == $this->get('options.query')) {
        $this->config = $menu_item;
        break;
      }
    }
    return $this;
  }

  /**
   * Creates database query.
   */
  public function read_query() {
    $path = drupal_get_normal_path($this->get('link_path'));

    $query = db_select('menu_links', 'ml');
    $query->fields('ml');
    $query->condition('ml.menu_name', $this->get('menu_name'));
    $query->condition('ml.link_path', $path);

    return $query->execute();
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves menu link to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes menu link from the database.
   */
  public function delete() {
    menu_link_delete($this->get('mlid'));
    return $this;
  }

}

