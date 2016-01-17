<?php

/**
 * @file
 * Feeds mapping implementation for the hms_feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function hms_field_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {

  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'hms_field') {
      $targets[$name] = array(
        'name'        => check_plain($instance['label']),
        'callback'    => 'hms_field_feeds_set_target',
        'description' => t('The field instance @label of @id', array(
          '@label' => $instance['label'],
          '@id'    => $name,
        )),
      );
    }
  }
}

/**
 * HMS field callback for mapping.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 *
 * @param $source
 *   A FeedsSource object.
 * @param $entity
 *   The entity to map to.
 * @param $target
 *   The target key on $entity to map to.
 * @param $value
 *   The value to map. MUST be an array.
 * @param $mapping
 *   Array of mapping settings for current value.
 * @param $input_format
 *   TRUE if an input format should be applied.
 */
function hms_field_feeds_set_target($source, $entity, $target, $value, $mapping, $input_format = FALSE) {

  // Don't do anything if we weren't given any data.
  if (empty($value)) {
    return;
  }

  // Assume that the passed in value could really be any number of values.
  if (is_array($value)) {
    $values = $value;
  }
  else {
    $values = array($value);
  }

  // Get some useful field information.
  $info = field_info_field($target);

  // Set the language of the field depending on the mapping.
  $language = isset($mapping['language']) ? $mapping['language'] : LANGUAGE_NONE;

  // Iterate over all values.
  $iterator = 0;
  $field = isset($entity->$target) ? $entity->$target : array();
  foreach ($values as $value) {

    // Only process if this value was set for this instance.
    if ($value) {
      $field[$language][$iterator]['value']   = $value;
    }

    // Break out of the loop if this field is single-valued.
    if ($info['cardinality'] == 1) {
      break;
    }

    $iterator++;
  }

  // Add the field to the entity definition.
  $entity->{$target} = $field;
}
