<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - class.
 */

class fcComplete {
  const SKIP_IF_EXISTS = TRUE;

  static public function build($entity_type, $entity) {
    return new self($entity_type, $entity);
  }
  static public function load($entity_type, $entity) {
    $fc = new self($entity_type, $entity);
    $fc->fetch();
    return $fc;
  }

  protected $entity_type, $entity, $id, $revision_id, $bundle;
  protected $complete, $percentage, $completeness;

  protected function __construct($entity_type, $entity) {
    $this->entity_type = $entity_type;
    $this->entity = $entity;
    list($this->id, $this->revision_id, $this->bundle) = entity_extract_ids($entity_type, $entity);
    $this->revision_id = is_null($this->revision_id) ? $this->id : $this->revision_id;
  }

  public function __get($var) {
    switch ($var) {
      case 'complete':
      case 'percentage':
      case 'completeness':
        $val = $this->$var;
        break;
      default:
        $val = NULL;
        break;
    }
    return $val;
  }

  public function completeness() {
    $field_info = field_info_fields();

    $instances = field_info_instances($this->entity_type, $this->bundle);

    // Allow other modules to remove instances from the completeness check
    $options = array(
      'bundle' => $this->bundle,
      'context' => 'view',
    );
    drupal_alter('fc_instances', $instances, $this->entity_type, $this->entity, $options);

    $this->completeness = array();
    foreach ($instances as $field_name => $instance) {
      $settings = $instance['settings']['fc'];
      if (!empty($settings['disable'])) {
        continue;
      }

      // We track the completeness of all fields.
      $this->completeness[$field_name] = FALSE;

      // Go through the field items if any are "not empty" then
      // we count that as complete (a bit simplistic but okay).
      $field_items = field_get_items($this->entity_type, $this->entity, $field_name);
      if (empty($field_items)) {
        if (!empty($settings['fc_allow_empty'])) {
          // Complex fields can be set so that they optionally
          // have content, and if they don't they must be
          // counted as complete
          $this->completeness[$field_name] = TRUE;
        }
        continue;
      }

      $field = $field_info[$field_name];

      // Choose the right plugin for the field type.
      $plugin = fc_get_plugin($field['type']);

      if ($function = ctools_plugin_get_function($plugin, 'completeness check')) {
        // Process the field to determine whether it's complete, normally we
        // just compare the number of complete field_items with the cardinality
        // but other fields might be more complex (like matrix fields).
        $cardinality = ctools_plugin_get_function($plugin, 'cardinality check');
        $this->completeness[$field_name] = $cardinality($function, $field_items, $instance, $field);
      }
    }

    $count_fields = count($this->completeness);
    $complete_fields = count(array_filter($this->completeness));

    $this->complete = $count_fields == $complete_fields;
    $this->percentage = $count_fields ? (int) ($complete_fields * 100 / $count_fields) : 100;
  }

  public function save($skipIfExists = FALSE) {
    $fc = db_select('fc', 'fc')
      ->fields('fc', array('percentage', 'completeness'))
      ->condition('entity_type', $this->entity_type)
      ->condition('entity_id', $this->entity_id)
      ->condition('revision_id', $this->revision_id)
      ->execute()->fetchObject();
    if (empty($fc)) {
      $this->insert();
    }
    elseif (!$skipIfExists) {
      $this->update();
    }
  }

  public function insert() {
    $object = (object) array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->id,
      'revision_id' => $this->revision_id,
      'complete' => $this->complete,
      'percentage' => $this->percentage,
      'completeness' => $this->completeness,
    );
    drupal_write_record('fc', $object);
  }

  public function update() {
    $object = (object) array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->id,
      'revision_id' => $this->revision_id,
      'complete' => $this->complete,
      'percentage' => $this->percentage,
      'completeness' => $this->completeness,
    );
    drupal_write_record('fc', $object, array('entity_type', 'entity_id', 'revision_id'));
  }

  public function fetch() {
    $fc = db_select('fc', 'fc')
      ->fields('fc', array('complete', 'percentage', 'completeness'))
      ->condition('entity_type', $this->entity_type)
      ->condition('entity_id', $this->entity_id)
      ->condition('revision_id', $this->revision_id)
      ->execute()->fetchObject();

    if (empty($fc)) {
      $this->completeness();
    }
    else {
      $this->complete = $fc->complete;
      $this->percentage = $fc->percentage;
      $this->completeness = $fc->completeness;
    }
  }

  public function debug() {
    return array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->id,
      'revision_id' => $this->revision_id,
      'complete' => $this->complete,
      'percentage' => $this->percentage,
      'completeness' => $this->completeness,
    );
  }
}
