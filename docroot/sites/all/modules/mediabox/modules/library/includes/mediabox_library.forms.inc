<?php

/**
 * @file
 * Mediabox library forms.
 */

/**
 * Bulk upload form.
 *
 * @param $form
 * @param $form_state
 * @param array $options
 *   Next options are possible:
 *     - 'upload_validators' => array(
 *         'file_validate_extensions' => array('jpg jpeg gif png'),
 *         'my_custom_file_validator => array('some validation criteria'),
 *       )
 *     - 'plupload_settings' => array(
 *         'runtimes' => 'html5',
 *         'chunk_size' => '1mb',
 *         'max_file_size' => '2mb',
 *       )
 *     - 'submit_element' => 'jquery selector'
 *     - 'event_callbacks' => array('FilesAdded' => 'callback', 'UploadComplete' => 'callback')
 *
 *   plupload_settings is array of settings, that will be passed to Plupload
 *   library. @see: http://www.plupload.com/documentation.php.
 * @return array
 *
 */
function mediabox_library_form_upload($form, &$form_state, $options = array()) {
  $items = array();
  $library = drupal_get_library('plupload', 'plupload');

  $default_plupload_settings = array(
    'runtimes' => 'html5',
    'chunk_size' => '1mb',
    'max_file_size' => file_upload_max_size(),
  );
  if (isset($options['plupload_settings'])) {
    $options['plupload_settings'] += $default_plupload_settings;
  }
  else {
    $options['plupload_settings'] = $default_plupload_settings;
  }

  if (!file_exists(_plupload_library_path())) {
    drupal_set_message(t('System can not find plupload library at path !path. Please,
      download the library !here and place it to desired location',
      array(
        '!path' => _plupload_library_path(),
        '!here' => l(t('here'), $library['website']))),
      'warning'
    );
  }

  $items['type'] = array(
    '#type' => 'select',
    '#options' => mediabox_db_get_bundles_keyed(),
  );

  $items['files'] = array(
    '#type' => 'plupload',
    '#autoupload' => TRUE,
    '#autosubmit' => TRUE,
    '#title' => t('Files'),
    '#upload_validators' => isset($options['upload_validators']) ? $options['upload_validators'] : NULL,
    '#plupload_settings' => $options['plupload_settings'],
    '#submit_element' => !empty($options['submit_element']) ? $options['submit_element'] : NULL,
    '#event_callbacks' =>!empty($options['event_callbacks']) ? $options['event_callbacks'] : NULL,
  );

  $items['bulk_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to edit medias after upload'),
  );

  $items['max_file_size'] = array(
    '#type' => 'item',
    '#title' => t('Maximum file upload size'),
    '#markup' => format_size($options['plupload_settings']['max_file_size']),
  );

  $items['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('mediabox_library_form_upload_submit'),
  );

  return $items;
}

/**
 * Submit bulk upload.
 *
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_upload_submit($form, &$form_state) {
  $ids = mediabox_library_plupload_save($form_state['values']['files'], $form_state['values']['type']);

  // Remembering selected ids so we can mark them as selected in browse page.
  $form_state['ids'] = $ids;

  // Query is not empty so we want to make bulk update.
  if (sizeof($ids) && $form_state['values']['bulk_update']) {
    $id = array_shift($ids);
    $form_state['redirect'] = array('admin/structure/mediabox-library/update/' . $id, array('query' => array('ids' => $ids)));
  }
  else {
    $form_state['redirect'] = 'admin/structure/mediabox-library';
  }

}

/**
 * Bulk update form.
 *
 * @todo - this we probably do not need any more because we are not using this
 * code currently in library upload worfklow.
 *
 * @param $form
 * @param $form_state
 * @param $mediabox
 * @return array
 */
function mediabox_library_form_bulk_update($form, &$form_state, $mediabox) {
  drupal_set_title(mediabox_page_edit_title($mediabox));

  form_load_include($form_state, 'inc', 'mediabox', 'includes/mediabox.forms');
  $form = mediabox_form_media($form, $form_state, $mediabox);

  $form['bulk_update_query'] = array(
    '#type' => 'value',
    '#value' => 'x',
  );

  // for all buttons there is same redirect
  $form['actions']['submit']['#submit'][] = 'mediabox_library_form_bulk_update_submit';
  $form['actions']['delete']['#submit'][] = 'mediabox_library_form_bulk_update_submit';
  $form['actions']['cancel']['#submit'][] = 'mediabox_library_form_bulk_update_submit';

  // cancel all button will stop bulk update
  $form['actions']['cancel_all'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel all'),
    '#submit' => array('mediabox_library_form_bulk_update_cancel'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_bulk_update_submit($form, &$form_state) {
  $params = drupal_get_query_parameters();

  $ids = array();
  if (isset($params['ids'])) {
    $ids = $params['ids'];
  }

  if (sizeof($ids)) {
    $id = array_shift($ids);
    $form_state['redirect'] = array(
      'admin/structure/mediabox-library/update/' . $id, array(
        'query' => array(
          'ids' => $ids,
        )
      )
    );
  }
  else {
    $form_state['redirect'] = 'admin/structure/mediabox-library';
  }
}

/**
 * Cancel whole bulk update
 *
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_bulk_update_cancel($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/mediabox-library';
}

/* Helpers */

/**
 * Create mediabox entites from uploaded files.
 *
 * @param $files array that are uploaded over plupload.
 * @param $bundle_name string mediabox type - bundle.
 * @param string $directory
 *   URI directory for files upload. If NULL default file scheme will be used.
 *   Directory needs to have '/' on the end.
 * @return array
 *   Entity ids of created mediabox entites.
 */
function mediabox_library_plupload_save($files, $bundle_name, $directory = NULL) {
  if (empty($directory)) {
    $directory = variable_get('file_default_scheme', 'public') . '://';
  }

  // Create destination if it does not exist and check is it writable.
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    return FALSE;
  }

  // We can't use file_save_upload() because of
  // http://www.jacobsingh.name/content/tight-coupling-no-not
  $ids = array();
  $saved_files = array();
  foreach ($files as $uploaded_file) {
    if ($uploaded_file['status'] == 'done') {
      $source = $uploaded_file['tmppath'];

      $uri = file_stream_wrapper_uri_normalize($directory . $uploaded_file['name']);
      $uri = file_unmanaged_move($source, $uri, FILE_EXISTS_RENAME);
      if ($uri) {
        $file = plupload_file_uri_to_object($uri);
        $result = mediabox_db_save_media_from_file($file, $bundle_name);
        if ($result) {
          $ids[] = $result->id;
        }
        $saved_files[] = $file;
      }
      else {
        drupal_set_message(t('Unable to save file. Check your !settings.'), array(
          '!settings' =>
            l(t('filesystem configuration'), 'admin/config/media/file-system')
        ), FALSE);
      }
    }
    else {
      form_set_error('pud', t('Upload of !file failed', array('file' => $uploaded_file['name'])));
    }
  }

  return $ids;
}
