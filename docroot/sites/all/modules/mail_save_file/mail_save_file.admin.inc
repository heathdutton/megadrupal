<?php
/**
 * @file
 * Administrative pages for Mail save file.
 */

/**
 * Form constructor for the administration form.
 */
function mail_save_file_admin_form($form, &$form_state) {

  $form['mail_save_file_enabled'] = array(
    '#title' => t('Enable saving emails to file'),
    '#type' => 'checkbox',
    '#description' => t('When turned on, all emails will be saved to file by default.'),
    '#default_value' => variable_get('mail_save_file_enabled', FALSE),
  );

  $form['mail_save_file_warning'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('messages', 'warning'),
    ),
    'msg' => array(
      '#markup' => t('Warning: Emails may contain sensitive information, please ensure you save them to a secure location, and control who can gain access to these files.'),
    ),
    '#states' => array(
      'visible' => array(
        '#edit-mail-save-file-enabled' => array('checked' => TRUE),
      ),
    ),
  );

  $form['mail_save_file_name'] = array(
    '#title' => t('File name format'),
    '#type' => 'textfield',
    '#maxlength' => 256,
    '#description' => t('Any global token may be used in this field (see replacements below). Additional tokens will be available in accordance with whatever data is passed into drupal_mail(). Directory separators may be included here, in which case mail_save_file will attempt to auto-create the sub-directory at the time of saving the file. This can be used as a mechanism for distributing many files over multiple directories to overcome OS limits. The directory must resolve to a sub-directory of <em>File directory</em> (below).'),
    '#default_value' => variable_get('mail_save_file_name', '[message:id]-[current-date:raw].txt'),
  );

  if (module_exists('token')) {
    $form['token_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Available tokens'),
      '#collapsable' => FALSE,
      '#collapsed' => TRUE,
      'token_tree_wrapper' => array(
        '#type' => 'container',
        'token_tree' => array(
          '#theme' => 'token_tree',
          '#token_types' => array(),
          '#show_restricted' => FALSE,
          '#weight' => 10,
          '#dialog' => TRUE,
        ),
      ),
      'additional_tokens' => array(
        '#type' => 'item',
        '#title' => t('The following tokens are additionally supported'),
        '#markup' => theme('item_list', array(
          'items' => array(
            t('<code>[message:mapped-message-id]</code>This will resolve to the result of the mapping configured below in <em>Message ID / File-name mapping</em>, or simply the original message ID if absent from the mapping.'),
          ),
        )),
      ),
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available tokens.', array('@drupal-token' => 'https://www.drupal.org/project/token')) . '</p>',
    );
  }

  $form['mail_save_file_message_id_to_name_mapping'] = array(
    '#title' => t('Message ID mapping'),
    '#type' => 'textarea',
    '#description' => t('To fully customize the name of the files, a custom mapping may be provided here, one for each message ID. Separate each mapping by a new-line, and from/to values should be pipe-delimited. For example: <pre>user_confirmation|NewUser</pre>.'),
    '#default_value' => mail_save_file_associative_array_to_pipe_delimited_string(variable_get('mail_save_file_message_id_to_name_mapping', array())),
    '#element_validate' => array('mail_save_file_element_validate_string_to_associative_array'),
  );

  $form['mail_save_file_directory'] = array(
    '#title' => t('File directory'),
    '#type' => 'textfield',
    '#description' => t('The directory to store all emails under. This may not contain tokens.'),
    '#default_value' => variable_get('mail_save_file_directory', 'private://emails'),
    '#after_build' => array('system_check_directory'),
  );

  $form['mail_save_file_whitelist'] = array(
    '#title' => t('Whitelist'),
    '#type' => 'textarea',
    '#description' => t('Separate each message_id by a new-line. If specified, only these emails will be saved. If empty, all files will be saved (except those in the blacklist below).'),
    '#default_value' => implode("\n", variable_get('mail_save_file_whitelist', array())),
    '#element_validate' => array('mail_save_file_element_validate_string_to_array'),
  );

  $form['mail_save_file_blacklist'] = array(
    '#title' => t('Blacklist'),
    '#type' => 'textarea',
    '#description' => t('Separate each message_id by a new-line. Any email specified here will never be saved, even if it appears in the whitelist above.'),
    '#default_value' => implode("\n", variable_get('mail_save_file_blacklist', array())),
    '#element_validate' => array('mail_save_file_element_validate_string_to_array'),
  );

  return system_settings_form($form);
}

/**
 * Element validate to change a newline separated string value to an array.
 */
function mail_save_file_element_validate_string_to_array($element, &$form_state) {
  $value = array();
  if (!empty($element['#value'])) {
    $value = array_filter(array_map('trim', explode("\n", $element['#value'])));
  }
  form_set_value($element, $value, $form_state);
  $element['#value'] = $value;
}

/**
 * Converts an associative array into a pipe-delimited string.
 *
 * @param array $array
 *   The associative array to convert.
 *
 * @return string
 *  A string suitable for the value of a textarea form element.
 */
function mail_save_file_associative_array_to_pipe_delimited_string($array) {
  $lines = array();
  foreach ($array as $key => $value) {
    $lines[] = "$key|$value";
  }
  return implode("\n", $lines);
}

/**
 * Element validate to change a newline separated string value to a hash map.
 *
 * We expect a value of the form:
 *
 *     key|value
 *     key2|value2
 */
function mail_save_file_element_validate_string_to_associative_array($element, &$form_state) {
  $value = array();
  if (!empty($element['#value'])) {
    $rows = array_filter(array_map('trim', explode("\n", $element['#value'])));
    foreach ($rows as $row_i => $row) {
      $row_array = explode('|', $row);
      if (count($row_array) !== 2) {
        form_error($element, t('!name should contain valid key|value pairs. Row !row_number should contain exactly one pipe character.', array('!name' => $element['#title'], '!row_number' => $row_i + 1)));
        return;
      }
      list($row_key, $row_value) = array_map('trim', $row_array);
      if ($row_key === '') {
        form_error($element, t('!name should contain valid key|value pairs. Row !row_number must have a non-empty key.', array('!name' => $element['#title'], '!row_number' => $row_i + 1)));
        return;
      }
      $value[$row_key] = $row_value;
    }
  }
  form_set_value($element, $value, $form_state);
  $element['#value'] = $value;
}
