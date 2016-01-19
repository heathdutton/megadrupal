<?php

/**
 * Creates field groups in code.
 *
 * @code
 * $field_group = CINC::init('FieldGroup')->set('group_name', 'my_content_type');
 * @endcode
 */
class CINC_FieldGroup extends CINC implements CINC_CRUD {

  public static $dependencies = array('field_group');
  public $config_machine_name = 'identifier';
  public $cinc_yaml_prefix = 'field_group.';
  public static $groups = array();

  /**
   * Constructor
   */
  public function __construct() {
    $this->config_filters['group_name'] = 'set_group_name';
    $this->config_filters['entity_type'] = 'set_entity_type';
    $this->config_filters['bundle'] = 'set_bundle';
    $this->config_filters['mode'] = 'set_mode';
    $this->config_filters['identifier'] = 'set_identifier';

    $this->config = array(
      'identifier' => '',
      'group_name' => '',
      'entity_type' => 'node',
      'bundle' => '',
      'mode' => 'form',
      'parent_name' => '',
      'table' => 'field_group',
      'type' => 'Normal',
      'disabled' => FALSE,
      'label' => '',
      'weight' => '0',
      'children' => array(),
      'format_type' => 'div',
      'format_settings' => array(
        'formatter' => '',
        'instance_settings' => array(
          'classes' => '',
        ),
      ),
    );

  }

  /**
   * Auto-adds label when group_name is set, cleans group_name and updates identifier.
   */
  public function set_group_name(&$group_name) {
    $group_name = CINC::clean_machine_name($group_name);
    if (empty($this->config['label'])) {
      $this->set('label', CINC::display_name_from_machine_name($group_name));
    }
    $this->set('identifier', implode('|', array($group_name, $this->config['entity_type'], $this->config['bundle'], $this->config['mode'])));
    return TRUE;
  }

  /**
   * Updates identifier.
   */
  public function set_entity_type(&$entity_type) {
    $this->set('identifier', implode('|', array($this->config['group_name'], $entity_type, $this->config['bundle'], $this->config['mode'])));
    return TRUE;
  }

  /**
   * Updates identifier.
   */
  public function set_bundle(&$bundle) {
    $this->set('identifier', implode('|', array($this->config['group_name'], $this->config['entity_type'], $bundle, $this->config['mode'])));
    return TRUE;
  }

  /**
   * Updates identifier.
   */
  public function set_mode(&$mode) {
    $this->set('identifier', implode('|', array($this->config['group_name'], $this->config['entity_type'], $this->config['bundle'], $mode)));
    return TRUE;
  }

  /**
   * Updates identifier parts based on identifier.
   */
  public function set_identifier(&$identifier) {
    // Set parts in $this->config directly to avoid set loop.
    list($this->config['group_name'], $this->config['entity_type'], $this->config['bundle'], $this->config['mode']) = explode('|', $identifier);
    return TRUE;
  }

  /**
   * Saves field group to the database.
   */
  public function create() {
    $group = (object) $this->config;
    ctools_export_crud_save('field_group', $group);
    return $this;
  }

  public function exists() {

    if (empty(CINC_FieldGroup::$groups)) {
      CINC_FieldGroup::$groups = field_group_read_groups();
    }

    $bundle = CINC_FieldGroup::$groups[$this->config['entity_type']][$this->config['bundle']];
    if ($bundle[$this->config['mode']][$this->config['group_name']]) {
      return TRUE;
    }

    return FALSE;

  }

  /**
   * Loads field group from the database.
   */
  public function read() {

    if (empty(CINC_FieldGroup::$groups)) {
      CINC_FieldGroup::$groups = field_group_read_groups();
    }

    $bundle = CINC_FieldGroup::$groups[$this->config['entity_type']][$this->config['bundle']];
    if ($bundle[$this->config['mode']][$this->config['group_name']]) {
      $this->config = (array) $bundle[$this->config['mode']][$this->config['group_name']];
    }

    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {

    $list = array();

    if (empty(CINC_FieldGroup::$groups)) {
      CINC_FieldGroup::$groups = field_group_read_groups();
    }

    foreach (CINC_FieldGroup::$groups as $entity_type => $bundles) {

      foreach ($bundles as $bundle => $modes) {

        foreach ($modes as $mode => $group_names) {

          foreach ($group_names as $group_name => $details) {

            $machine_name = implode('|', array($group_name, $entity_type, $bundle, $mode));

            $list[$machine_name] = array(
              'display' => $details->label,
            );

          }

        }

      }
    }

    return $list;

  }

  /**
   * Saves field group to the database and migrates previous data if machine name changed.
   */
  public function update() {

    $this->config['export_type'] = EXPORT_IN_DATABASE;
    $this->create();

    return $this;
  }

  /**
   * Removes field group from the database.
   */
  public function delete() {
    $group = (object) $this->config;
    ctools_export_crud_delete('field_group', $group);

    return $this;
  }

  /**
   * Add fields to the field group.
   *
   * @param object $field
   *   An instance of a CINC_Field[type].
   *
   * @return object
   *   Returns $this
   */
  public function add_field($field) {
    if (!($field instanceof CINC_Field)) {
      if ($field instanceof CINC_MissingDependencies) {
        $dependencies = implode(', ', $field->missing_dependencies);
        drupal_set_message(t('CINC_FieldGroup::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), 'warning');
        watchdog('CINC', format_string('CINC_FieldGroup::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), array(), WATCHDOG_WARNING);
        return $this;
      }

      $type = gettype($field);
      $type_string = $type;
      $class = NULL;
      if ($type === 'object') {
        $class = get_class($field);
        $type_string .= '(' . $class . ')';
      }
      throw new Exception('CINC_FieldGroup::add_field requires a CINC_Field instance as the first parameter: ' . $type_string);
    }

    $children =& $this->config['children'];

    if (!in_array($field->config['field_name'], $children)) {
      $children[] = $field->config['field_name'];
    }

    return $this;
  }

  /**
   * Add multiple fields.
   *
   * @param array $fields
   *   An array of field instances.
   *
   * @return object
   *   Returns $this
   */
  public function add_fields(array $fields) {
    foreach ($fields as $field) {
      $this->add_field($field);
    }
    return $this;
  }

  /**
   * Add a field group to this field group.
   */
  public function add_group($group) {
    if (!($group instanceof CINC_FieldGroup)) {
      throw new Exception('CINC_FieldGroup::add_group requires a CINC_FieldGroup instance as the first parameter');
    }

    $children =& $this->config['children'];

    if (!in_array($group->config['group_name'], $children)) {
      $children[] = $group->config['group_name'];
    }

    return $this;
  }

}
