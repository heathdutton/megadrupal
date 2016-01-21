<?php
/**
 * @file
 * Export a form config.
 */

/**
 * The export form callback.
 */
function magic_forms_ui_export_form($form, &$form_state, $name = '') {
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

  $form['output'] = array(
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
    '#markup' => json_encode($config),
  );

  return $form;
}
