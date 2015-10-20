<?php

function asaf_example_gui_simplest($form, &$form_state) {
  drupal_set_message(t('To ajaxify this form you have to open !link and put current form ID (%formid) in the forms textarea. ', array('!link' => l('asaf admin page', 'admin/config/system/asaf'), '%formid' => __FUNCTION__)), 'warning');

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

  return $form;
}

function asaf_example_gui_simplest_submit($form, &$form_state) {
  drupal_set_message(t('Specified name: %name', array('%name' => $form_state['values']['name'])));
}