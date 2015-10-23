<?php

/**
 * Module settings form.
 */
function apachesolr_localfiles_settings_form($form, &$form_state) {

  $t = get_t();

  $form['#tree'] = TRUE;

  $stored_localfile_folders = variable_get('apachesolr_localfiles_folder', array());

  if (!isset($form_state['num_localfile_folders'])) {
    $stored_localfile_folders_size = count($stored_localfile_folders);
    $form_state['num_localfile_folders'] = $stored_localfile_folders_size > 0 ? $stored_localfile_folders_size : 1;
  }

  for ($i = 0; $i < $form_state['num_localfile_folders']; $i++) {

    $default_value_folder_from = isset($stored_localfile_folders[$i]) ? $stored_localfile_folders[$i]['from'] : NULL;
    $form['apachesolr_localfiles_folder'][$i]['folders-mapping'] = array(
      '#type' => 'fieldset',
      '#title' => t('Folders mapping @number', array('@number' => ($i + 1))),
    );
    $form['apachesolr_localfiles_folder'][$i]['folders-mapping']['from'] = array(
      '#type' => 'textfield',
      '#title' => t('Server-side mapping'),
      '#description' => t('Type a server-side folder (complete path) whose files should be indexed by Solr.'),
      '#default_value' => $default_value_folder_from,
      '#required' => TRUE,
    );

    $default_value_folder_to = isset($stored_localfile_folders[$i]) ? $stored_localfile_folders[$i]['to'] : NULL;
    $form['apachesolr_localfiles_folder'][$i]['folders-mapping']['to'] = array(
      '#type' => 'textfield',
      '#title' => t('Client-side mapping'),
      '#description' => t('Type a client-side folder (complete path) which should be an alias to the server-side folder.'),
      '#default_value' => $default_value_folder_to,
      '#required' => TRUE,
    );
  }

  // Adds "Add another folder" button
  $form['add_folder'] = array(
    '#type' => 'submit',
    '#value' => t('Add another folder'),
    '#submit' => array('apachesolr_localfiles_settings_form_add_folder'),
  );

  // If we have more than one folder, this button allows removal of the
  // last folder.
  if ($form_state['num_localfile_folders'] > 1) {
    $form['remove_folder'] = array(
      '#type' => 'submit',
      '#value' => 'Remove the latest folder',
      '#submit' => array('apachesolr_localfiles_settings_form_remove_folder'),
      // Since we are removing a name, don't validate until later.
      '#limit_validation_errors' => array(),
    );
  }

  $file_extensions_to_be_searched = array(
    'doc',
    'docx',
    'odt',
    'pdf'
  );
  $options = drupal_map_assoc($file_extensions_to_be_searched);
  $form['apachesolr_localfiles_mimetypes_allowed'] = array(
    '#type' => 'checkboxes',
    '#title' => $t('Which filetypes should be indexed?'),
    '#description' => $t('The types specified here will be used as filters in the search for files to be indexed.'),
    '#options' => $options,
    '#default_value' => variable_get('apachesolr_localfiles_mimetypes_allowed', $options),
    '#required' => TRUE,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

function apachesolr_localfiles_settings_form_add_folder($form, &$form_state) {
  $form_state['num_localfile_folders']++;

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

function apachesolr_localfiles_settings_form_remove_folder($form, &$form_state) {
  if ($form_state['num_localfile_folders'] > 1) {
    $form_state['num_localfile_folders']--;
  }
  drupal_set_message(t('The folders mapping changes will have effect only after you click on') . ' ' . t('Save configuration') . t(' button.'), 'warning');
  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

function apachesolr_localfiles_settings_form_validate($form, &$form_state) {

  $clicked_button = isset($form_state['clicked_button']['#submit'][0]) ? $form_state['clicked_button']['#submit'][0] : NULL;
  if ('apachesolr_localfiles_settings_form_add_folder' == $clicked_button
    || 'apachesolr_localfiles_settings_form_remove_folder' == $clicked_button) {
    return;
  }

  for ($i = 0; $i < $form_state['num_localfile_folders']; $i++) {

    $localfiles_folder_from = $form_state['values']['apachesolr_localfiles_folder'][$i]['folders-mapping']['from'];
    if (!is_dir($localfiles_folder_from)) {
      form_set_error('apachesolr_localfiles_folder', t('Type a valid directory to the server-side folder') . ' ' . ($i + 1));
    }
  }
}

function apachesolr_localfiles_settings_form_submit($form, &$form_state) {

  $apachesolr_localfiles_folder = array();
  for ($i = 0; $i < $form_state['num_localfile_folders']; $i++) {

    $apachesolr_localfiles_folder[$i] = array();

    $folder_from = $form_state['values']['apachesolr_localfiles_folder'][$i]['folders-mapping']['from'];
    if (substr($folder_from, -1) == '/') {//removing the last char if it's '/'
      $folder_from = substr_replace($folder_from, '', -1);
      $form_state['values']['apachesolr_localfiles_folder'][$i]['folders-mapping']['from'] = $folder_from;
    }
    $apachesolr_localfiles_folder[$i]['from'] = $folder_from;


    $folder_to = $form_state['values']['apachesolr_localfiles_folder'][$i]['folders-mapping']['to'];
    if (substr($folder_to, -1) == '/') {//removing the last char if it's '/'
      $folder_to = substr_replace($folder_to, '', -1);
      $form_state['values']['apachesolr_localfiles_folder'][$i]['folders-mapping']['to'] = $folder_to;
    }
    $apachesolr_localfiles_folder[$i]['to'] = $folder_to;
  }

  variable_set('apachesolr_localfiles_folder', $apachesolr_localfiles_folder);
  variable_set('apachesolr_localfiles_mimetypes_allowed', $form_state['values']['apachesolr_localfiles_mimetypes_allowed']);
}