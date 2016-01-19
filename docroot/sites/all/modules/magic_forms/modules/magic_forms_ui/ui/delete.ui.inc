<?php
/**
 * @file
 * Delete a form config.
 */

/**
 * The delete form callback.
 */
function magic_forms_ui_delete_form($form, &$form_state, $name = '') {
  $form = array();

  if (!isset($form_state['config'])) {
    $config = magic_forms_ui_config($name);
    if (!$config) {
      drupal_set_message(t('Failed to find the form configuration "@name".', array('@name' => $name)), 'error');
      return drupal_goto(MAGIC_FORMS_BASE_PATH);
    }

    $form_state['config'] = $config;
  }
  else {
    $config = $form_state['config'];
  }

  return confirm_form(
    $form,
    t('Are you sure you want to delete this form config?'),
    MAGIC_FORMS_UI_BASE_PATH
  );
}

/**
 * The delete form submit callback.
 */
function magic_forms_ui_delete_form_submit($form, &$form_state) {
  $config = $form_state['config'];
  db_delete('magic_forms')
    ->condition('name', $config->name)
    ->execute();

  drupal_set_message(t('The form config @name has successfully been deleted.', array('@name' => $config->name)));
  $form_state['redirect'] = MAGIC_FORMS_UI_BASE_PATH;
}
