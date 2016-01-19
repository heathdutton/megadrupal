<?php

function resource_booking_settings_form($form, &$form_state) {
  $form['resource_booking_resource_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Resource Date Field'),
    '#description' => t('Enter the date field machine name in your resource content type e.g. field_bookings.'),
    '#default_value' => variable_get('resource_booking_resource_field', ''),
  );
  $form['resource_booking_resource_regen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update bookings'),
    '#description' => t('Select this if you are migrating from 7.x-1.0 or if you want to update all the resource bookings.'),
  );
  $rdate = variable_get('resource_booking_resource_field', '');
  if (!empty($rdate)) {
    //check if such a the table exists
    if (!db_table_exists('field_data_' . $rdate)) {
      variable_set('resource_booking_resource_field', '');
      form_set_error('resource_booking_resource_field', t('Resource Date Field is invalid. Check the date field\'s machine name.'));
    }
  }
  if (variable_get('resource_booking_resource_regen', FALSE)) {
    variable_set('resource_booking_resource_regen', FALSE);
    if (empty($rdate)) {
      form_set_error('resource_booking_resource_field', t('Resource Date Field has not been specified.'));
    }
    else {
      $fields = variable_get('resource_booking_fields', array());
      foreach ($fields as $field_name) {
        _resource_booking_recreate_entries($field_name, NULL, array());          
      }
    }
  }
  
  return system_settings_form($form);
}
