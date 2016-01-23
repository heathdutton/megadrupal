<?php

/**
 * Creates text fields that can be saved to content types.
 *
 * @code
 * $my_field = CINC::init('FieldCollectionField')->set('field_name', 'my_field');
 * @endcode
 */
class CINC_FieldCollectionField extends CINC_Field {

  function __construct() {

    parent::__construct();

    $this
      ->set('type', 'field_collection')
      ->set('module', 'field_collection');

    $this
      ->set_instance('widget', array(
        'type' => 'field_collection_embed',
      ));

  }

  /**
   * Adds field (instance) to the field collection.
   *
   * @param CINC_Field $field_instance
   *
   * @return $this
   */
  public function add_field($field, $weight = NULL) {
    if (!($field instanceof CINC_Field)) {
      if ($field instanceof CINC_MissingDependencies) {
        $dependencies = implode(', ', $field->missing_dependencies);
        drupal_set_message(t('CINC_FieldCollection::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), 'warning');
        watchdog('CINC', format_string('CINC_FieldCollection::add_field tried to add field @field_name but is missing dependencies: @dependencies', array('@field_name' => $field->config['field_name'], '@dependencies' => $dependencies)), array(), WATCHDOG_WARNING);
      }
      else {
        throw new Exception('CINC_FieldCollection::add_field requires a CINC_Field instance as the first parameter');
      }
    }
    else {
      // Use the CINC_Field Class method to save the field to this field collection.
      $field->create_instance_for('field_collection_item', $this->get('field_name'), $weight);
    }

    return $this;
  }

  /**
   * Adds field only if it isn't already added.
   */
  public function add_field_if_missing($field, $weight = NULL) {
    $field->instance->set('entity_type', 'field_collection_item')->set('bundle', $this->get('field_name'));
    if (!$field->instance->exists()) {
      $this->add_field($field, $weight);
    }
    return $this;
  }

}
