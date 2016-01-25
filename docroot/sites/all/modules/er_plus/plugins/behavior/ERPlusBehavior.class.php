<?php

class ERPlusBehavior extends EntityReference_BehaviorHandler_Abstract {

  public function schema_alter(&$schema, $field) {
    $schema['columns']['view_mode'] = array(
      'description' => 'Target view mode machine name.',
      'type' => 'varchar',
      'length' => 32,
      'default' => 'full',
      'not null' => FALSE,
    );
    $schema['columns']['header'] = array(
      'description' => 'Header',
      'type' => 'varchar',
      // TODO: validate form element for this length.
      'length' => 255,
      'default' => '',
      'not null' => FALSE,
    );
  }

  public function property_info_alter(&$info, $entity_type, $field, $instance, $field_type) {

  }

  public function load($entity_type, $entities, $field, $instances, $langcode, &$items) {
    //drupal_set_message(t('Do something on load!'));
  }

  public function insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
    //drupal_set_message(t('Do something on insert!'));
  }

  public function update($entity_type, $entity, $field, $instance, $langcode, &$items) {
    //drupal_set_message(t('Do something on update!'));
  }

  public function delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
    //drupal_set_message(t('Do something on delete!'));
  }

  /**
   * Generate a settings form for this handler.
   */
  public function settingsForm($field, $instance) {
    $form['er_plus'] = array(
      '#type' => 'checkbox',
      '#title' => t('Change schema'),
    );
    return $form;
  }
}
