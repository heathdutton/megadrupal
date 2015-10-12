<?php

function asaf_example_api_simplest($form, &$form_state) {
  $form['up'] = array(
    '#markup' => l('← Go to the examples list', 'examples/asaf_example'),
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  asaf_prepare_form($form, $form_state);

  return $form;
}

function asaf_example_api_simplest_submit($form, &$form_state) {
  drupal_set_message(t('Specified name: %name', array('%name' => $form_state['values']['name'])));
}