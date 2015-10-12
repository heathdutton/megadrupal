<?php
/**
 * @file
 * Provides the simple test form.
 */

/**
 * Builds the simple test form.
 */
function forms_test_simple_form($form, &$form_state) {
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['country'] = array(
    '#title' => t('Country'),
    '#type' => 'select',
    '#options' => forms_test_get_countries(),
  );

  $form['submit'] = array(
    '#value' => t('Send'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Validation handler for the simple test form.
 */
function forms_test_simple_form_validate($form, &$form_state) {
  $v = $form_state['values'];

  $name = trim($v['name']);
  if (drupal_strlen($name) < 2 || !preg_match('%^[A-Z]\'?[- a-zA-Z]+$%', $name)) {
    form_set_error('name', t('Please enter a valid name.'));
  }
}

/**
 * Submit handler for the simple test form.
 */
function forms_test_simple_form_submit($form, $form_state) {
  $v = $form_state['values'];

  $name = trim($v['name']);
  $countries = forms_test_get_countries();
  if ($v['country'] == 4) {
    drupal_set_message(t('Hello @name from some other country!', array(
      '@name' => $name,
    )));
  }
  else {
    drupal_set_message(t('Hello @name from @country.', array(
      '@name' => $name,
      '@country' => $countries[$v['country']],
    )));
  }

}