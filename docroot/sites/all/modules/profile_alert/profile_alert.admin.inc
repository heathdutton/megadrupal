<?php
/**
 * @file
 * Administration page callbacks for the Profile Alert module.
 */

/**
 * Implements hook_form().
 *
 * Module configuration form.
 */
function profile_alert_configuration_form($form, &$form_state) {

  $options = array();
  $fields_info = field_info_instances('user', 'user');
  foreach ($fields_info as $field_info) {
    $options[$field_info['field_name']] = $field_info['label'];
  }

  if (!empty($options)) {
    $form['profile_alert_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('User profile fields'),
      '#options' => $options,
      '#default_value' => variable_get('profile_alert_fields', array()),
      '#description' => t('If one of this fields are empty in user profile alert appear in custom module block.'),
    );

    // Filter checkbox values array before save.
    $form['array_filter'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );

    return system_settings_form($form);
  }
  else {
    $form['profile_alert_fields'] = array(
      '#type' => 'item',
      '#markup' => t('You have no custom fields defined in user profile. <a href="@add-fields">Add some</a> before continue in <em>Profile Alert</em> configuration.', array('@add-fields' => url('admin/config/people/accounts/fields'))),
    );

    return $form;
  }
}
