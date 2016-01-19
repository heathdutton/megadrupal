<?php

/**
 * Implements hook_preprocessor_settings_form().
 */
function coffeescript_preprocessor_settings_form($form, $form_state) {
  extract($form_state['prepro']);
  $local += array(
    'errors' => 'watchdog',
  );

  $form['errors'] = array(
    '#type' => 'select',
    '#title' => 'Error reporting method',
    '#description' => t('How should the compiler record/display errors from processing?'),
    '#options' => array(
      'silent' => 'Silent: the file will fail to compile but no errors are shown anywhere.',
      'watchdog' => 'Watchdog: errors are recorded by the Watchdog to be viewed by an administrator.',
      'output' => 'Show on page: errors and a stacktrace are shown on the page in compatible browsers.',
    ),
    '#default_value' => $local['errors'],
  );

  return $form;
}
