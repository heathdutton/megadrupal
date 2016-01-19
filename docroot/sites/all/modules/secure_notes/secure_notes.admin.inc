<?php

/**
 * @file
 * Provide the admin related functions for Secure Notes.
 */

/**
 * Defines the Secure Notes admin settings form.
 */
function secure_notes_admin_settings_form() {
  // Make sure the secure note type hasn't been deleted.
  $default = secure_notes_get_types();
  $form = array();

  $form['secure_notes_types'] = array(
    '#title' => t('The content types that are allowed to be viewed or created as secure notes.'),
    '#description' => t('Only specified node types will be allowed to be accessed or created using secure notes.'),
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => $default,
  );

  $form['secure_notes_types_original'] = array(
    '#type' => 'value',
    '#value' => $default,
  );

  $default_expire = variable_get('secure_notes_default_expiration', 48);
  $form['secure_notes_default_expiration'] = array(
    '#title' => t('Token expiration'),
    '#description' => t('Secure note tokens will expire if not used after this amount of time.'),
    '#type' => 'select',
    '#options' => array(
      1 => t('1 hour'),
      2 => t('2 hours'),
      4 => t('4 hours'),
      8 => t('8 hours'),
      12 => t('12 hours'),
      24 => t('24 hours (1 day)'),
      48 => t('48 hours (2 days)'),
      72 => t('72 hours (3 days)'),
      120 => t('120 hours (5 days)'),
      168 => t('168 hours (1 week)'),
    ),
    '#default_value' => $default_expire,
  );

  $default_add_message = secure_notes_default_email_message('add');

  $default_view_message = secure_notes_default_email_message('view');

  $form['secure_notes_default_message_add'] = array(
    '#title' => t('Default Email Message for Adding a Note'),
    '#description' => t('The default message that will be prefilled when creating a new secure note invitation to add content. Be sure to include the [invite-token]. You can also add the [username] token.'),
    '#type' => 'textarea',
    '#default_value' => check_plain(variable_get('secure_notes_default_message_add', $default_add_message)),
  );

  $form['secure_notes_default_message_view'] = array(
    '#title' => t('Default Email Message for Viewing a Note'),
    '#description' => t('The default message that will be prefilled when creating a new secure note invitation to view content. Be sure to include the [invite-token]. You can also add the [username] token.'),
    '#type' => 'textarea',
    '#default_value' => check_plain(variable_get('secure_notes_default_message_view', $default_view_message)),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'secure_notes_secure_notes_admin_settings_additional_submit';
  return $form;
}

/**
 * Additional submission handler for secure_notes_admin_settings_form(). Flag
 * the access permissions for rebuild if and only if the list of allowed secure
 * notes types has changed.
 *
 * @see secure_notes_admin_settings_form()
 */
function secure_notes_secure_notes_admin_settings_additional_submit($form, &$form_state) {
  $diff = array_diff_assoc($form_state['values']['secure_notes_types'], $form_state['values']['secure_notes_types_original']);
  if (!empty($diff)) {
    // Rebuild node access database.
    node_access_needs_rebuild(TRUE);
  }
}
