<?php

/**
 * @file
 * Flipping Book Pages.
 */

/**
 * Flipping Book Management form.
 */
function flipping_book_management_form($form, &$form_state) {

  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'flipping_book') . '/css/flipping_book.css',
    )
  );

  $max_upload_size = ini_get('upload_max_filesize');

  $form['filters']['fb'] = array(
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Title'), t('File'), t('Import')),
    '#rows' => array(),
  );

  $title = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
  );

  $file = array(
    '#type' => 'file',
    '#title' => t('Flipping Book'),
    '#description' => t('Allowed extension: zip. Files must be less than @max_upload_size', array('@max_upload_size' => $max_upload_size)),
  );

  $submit = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  $form['filters']['fb'][] = array(
    'title' => &$title,
    'file' => &$file,
    'submit' => &$submit,
  );

  $form['filters']['fb']['#rows'][] = array(
    array('data' => &$title),
    array('data' => &$file),
    array('data' => &$submit),
  );

  unset($title);
  unset($file);
  unset($submit);

  $form['#suffix'] = views_embed_view('flipping_books', 'block_1');

  return $form;
}

/**
 * Validation callback.
 *
 * @see flipping_book_management_form()
 */
function flipping_book_management_form_validate($form, &$form_state) {
  $file = file_save_upload('fb', array('file_validate_extensions' => array('zip')));

  if ($file) {
    $form_state['values']['fb'][0]['file'] = $file;
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * Submit callback.
 * @see flipping_book_management_form()
 */
function flipping_book_management_form_submit($form, &$form_state) {
  $title = $form_state['values']['fb'][0]['title'];

  $file = $form_state['values']['fb'][0]['file'];
  $filepath = drupal_realpath($file->uri);

  if (module_exists('transliteration')) {
    $dir = pathinfo(transliteration_clean_filename($file->filename), PATHINFO_FILENAME);
  }
  else {
    $dir = pathinfo(_flipping_book_clean_filename($file->filename), PATHINFO_FILENAME);
  }

  $fdir = flipping_book_dir();
  file_prepare_directory($fdir, FILE_CREATE_DIRECTORY);
  $destination = file_destination($fdir . '/' . $dir, FILE_EXISTS_RENAME);

  $zip = new ZipArchive();
  $res = $zip->open($filepath);

  if ($res) {
    try {
      $zip->extractTo(drupal_realpath($destination) . '/');

      $record = array(
        'title' => $title,
        'dir' => str_replace(flipping_book_dir() . '/', '', $destination),
        'created' => REQUEST_TIME,
      );

      if (drupal_write_record('flipping_book', $record)) {
        drupal_set_message(t('Your Flipping Book has been imported.'));
      }
      else {
        drupal_set_message(t('There was an error saving your Flipping Book.'), 'error');
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('Cannot extract Flipping Book file: @message', array(
        '@message',
        $e->getMessage(),
      )), 'error');
    }
  }
  else {
    drupal_set_message(t('Cannot save Flipping Book informations'), 'error');
  }
}

/**
 * Helper function to sanitize flipping_book filename
 */
function _flipping_book_clean_filename($filename) {
  // Replace whitespace.
  $filename = str_replace(' ', '_', $filename);
  // Remove remaining unsafe characters.
  $filename = preg_replace('![^0-9A-Za-z_.-]!', '', $filename);
  // Remove multiple consecutive non-alphabetical characters.
  $filename = preg_replace('/(_)_+|(\.)\.+|(-)-+/', '\\1\\2\\3', $filename);
  // Force lowercase to prevent issues on case-insensitive file systems.
  $filename = strtolower($filename);

  return $filename;
}
