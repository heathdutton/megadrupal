<?php

/**
 * @file
 * Flipping Book Admin UI.
 */

/**
 * Flipping Book settings form.
 */
function flipping_book_settings_form($form, &$form_state) {
  $options = array(
    'public' => t('Public folder'),
    'private' => t('Private folder'),
  );
  $private = variable_get('file_private_path', '');

  if (empty($private)) {
    unset($options['private']);
  }

  $form['flipping_book_schema'] = array(
    '#title' => t('Import location'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('flipping_book_schema', 'public'),
    '#required' => TRUE,
    '#description' => t('Choose where you want to import your flipping books.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'flipping_book_settings_form_submit';

  return $form;
}

/**
 * Submit callback.
 *
 * @see flipping_book_settings_form()
 */
function flipping_book_settings_form_submit($form, &$form_state) {
  flipping_book_prepare_directory();

  $items = db_select('flipping_book', 'f')
    ->fields('f', array('dir'))
    ->execute()
    ->fetchCol();

  $schema = variable_get('flipping_book_schema', 'public');
  $old_schema = ($schema == 'private') ? 'public' : 'private';
  $source_dir = drupal_realpath("$old_schema://flipping_book");
  $destination_dir = drupal_realpath("$schema://flipping_book");

  foreach ($items as $item) {
    $source = "$source_dir/$item";
    if (file_exists($source)) {
      $destination = "$destination_dir/$item";
      file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
      rename($source, $destination);
    }
  }

  if (file_exists($source_dir)) {
    rmdir($source_dir);
  }
}

/**
 * Form constructor for the flipping_book deletion confirmation form.
 *
 * @see flipping_book_delete_confirm_submit()
 */
function flipping_book_delete_confirm($form, &$form_state, $flipping_book) {
  $form['#flipping_book'] = $flipping_book;

  // Always provide entity id in the same form key as in the entity edit form.
  $form['fbid'] = array('#type' => 'value', '#value' => $flipping_book->fbid);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $flipping_book->title)), FLIPPING_BOOK_MANAGEMENT_PATH,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes flipping book deletion.
 *
 * @see flipping_book_delete_confirm()
 */
function flipping_book_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $flipping_book = flipping_book_load($form_state['values']['fbid']);
    flipping_book_delete($form_state['values']['fbid']);

    watchdog('content', '@type: deleted %title.', array(
      '@type' => 'flipping_book',
      '%title' => $flipping_book->title,
    ), WATCHDOG_WARNING);

    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => 'Flipping Book',
      '%title' => $flipping_book->title,
    )));
  }

  $form_state['redirect'] = FLIPPING_BOOK_MANAGEMENT_PATH;
}
