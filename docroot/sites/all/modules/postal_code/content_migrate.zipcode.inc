<?php

/**
 * @file
 * Zipcode content_migrate file
 */


function _postal_code_content_migrate_is_supported_country($country){

  $supported_countries = &drupal_static(__FUNCTION__);

  if (!is_array($supported_countries)) {
    $supported_countries = postal_code_validation();
  }

  if($country == 'uk'){
    $country = 'gb';
  }

  return isset($supported_countries[$country]);
}

/**
 * Implement this hook to alter the field definition of the migrated content.
 *
 * @param $field_value
 * @param $instance_value
 */
function postal_code_content_migrate_field_alter(&$field_value, $instance_value) {

  switch ($instance_value['widget']['module']) {
    case 'zipcode':

      $country = str_replace('_zipcode', '', $field_value['type']);

      if(_postal_code_content_migrate_is_supported_country($country)){

        // Module names and types changed.
        $field_value['module'] = 'postal_code';
        $field_value['type'] = 'postal_code';

      }

      break;
  }
}

/**
 * Implements this hook to alter the instance definition of the migrated content.
 *
 * Use this to tweak the conversion of instance or widget settings from the D6
 * style to the D7 style for specific situations not handled by basic
 * conversion, as when formatter or widget names or settings are changed.
 *
 * @param $instance_value
 * @param $field_value
 */
function postal_code_content_migrate_instance_alter(&$instance_value, $field_value) {
  switch ($instance_value['widget']['module']) {
    case 'zipcode':

      $country = str_replace('_zipcode', '', $field_value['type']);

      if(_postal_code_content_migrate_is_supported_country($country)){

        $widget_type = 'postal_code_' . $country . '_postal_code_form';
        $instance_value['widget_type'] = $widget_type;
        $instance_value['widget']['type'] = $widget_type;
        $instance_value['widget']['module'] = 'postal_code';

      }

      break;
  }
}

/**
 * Implement this hook to alter individual data records as they are migrated.
 *
 * @param $record
 *  The data record, as read by _content_migrate_batch_process_migrate_data().
 *  If the ordering of the D6 and D7 field columns remain the same, no action
 *  is required. If the columns were re-ordered or the data format was changed,
 *  $record should be modified to fit the new field definition.
 * @param $field
 *
 */
function postal_code_content_migrate_data_record_alter(&$record, $field, $instance) {

  switch($field['type']) {
    case 'postal_code':
      $record[$field['field_name'] . '_postal'] = $record[$field['field_name']. '_value'];
      break;
  }
}
