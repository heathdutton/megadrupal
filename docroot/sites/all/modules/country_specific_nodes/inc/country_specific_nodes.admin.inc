<?php

/**
 * @file
 * Country specific entities admin section.
 */

/**
 * Form creation to enable or disable content type selection.
 */
function country_specific_nodes_content_type($form, &$form_state) {
  $form = array();

  // Fieldset for our data.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of option available where user can enable country specific content display'),
  );

  // Get list of active content types.
  $types = node_type_get_names();

  // Get include content types.
  $include_list_string = variable_get('country_specific_nodes_include_list', '');
  $include_list = explode(',', $include_list_string);

  // Check if field is attached to content types.
  foreach ($types as $key => $type_val) {
    $has_instance = field_info_instance('node', 'field_countries_cce', $key);
    $field_exist[] = isset($has_instance['bundle']) ? $has_instance['bundle'] : '';
    if (!empty($include_list)) {
      $field_include_exist[] = in_array($key, $include_list) ? $key : '';
    }
  }

  // List cotnent types with checkboxes.
  $form['options']['content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#options' => $types,
    '#default_value' => $field_exist,
  );

  // List cotnent types with checkboxes to invert modules functionality.
  $form['options']['include'] = array(
    '#type' => 'checkboxes',
    '#title' => t("Invert Functionality (Select nodes type's for which you want to invert functionality)"),
    '#options' => $types,
    '#default_value' => $field_include_exist,
    '#description' => t('Select the content types for which the nodes must be made visible for selected countries.'),
  );

  // Submit button.
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Markup text to set a message for user.
  $form['options']['note'] = array(
    '#markup' => '<div><b>' . t('Select content type to attach field and uncheck it to remove field from that particular content type.') . '</b></div><br><div><b>' . t('Invert Functionality : Select content type for which the nodes must be made visible for selected countries.') . '</b></div>',
  );
  return $form;
}

/**
 * Submit Select and enable content type for CSN.
 */
function country_specific_nodes_content_type_submit($form, &$form_state) {
  // Get selected/non-selected content types.
  $selected_types = $form_state['values']['content_types'];

  // Save the content types for which the functionality needs to be inverted.
  $selected_include_types_array = $form_state['values']['include'];
  $selected_include_types_string = implode(",", $selected_include_types_array);
  variable_set('country_specific_nodes_include_list', $selected_include_types_string);

  foreach ($selected_types as $key => $type_val) {
    // Check for field is already attached to content type.
    $has_instance = field_info_instance('node', 'field_countries_cce', $key);

    // Check which content types have been selected by user.
    if ($type_val) {
      if (!is_array($has_instance)) {
        // Creating instance of the field.
        $country_instance = array(
          'field_name' => 'field_countries_cce',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Countries'),
          'description' => t('Select countries to filter the node based on user country.'),
          'default_value' => NULL,
          'display' => array(
            'default' => array(
              'label' => 'above',
              'module' => 'list',
              'settings' => array(),
              'type' => 'list_default',
              'weight' => -1,
            ),
            'teaser' => array(
              'label' => 'above',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => -1,
            ),
          ),
          'required' => 0,
          'settings' => array(
            'allowed_values_function' => 'country_get_list',
          ),
          'widget' => array(
            'active' => 1,
            'module' => 'options',
            'settings' => array('allowed_values_function' => 'country_get_list'),
            'type' => 'options_select',
            'weight' => '-1',
          ),
        );

        // Check if field already exits, if not create it.
        if (!field_info_field('field_countries_cce')) {
          // Ok, now lets create a field for our country selection drop down.
          $country_field = array(
            'field_name' => 'field_countries_cce',
            'type' => 'list_text',
            'cardinality' => -1,
            'settings' => array(
              'label' => t('Countries'),
              'description' => t('Provides country list to be included/excluded for nodes.'),
              'type' => 'list_text',
              'default_widget' => 'options_select',
              'allowed_values_function' => 'country_get_list',
            ),
          );

          // Create a field.
          field_create_field($country_field);
        }
        // Create field instance.
        field_create_instance($country_instance);
      }
    }
    else {
      if (is_array($has_instance)) {
        // Creating instance of the field.
        $country_instance_del = array(
          'field_name' => 'field_countries_cce',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Countries'),
          'description' => t('Select countries to filter the node based on user country.'),
          'default_value' => NULL,
          'display' => array(
            'default' => array(
              'label' => 'above',
              'module' => 'list',
              'settings' => array(),
              'type' => 'list_default',
              'weight' => -1,
            ),
            'teaser' => array(
              'label' => 'above',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => -1,
            ),
          ),
          'required' => 0,
          'settings' => array(
            'allowed_values_function' => 'country_get_list',
          ),
          'widget' => array(
            'active' => 1,
            'module' => 'options',
            'settings' => array('allowed_values_function' => 'country_get_list'),
            'type' => 'options_select',
            'weight' => '-1',
          ),
        );

        // Delete field for unchecked content types.
        field_delete_instance($country_instance_del);
      }
    }
  } // End of Foreach.
  // Run field cron only if field created/deleted.
  if (!empty($country_instance) || !empty($country_instance_del)) {
    // Run field cron to cleanup tables.
    field_cron();
  }

  drupal_set_message(t('Settings have been successfully saved.'));
}

/**
 * Callback function for setting default country.
 */
function country_specific_nodes_default_country() {
  $form = array();

  $form['country_specific_nodes_def_cn'] = array(
    '#title' => t('Default Country Code'),
    '#description' => t('Please select the default country for the nodes.'),
    '#type' => 'select',
    '#options' => country_get_list(),
    '#default_value' => variable_get('country_specific_nodes_def_cn', ''),
  );

  return system_settings_form($form);
}
