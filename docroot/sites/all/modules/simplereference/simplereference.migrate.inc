<?php

/**
 * @file
 * Support for processing entity reference fields in Migrate.
 */

/**
 * Implement hook_migrate_api().
 */
function simplereference_migrate_api() {
  return array(
    'api' => 2,
    'field handlers' => array('MigrateSimpleReferenceFieldHandler'),
  );
}

class MigrateSimpleReferenceFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('simplereference'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    $migration = Migration::currentMigration();
    $destination = $migration->getDestination();
    $current_language = $this->getFieldLanguage($entity, $field_info, $arguments);
    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $object = _simplereference_get_title_by_entity_id(array('destination_entity_type' => $value['entity_type'], 'destination_entity_id' => $value['entity_id']));
      $item['value'] = $object->title;
      $item['destination_entity_type'] = $value['entity_type'];
      $item['destination_entity_bundle'] = $value['entity_bundle'];
      $item['destination_entity_id'] = $value['entity_id'];
      $return[$current_language][] = $item;
      $delta++;
    }
    return isset($return) ? $return : NULL;

  }

}
