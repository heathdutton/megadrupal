<?php
/**
 * Creates Menu Position rules in code
 */
class CINC_MenuPosition extends CINC implements CINC_CRUD {
  public static $dependencies = array('menu_position');
  public $config_machine_name = 'menu_position';
  public $cinc_yaml_prefix = 'system.menu_position';

  public $parent_found = FALSE;

  protected $menuPositionLoaded = FALSE;

  /**
   * Constructor
   */
  public function __construct() {
    $this->config = array(
      'menu_name' => '',
      'admin_title' => '',
      'plid' => '',
      'conditions' => array(),
      'enabled' => 1,
    );
  }

  /**
   * Let people call config settings as methods.
   */
  public function __call($name, $arguments) {
    if (isset($this->config[$name])) {
      array_unshift($arguments, $name);
      call_user_func_array(array($this, 'set'), $arguments);
    }
    return $this;
  }

  /**
   * Create a rule.
   */
  public function create() {
    $this->loadMenuPosition();
    if (isset($this->config['plid']) && $this->config['plid']) {
      menu_position_add_rule($this->config);
    }
    else {
      watchdog('cinc', 'MenuPosition not created: no plid specified for :title', array(':title' => $this->config['title']), WATCHDOG_NOTICE);
    }
    return $this;
  }

  /**
   * Check if the menu position rule already exists.
   */
  public function exists() {
    if ($this->config['rid']) {
      // check if the rule exists by rid
      $exists = intval(db_query('SELECT COUNT(rid) AS c FROM menu_position_rules WHERE rid=:rid', array(':rid' => $this->config['rid']))->fetchColumn());
      if ($exists) {
        return TRUE;
      }
      return FALSE;
    }
  }

  /**
   * Read the rule from the database.
   */
  public function read() {
    $this->loadMenuPosition();
    if ($this->config['rid']) {
      $this->config = menu_position_read_rule($this->config['rid']);
    }
    else {
      $query = db_select('menu_position_rules', 'r');
      if ($this->config['admin_title']) {
        $query->condition('admin_title', $this->config['admin_title'], '=');
      }

      if ($this->config['plid']) {
        $query->condition('plid', $this->config['plid'], '=');
      }

      if ($this->config['conditions']) {
        $query->condition('conditions', serialize($this->config['conditions']), '=');
      }
      $query->fields('r');

      $results = $query->execute();
      if ($results->rowCount() > 0) {
        $this->config = $results->fetchAssoc();
        $this->config['conditions'] = unserialize($this->config['conditions']);
      }
    }
    return $this;
  }

  /**
   * Update the menu position rule.
   */
  public function update() {
    $this->loadMenuPosition();
    menu_position_edit_rule($this->config);
    return $this;
  }

  /**
   * Delete the menu position rule.
   */
  public function delete() {
    $this->loadMenuPosition();
    if ($this->exists()) {
      menu_position_delete_rule($this->config['rid']);
    }
    return $this;
  }

  /**
   * Find a menu item with the path specified to use as the parent.
   *
   * @param string $path
   *   The parent path.
   */
  public function parent_path($path) {
    $menu_name = $this->config['menu_name'];
    $menu = db_query('SELECT * FROM menu_links WHERE menu_name=:menu_name AND link_path=:path', array(':menu_name' => $menu_name, ':path' => $path))->fetchAssoc();
    if ($menu) {
      $this->config['plid'] = $menu['mlid'];
      $this->parent_found = TRUE;
    }
    return $this;
  }

  /**
   * Load the menu position files we need to get to the API functions.
   */
  protected function loadMenuPosition() {
    if (!$this->menuPositionLoaded) {
      module_load_include('inc', 'menu_position', 'menu_position.admin');
      $this->menuPositionLoaded = TRUE;
    }
  }
}
