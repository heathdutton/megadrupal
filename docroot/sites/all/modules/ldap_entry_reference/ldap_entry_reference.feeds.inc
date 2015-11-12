<?php

/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function ldap_entry_reference_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'ldap_entry_reference') {
      if (array_key_exists('dn', $info['columns'])) {
        $targets[$name] = array(
          'name' => t('@label', array('@label' => $instance['label'])),
          'callback' => 'ldap_entry_reference_feeds_set_target',
          'description' => t('The distinguished name (DN) for the @name field. NOTE: for performance, no check for existence is performed.', array('@name' => $instance['label'])),
        );
      }
    }
  }
}

/**
 * Callback for mapping ldap_entry_reference fields.
 */
function ldap_entry_reference_feeds_set_target(FeedsSource $source, $entity, $target, $value) {
  if (empty($value)) {
    return;
  }

  if (!is_array($value)) {
    $value = array($value);
  }

  $info = field_info_field($target);

  // Iterate over all values.
  $field = isset($entity->$target) ? $entity->$target : array(LANGUAGE_NONE => array());

  // Allow for multiple mappings to the same target.
  $delta = count($field[LANGUAGE_NONE]);

  foreach ($value as $v) {
    if ($info['cardinality'] == $delta) {
      break;
    }

    if (is_object($v) && ($v instanceof FeedsElement)) {
      $v = $v->getValue();
    }

    if (is_scalar($v)) {
      $field[LANGUAGE_NONE][$delta]['dn'] = $v;

      $delta++;
    }
  }

  $entity->$target = $field;
}
