<?php

/**
 * @file
 * Feeds mapping implementation for the Chinese identity card module.
 */

/**
 * Implements hook_feeds_processor_targets().
 */
function chinese_identity_card_feeds_processor_targets($entity_type, $bundle) {
  $targets = array();
  foreach (field_info_instances($entity_type, $bundle) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'chinese_identity_card') {
      $targets[$name] = array(
        'name' => t('@name', array('@name' => $instance['label'])),
        'callback' => 'chinese_identity_card_feeds_set_target',
        'description' => t('The @label field of the entity.', array('@label' => $instance['label'])),
      );
    }
  }

  return $targets;
}

/**
 * Callback for mapping chinese identity card fields.
 */
function chinese_identity_card_feeds_set_target(FeedsSource $source, $entity, $target, array $values) {
  $field_name = $target;
  $field = isset($entity->$field_name) ? $entity->$field_name : array(LANGUAGE_NONE => array());

  // Iterate over all values.
  $delta = 0;
  foreach ($values as $value) {
    if (is_object($value) && $value instanceof FeedsElement) {
      $value = $value->getValue();
    }
    if (is_scalar($value) && chinese_identity_card_check($value)) {
      $field[LANGUAGE_NONE][$delta] = array('chinese_identity_card' => $value);
    }
    $delta++;
  }

  $entity->$field_name = $field;
}
