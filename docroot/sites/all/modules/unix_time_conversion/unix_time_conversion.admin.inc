<?php

/**
 * @file
 * Contains code for admin settings form.
 */

/**
 * Form constructor for admin settings.
 *
 * @ingroup forms
 */
function unix_time_conversion_output_configuration_form($form, &$form_state) {
  // Include the helper functions file.
  module_load_include('inc', 'unix_time_conversion', 'unix_time_conversion.helper_functions');
  // Empty rorm array.
  $form = array();
  /*
   * Timestamp To Date.
   */
  $form['unix_time_conversion_time_to_date'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timestamp To Date'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Timestamp field title.
  $form['unix_time_conversion_time_to_date']['unix_time_conversion_timestamp_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Timestamp Field Title'),
    '#description' => t('Will serve as field title for input timestamp field. Ex: Timestamp.'),
    '#required' => TRUE,
    '#default_value' => variable_get('unix_time_conversion_timestamp_field_title', 'Timestamp'),
  );
  // Timestamp field description.
  $form['unix_time_conversion_time_to_date']['unix_time_conversion_timestamp_field_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Timestamp Field Description'),
    '#description' => t('Will serve as field description for input timestamp field.'),
    '#default_value' => variable_get('unix_time_conversion_timestamp_field_description', 'Ex: ' . time()),
  );
  // Output format.
  $form['unix_time_conversion_time_to_date']['unix_time_conversion_time_to_date_output_format'] = array(
    '#title' => t('Date Output Format'),
    '#type' => 'textfield',
    '#description' => t('A user-defined date format. See the !phpmanual for available options.', array(
      '!phpmanual' => l(t('php manual'), 'http://php.net/manual/en/function.date.php', array('attributes' => array('target' => '_blank'))),
        )
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('unix_time_conversion_time_to_date_output_format', 'l F-d-Y h:i:s A'),
  );
  /*
   * Date To Timestamp.
   */
  $form['unix_time_conversion_date_to_time'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date To Timestamp'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Date field title.
  $form['unix_time_conversion_date_to_time']['unix_time_conversion_date_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Field Title'),
    '#description' => t('Will serve as field title for input date field. Ex: Date.'),
    '#required' => TRUE,
    '#default_value' => variable_get('unix_time_conversion_date_field_title', 'Date'),
  );
  // Time field title.
  $form['unix_time_conversion_date_to_time']['unix_time_conversion_time_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Field Title'),
    '#description' => t('Will serve as field title for input time field. Ex: Time.'),
    '#required' => TRUE,
    '#default_value' => variable_get('unix_time_conversion_time_field_title', 'Time'),
  );
  // Return the form.
  return system_settings_form($form);
}
