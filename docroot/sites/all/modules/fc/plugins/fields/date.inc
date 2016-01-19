<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - date field plugin.
 */

$plugin = array(
  'title' => t('FC: Date field plugin'),
  'zero can be empty' => TRUE,

  'completeness check' => 'fc_date_field_is_complete',
  'incomplete process' => 'fc_date_incomplete_process',
);

/**
 * Use the date 'empty' hook to see if each entry has content,
 * this also fixes the "todate" bug
 */
function fc_date_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $zero_as_empty = !empty($instance['settings']['fc']['fc_check_zero']);
  $field['settings'] += array('todate' => '');

  foreach ($items as $delta => $item) {
    if (!empty($item['value']) && $item['value'] === '0' && $zero_as_empty) {
      $item['value'] = '';
    }

    $completed[$delta] = !module_invoke($module, 'field_is_empty', $item, $field);
  }
}

function fc_date_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $incomplete = new fcIncomplete($instance['field_name'], $parent);
  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
