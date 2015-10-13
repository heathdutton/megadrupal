<?php

/**
 * @file
 * Administrative page callbacks for the pathed_files module.
 */

/**
 * Administration settings form.
 */
function pathed_files_admin_list() {

  $build = array();

  $header = array(
    array('data' => t('ID'), 'field' => 'f.id'),
    array('data' => t('Description'), 'field' => 'f.file_description'),
    array('data' => t('File Name'), 'field' => 'f.file_name'),
    array('data' => t('Path'), 'field' => 'f.file_uri_path'),
    array('data' => t('Enabled'), 'field' => 'f.enabled_flag'),
    array('data' => t('Options'), 'field' => '', 'colspan' => 3),
  );

  $rows = array();

  $query = db_select('pathed_files', 'f')->extend('TableSort');
  $query->fields('f', array(
    'id',
    'file_description',
    'file_name',
    'file_uri_path',
    'file_id',
    'enabled_flag'));
  $result = $query
    ->orderByHeader($header)
    ->execute();

  foreach ($result as $pathed_file) {

    $rows[] = array(
      // Cells. Must be in the correct order to match $headers.
      $pathed_file->id,
      check_plain($pathed_file->file_description),
      check_plain($pathed_file->file_name),
      check_plain($pathed_file->file_uri_path),
      empty($pathed_file->enabled_flag) ? t('No') : t('Yes'),
      array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('View'),
          '#href' => trim($pathed_file->file_uri_path, '/'))),
      array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => 'admin/content/file/pathed-files/' . $pathed_file->id . '/edit')),
      array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Delete'),
          '#href' => 'admin/content/file/pathed-files/' . $pathed_file->id . '/delete')),
    );

  }

  $build['hook_table'] = array(
    '#theme' => 'table__pathedfiles__hooks',
    '#header' => $header,
    '#rows' => $rows,
  );

  $build['add_button'] = array(
    '#type' => 'link',
    '#title' => t('New pathed file'),
    '#href' => 'admin/content/file/pathed-files/new',
    '#attributes' => array('class' => 'button'),
    '#weight' => 0,
    // Container .form-item gives nice styles.
    '#prefix' => '<div class="form-item">',
    '#suffix' => '</div>',
  );

  return $build;
}

/**
 * Build form to insert new or edit existing pathed file.
 */
function pathed_files_admin_edit($form, &$form_state, $pathed_file_id = NULL) {

  // Are we starting a new file or editing existing one?
  $new = TRUE;
  if ($pathed_file_id) {
    $pathed_file = _pathed_files_get_entry($pathed_file_id);
    if (!empty($pathed_file)) {
      $new = FALSE;
    }
  }

  if (!$new) {
    $form['pathed_file_id'] = array('#type' => 'hidden', '#value' => $pathed_file_id);
  }

  $form['file_name'] = array(
    '#type' => 'textfield',
    '#title' => t('File name'),
    '#maxlength' => '199',
    '#size' => '20',
    '#required' => TRUE,
    '#default_value' => !$new ? $pathed_file->file_name : '',
  );
  $form['file_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#maxlength' => '199',
    '#size' => '20',
    '#required' => TRUE,
    '#default_value' => !$new ? $pathed_file->file_description : '',
  );
  $form['file_uri_path'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Path'),
    '#maxlength' => '199',
    '#size' => '40',
    '#required' => TRUE,
    '#default_value' => !$new ? $pathed_file->file_uri_path : '',
    '#field_prefix' => url('', array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );
  $form['use_file_content'] = array(
    '#type' => 'radios',
    '#title' => t('File entry type'),
    '#options' => array(0 => t('Upload file'), 1 => t('Manually enter file contents')),
    '#default_value' => isset($pathed_file->use_file_content) ? $pathed_file->use_file_content : 0,
  );
  if (!$new && !empty($pathed_file->file_id)) {
    $form['file_id'] = array(
      '#type' => 'hidden',
      '#value' => $pathed_file->file_id,
    );
    $form['current_file'] = array(
      '#type' => 'item',
      '#title' => t('Existing uploaded file:'),
      '#markup' => l(
        $pathed_file->file_uri_path,
        _pathed_files_get_managed_file_path($pathed_file->file_id, TRUE)
      ),
      '#states' => array(
        'visible' => array(
          ':input[name="use_file_content"]' => array('value' => '0'),
        ),
      ),
    );
  }
  // The empty array in #upload_validators will allow all extensions.
  $form['new_file_id'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload New File'),
    '#description' => t('Warning: If a new file is uploaded, it will replace the current one.'),
    '#size' => '40',
    '#upload_location' => 'public://pathed_files/',
    '#upload_validators' => array('file_validate_extensions' => array()),
    '#states' => array(
      'visible' => array(
        ':input[name="use_file_content"]' => array('value' => '0'),
      ),
    ),
  );
  $form['file_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter Text to use as file'),
    '#cols' => 60,
    '#rows' => 10,
    '#default_value' => !$new ? $pathed_file->file_content : '',
    '#states' => array(
      'visible' => array(
        ':input[name="use_file_content"]' => array('value' => '1'),
      ),
    ),
  );
  $form['enabled_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#return_value' => '1',
    '#default_value' => isset($pathed_file->enabled_flag) ? $pathed_file->enabled_flag : 0,
  );

  $form['bypass_file_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bypass file check'),
    '#description' => t('If you check this box, the system will not check
      whether an actual file at this path exists.'),
    '#default_value' => 0,
  );

  if (!$new) {
    $form['modified_info'] = array(
      '#type' => 'item',
      '#markup' => '<em>' . t('Last modified by @user on @date', array(
        '@user' => $pathed_file->user_modified_by,
        '@date' => $pathed_file->date_modified,
        )) . '</em>',
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  if (!$new) {
    $form['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => 'admin/content/file/pathed-files/' . $pathed_file->id . '/delete',
      '#attributes' => array('class' => 'button'),
    );
  }

  return $form;
}

/**
 * Validate the new/existing pathed_file form.
 */
function pathed_files_admin_edit_validate($form, &$form_state) {
  $new = !isset($form_state['values']['pathed_file_id']);

  if (!$form_state['values']['bypass_file_check']
    && file_exists($form_state['values']['file_uri_path'])) {
    form_set_error(
      'file_uri_path',
      t(
        'Another file at this path exists. Check %title to skip this step.',
        array('%title' => $form['bypass_file_check']['#title'])
      )
    );
  }

  // Check for any duplicates.
  foreach (_pathed_files_get_list() as $pathed_file) {
    // Ignore the existing DB record for the current form data item.
    if (!$new && $pathed_file->id == $form_state['values']['pathed_file_id']) {
      continue;
    }

    $edit_link = l($pathed_file->id, 'admin/content/file/pathed-files/' . $pathed_file->id . '/edit');
    if (strtolower(trim($pathed_file->file_name)) == strtolower(trim($form_state['values']['file_name']))) {
      form_set_error('file_name', t('Name already exists for ID !link.', array('!link' => $edit_link)));
    }
    if (strtolower(trim($pathed_file->file_uri_path)) == strtolower(trim($form_state['values']['file_uri_path']))) {
      form_set_error('file_uri_path', t('URL Path already exists for ID !link.', array('!link' => $edit_link)));
    }
  }
}

/**
 * Form submission.
 */
function pathed_files_admin_edit_submit($form, &$form_state) {

  global $user;
  $enabled_flag = !empty($form_state['values']['enabled_flag']) ? 1 : 0;
  $new = !isset($form_state['values']['pathed_file_id']);

  // Check if a replacement file was uploaded.
  $current_file_id = NULL;
  if (!$new) {
    $pathed_file = _pathed_files_get_entry($form_state['values']['pathed_file_id']);
    if (!empty($pathed_file->file_id)) {
      $current_file_id = $pathed_file->file_id;
    }
  }

  if ($form_state['values']['new_file_id'] !== 0) {
    $new_file_id = $form_state['values']['new_file_id'];
    // If replacement file uploaded, delete current file.
    if (!empty($form_state['values']['file_id'])) {
      $current_file_id = $form_state['values']['file_id'];
      file_delete(file_load($current_file_id));
    }
    // Set new file ID as file ID.
    $current_file_id = $new_file_id;
    // Mark new file as permanent.
    $new_file = file_load($current_file_id);
    $new_file->status = FILE_STATUS_PERMANENT;
    file_save($new_file);
  }

  $db_values = array(
    'file_name' => $form_state['values']['file_name'],
    'file_description' => $form_state['values']['file_description'],
    'file_uri_path' => $form_state['values']['file_uri_path'],
    'file_id' => $current_file_id,
    'file_content' => $form_state['values']['file_content'],
    'use_file_content' => $form_state['values']['use_file_content'],
    'date_modified' => date("Y-m-d h:i:s"),
    'user_modified_by' => $user->name,
    'enabled_flag' => $enabled_flag,
  );

  db_merge('pathed_files')
    ->key(array('id' => $new ? NULL : $form_state['values']['pathed_file_id']))
    ->fields($db_values)
    ->execute();

  // Rebuild menus so new callback is registered.
  menu_rebuild();
  _pathed_files_clear_cached_page($form_state['values']['file_uri_path']);

  $form_state['redirect'] = 'admin/content/file/pathed-files';
}

/**
 * Build form to delete a pathed file.
 */
function pathed_files_admin_delete($form, &$form_state, $pathed_file_id) {

  $pathed_file = _pathed_files_get_entry($pathed_file_id);
  if (empty($pathed_file)) {
    drupal_set_message(t('Invalid pathed file ID @id.', array('@id' => $pathed_file_id)), 'error');
    drupal_goto('admin/content/file/pathed-files');
  }

  $form['message'] = array(
    '#type' => 'item',
    '#markup' => t('Are you sure you wish to delete %name? You can disable it
      by !edit. If you delete it, it cannot be recovered.',
      array(
        '%name' => $pathed_file->file_name,
        '!edit' => l(t('editing the file'), 'admin/content/file/pathed-files/' . $pathed_file->id . '/edit'))),
  );
  $form['pathed_file_id'] = array('#type' => 'hidden', '#value' => $pathed_file->id);
  $form['pathed_managed_file_id'] = array('#type' => 'hidden', '#value' => $pathed_file->file_id);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Confirm Delete'));

  return $form;
}

/**
 * Submission handler to delete the pathed file.
 */
function pathed_files_admin_delete_submit($form, &$form_state) {
  $pathed_file_id = $form_state['values']['pathed_file_id'];
  $pathed_file = _pathed_files_get_entry($pathed_file_id);
  db_delete('pathed_files')->condition('id', $pathed_file_id)->execute();

  // Delete uploaded file.
  if (!empty($form_state['values']['pathed_managed_file_id'])
    && $file = file_load($form_state['values']['pathed_managed_file_id'])) {
    file_delete($file);
  }
  drupal_set_message(t('File %name has been deleted.', array('%name' => $pathed_file->file_name)));

  // Rebuild menus so deleted path is no longer registered.
  menu_rebuild();
  _pathed_files_clear_cached_page($pathed_file->file_uri_path);

  $form_state['redirect'] = 'admin/content/file/pathed-files';
}

/**
 * Get URI of uploaded file.
 */
function _pathed_files_get_managed_file_path($file_id, $real_url = FALSE) {
  if ($loaded_file = file_load($file_id)) {
    return $real_url ? file_create_url($loaded_file->uri) : $loaded_file->uri;
  }
  else {
    return '';
  }
}

/**
 * Flush page cache for a given path.
 */
function _pathed_files_clear_cached_page($path) {
  if (!variable_get('cache', 0)) {
    return;
  }

  cache_clear_all(url($path, array('absolute' => TRUE)), 'cache', TRUE);
}
