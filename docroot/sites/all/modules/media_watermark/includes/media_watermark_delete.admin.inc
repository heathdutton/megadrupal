<?php

/**
 * @file
 * Media watermark module integration.
 *
 * Main interface for delete actions.
 */

/**
 * Form constructor for the media watermark delete form.
 *
 * @see media_watermark_delete_form_submit()
 *
 * @ingroup forms
 */
function media_watermark_delete_form($form, &$form_state) {
  $form_state['watermark_arg'] = arg(5);
  $name = media_watermark_get_name($form_state['watermark_arg']);
  if (!empty($name)) {
    $question = t('Are you sure you want to delete %name?', array('%name' => $name));
    $path = 'admin/config/media/watermark';
    return confirm_form($form, $question, $path, NULL, t('Delete'), NULL, 'confirm');
  }
  else {
    return t('There is no watermark with this id, or it is misconfigured.');
  }
}

/**
 * Form submission handler for media_watermark_delete_form().
 */
function media_watermark_delete_form_submit($form, &$form_state) {
  db_delete('media_watermark')
    ->condition('wid', $form_state['watermark_arg'])
    ->execute();
  drupal_set_message(t('Watermark has been deleted.'));
  $form_state['redirect'] = 'admin/config/media/watermark';
}
