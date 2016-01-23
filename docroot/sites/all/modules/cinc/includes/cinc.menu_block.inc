<?php

/**
 * Creates menu blocks in code.
 *
 * @code
 * $my_block = CINC::init('MenuBlock')->machine_name('1')->read();
 * @endcode
 */
class CINC_MenuBlock extends CINC_Block implements CINC_CRUD {

  public static $dependencies = array('menu_block');
  public $cinc_yaml_prefix = 'menu_block';
  public $config_machine_name = 'menu_block.admin_title';

  function __construct() {

    parent::__construct();

    $this->config_filters['menu_block.admin_title'] = 'set_admin_title';

    $this->set('module', 'menu_block');
    $this->set('menu_block', array(
      'title_link' => 0,
      'admin_title' => '',
      'parent' => '',
      'level' => 1,
      'follow' => 0,
      'depth' => 1,
      'expanded' => 0,
      'sort' => 0,
    ));

  }

  /**
   * Loads the delta when admin title is set.
   */
  function set_admin_title(&$admin_title) {

    $block_ids = variable_get('menu_block_ids', array());

    foreach ($block_ids as $block_id) {
      $title = variable_get('menu_block_' . $block_id . '_admin_title', '');
      if ($title == $admin_title) {
        $this->set('delta', $block_id);
      }
    }

    return TRUE;
  }

  /**
   * Saves menu block to the database.
   */
  public function create() {

    $block_ids = variable_get('menu_block_ids', array());
    $delta = empty($block_ids) ? 1 : max($block_ids) + 1;
    $block_ids[] = $delta;
    variable_set('menu_block_ids', $block_ids);

    $this->set('delta', $delta);
    $this->save_menu_block_variables();

    return parent::create();
  }

  /**
   * Reads menu block.
   */
  public function read() {

    parent::read();

    $delta = $this->get('delta');

    $this->set('menu_block.title_link', variable_get("menu_block_{$delta}_title_link", ''));
    $this->set('menu_block.admin_title', variable_get("menu_block_{$delta}_admin_title", ''));
    $this->set('menu_block.parent', variable_get("menu_block_{$delta}_parent", ''));
    $this->set('menu_block.level', variable_get("menu_block_{$delta}_level", ''));
    $this->set('menu_block.follow', variable_get("menu_block_{$delta}_follow", ''));
    $this->set('menu_block.depth', variable_get("menu_block_{$delta}_depth", ''));
    $this->set('menu_block.expanded', variable_get("menu_block_{$delta}_expanded", ''));
    $this->set('menu_block.sort', variable_get("menu_block_{$delta}_sort", ''));

    return $this;

  }

  /**
   * Updates menu block.
   */
  public function update() {
    $this->save_menu_block_variables();
    return parent::update();
  }

  /**
   * Saves menu block variables.
   */
  public function save_menu_block_variables() {

    $delta = $this->get('delta');

    variable_set("menu_block_{$delta}_title_link", $this->get('menu_block.title_link'));
    variable_set("menu_block_{$delta}_admin_title", $this->get('menu_block.admin_title'));
    variable_set("menu_block_{$delta}_parent", $this->get('menu_block.parent'));
    variable_set("menu_block_{$delta}_level", $this->get('menu_block.level'));
    variable_set("menu_block_{$delta}_follow", $this->get('menu_block.follow'));
    variable_set("menu_block_{$delta}_depth", $this->get('menu_block.depth'));
    variable_set("menu_block_{$delta}_expanded", $this->get('menu_block.expanded'));
    variable_set("menu_block_{$delta}_sort", $this->get('menu_block.sort'));
  }

}
