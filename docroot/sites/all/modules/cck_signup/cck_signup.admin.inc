<?php
/**
 * @file
 * Admin forms and callbacks for the CCK Sign-up module.
 */

/**
 * Validation callback for node_type_form.
 */
function cck_signup_form_node_type_form_validate($form, &$form_state) {
  if ($form_state['values']['cck_signup_type'] && !$form_state['values']['cck_signup_field']) {
    form_set_error('cck_signup_field', t('Must select a reference field if sign-ups are enabled for this node type.'));
  }
}

/**
 * Set the signup-node type.
 */
function cck_signup_form_node_type_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['cck_signup_field'])) {
    $field = field_info_field($form_state['values']['cck_signup_field']);
    if (!empty($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type_name) {
        variable_set('cck_signup_signup_type_' . $type_name, $form_state['values']['cck_signup_field']);
      }
    }
  }
}

/**
 * Get nodereference fields pointing back at $type.
 */
function _cck_signup_node_reference_fields($type) {
  $options = array();
  $fields = field_info_fields();
  foreach ($fields as $field) {
    if ($field['type'] == 'node_reference'
       && !empty($field['settings']['referenceable_types'])
       && in_array($type, $field['settings']['referenceable_types'])) {
      // @TODO get bundle info for proper label.
      $options[$field['field_name']] = $field['field_name'];
    }
  }
  return $options;
}

/**
 * Get date fields associated with node $type.
 *
 * @param string $type
 *   The node type to retrieve date fields for.
 *
 * @return array
 *   An array of numeric fields, keyed by machine name.
 */
function _cck_signup_date_fields($type) {
  $options = array();
  if (module_exists('date')) {
    $fields = field_info_fields();
    foreach ($fields as $field_name => $field) {
      // Get field/bundle info.
      $instance = field_info_instance('node', $field_name, $type);
      if (empty($instance)) {
        continue;
      }
      if (in_array($field['type'], array('date', 'datestamp', 'datetime'))) {
        $options[$field['field_name']] = $instance['label'];
      }
    }
  }
  return $options;
}

/**
 * Get integer fields associated with node $type.
 *
 * @param string $type
 *   The node type to retrieve numeric fields for.
 *
 * @return array
 *   And array of numeric fields, keyed by machine name.
 */
function _cck_signup_numeric_fields($type) {
  $options = array();
  $fields = field_info_fields();
  foreach ($fields as $field_name => $field) {
    // Get field/bundle info.
    $instance = field_info_instance('node', $field_name, $type);
    if (empty($instance)) {
      continue;
    }
    if (in_array($field['type'], array('number_integer', 'list_integer'))) {
      $options[$field['field_name']] = $instance['label'];
    }
  }
  return $options;
}

/**
 * Helper implementation of hook_form_FORM_ID_alter().
 */
function _cck_signup_form_node_type_form_alter(&$form, $form_state) {
  $type = $form['old_type']['#value'];
  if ($type) {
    $form['cck_signup'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => !variable_get('cck_signup_type_' . $type, 0),
      '#title' => t('CCK Sign-up'),
      '#group' => 'additional_settings',
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'cck_signup') . '/cck-signup.js'),
      ),
      '#attributes' => array(
        'class' => array('node-form-cck-signup'),
      ),
    );
    $form['cck_signup']['cck_signup_type'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('cck_signup_type_' . $type, 0),
      '#title' => t('Enable Sign-ups for %type nodes', array('%type' => $type)),
    );
    if (($options = _cck_signup_node_reference_fields($type)) && !empty($options)) {
      // Potential node reference fields pointing back at this node type.
      array_unshift($options, t('-- Choose --'));
      $form['cck_signup']['cck_signup_field'] = array(
        '#type' => 'select',
        '#title' => t('Node reference field to associate sign-up nodes'),
        '#options' => $options,
        '#default_value' => variable_get('cck_signup_field_' . $type, 0),
      );
    }
    else {
      // Disable checkbox since no reference fields exist.
      $form['cck_signup']['cck_signup_type']['#disabled'] = TRUE;
      $form['cck_signup']['cck_signup_type']['#description'] = t('In order to use this type as a CCK Sign-up node, there must be a nodereference field from a different type referencing this node type.');
    }

    if (($options = _cck_signup_date_fields($type)) && !empty($options)) {
      // Date fields.
      array_unshift($options, t('-- Choose --'));
      $form['cck_signup']['cck_signup_field_date'] = array(
        '#type' => 'select',
        '#title' => t('Date field to associate with sign-ups'),
        '#options' => $options,
        '#default_value' => variable_get('cck_signup_field_date_' . $type, 0),
      );
    }

    if (($options = _cck_signup_numeric_fields($type)) && !empty($options)) {
      // Potential capacity fields.
      array_unshift($options, t('-- Choose --'));
      $form['cck_signup']['cck_signup_field_capacity'] = array(
        '#type' => 'select',
        '#title' => t('Capacity field to associate with sign-ups'),
        '#options' => $options,
        '#default_value' => variable_get('cck_signup_field_capacity_' . $type, 0),
        '#description' => t('If selected, sign-ups on the event will be closed when the capacity is reached.'),
      );

      // Potential status fields.
      $form['cck_signup']['cck_signup_field_status'] = array(
        '#type' => 'select',
        '#title' => t('Status field to associate with event sign-up status'),
        '#options' => $options,
        '#default_value' => variable_get('cck_signup_field_status_' . $type, 0),
        '#description' => t('Should be an integer field, with allowed values, 0 for closed and 1 for open.'),
      );
    }

    // Potential attendance status fields.
    if (($signup_field = variable_get('cck_signup_field_' . $type, 0))
      && ($types = cck_signup_get_signup_types($signup_field))) {
      // @TODO support multiple types?
      $signup_type = array_pop($types);
      if (($options = _cck_signup_numeric_fields($signup_type))
        && !empty($options)) {
        array_unshift($options, t('-- Choose --'));
        $form['cck_signup']['cck_signup_field_attendance_status'] = array(
          '#type' => 'select',
          '#title' => t('Attendance status to associate with sign-ups'),
          '#options' => $options,
          '#default_value' => variable_get('cck_signup_field_attendance_status', 0),
          '#description' => t('An integer field attached to the sign-up type for confirming attendance (eg, a Yes/No/Maybe field). Anything above 0 will count towards capacity, while 0 or less will not count. In the case of Yes/No/Maybe, the allowed values might look like <pre>1|Yes
0|No
-1|Maybe'),
        );
      }
    }

    // Attach additional validation callback.
    if (!in_array('cck_signup_form_node_type_form_validate', $form['#validate'])) {
      $form['#validate'][] = 'cck_signup_form_node_type_form_validate';
    }
    if (!in_array('cck_signup_form_node_type_form_submit', $form['#submit'])) {
      $form['#submit'][] = 'cck_signup_form_node_type_form_submit';
    }
  }
}
