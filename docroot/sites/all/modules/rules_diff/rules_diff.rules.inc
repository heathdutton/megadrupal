<?php

/**
 * @file rules integration for the node module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_data_info() on behalf of the diff module.
 */
function rules_diff_rules_data_info() {
 return array(
   'diff' => array(
     'label' => t('difference'),
     'wrap' => TRUE,
     'property info' => _rules_diff_difference_data_info(),
   ),
   'diff_property' => array(
     'label' => t('properties with differences'),
     'wrap' => TRUE,
     'property info' => _rules_diff_property_diff_data_info(),
   ),
 );
}

/**
 * Implements hook_rules_action_info() on behalf of the diff module.
 */
function rules_diff_rules_action_info() {
  return array(
    'rules_diff_calculate_differences' => array(
      'label' => t('Calculate differences between entities'),
      'group' => t('Diff'),
      'parameter' => array(
        'original_entity' => array(
          'type' => 'entity',
          'label' => t('Original entity'),
          'description' => t('The entity for which the differences will be calculated from.'),
        ),
        'changed_entity' => array(
          'type' => 'entity',
          'label' => t('Changed entity'),
          'description' => t('The entity for which the differences will be calculated against.'),
        ),
        'extra' => array(
          'type' => 'boolean',
          'label' => t('Include extra fields'),
          'description' => t('If you check this, extra fields will be taken into account. Extra fields in node entities include title, changed timestamp, is_new flag, author uid, etc.'),
          'default value' => TRUE,
        ),
      ),
      'provides' => array(
        'differences_list' => array(
          'type' => 'list<diff_property>',
          'label' => t('Properties with differences'),
        ),
      ),
      'callbacks' => array(
        'validate' => 'rules_diff_calculate_differences_validate',
      ),
    ),
  );
}

/**
 * Defines property info for diff entries.
 */
function _rules_diff_difference_data_info() {
  return array(
    'type' => array(
      'type' => 'integer',
      'label' => t('The difference type'),
      'options list' => '_rules_diff_differences_types',
    ),
    'subject_name' => array(
      'type' => 'text',
      'label' => t('The name of the comparison subject'),
    ),
    'line' => array(
      'type' => 'integer',
      'label' => t('The line where the difference appears'),
    ),
    'difference' => array(
      'type' => 'text',
      'label' => t('The difference'),
    ),
    'order' => array(
      'type' => 'integer',
      'label' => t('Order'),
    ),
  );
}

/**
 * Defines property info for property diff entries.
 */
function _rules_diff_property_diff_data_info() {
  return array(
    'name' => array(
      'type' => 'text',
      'label' => t('Property name'),
    ),
    'differences' => array(
      'type' => 'list<diff>',
      'label' => t('The differences for the property.'),
    ),
    'count' => array(
      'type' => 'integer',
      'label' => t('The number of differences found.'),
    ),
  );
}

/**
 * Callback that returns the options for the type's select list.
 */
function _rules_diff_differences_types() {
  return array(
    DIFF_ADD => t('Add'),
    DIFF_DEL => t('Remove'),
  );
}

/**
 * Action callback function.
 */
function rules_diff_calculate_differences($original_entity, $changed_entity, $extra) {
  return array(
    'differences_list' => _rules_diff_wrap_data_type($original_entity->value(), $changed_entity->value(), $original_entity->type(), $extra),
  );
}

/**
 * Validates that the selected entities are of the same type.
 */
function rules_diff_calculate_differences_validate($element, $callback_type) {
  $old = $element->getArgumentInfo('original_entity');
  $new = $element->getArgumentInfo('changed_entity');

  if ($old['type'] != $new['type']) {
    throw new RulesIntegrityException(t('Original and chaged entities must be of the same type.'), array($element, 'parameter', 'changed_entity'));
  }
}

/**
 * Generates diff data types for the provided entities.
 *
 * Uses diff's module diff_compare_entities to get the differences between the
 * entities.
 *
 * @param entity $original_entity
 *   The entity for which the differences will be calculated from.
 * @param entity $changed_entity
 *   The entity for which the differences will be calculated against.
 * @param string $entity_type
 *   The type for the entity.
 *
 * @return array
 *   A list<diff> containing the differences.
 *
 * @see diff_compare_entities()
 */
function _rules_diff_wrap_data_type($original_entity, $changed_entity, $entity_type, $extra) {
  module_load_include('inc', 'diff', 'diff.pages');
  $context = array('entity_type' => $entity_type);
  $diffs = diff_compare_entities($original_entity, $changed_entity, $context);
  if ($extra) {
    $diffs += rules_diff_compare_extra_fields($original_entity, $changed_entity, $context);
  }
  $output = array();
  $i = 0;
  foreach ($diffs as $property_name => $property_diffs) {
    $differences = array();
    $raw = $property_diffs['#states']['raw'];
    $differences[DIFF_DEL] = array_diff($raw['#old'], $raw['#new']);
    $differences[DIFF_ADD] = array_diff($raw['#new'], $raw['#old']);
    if (!empty($differences[DIFF_DEL]) || !empty($differences[DIFF_ADD])) {
      $pdiff = array(
        'differences' => array(),
        'name' => '',
        'count' => 0,
      );
    }
    foreach ($differences[DIFF_DEL] as $line => $value) {
      $pdiff['differences'][] = array(
        // Add the difference to the output array
        'type' => DIFF_DEL,
        'subject_name' => $property_name,
        'difference' => $value,
        'line' => $line,
        'order' => ++$i,
      );
      // If there is an addition in the same line than the deletion may indicate
      // its an edition. Show the addition next to the deletion.
      if (isset($differences[DIFF_ADD][$line])) {
        $pdiff['differences'][] = array(
          'type' => DIFF_ADD,
          'subject_name' => $property_name,
          'line' => $line,
          'difference' => $differences[DIFF_ADD][$line],
          'order' => ++$i,
        );
        // Unset it to avoid duplicates.
        unset($differences[DIFF_ADD][$line]);
      }
    }
    foreach ($differences[DIFF_ADD] as $line => $value) {
      $pdiff['differences'][] = array(
        'type' => DIFF_ADD,
        'subject_name' => $property_name,
        'line' => $line,
        'difference' => $value,
        'order' => ++$i,
      );
    }
    if (!empty($differences[DIFF_DEL]) || !empty($differences[DIFF_ADD])) {
      $pdiff['name'] = $property_name;
      $pdiff['count'] = count($pdiff['differences']);
      $output[] = $pdiff;
    }
  }
  return $output;
}

/**
 * Helper function to compare extra fields.
 *
 * Parameters are the same as diff_compare_entities.
 *
 * @see diff_compare_entities().
 */
function rules_diff_compare_extra_fields($original_entity, $changed_entity, $context) {
  $differences = array();
  // Get an entity wrapper for each entity.
  $wrapper_original = entity_metadata_wrapper($context['entity_type'], $original_entity);
  $wrapper_changed = entity_metadata_wrapper($context['entity_type'], $changed_entity);
  // Iterate over all properties.
  foreach ($wrapper_original->getPropertyInfo() as $prop_name => $property) {
    // Ignore field properties.
    if (empty($property['field'])) {
      // Check the value.
      if ($wrapper_original->{$prop_name}->value() != $wrapper_changed->{$prop_name}->value()) {
        $value_original = $wrapper_original->{$prop_name}->value();
        $value_changed = $wrapper_changed->{$prop_name}->value();
        // Some possible return values are not stringables. Get their ID.
        $types = array('text', 'integer', 'uri', 'token', 'decimal', 'date', 'duration', 'boolean');
        if (!in_array($wrapper_original->{$prop_name}->type(), $types)) {
          if (method_exists($wrapper_original->{$prop_name}, 'getIdentifier')) {
            $value_original = $wrapper_original->{$prop_name}->getIdentifier();
          }
          else {
            $value_original = serialize($value_original);
          }
        }
        if (!in_array($wrapper_changed->{$prop_name}->type(), $types)) {
          if (method_exists($wrapper_changed->{$prop_name}, 'getIdentifier')) {
            $value_changed = $wrapper_changed->{$prop_name}->getIdentifier();
          }
          else {
            $value_changed = serialize($value_changed);
          }
        }
        $differences[$prop_name] = array(
          '#states' => array(
            'raw' => array(
              '#old' => array((string) $value_original),
              '#new' => array((string) $value_changed),
            ),
          )
        );
      }
    }
  }
  return $differences;
}

/**
 * @}
 */
