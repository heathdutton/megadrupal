<?php

/**
 * Creates bean types in code.
 *
 * @code
 * $my_bean_type = CINC::init('BeanType')->machine_name('my_bean_type')->read();
 * @endcode
 */
class CINC_BeanType extends CINC implements CINC_CRUD {

  public static $dependencies = array('bean_admin_ui');
  public $config_machine_name = 'name';
  public $cinc_yaml_prefix = 'bean_type';
  public $fields = array();
  public static $types = array();

  function __construct() {

    $this->config = array(
      'name' => '',
      'label' => '',
      'type' => 'Normal',
      'description' => '',
      'table' => 'bean_type',
    );

  }

  /**
   * Saves bean type to the database.
   */
  public function create() {
    $plugin_info = _bean_admin_default_plugin();
    $plugin_info['name'] = '';

    $bean_type = new BeanCustom($plugin_info);
    $bean_type->type = $this->get('name');
    $bean_type->setLabel($this->get('label'));
    $bean_type->setDescription($this->get('description'));
    $bean_type->save(TRUE);

    return $this;
  }

  /**
   * Tests if bean type exists.
   */
  public function exists() {
    $types = CINC_BeanType::get_types();
    if (isset($types[$this->get('name')])) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads bean type from the database.
   */
  public function read() {
    $types = CINC_BeanType::get_types();
    if (isset($types[$this->get('name')])) {
      $this->config = (array) $types[$this->get('name')];
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    $types = CINC_BeanType::get_types();
    foreach ($types as $machine_name => $type) {
      $list[$machine_name] = array(
        'display' => $type->label,
      );
    }
    return $list;
  }

  /**
   * Saves bean type to the database.
   */
  public function update() {
    $plugin_info = _bean_admin_default_plugin();
    $plugin_info['name'] = '';

    $bean_type = new BeanCustom($plugin_info);
    $bean_type->type = $this->get('name');
    $bean_type->setLabel($this->get('label'));
    $bean_type->setDescription($this->get('description'));
    $bean_type->save(FALSE);

    return $this;
  }

  /**
   * Removes bean type from the database.
   */
  public function delete() {
    $plugin_info = _bean_admin_default_plugin();
    $plugin_info['name'] = '';

    $bean_type = new BeanCustom($plugin_info);
    $bean_type->type = $this->get('name');
    $bean_type->delete();

    return $this;
  }

  /**
   * Loads types.
   */
  static public function get_types() {
    if (empty(CINC_BeanType::$types)) {
      CINC_BeanType::$types = bean_admin_ui_get_types();
    }
    return CINC_BeanType::$types;
  }

  /**
   * Adds field (instance) to the bean type.
   *
   * @param CINC_Field $field_instance
   *
   * @return $this
   */
  public function add_field($field, $weight = NULL) {
    if (!($field instanceof CINC_Field)) {
      if ($field instanceof CINC_MissingDependencies) {
        $dependencies = implode(', ', $field->missing_dependencies);
        drupal_set_message(t('CINC_ContentType::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), 'warning');
        watchdog('CINC', format_string('CINC_ContentType::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), array(), WATCHDOG_WARNING);
      }
      else {
        throw new Exception('CINC_ContentType::add_field requires a CINC_Field instance as the first parameter');
      }
    }
    else {
      $field->create_instance_for('bean', $this->get('name'), $weight);
      array_push($this->fields, $field);
    }

    return $this;
  }

  /**
   * Adds field only if it isn't already added.
   */
  public function add_field_if_missing($field, $weight = NULL) {
    $field->instance->set('entity_type', 'bean')->set('bundle', $this->get('name'));
    if (!$field->instance->exists()) {
      $this->add_field($field, $weight);
    }
    return $this;
  }

}

/**
 * Creates beans in code.
 *
 * @code
 * $my_bean = CINC::init('Bean')->machine_name('my_bean_type')->read();
 * @endcode
 */
class CINC_Bean extends CINC implements CINC_CRUD {

  public static $dependencies = array('bean_admin_ui');
  public $config_machine_name = 'delta';
  public $cinc_yaml_prefix = 'bean';

  function __construct() {

    $this->config_filters['delta'] = 'set_delta';

    $this->config = array(
      'delta' => '',
      'label' => '',
      'description' => '',
      'title' => '',
      'type' => '',
      'view_mode' => 'default',
    );

  }

  /**
   * Auto-adds label when delta is set, and cleans delta.
   */
  public function set_delta(&$delta) {
    $delta = CINC::clean_machine_name($delta);
    if (empty($this->config['label'])) {
      $this->set('label', CINC::display_name_from_machine_name($delta));
    }
    return TRUE;
  }

  /**
   * Saves bean to the database.
   */
  public function create() {
    $bean = bean_create($this->config);
    bean_save($bean);
    $this->set('bid', $bean->bid);
    $this->set('vid', $bean->vid);
    return $this;
  }

  /**
   * Tests if bean exists.
   */
  public function exists() {
    $bean = bean_load_delta($this->get('delta'));
    if ($bean) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads bean from the database.
   */
  public function read() {
    $bean = bean_load_delta($this->get('delta'));
    if ($bean) {
      $this->config = (array) $bean;
      foreach ($this->config as $index => $value) {
        if (preg_match('/^\x00/', $index)) {
          unset($this->config[$index]);
        }
      }
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
   * Saves bean to the database.
   */
  public function update() {
    $bean = bean_load_delta($this->get('delta'));
    foreach ($this->config as $key => $value) {
      $bean->{$key} = $value;
    }
    bean_save($bean);
    return $this;
  }

  /**
   * Removes bean from the database.
   */
  public function delete() {
    $bean = bean_create($this->config);
    bean_delete($bean);
    return $this;
  }

}
