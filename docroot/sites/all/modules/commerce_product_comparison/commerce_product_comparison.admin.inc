<?php

/**
 * @file
 * Admin page callbacks for commerce_product_comparison module.
 */

/**
 * Build a setting form for the compare table.
 */
function commerce_product_comparison_config_form($form, &$form_state) {
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Compare page'),
  );
  $form['fieldset']['commerce_product_comparison_filter_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable filters in compare page.'),
    '#default_value' => variable_get('commerce_product_comparison_filter_enabled', 1),
  );
  return system_settings_form($form);
}


/**
 * Build a form to make two different fields comparable with each other.
 */
function commerce_product_comparison_set_fields_form($form, &$form_state) {
  $j = 1;
  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;

  // Get all field of the entity commerce_product.
  $commerce_product_fields = array('' => 'Select a field');
  foreach (field_read_instances(array('entity_type' => 'commerce_product')) as $field) {
    if (!empty($field['field_name'])) {
      $commerce_product_fields[$field['field_name']] = $field['label'] . ' (' . $field['bundle'] . ' - ' . $field['field_name'] . ')';
    }
  }
  // Retreive each combination that has been saved.
  foreach (variable_get('commerce_product_comparison_fields', array()) as $field_one => $field_two) {
    // When we save a combination we save field1 => field2 and field2 => field1,
    // so its easier when rendering comparison table.
    // So display only one time the combination.
    if ($j % 2 == 1) {
      $form['fields'][$field_one] = array(
        '#type' => 'fieldset',
        '#title' => t('Couple of field'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#attributes' => array(
          'id' => 'remove_' . $field_one,
        ),
      );
      $form['fields'][$field_one]['field_one'] = array(
        '#type' => 'select',
        '#options' => $commerce_product_fields,
        '#value' => $field_one,
        '#title' => t('Choose a field'),
        '#description' => t("Enter the field you want to have an equivalent for."),
        '#ajax' => array(
          'callback' => 'commerce_product_comparison_set_fields_update_field',
          'wrapper' => $field_one,
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      // Get matching fields for the field one.
      $matching_fields = commerce_product_comparison_get_second_field($field_one);
      $form['fields'][$field_one]['field_two'] = array(
        '#type' => 'select',
        '#options' => $matching_fields,
        '#value' => $field_two,
        '#title' => t('Choose a field'),
        '#description' => t("Choose the equivalent field."),
        '#prefix' => '<div id="' . $field_one . '">',
        '#suffix' => '</div>',
      );
      // Remove field.
      $form['fields'][$field_one]['remove_field'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#name' => $field_one,
        '#ajax' => array(
          'callback' => 'commerce_product_comparison_set_fields_remove_field',
          'wrapper' => 'remove_' . $field_one,
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
    }
    $j++;
  }
  if (empty($form_state['num_fields'])) {
    $form_state['num_fields'] = 1;
  }
  // Build the number of name fieldsets indicated by $form_state['num_fields']
  for ($i = 1; $i <= $form_state['num_fields']; $i++) {
    $form['fields']['field_' . $i] = array(
      '#type' => 'fieldset',
      '#title' => t('Couple of field'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array(
        'id' => 'remove_field_' . $i,
      ),
    );
    $form['fields']['field_' . $i]['field_one'] = array(
      '#type' => 'select',
      '#options' => $commerce_product_fields,
      '#default_value' => '',
      '#title' => t('Choose a field'),
      '#description' => t("Enter the field you want to have an equivalent for."),
      '#ajax' => array(
        'callback' => 'commerce_product_comparison_set_fields_update_field',
        'wrapper' => 'field_' . $i,
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
    $form['fields']['field_' . $i]['field_two'] = array(
      '#type' => 'select',
      '#options' => $commerce_product_fields,
      '#title' => t('Choose a field'),
      '#description' => t("Choose the equivalent field."),
      '#prefix' => '<div id="field_' . $i . '">',
      '#suffix' => '</div>',
    );
    // Remove field.
    $form['fields']['field_' . $i]['remove_field'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => 'field_' . $i,
      '#ajax' => array(
        'callback' => 'commerce_product_comparison_set_fields_remove_field',
        'wrapper' => 'remove_field_' . $i,
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }
  // Submit the combination(s).
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  // Adds "Add another combination" button.
  $form['add_field'] = array(
    '#type' => 'submit',
    '#value' => t('Add a new combination'),
    '#submit' => array('commerce_product_comparison_set_fields_add_field'),
  );
  // Helper.
  $form['help'] = array(
    '#markup' => '<p>' . t('When you install this module, it only compares fields which have the exact same machin-name. If you want to compare different fields with each other then set the combination here.') . '</p>',
  );
  return $form;
}

/**
 * Add another combination of field to the form.
 */
function commerce_product_comparison_set_fields_add_field($form, &$form_state) {
  // Everything in $form_state is persistent, so we'll just use
  // $form_state['num_fields']
  $form_state['num_fields']++;

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

/**
 * Delete a combination of field from the form.
 */
function commerce_product_comparison_set_fields_remove_field($form, &$form_state) {
  $triggering_element = $form_state['triggering_element']['#name'];
  if ($form_state['values']['fields'][$triggering_element]['field_one'] != "" && $form_state['values']['fields'][$triggering_element]['field_two'] != "") {
    $saved_fields = variable_get('commerce_product_comparison_fields', array());
    unset($saved_fields[$form_state['values']['fields'][$triggering_element]['field_one']]);
    unset($saved_fields[$form_state['values']['fields'][$triggering_element]['field_two']]);
    variable_set('commerce_product_comparison_fields', $saved_fields);
  }
  return '';
}

/**
 * Save the different combinations of field.
 */
function commerce_product_comparison_set_fields_form_submit($form, $form_state) {
  $saved_fields = variable_get('commerce_product_comparison_fields', array());
  foreach ($form_state['values']['fields'] as $fields) {
    if (!empty($fields['field_one']) && !empty($fields['field_two'])) {
      $saved_fields[$fields['field_one']] = $fields['field_two'];
      $saved_fields[$fields['field_two']] = $fields['field_one'];
    }
  }
  variable_set('commerce_product_comparison_fields', $saved_fields);
}

/**
 * Update available fields in the second select list.
 */
function commerce_product_comparison_set_fields_update_field(&$form, &$form_state) {
  $form_field = $form_state['triggering_element']['#ajax']['wrapper'];
  $commerce_product_fields = commerce_product_comparison_get_second_field($form_state['values']['fields'][$form_field]['field_one']);
  if (count($commerce_product_fields) < 2) {
    $form['fields'][$form_field]['field_two']['#description'] = t('This field is attached to every bundles, you can not match it with another one');
  }
  $form['fields'][$form_field]['field_two']['#options'] = $commerce_product_fields;

  return $form['fields'][$form_field]['field_two'];
}

/**
 * Get possible matching fields for a field.
 *
 * @param string $field_one
 *   The first field we have to find matches for.
 *
 * @return array
 *   The array of matching fields.
 */
function commerce_product_comparison_get_second_field($field_one) {
  $field_info = field_info_field($field_one);
  $commerce_product_fields = array('' => 'Select a field');
  if (!empty($field_info['bundles']['commerce_product'])) {
    // Get all bundles the field is attached to.
    $bundles = array();
    foreach ($field_info['bundles']['commerce_product'] as $bundle) {
      $bundles[] = $bundle;
    }
    // Get all fields not attached to the same bundle of commerce product.
    foreach (field_read_instances(array('entity_type' => 'commerce_product')) as $field) {
      if (!in_array($field['bundle'], $bundles)) {
        $commerce_product_fields[$field['field_name']] = $field['label'] . ' (' . $field['bundle'] . ' - ' . $field['field_name'] . ')';
      }
    }
  }
  return $commerce_product_fields;
}
