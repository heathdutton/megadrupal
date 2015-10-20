<?php

function asaf_example_gui_buttons($form, &$form_state) {
  drupal_set_message(t('To ajaxify this form you have to open !link and put current form ID (%formid) in the forms textarea. You can enable or disable ajax just for some buttons (button IDs: %increment, %decrement, %multiply). Follow the instructions on the !link.', array(
    '!link' => l('asaf admin page', 'admin/config/system/asaf'),
    '%formid' => __FUNCTION__,
    '%increment' => 'actions][increment',
    '%decrement' => 'actions][decrement',
    '%multiply' => 'actions][multiply')), 'warning');

  $form['up'] = array(
    '#markup' => l('← Go to the examples list', 'examples/asaf_example'),
  );

  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('x'),
    '#required' => TRUE
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );

  $form['actions']['increment'] = array(
    '#type' => 'submit',
    '#value' => t('x+1'),
    '#process' => array('ajax_process_form', 'asaf_example_gui_buttons_button_process'),
  );
  $form['actions']['decrement'] = array(
    '#type' => 'submit',
    '#value' => t('x-1'),
    '#process' => array('ajax_process_form', 'asaf_example_gui_buttons_button_process'),
  );

  $form['actions']['multiply'] = array(
    '#type' => 'submit',
    '#value' => t('x*2'),
    '#process' => array('ajax_process_form', 'asaf_example_gui_buttons_button_process'),
  );

  return $form;
}

function asaf_example_gui_buttons_validate($form, &$form_state) {
  if (!is_numeric(trim($form_state['values']['number']))) {
    form_set_error('number', t('You should specify a correct number'));
  }
}

function asaf_example_gui_buttons_submit($form, &$form_state) {
  $x = trim($form_state['values']['number']);

  switch (end($form_state['clicked_button']['#parents'])) {
    case 'increment':
      drupal_set_message(t('Result: %result', array('%result' => $x+1)));
      break;
    case 'decrement':
      drupal_set_message(t('Result: %result', array('%result' => $x-1)));
      break;
    case 'multiply':
      drupal_set_message(t('Result: %result', array('%result' => $x*2)));
      break;
  }
}

function asaf_example_gui_buttons_button_process($element) {
  $element['#value'] .= isset($element['#ajax']) ? ' (ajaxified)' : '';
  return $element;
}