<?php
/**
 * @file
 * A Feeds Tamper plugin that allows appending to multivalue fields rather than overwriting them.
 * Only works with type 'update' not 'replace' in the Node Processor settings.
 */

$plugin = array(
  'form' => 'feeds_tamper_conditional_form',
  'callback' => 'feeds_tamper_conditional_callback',
  'name' => 'Conditional',
  'multi' => 'skip',
  'category' => 'Other',
);

function feeds_tamper_conditional_form($importer, $element_key, $settings) {

  $form = array();

  $mappings = $importer->processor->config['mappings'];

  $form['help'] = array(
    '#markup' => t('Only writes the field if a condition is met.  Condition can be based on the value
      of any field.'),
  );

  // @todo add a hook here to allow modules to add their own format for the options in the dropdown.
  $feeds_filemaker = (get_class($importer->parser) == 'FeedsFileMakerParser')
    ? true
    : false;

  // Grab all of the mapping sources, for addition to the conditional field select
  $replace = array();
  foreach ($mappings as $mapping) {
    if($feeds_filemaker) {
      $prefix = $importer->config['parser']['config']['mappings'][$mapping['source']]['mapping'] . ' - ';
    }
    else {
      $prefix = '';
    }
    $replace[$mapping['source']] = $prefix . $mapping['target'] . ' [' . $mapping['source'] . ']';
  }
  $form['conditional_field'] = array(
    '#type' => 'select',
    '#title' => t('Field to check'),
    '#options' => $replace,
    '#required' => true,
    '#default_value' => isset($settings['conditional_field'])
      ? $settings['conditional_field']
      : '',
  );

  // The operators that can be used to check whether the condition has been met
  $operators = array(
    'equals' => 'Equals',
    'not_equals' => 'Does not equal',
    'greater' => 'Greater than',
  	'greater_equal' => 'Greater than or equal',
    'less' => 'Less than',
  	'less_equal' => 'Less than or equal',
  	'contains' => 'Contains',
  	'not_contains' => 'Does not contain',
  );
  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => $operators,
  	'#default_value' => isset($settings['operator'])
      ? $settings['operator']
      : '',
  	'#required' => true,
  );

  // The value to check for
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value to check'),
    '#default_value' => isset($settings['value'])
      ? $settings['value']
      : '',
    '#required' => FALSE,
  );
  return $form;
}

function feeds_tamper_conditional_callback(&$result, $item_key, $element_key, &$field, $settings) {

  $condition_met = false;
  $conditional_field_value = $result->items[$item_key][$settings['conditional_field']];

  switch($settings['operator']) {
    case 'equals': {
      $condition_met = ($conditional_field_value == $settings['value']) ? true : false;
    }
    break;
    case 'not_equals': {
      $condition_met = ($conditional_field_value != $settings['value']) ? true : false;
    }
    break;
    case 'greater': {
      $condition_met = ($conditional_field_value > $settings['value']) ? true : false;
    }
    break;
    case 'greater_equal': {
      $condition_met = ($conditional_field_value >= $settings['value']) ? true : false;
    }
    break;
    case 'less': {
      $condition_met = ($conditional_field_value < $settings['value']) ? true : false;
    }
    break;
    case 'less_equal': {
      $condition_met = ($conditional_field_value <= $settings['value']) ? true : false;
    }
    break;
    case 'contains': {
      $condition_met = is_int(stripos($conditional_field_value, $settings['value'])) ? true : false;
    }
    break;
    case 'not_contains': {
      $condition_met = is_int(stripos($conditional_field_value, $settings['value'])) ? false : true;
    }
    break;
  }

  if(!$condition_met) {
    $result->items[$item_key][$element_key] = '';
  }
}
