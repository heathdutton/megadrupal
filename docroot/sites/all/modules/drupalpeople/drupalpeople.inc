<?php

function drupalpeople_admin_form() {
  $form['drupalpeople'] = array(
    '#title' => t('DrupalPeople'),
    '#type' => 'textfield',
    '#description' => t('Enter a comma separated list of Drupal.org user IDs of all participants in the creation of this website.'),
    '#default_value' => variable_get('drupalpeople', ''),
  );
  return system_settings_form($form);
}

function drupalpeople_admin_form_validate($form, $form_state){
  $csv = $form_state['values']['drupalpeople'];
  $digits = str_replace(array(',', ' '), '', $csv);
  if (!ctype_digit($digits)) {
    form_set_error('drupalpeople', t('Should be only numbers seperated by commas.'));
  }
}
