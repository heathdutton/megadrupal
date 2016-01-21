<?php

/**
 * @file
 * Administrative forms and functions for CCK Sign-up Restrictions module.
 */

/**
 * Helper implementation of hook_form_FORM_ID_alter().
 */
function _cck_signup_restrictions_form_node_type_form_alter(&$form, $form_state) {
  $type = $form['old_type']['#value'];
  if ($type) {
    $options = _cck_signup_restrictions_text_fields($type);
    array_unshift($options, t('-- Choose --'));
    $form['cck_signup']['cck_signup_restrictions_field'] = array(
      '#type' => 'select',
      '#title' => t('Sign-up restrictions field'),
      '#description' => t('Field should use key/value pairs where the key is the field name that is attached to the sign-up node.'),
      '#options' => $options,
      '#default_value' => variable_get('cck_signup_restrictions_field_' . $type, 0),
    );
  }
}

/**
 * Get text fields associated with node $type.
 */
function _cck_signup_restrictions_text_fields($type) {
  $options = array();
  $fields = field_info_fields();
  foreach ($fields as $field_name => $field) {
    // Get field/bundle info.
    $instance = field_info_instance('node', $field_name, $type);
    if (empty($instance)) {
      continue;
    }
    if (in_array($field['type'], array('list_text'))) {
      $options[$field['field_name']] = $instance['label'];
    }
  }
  return $options;
}
