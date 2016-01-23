<?php

/**
 * Creates contexts in code.
 *
 * @code
 * $context = CINC::init('Context')->set('name', 'my_context');
 * @endcode
 */
class CINC_Context extends CINC implements CINC_CRUD {

  public static $dependencies = array('context');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'context';

  function __construct() {

    $this->config_filters['name'] = 'set_name';

    $this->config = array(
      'name' => '',
      'tag' => '',
      'description' => '',
      'conditions' => array(),
      'reactions' => array(),
      'condition_mode' => 1,
      'table' => 'context',
      'type' => 'Normal',
      'export_type' => 1,
    );

  }

  /**
   * Cleans name.
   */
  public function set_name(&$name) {
    $name = CINC::clean_machine_name($name);
    return TRUE;
  }

  /**
   * Saves context to the database.
   */
  public function create() {
    $context = (object) $this->config;
    context_save($context);
    $this->read();
    return $this;
  }

  /**
   * Tests if content type exists.
   */
  public function exists() {
    $context = context_load($this->config['name']);
    if ($context) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Loads context from the database.
   */
  public function read() {
    $context = context_load($this->config['name']);
    if ($context !== FALSE) {
      $this->config = (array) context_load($this->config['name']);
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $contexts = context_load(NULL, TRUE);

    foreach ($contexts as $machine_name => $context) {
      $list[$machine_name] = array(
        'display' => $machine_name,
      );
    }

    return $list;

  }

  /**
   * Saves context to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes context from the database.
   */
  public function delete() {
    $context = (object) $this->config;
    context_delete($context);
    return $this;
  }

  public function add_node_condition($types, $node_form = FALSE) {
    $values = array();

    foreach ($types as $type) {
      $values[$type] = $type;
    }

    $this->set('conditions.node', array(
      'values' => $values,
      'options' => array(
        'node_form' => (int) $node_form,
      ),
    ));

    return $this;
  }

  public function add_global_condition() {
    $this->set('conditions.sitewide.values.1', 1);

    return $this;
  }

  public function add_path_condition($paths) {
    $values = array();

    foreach ($paths as $path) {
      $values[$path] = $path;
    }

    $this->set('conditions.path.values', $values);

    return $this;
  }


  public function add_block_reaction($module, $delta, $region, $weight = 0) {

    $block_id = $module . '-' . $delta;

    $this->set('reactions.block.blocks.' . $block_id, array(
      'module' => $module,
      'delta' => $delta,
      'region' => $region,
      'weight' => $weight,
    ));

    return $this;

  }

  public function disable() {

    $status = variable_get('context_status', array());
    $status[$this->config['name']] = TRUE;
    variable_set('context_status', $status);

    $this->read();
    return $this;

  }

}
