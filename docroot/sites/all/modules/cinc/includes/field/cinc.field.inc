<?php

/**
 * Creates fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('Field')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_Field extends CINC implements CINC_CRUD {

  public static $dependencies = array('field');
  public $config_machine_name = 'field_name';
  public $cinc_yaml_prefix = 'field.storage';
  public $cinc_yaml_regex = '/^field\.storage\.[^.]+/';
  public $instance = NULL;

  function __construct() {

    $this->config_filters['field_name'] = 'set_field_name';
    $this->config_filters['settings'] = 'set_settings';

    $this->config = array(
      'field_name' => '',
      'active' => 1,
      'locked' => 0,
      'cardinality' => 1,
      'deleted' => 0,
    );

    $this->instance = CINC::init('FieldInstance');

  }

  /**
   * Converts settings.
   */
  public function set_settings(&$settings) {
    if (isset($settings['vocabulary'])) {
      $settings['allowed_values'] = array(
        array(
          'vocabulary' => $settings['vocabulary'],
          'parent' => 0,
        ),
      );
      unset($settings['vocabulary']);
    }
    return TRUE;
  }

  /**
   * Auto-adds label when field_name is set, and cleans field_name.
   */
  public function set_field_name(&$field_name) {
    $field_name = CINC::clean_machine_name($field_name);
    if (empty($this->instance_config['label'])) {
      $this->set_instance('label', CINC::display_name_from_machine_name($field_name));
    }
    if (empty($this->instance_config['field_name'])) {
      $this->set_instance('field_name', $field_name);
    }
    return TRUE;
  }

  /**
   * Sets properties of the field instance.
   *
   * @code
   * CINC::init('Field')
   *   ->set('field_name', 'field_my_field')
   *   ->set_instance('settings', array('user_register_form' => TRUE));
   * @endcode
   *
   * @param string $property
   *   Property to set.
   * @param string|array $value
   *   Value to set on property.
   * @return $this
   */
  public function set_instance($property, $value) {
    $this->instance->set($property, $value);
    return $this;
  }

  /**
   * Saves field information to the database.
   */
  public function create() {
    // enforce settings as an array to prevent errors in field.crud.inc
    if (!isset($this->config['settings']) || (isset($this->config['settings']) && $this->config['settings'] === NULL)) {
      $this->config['settings'] = array();
    }
    field_create_field($this->config);
    return $this;
  }

  /**
   * Tests if field exists.
   */
  public function exists() {
    $field_config = field_read_field($this->config['field_name'], array('include_inactive' => TRUE));
    if ($field_config) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads field information from the database.
   */
  public function read() {
    $field_config = field_read_field($this->config['field_name'], array('include_inactive' => TRUE));
    if ($field_config) {
      $this->config = $field_config;
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $fields = field_read_fields();

    foreach ($fields as $machine_name => $field) {
      $list[$machine_name] = array(
        'display' => $machine_name,
      );
    }

    return $list;

  }

  /**
   * Loads field instance information from the database.
   */
  public function read_instance_for($entity_type, $bundle) {
    $this->instance->set('entity_type', $entity_type);
    $this->instance->set('bundle', $bundle);
    $this->instance->read();
    return $this;
  }

  /**
   * Saves field information to the database and migrates previous data if machine name changed.
   */
  public function update() {

    $previous = clone $this;
    $previous->read();

    if ($previous->get('type') != $this->get('type')) {
      $this->update_by_replace_keeping_content($previous);
    }
    else {
      field_update_field($this->config);
    }
    return $this;
  }

  public function update_by_replace_keeping_content($previous) {
    // Copy data from previous field.
    $previous_field = field_info_field($previous->get('field_name'));
    $previous_field_storage = $previous_field['storage']['details']['sql'];
    list($previous_data_table) = array_keys($previous_field_storage['FIELD_LOAD_CURRENT']);
    $previous_keys = array_keys($previous_field_storage['FIELD_LOAD_CURRENT'][$previous_data_table]);

    db_query('CREATE TABLE {cinc_field_copy_temp} LIKE {' . $previous_data_table . '}');
    db_query('INSERT INTO {cinc_field_copy_temp} SELECT * FROM {' . $previous_data_table . '}');

    // Update field
    $previous->delete_and_purge();
    $this->create();

    // Put data in new field.
    $new_field = field_info_field($this->get('field_name'));
    $new_field_storage = $new_field['storage']['details']['sql'];
    list($new_data_table) = array_keys($new_field_storage['FIELD_LOAD_CURRENT']);
    $new_keys = array_keys($new_field_storage['FIELD_LOAD_CURRENT'][$new_data_table]);

    // Only move if tables have same keys.
    if ($previous_keys == $new_keys) {

      // Disbale STRICT_TRANS_TABLES for this session, to auto-truncate inserted data.
      db_query("SET @@session.sql_mode = ''");

      $previous_fields = array('entity_type', 'bundle', 'deleted', 'entity_id', 'revision_id', 'language', 'delta');
      $new_fields = $previous_fields;

      foreach ($previous_keys as $previous_key) {
        $previous_fields[] = $previous_field_storage['FIELD_LOAD_CURRENT'][$previous_data_table][$previous_key];
        $new_fields[] = $new_field_storage['FIELD_LOAD_CURRENT'][$new_data_table][$previous_key];
      }

      $sql = 'INSERT INTO {' . $new_data_table . '}
      (' . implode(', ', $new_fields) . ')
      SELECT ' . implode(', ', $previous_fields) . '
      FROM {cinc_field_copy_temp}';
      db_query($sql);

      db_drop_table('cinc_field_copy_temp');

    }

  }

  /**
   * Updates an instance.
   * Instance must first be loaded using read_instance_for().
   */
  public function update_instance() {
    field_update_instance($this->instance->config);
    return $this;
  }

  /**
   * Removes field information from the database.
   */
  public function delete() {
    field_delete_field($this->config['field_name']);
    return $this;
  }

  public function delete_and_purge() {
    $this->delete();
    field_purge_batch(10);
  }

  /**
   * Saves a field to an entity bundle.
   *
   * @code
   * $field = CINC::init('Field')->set('field_name', 'my_field')->create();
   * $field->create_instance_for('node', 'my_content_type');
   * @endcode
   *
   * @param string $entity_type
   * @param string $bundle
   * @param integer $weight
   */
  public function create_instance_for($entity_type, $bundle, $weight = NULL) {

    $this->instance->set('entity_type', $entity_type);
    $this->instance->set('bundle', $bundle);
    if ($weight !== NULL) {
      $this->instance->set('widget', array('weight' => $weight));
    }

    $this->instance->create();

    return $this;
  }

  public function create_or_update_instance_for($entity_type, $bundle, $weight = NULL) {

    $this->instance->set('entity_type', $entity_type);
    $this->instance->set('bundle', $bundle);
    if ($weight !== NULL) {
      $this->instance->set('widget', array('weight' => $weight));
    }

    if ($this->instance->exists()) {
      $this->instance->update();
    }
    else {
      $this->create_instance_for($entity_type, $bundle, $weight);
    }

    return $this;
  }

}

/**
 * Creates field instancess that can be saved to content types.
 *
 * @code
 * $my_field_instance = CINC::init('FieldInstance')->machine_name('field_my_field');
 * @endcode
 */
class CINC_FieldInstance extends CINC implements CINC_CRUD {

  public $config_machine_name = 'bundle.field_name';
  public $cinc_yaml_prefix = 'field.field';
  public $cinc_yaml_regex = '/^field\.field\.[^.]+/';

  function __construct() {

    $this->config_filters['settings'] = 'set_settings';

    $this->config = array(
      'label' => '',
      'required' => 0,
      'description' => '',
      'field_name' => '',
      'entity_type' => 'node',
      'bundle' => '',
      'deleted' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
      ),
    );

  }

  /**
   * Converts settings.
   */
  public function set_settings(&$settings) {
    if (isset($settings['wysiwyg'])) {
      $settings['text_processing'] = $settings['wysiwyg'];
      unset($settings['wysiwyg']);
    }
    return TRUE;
  }

  public function machine_name($value = '') {

    if (empty($value)) {

      if (isset($this->config_machine_name)) {
        return $this->get($this->config_machine_name);
      }
      return NULL;
    }
    else {

      if (isset($this->config_machine_name)) {
        $names = explode('.', $value);
        $this->set('bundle', $names[0]);
        $this->set('field_name', $names[1]);
      }
      return $this;

    }

  }

  /**
   *  Parses yaml filename into unqiue machine name.
   */
  public function yaml_parse_machine_name($file_name_suffix) {
    $parts = array_values(array_filter(explode('.', $file_name_suffix)));
    return $parts[0] . '.' . $parts[1];

  }

  /**
   * Saves field instance information to the database.
   */
  public function create() {
    field_create_instance($this->config);
    return $this;
  }

  /**
   * Tests if field instance exists.
   */
  public function exists() {
    $field_instance_config = field_read_instance($this->config['entity_type'], $this->config['field_name'], $this->config['bundle']);
    if ($field_instance_config) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads field instance information from the database.
   */
  public function read() {
    $field_instance_config = field_read_instance($this->config['entity_type'], $this->config['field_name'], $this->config['bundle']);
    if ($field_instance_config) {
      $this->config = $field_instance_config;
    }
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    $fields = field_read_instances();

    foreach ($fields as $field) {
      $list[$field['field_name']] = array(
        'display' => $field['entity_type'] . ' ' . $field['bundle'] . ' ' . $field['label'],
      );
    }

    return $list;

  }

  /**
   * Saves field instance information to the database.
   */
  public function update() {
    field_update_instance($this->config);
    return $this;
  }

  /**
   * Removes field instance information from the database.
   */
  public function delete() {
    field_delete_instance($this->config);
    return $this;
  }

}
