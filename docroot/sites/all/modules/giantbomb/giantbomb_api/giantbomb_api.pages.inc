<?php
/**
 * @file
 * Full pages for informational purposes.
 */

/**
 * Menu callback for Giant Bomb API configuration page.
 * 
 * @see giantbomb_api_menu()
 * @return array
 *   Render array.
 */
function giantbomb_api_configuration_form() {
  $form = array();
  $keys = variable_get('giantbomb_api_keys', array());
  $endpoints = giantbomb_api_endpoints();

  $form['keys']['#tree'] = TRUE;
  foreach ($endpoints as $name => $endpoint) {
    $form['keys'][$name] = array(
      '#type' => 'textfield',
      '#title' => t('@name API Key', array(
        '@name' => $endpoint['title'],
      )),
      '#default_value' => isset($keys[$name]) ? $keys[$name] : '',
      '#maxlength' => 64,
    );
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['actions']['submit_test'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Test'),
  );

  return $form;
}

/**
 * Form validation callback for giantbomb_api_configuration_form().
 */
function giantbomb_api_configuration_form_validate(&$form, &$form_state) {
  $keys = $form_state['values']['keys'];
  foreach ($keys as $name => $key) {
    // This pattern is undocumented, and is based off known keys.
    if (!empty($key) && !preg_match('/^[a-z0-9]{8,64}$/', $key)) {
      form_set_error('keys][' . $name, t('Invalid characters.'));
    }
  }
}

/**
 * Form submission callback for giantbomb_api_configuration_form().
 */
function giantbomb_api_configuration_form_submit(&$form, &$form_state) {
  $keys = $form_state['values']['keys'];
  $keys_var = array();

  foreach ($keys as $name => $key) {
    if (!empty($key)) {
      $keys_var[$name] = $key;
    }
  }

  variable_set('giantbomb_api_keys', $keys_var);
  drupal_set_message(t('Configuration saved.'));
}