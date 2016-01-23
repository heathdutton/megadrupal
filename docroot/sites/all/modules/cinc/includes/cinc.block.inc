<?php

/**
 * Creates blocks in code.
 *
 * @code
 * $my_block = CINC::init('Block')->machine_name('system:main-menu')->read();
 * @endcode
 */
class CINC_Block extends CINC implements CINC_CRUD {

  public static $dependencies = array('block');
  public $config_machine_name = 'module_delta';
  public $cinc_yaml_prefix = 'block';

  function __construct() {

    $this->config_filters['module_delta'] = 'set_module_delta';

    $this->config = array(
      'module' => 'system',
      'delta' => '',
      'title' => '',
      'visibility' => '',
      'pages' => '',
      'custom' => '',
      'status' => 0,
      'weight' => 0,
      'cache' => DRUPAL_NO_CACHE,
      'region' => array(),
    );

  }

  /**
   * Sets module and delta on module_delta.
   */
  public function set_module_delta(&$module_delta) {

    list($module, $delta) = explode(':', $module_delta);

    $this->set('module', $module);
    $this->set('delta', $delta);

    return FALSE;

  }

  /**
   * Saves block to the database.
   */
  public function create() {

    $query = db_insert('block')->fields(array(
      'visibility',
      'pages',
      'custom',
      'title',
      'module',
      'theme',
      'region',
      'status',
      'weight',
      'delta',
      'cache',
    ));

    $regions = $this->get('region');

    foreach ($regions as $theme => $region) {

      $query->values(array(
        'visibility' => $this->get('visibility'),
        'pages' => $this->get('pages'),
        'custom' => $this->get('custom'),
        'title' => $this->get('title'),
        'module' => $this->get('module'),
        'theme' => $theme,
        'region' => $region,
        'status' => $this->get('status'),
        'weight' => $this->get('weight'),
        'delta' => $this->get('delta'),
        'cache' => $this->get('cache'),
      ));

    }
    $query->execute();

    return $this;
  }

  /**
   * Tests if block exists.
   */
  public function exists() {
    $block = block_load($this->get('module'), $this->get('delta'));
    if (isset($block->bid)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads block from the database.
   */
  public function read() {
    $block = (array) block_load($this->get('module'), $this->get('delta'));
    $this->set('title', $block['title']);
    $this->set('pages', $block['pages']);
    $this->set('visibility', $block['visibility']);
    $this->set('custom', $block['custom']);

    foreach (list_themes() as $key => $theme) {
      $region = db_query("SELECT region FROM {block} WHERE module = :module AND delta = :delta AND theme = :theme", array(
        ':module' => $this->get('module'),
        ':delta' => $this->get('delta'),
        ':theme' => $key,
      ))->fetchField();
      $this->set('region.' . $key, $region);
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
   * Saves block to the database.
   */
  public function update() {
    db_update('block')
      ->fields(array(
        'visibility' => (int) $this->get('visibility'),
        'pages' => trim($this->get('pages')),
        'custom' => (int) $this->get('custom'),
        'title' => $this->get('title'),
      ))
      ->condition('module', $this->get('module'))
      ->condition('delta', $this->get('delta'))
      ->execute();

    $regions = $this->get('region');

    foreach ($regions as $theme => $region) {

      db_update('block')
        ->fields(array(
          'region' => $region,
        ))
        ->condition('module', $this->get('module'))
        ->condition('delta', $this->get('delta'))
        ->condition('theme', $theme)
        ->execute();

    }

    return $this;
  }

  /**
   * Removes block from the database.
   */
  public function delete() {
    db_delete('block')
      ->condition('module', $this->get('module'))
      ->condition('delta', $this->get('delta'))
      ->execute();

    return $this;
  }

}
