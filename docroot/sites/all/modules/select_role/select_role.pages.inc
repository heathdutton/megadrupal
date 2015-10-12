<?php

/**
 * @file
 *
 * Build landing page to select role.
 */

/**
 * Return form of landing page.
 */
function select_role_landing($form, &$form_state) {
  $select_role_options = select_role_options();
  $count = count($select_role_options);
  switch ($count) {
    case 0:
      $_SESSION['select_role'] = array(
        'role_selected' => NULL,
        'role_removed' => NULL,
      );
      drupal_goto();
      break;

    case 1:
      $_SESSION['select_role'] = array(
        'role_selected' => $select_role_options,
        'role_removed' => NULL,
      );
      list($label) = array_values($select_role_options);
      drupal_set_message(t('Your current role now set to <strong>@role</strong>', array('@role' => $label)));
      drupal_goto();
      break;

    default:
      $form['help'] = array(
        '#markup' => isset($_SESSION['select_role']) ? t('Select your role to change.') : t('Select your role first before continue.'),
      );
      if (isset($_SESSION['select_role']['role_selected'])) {
        $_default_value = array_keys($_SESSION['select_role']['role_selected']);
        list($default_value) = $_default_value;
      }
      $form['role_selected'] = array(
        '#type' => 'radios',
        '#required' => TRUE,
        '#options' => $select_role_options,
        '#default_value' => isset($default_value) ? $default_value : NULL,
      );
      $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Select'),
      );
      break;
  }
  return $form;
}

/**
 * Create session of select role.
 */
function select_role_landing_submit($form, &$form_state) {
  $select_role_options = select_role_options();
  $_role_selected = $form_state['values']['role_selected'];
  $role_selected = array($_role_selected => $select_role_options[$_role_selected]);
  // Remove one.
  unset($select_role_options[$_role_selected]);
  // Set Session.
  $_SESSION['select_role'] = array(
    'role_selected' => $role_selected,
    'role_removed' => $select_role_options,
  );
  drupal_set_message(t('Your current role now set to <strong>@role</strong>', array('@role' => $role_selected[$_role_selected])));
  drupal_goto();
}
