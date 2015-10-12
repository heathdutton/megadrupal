<?php

/**
 * @file
 * Flipping Book Admin UI.
 */

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
