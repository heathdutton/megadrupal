<?php

/**
 * @file
 * Forms and submit handlers for CCK sign-up group confirmation module.
 */

/**
 * Helper implementation of hook_form_FORM_ID_alter().
 */
function _cck_signup_group_confirmation_form_node_type_form_alter(&$form, $form_state) {
  $type = $form['old_type']['#value'];
  if ($type) {
    $options = _cck_signup_node_reference_fields($type);
    array_unshift($options, t('-- Choose --'));
    $form['cck_signup']['cck_signup_group']['cck_signup_group_confirmation_field'] = array(
      '#type' => 'select',
      '#default_value' => variable_get('cck_signup_group_confirmation_field_' . $type, 0),
      '#title' => t('Group Confirmation type'),
      '#description' => t('Select a node reference field to attach a confirmation type to this group sign-up.'),
      '#options' => $options,
    );

    // Set submit handler.
    if (!in_array('cck_signup_group_confirmation_form_node_type_form_submit', $form['#submit'])) {
      $form['#submit'][] = 'cck_signup_group_confirmation_form_node_type_form_submit';
    }
  }
}

/**
 * Set the group confirmation node type.
 */
function cck_signup_group_confirmation_form_node_type_form_submit($form, &$form_state) {
  if ($form_state['values']['cck_signup_group_confirmation_field']) {
    $type_info = _content_type_info();
    foreach ($type_info['content types'] as $type_name => $type) {
      if (in_array($form_state['values']['cck_signup_group_confirmation_field'], array_keys($type['fields']))) {
        variable_set('cck_signup_group_confirmation_type_' . $type_name, $form_state['values']['cck_signup_group_confirmation_field']);
      }
    }
  }
}
