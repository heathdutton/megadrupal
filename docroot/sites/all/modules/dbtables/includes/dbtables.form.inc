<?php

/**
 * @file
 * DBTables form functions.
 */

/**
 * Implements hook_form().
 */
function dbtables_options_form() {
  $options = _dbtables_get_options_select();

  $form['tables'] = array(
    '#title' => t('Tables'),
    '#type' => 'select',
    '#description' => t('Please select a table.'),
    '#options' => $options,
  );

  $form['options'] = array(
    '#type' => 'radios',
    '#title' => t('Options'),
    '#options' => array(
      0 => t('Info'),
      1 => t('View Content'),
    ),
    '#description' => '',
    '#default_value' => 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Gets a list with all the db tables names.
 *
 * @return array
 *   The db tables names.
 */
function _dbtables_get_options_select() {
  $options = array();
  $schema = drupal_get_schema();
  ksort($schema);
  foreach ($schema as $key => $value) {
    $options[$key] = $key;
  }
  return $options;
}

/**
 * Implements hook_submit().
 */
function dbtables_options_form_submit($form, &$form_state) {
  switch ($form_state['values']['options']) {
    case '0':
      $url = 'admin/content/dbtables/' . $form_state['values']['tables'] . '/info';
      drupal_goto($url);
      break;

    case '1':
      $url = 'admin/content/dbtables/' . $form_state['values']['tables'] . '/view';
      drupal_goto($url);
      break;
  }
}
