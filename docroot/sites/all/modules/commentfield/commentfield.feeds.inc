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
function commentfield_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'commentfield') {
      foreach ($info['columns'] as $subfield => $column) {
        $targets["$name:$subfield"] = array(
          'name' => $instance['label'] . ':' . $subfield,
          'callback' => 'commentfield_feeds_set_target',
          'description' => t(ucfirst($subfield) . ' subfield of the @name field.', array('@name' => $instance['label'])),
          'real_target' => $name,
        );
      }
    }
  }
}
/**
 * Callback function for mapping email field.
 *
 * This function is invoked via hook_feeds_processor_targets_alter().
 * Here is where the actual mapping happens.
 *
 * @param $target
 *   the name of the field the user has decided to map to.
 * @param $value
 *   the value of the feed item element the user has picked as a source.
 */
function commentfield_feeds_set_target($source, $entity, $target, $value) {
  if (empty($value)) {
    return;
  }
  // Handle non-multiple value fields.
  $value = is_array($value) ? $value : array($value);
  list($field_name, $subfield) = explode(':', $target);
  $info = field_info_field($field_name);
  // Iterate over all values.
  foreach ($value as $delta => $item) {
    if (!is_array($item) && !is_object($item)) {
      $entity->{$field_name}[LANGUAGE_NONE][$delta][$subfield] = $item;
    }
  }
}
