<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - default field plugin.
 */

$plugin = array(
  'title' => t('FC: Default field plugin'),
  'zero can be empty' => TRUE,

  'completeness check' => 'fc_default_field_is_complete',
  'incomplete process' => 'fc_default_incomplete_process',
);

/**
 * Simply use the default 'empty' hook to see if each entry has content
 */
function fc_default_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $zero_as_empty = !empty($instance['settings']['fc']['fc_check_zero']);

  foreach ($items as $delta => $item) {
    if (!empty($item['value']) && $item['value'] === '0' && $zero_as_empty) {
      $item['value'] = '';
    }

    $completed[$delta] = !module_invoke($module, 'field_is_empty', $item, $field);
  }
}

function fc_default_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $incomplete = new fcIncomplete($instance['field_name'], $parent);
  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
